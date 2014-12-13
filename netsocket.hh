#ifndef CHORD_NETSOCKET_HH
#define CHORD_NETSOCKET_HH

#include <QUdpSocket>

class NetSocket : public QUdpSocket
{
	Q_OBJECT

public:
	NetSocket();

	// Bind this socket to a Peerster-specific default port.
	bool bind();
  static NetSocket *sock;
  static int myPort;
  static QHostAddress myIP;

private:
	int myPortMin, myPortMax;

  void getIP();
};

#endif // CHORD_NETSOCKET_HH
