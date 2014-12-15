#ifndef CHORD_COMMUNICATOR_HH
#define CHORD_COMMUNICATOR_HH

#include <QByteArray>
#include <QHostAddress>

#include "netsocket.hh"
#include "peer.hh"

class Communicator : public QObject
{
  Q_OBJECT

public:
  Communicator();

  static Communicator *comm;
  NetSocket *sock;

  void processDatagram(QByteArray *datagram, QHostAddress *sender, quint16 senderPort);
  void sendVariantMap(QVariantMap *msg, QHostAddress *IP, quint16 port); 
  void sendVariantMap(QVariantMap *msg, Peer *peer);
  void sendBlock(quint64, QByteArray *block);

public slots:
  void gotReadyRead();

};

#endif // CHORD_COMMUNICATOR_HH
