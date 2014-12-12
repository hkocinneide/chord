#include <QCryptographicHash>
#include <QDebug>

#include "peer.hh"

Peer::Peer(QHostAddress IP, quint16 port)
{
  qDebug() << "Constructing a peer:" << IP.toString() << ":" << port;
  IPAddress = IP;
  this->port = port;
  quint64 hashData;
  if (IP.protocol() == QAbstractSocket::IPv4Protocol)
  {
    hashData = (quint64)IP.toIPv4Address() << 16;
    hashData += port;
    qDebug() << "Peer's data to be hashed:" << hashData;
  }
  else
  {
    qDebug() << "Getting IPv6 Address, abort!\n";
    hashData = 0;
  }

  QByteArray blockToHash;
  QDataStream stream(&blockToHash, QIODevice::ReadWrite);
  stream << hashData;

  qDebug() << "Block to hash from IP and port:" << blockToHash.toHex();

  name = QCryptographicHash::hash(blockToHash, QCryptographicHash::Sha1);
  qDebug() << "Peer's name:" << name.toHex();
}
