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

	ChordDialog dialog;
	dialog.show();

	NetSocket sock;
	if (!sock.bind())
		exit(1);

  Communicator comm(&sock);

  if (TESTMAIN)
  {
    Peer john(QHostAddress(1), 1);
    Peer joe(QHostAddress(0xffffffff), 0xffff);
  }

	return app.exec();
}

