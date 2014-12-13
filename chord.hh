#ifndef CHORD_CHORD_HH
#define CHORD_CHORD_HH

#include<QObject>

#include "peer.hh"

#define NBITSSHA1 160

class Chord : public QObject
{
  Q_OBJECT

public:
  Chord();

  static Chord *chord;
  Peer *finger;
  Peer *smallest;

  void respondInit(QHostAddress *, quint16);
  void beginSync();
  void endSync();
  void syncPeer(QVariantMap *map);
  void notified(QVariantMap *msg);

private:
  bool sync;
  void addPeerToFinger(Peer *p);
  void printFinger();
  void notifyOthers();
};

#endif // CHORD_CHORD_HH
