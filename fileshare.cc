#include <QFileDialog>
#include <QDebug>
#include <QDataStream>

#include "fileshare.hh"

#define BLOCKSIZE 8000

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
    QList<QByteArray*> *blockList = processFile(&file, &fileSize);
  }
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
  qDebug() << "FileShare.processFile: Processed. Size:" << *fsize;
  return blockList;
}
