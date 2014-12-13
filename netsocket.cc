#include <unistd.h>
#include <QTcpSocket>
#include "netsocket.hh"

////////////////////
// Static Members //
////////////////////

QHostAddress NetSocket::myIP = QHostAddress::Null;
NetSocket *NetSocket::sock = NULL;
int NetSocket::myPort = -1;

////////////////////
// Public Methods //
////////////////////

NetSocket::NetSocket()
{
	myPortMin = 32768 + (getuid() % 4096)*4;
	myPortMax = myPortMin + 3;
  sock = this;
}

bool NetSocket::bind()
{
	for (int p = myPortMin; p <= myPortMax; p++) {
		if (QUdpSocket::bind(p)) {
			qDebug() << "bound to UDP port " << p;
      myPort = p;
      getIP();
			return true;
		}
	}

	qDebug() << "Oops, no ports in my default range " << myPortMin
		<< "-" << myPortMax << " available";
	return false;
}

// Code found at:
// stackoverflow.com/questions/12927273/
// by rohanpm
void NetSocket::getIP()
{
  QTcpSocket socket;
  // Connect to Google DNS
  socket.connectToHost("8.8.8.8", 53);
  if (socket.waitForConnected())
  {
    qDebug() << "Local IPv4 address for Internet connectivity is"
             << socket.localAddress();
    myIP = socket.localAddress();
  }
  else
  {
    qDebug() << "Could not determine lcoal IPv4 address:"
             << socket.errorString();
    myIP = QHostAddress::LocalHost;
  }
}
