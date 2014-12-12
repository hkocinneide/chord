#include <QDataStream>
#include <QIODevice>

#include "communicator.hh"

Communicator::Communicator(NetSocket *sock)
{
  this->sock = sock;

  connect(sock, SIGNAL(readyRead()),
          this, SLOT(gotReadyRead()));
}

void Communicator::processDatagram(QByteArray *datagram, QHostAddress *sender, quint16 senderPort)
{
  QDataStream stream(datagram, QIODevice::ReadOnly);
  QVariantMap *map = new QVariantMap();
  stream >> *map;

  // Deal with the different kinds of messages we can receive here
}

void Communicator::sendVariantMap(QVariantMap *msg, QHostAddress *IP, quint16 port)
{
  QByteArray data;
  QDataStream serializer(&data, QIODevice::WriteOnly);
  serializer << msg;

  sock->writeDatagram(data, *IP, port);
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
  while (sock->hasPendingDatagrams())
  {
    QByteArray datagram;
    datagram.resize(sock->pendingDatagramSize());
    QHostAddress sender;
    quint16 port;

    sock->readDatagram(datagram.data(), datagram.size(), &sender, &port);
    processDatagram(&datagram, &sender, port);
  }
}
