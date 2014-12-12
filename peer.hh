#ifndef CHORD_PEER_HH
#define CHORD_PEER_HH

#include <QByteArray>
#include <QHostAddress>

class Peer : public QObject
{
  Q_OBJECT

public:
  Peer(QHostAddress IP, quint16 port);

  QHostAddress IPAddress;
  quint16 port;
  QByteArray name;
};

#endif // CHORD_PEER_HH
