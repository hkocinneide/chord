#include <QApplication>
#include <QDebug>
#include <QCryptographicHash>
#include <QDataStream>

#include "main.hh"
#include "chorddialog.hh"
#include "netsocket.hh"
#include "communicator.hh"
#include "peer.hh"
#include "fileshare.hh"
#include "search.hh"

#define TESTMAIN false

int main(int argc, char **argv)
{
	QApplication app(argc,argv);

	NetSocket sock;
	if (!sock.bind())
		exit(1);

  Communicator comm;
  FileShare share;

	ChordDialog dialog;
	dialog.show();

  if (TESTMAIN)
  {
    // Peer john(QHostAddress(1), 1);
    // Peer joe(QHostAddress(0xffffffff), 0xffff);
    // QVariantMap msg;
    // msg.insert("Message", QVariant("It works!"));
    // QHostAddress address = QHostAddress::LocalHost;
    // comm.sendVariantMap(&msg, &address, NetSocket::sock->myPort);
    if (NetSocket::sock->myPort == 38148)
    {
      dialog.initChord("New");
    }
    else
    {
      dialog.initChord("aphid.zoo.cs.yale.edu:38148");
    }
  }

	return app.exec();
}

quint64 Sha1Mod64(QByteArray *data)
{
  // qDebug() << "Block to hash from IP and port:" << data->toHex();
  QByteArray rawHash =
    QCryptographicHash::hash(*data, QCryptographicHash::Sha1);
  // qDebug() << "Hash of that block:" << rawHash.toHex();
  QByteArray rawHashMod64 = rawHash.right(8);
  // qDebug() << "Right eight bytes of that:" << rawHashMod64.toHex();
  QDataStream hashStream(&rawHashMod64, QIODevice::ReadOnly);
  quint64 r;
  hashStream >> r;
  return r;
}

QString quint64ToHex(quint64 num)
{
  return QString::number(num, 16).rightJustified(16, '0');
}
