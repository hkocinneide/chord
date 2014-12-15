#include <unistd.h>
#include <QFileDialog>
#include <QDebug>
#include <QDataStream>
#include <QCryptographicHash>
#include <QFile>

#include "fileshare.hh"
#include "main.hh"
#include "communicator.hh"

#define BLOCKSIZE 8000

FileShare *FileShare::share = NULL;

FileShare::FileShare()
{
  share = this;
  sharedFiles = new QMap<QString, quint64>();
  storedBlocks = new QSet<quint64>();
  blocksToRequest = new QList<quint64>();
}

void FileShare::fileShareButtonPressed(void)
{
  QFileDialog fileDialog;
  QStringList files;
  int fileSize;
  
  fileDialog.setFileMode(QFileDialog::ExistingFiles);
  if (fileDialog.exec())
  {
    files = fileDialog.selectedFiles();
  }
  for (int ii = 0; ii < files.size(); ii++)
  {
    QString filename = files.at(ii);
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly))
    {
      qDebug() << "Successfully opened a file";
      QList<QByteArray*> *blocks = processFile(&file, &fileSize);
      QByteArray *blockList = constructBlockList(blocks);
      quint64 blockListHash = Sha1Mod64(blockList);
      qDebug() << "Sending block list";
      Communicator::comm->sendBlock(blockListHash, blockList);
      qDebug() << "Telling everyone else about our file";
      Communicator::comm->shareFile(filename, blockListHash);
    }
  }
}

void FileShare::addFile(QVariantMap *msg)
{
  if (msg->contains("ShareFile") && msg->contains("Filename") &&
      msg->contains("BlockListHash"))
  {
    quint64 h = msg->value("BlockListHash").toULongLong();
    QString file = msg->value("Filename").toString();
    qDebug() << "Adding new file:" << file
             << "to our list, with hash" << quint64ToHex(h);
    sharedFiles->insert(file, h);
  }
}

void FileShare::receiveBlock(QVariantMap *msg)
{
  if (msg->contains("Hash") && msg->contains("Data") &&
      msg->contains("BlockStore"))
  {
    quint64 h = msg->value("Hash").toULongLong();
    QByteArray data = msg->value("Data").toByteArray();
    qDebug() << "Storing this block:"
             << quint64ToHex(h);
    saveBlockToDisk(quint64ToHex(h), &data);
    storedBlocks->insert(h);
  }
  else
  {
    qWarning() << "CAN'T RECEIVE, ERROR!";
  }
}

QByteArray *FileShare::readBlockFromDisk(QString name)
{
  qDebug() << "Reading file" << name << "from disk";
  QString dlDir = "client_downloads/";
  QString clientDir = dlDir + quint64ToHex(Peer::myName) + "/";
  QString fileName = clientDir + name;
  QFile file(fileName);
  if (file.exists())
  {
    if (file.open(QIODevice::ReadOnly))
    {
      QDataStream data(&file);
      char *s = new char[BLOCKSIZE * 2];
      int nReadBytes = data.readRawData(s, BLOCKSIZE * 2);
      qDebug() << "Read" << nReadBytes << "Bytes from disk";
      return new QByteArray(s, nReadBytes);
    }
    else
    {
      qWarning() << "Could not open file";
    }
  }
  else
  {
    qWarning() << "FILE DOESN'T EXIT WHAT DO WE DO";
  }
  return NULL;
}

void FileShare::saveBlockToDisk(QString name, QByteArray *data)
{
  QString downloadFolder = "client_downloads/";
  QString clientDir = downloadFolder + quint64ToHex(Peer::myName) + "/";
  QString fileName = clientDir + name;
  if (!QDir(downloadFolder).exists())
  {
    QDir().mkdir(downloadFolder);
  }
  if (!QDir(clientDir).exists())
  {
    QDir().mkdir(clientDir);
  }
  qDebug() << "saveBlockToDisk params:" << name << data;
  QFile file(fileName);
  if (!file.open(QIODevice::WriteOnly))
  {
    qWarning() << "Could not open the file";
  }
  file.write(*data);
  file.close();
}


QList<QByteArray*> *FileShare::processFile(QFile *file, int *fsize)
{
  QList<QByteArray*> *blockList = new QList<QByteArray*>();
  QDataStream data(file);
  char s[BLOCKSIZE];
  int readSize;
  *fsize = 0;
  while ((readSize = data.readRawData(s, BLOCKSIZE)) > 0)
  {
    *fsize += readSize;
    QByteArray *block = new QByteArray(s, readSize);
    blockList->append(block);
  }
  qDebug() << "readSize:" << readSize;
  qDebug() << "FileShare.processFile: Processed. Size:" << *fsize;
  return blockList;
}

QByteArray *FileShare::constructBlockList(QList<QByteArray*> *blockList)
{
  QByteArray *blockHash = new QByteArray();
  for (int i = 0; i < blockList->length(); i++)
  {
    QByteArray *block = blockList->at(i);
    quint64 h = Sha1Mod64(block);
    QDataStream stream(blockHash, QIODevice::Append);
    stream << h;

    Communicator::comm->sendBlock(h, block);
    qDebug() << "blockHash size:" << blockHash->size();
    usleep(5000);
  }
  return blockHash;
}

void FileShare::constructFile()
{
  QStringList sl = fileBeingDownloadedName.split("/");
  QString fn = sl.last();
  qDebug() << "Creating File";
  if (!QDir("downloads").exists())
  {
    qDebug() << "Making downloads folder";
    QDir().mkdir("downloads");
  }
  QString clientDlFolder = "downloads/" + quint64ToHex(Peer::myName) + "/";
  if (!QDir(clientDlFolder).exists())
  {
    qDebug() << "Making client downloads folder";
    QDir().mkdir(clientDlFolder);
  }
  QFile file(clientDlFolder + fn);
  if (file.open(QIODevice::WriteOnly))
  {
    file.write(*fileBeingDownloaded);
    file.close();
  }
  else
  {
    qWarning() << "Was not able to open file";
  }
}

void FileShare::requestNextBlock()
{
  if (blockListPos == blocksToRequest->length())
  {
    qDebug() << "Should construct the file now";
    qDebug() << "File size:" << fileBeingDownloaded->size();
    constructFile();
  }
  else
  {
    Communicator::comm->requestBlock(blocksToRequest->at(blockListPos));
    blockListPos++;
  }
}

void FileShare::receiveBlockList(QVariantMap *msg)
{
  qDebug() << "In receiveBlockList";
  if (msg->contains("Data") && msg->contains("Hash") &&
      msg->contains("BlockReply"))
  {
    QByteArray data = msg->value("Data").toByteArray();
    quint64 h = msg->value("Hash").toULongLong();
    if (h == blockWaitingOn)
    {
      waitingOnBlockList = false;
      int numHashes = data.size() / 8;
      qDebug() << "Size of data" << data.size();
      if (data.size() % 8 == 0)
      {
        delete blocksToRequest;
        blocksToRequest = new QList<quint64>();
        QDataStream stream(&data, QIODevice::ReadOnly);
        quint64 readHash;
        for (int ii = 0; ii < numHashes; ii++)
        {
          stream >> readHash;
          blocksToRequest->append(readHash);
        }
        blockListPos = 0;
        qDebug() << "Successfully read" << numHashes << "hashes";
        for (int ii = 0; ii < numHashes; ii++)
          qDebug() << quint64ToHex(blocksToRequest->at(ii));
        requestNextBlock();
      }
      else
      {
        qWarning() << "Not right size to be block list";
      }
    }
    else
    {
      qWarning() << "This isn't the block you're looking for";
    }
  }
  else
  {
    qWarning() << "Malformed Block Reply";
  }
}

void FileShare::receiveBlockReply(QVariantMap *msg)
{
  qDebug() << "In receiveBlockReply";
  if (msg->contains("Data") && msg->contains("Hash") &&
      msg->contains("BlockReply"))
  {
    QByteArray data = msg->value("Data").toByteArray();
    quint64 h = msg->value("Hash").toULongLong();
    if (h == blockWaitingOn)
    {
      fileBeingDownloaded->append(data);
      qDebug() << "Appened the data";
      requestNextBlock();
    }
    else
    {
      qWarning() << "Not the block you're looking for";
    }
  }
  else
  {
    qWarning() << "Malformed Block Reply";
  }
}

void FileShare::receiveBlockRequest(QVariantMap *msg, QHostAddress *replyIP, quint16 replyPort)
{
  qDebug() << "Got a block request!";
  if (msg->contains("BlockRequest") && msg->contains("Hash"))
  {
    quint64 h = msg->value("Hash").toULongLong();
    if (storedBlocks->contains(h))
    {
      QByteArray *reply = readBlockFromDisk(quint64ToHex(h));
      qDebug() << "Size of reply" << reply->size();
      QVariantMap replyMsg;
      replyMsg.insert("BlockReply", QVariant(1));
      replyMsg.insert("Data", QVariant(*reply));
      replyMsg.insert("Hash", QVariant(h));
      Communicator::comm->sendVariantMap(&replyMsg, replyIP, replyPort);
    }
    else
    {
      qWarning() << "ERROR don't have this block";
    }
  }
  else
  {
    qWarning() << "ERROR malformed block request";
  }
}

void FileShare::itemActivated(QListWidgetItem *item)
{
  if (sharedFiles->contains(item->text()))
  {
    waitingOnBlockList = true;
    fileBeingDownloaded = new QByteArray();
    fileBeingDownloadedName = item->text();
    quint64 blockListHash = sharedFiles->value(item->text());
    Communicator::comm->requestBlock(blockListHash);
  }
  else
  {
    qWarning() << "ERROR: can't find file";
  }
}
