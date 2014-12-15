#ifndef CHORD_FILESHARE_HH
#define CHORD_FILESHARE_HH

#include <QList>
#include <QByteArray>
#include <QFile>
#include <QVariantMap>

class FileShare : public QObject
{
  Q_OBJECT

public:
  FileShare();

  static FileShare *share;

  QMap<QString, quint64> *sharedFiles;
  QSet<quint64> *storedBlocks;
  void receiveBlock(QVariantMap *msg);
  void addFile(QVariantMap *msg);

public slots:
  void fileShareButtonPressed(void);

private:
  QList<QByteArray*> *processFile(QFile *file, int *fileSize);
  QByteArray *constructBlockList(QList<QByteArray*> *blocks);
  void saveBlockToDisk(QString name, QByteArray *data);
  QByteArray *readBlockFromDisk(QString name);
};

#endif // CHORD_FILESHARE_HH
