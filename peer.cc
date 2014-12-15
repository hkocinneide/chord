#include <QCryptographicHash>
#include <QDebug>
#include <QStringList>
#include <QHostInfo>

#include "peer.hh"
#include "main.hh"
#include "netsocket.hh"

quint64 Peer::myName = 0;

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
  }
  else
  {
    qWarning() << "Getting IPv6 Address, abort!\n";
    hashData = 0;
  }

  QByteArray blockToHash;
  QDataStream blockStream(&blockToHash, QIODevice::WriteOnly);
  blockStream << hashData;

  name = Sha1Mod64(&blockToHash);
  qDebug() << "Peer's name:" << QString::number(name, 16);
}

// Make a local host peer
void Peer::setLocal()
{
  IPAddress = QHostAddress::LocalHost;
  port = NetSocket::myPort;
  myName = name;
  next = this;
  prev = this;
}

// Static function
Peer *Peer::fromString(QString connection)
{
  QStringList strlst = connection.split(":");
  if (strlst.count() != 2)
  {
    qDebug() << "Invalid IP:Port";
    return NULL;
  }
  QString hostname = strlst[0];
  QString port = strlst[1];
  bool ok;
  int udpport = strlst[1].toInt(&ok);
  if (!ok || udpport > 65535 || udpport < 0)
  {
    qDebug() << "Invalid Port";
    return NULL;
  }

  Peer *peer;
  QHostInfo hostInfo = QHostInfo::fromName(hostname);
  if (hostInfo.error() != QHostInfo::NoError ||
      hostInfo.addresses().isEmpty())
  {
    qDebug() << "Cannot find host";
    peer = NULL;
  }
  else
  {
    QHostAddress address = hostInfo.addresses().first();
    peer = new Peer(address, udpport);
  }
  return peer;
}
