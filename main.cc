/* Currently interviewing in SF so apologies for the barebones
 * repository. This is obviously a framework for peerster, but
 * I plan on using the same basic qt features to get my chord
 * implementation running.
 *
 * -Hugh
 */

#include <QApplication>
#include <QDebug>

#include "main.hh"
#include "chorddialog.hh"
#include "netsocket.hh"

int main(int argc, char **argv)
{
	QApplication app(argc,argv);

	ChordDialog dialog;
	dialog.show();

	NetSocket sock;
	if (!sock.bind())
		exit(1);

	return app.exec();
}

