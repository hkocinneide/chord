#include <QDataStream>
#include <QIODevice>

#include "communicator.hh"
#include "chord.hh"

Communicator *Communicator::comm = NULL;

Communicator::Communicator()
{
  comm = this;

  connect(NetSocket::sock, SIGNAL(readyRead()),
          this, SLOT(gotReadyRead()));
}

void Communicator::processDatagram(QByteArray *datagram,
                                   QHostAddress *sender,
                                   quint16 senderPort)
{
  qDebug() << "Size of Datagram:" << datagram->size();
  QDataStream stream(datagram, QIODevice::ReadOnly);
  QVariantMap map;
  stream >> map;

  qDebug() << "Size of Map:" << map.count();

  // Deal with the different kinds of messages we can receive here
  if (map.contains("Message"))
  {
    qDebug() << "Got a message!! Contents:\n"
             << map.value("Message").toString();
  }
  else if (map.contains("Init"))
  {
    qDebug() << "Got Init";
    Chord::chord->respondInit(sender, senderPort);
  }
  else if (map.contains("BeginSync"))
  {
    qDebug() << "Begin Sync";
    new Chord();
    Chord::chord->beginSync();
  }
  else if (map.contains("Sync"))
  {
    qDebug() << "Sync";
    Chord::chord->syncPeer(&map);
  }
  else if (map.contains("EndSync"))
  {
    qDebug() << "End Sync";
    Chord::chord->endSync();
  }
  else if (map.contains("Notify"))
  {
    Chord::chord->notified(&map);
  }
  else
  {
    qDebug() << "Got an unknown message";
  }
}

void Communicator::sendVariantMap(QVariantMap *msg,
                                  QHostAddress *IP,
                                  quint16 port)
{
  qDebug() << "Sending Datagram, map size:" << msg->count();
  qDebug() << "Sending to" << *IP << port;
  QByteArray data;
  QDataStream *serializer = new QDataStream(&data, QIODevice::WriteOnly);
  *serializer << *msg;
  delete serializer;

  qDebug() << "Writing data to wire, size (B):" << data.size();

  NetSocket::sock->writeDatagram(data, *IP, port);
}

void Communicator::sendVariantMap(QVariantMap *msg, Peer *p)
{
  sendVariantMap(msg, &p->IPAddress, p->port);
}

///////////
// SLOTS //
///////////

void Communicator::gotReadyRead()
{
  qDebug() << "Got ready read!";
  while (NetSocket::sock->hasPendingDatagrams())
  {
    QByteArray datagram;
    datagram.resize(NetSocket::sock->pendingDatagramSize());
    QHostAddress sender;
    quint16 port;

    NetSocket::sock->readDatagram(datagram.data(),
                                  datagram.size(),
                                  &sender,
                                  &port);
    processDatagram(&datagram, &sender, port);
  }
}
