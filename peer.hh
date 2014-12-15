#ifndef CHORD_PEER_HH
#define CHORD_PEER_HH

#include <QByteArray>
#include <QHostAddress>

class Peer : public QObject
{
  Q_OBJECT

public:
  Peer(QHostAddress IP, quint16 port);

  static quint64 myName;

  Peer *next;
  Peer *prev;
  QHostAddress IPAddress;
  quint16 port;
  quint64 name;

  static Peer *fromString(QString connection);

  void setLocal();
};

#endif // CHORD_PEER_HH
