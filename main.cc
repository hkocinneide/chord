#include <QApplication>
#include <QDebug>

#include "main.hh"
#include "chorddialog.hh"
#include "netsocket.hh"
#include "communicator.hh"
#include "peer.hh"

#define TESTMAIN true

int main(int argc, char **argv)
{
	QApplication app(argc,argv);

	NetSocket sock;
	if (!sock.bind())
		exit(1);

  Communicator comm;

	ChordDialog dialog;
	dialog.show();

  if (TESTMAIN)
  {
    Peer john(QHostAddress(1), 1);
    Peer joe(QHostAddress(0xffffffff), 0xffff);
    QVariantMap msg;
    msg.insert("Message", QVariant("It works!"));
    QHostAddress address = QHostAddress::LocalHost;
    comm.sendVariantMap(&msg, &address, 38148);
  }

	return app.exec();
}

