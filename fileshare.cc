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
    }
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
    blockHash->append(h);
    Communicator::comm->sendBlock(h, block);
  }
  return blockHash;
}
