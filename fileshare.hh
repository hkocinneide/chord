#ifndef CHORD_FILESHARE_HH
#define CHORD_FILESHARE_HH

#include <QList>
#include <QByteArray>
#include <QFile>

class FileShare : public QObject
{
  Q_OBJECT

public slots:
  void fileShareButtonPressed(void);

private:
  QList<QByteArray*> *processFile(QFile *file, int *fileSize);
};

#endif // CHORD_FILESHARE_HH
