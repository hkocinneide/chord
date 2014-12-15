#ifndef CHORD_FILESHARE_HH
#define CHORD_FILESHARE_HH

#include <QList>
#include <QByteArray>
#include <QFile>
#include <QVariantMap>
#include <QListWidget>
#include <QHostAddress>

class FileShare : public QObject
{
  Q_OBJECT

public:
  FileShare();

  static FileShare *share;

  QMap<QString, quint64> *sharedFiles;
  QSet<quint64> *storedBlocks;

  QString fileBeingDownloadedName;
  QByteArray *fileBeingDownloaded;
  bool waitingOnBlockList;
  int blockListPos;
  QList<quint64> *blocksToRequest;
  quint64 blockWaitingOn;

  void receiveBlock(QVariantMap *msg);
  void addFile(QVariantMap *msg);
  void itemActivated(QListWidgetItem *item);
  void receiveBlockRequest(QVariantMap *msg, QHostAddress *sender, quint16 port);
  void receiveBlockList(QVariantMap *msg);
  void receiveBlockReply(QVariantMap *msg);
  void requestNextBlock();
  void constructFile();

public slots:
  void fileShareButtonPressed(void);

private:
  QList<QByteArray*> *processFile(QFile *file, int *fileSize);
  QByteArray *constructBlockList(QList<QByteArray*> *blocks);
  void saveBlockToDisk(QString name, QByteArray *data);
  QByteArray *readBlockFromDisk(QString name);
};

#endif // CHORD_FILESHARE_HH
