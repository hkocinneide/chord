#include "chord.hh"
#include "netsocket.hh"
#include "communicator.hh"

Chord *Chord::chord = NULL;

Chord::Chord()
{
  // Initialize the finger table to us
  chord = this;
  sync = false;
  finger = new Peer(NetSocket::myIP, NetSocket::myPort);
  finger->setLocal();
  smallest = finger;
}

void Chord::respondInit(QHostAddress *sender, quint16 senderPort)
{
  if (Chord::chord == NULL)
  {
    qWarning() << "Got a rogue respondInit";
    return;
  }
  QVariantMap msg;
  // Tell them we're starting
  msg.insert("BeginSync", QVariant(1));
  Communicator::comm->sendVariantMap(&msg, sender, senderPort);
  msg.remove("BeginSync");

  // Sync current node
  msg.insert("Sync", QVariant(1));
  msg.insert("IPAddress", QVariant(NetSocket::myIP.toIPv4Address()));
  msg.insert("Port", QVariant(NetSocket::myPort));
  msg.insert("Name", QVariant(Peer::myName));
  Communicator::comm->sendVariantMap(&msg, sender, senderPort);

  Peer *f = finger->next;
  while (f != finger)
  {
    msg.insert("IPAddress", QVariant(f->IPAddress.toIPv4Address()));
    msg.insert("Port", QVariant(f->port));
    msg.insert("Name", QVariant(f->name));
    Communicator::comm->sendVariantMap(&msg, sender, senderPort);
    f = f->next;
  }
  msg.remove("Sync");
  msg.remove("IPAddress");
  msg.remove("Port");
  msg.remove("Name");

  // We're done syncing
  msg.insert("EndSync", QVariant(1));
  Communicator::comm->sendVariantMap(&msg, sender, senderPort);
}

void Chord::beginSync()
{
  sync = true;
}

void Chord::endSync()
{
  sync = false;
  notifyOthers();
}

void Chord::notified(QVariantMap *msg)
{
  syncPeer(msg);
}

void Chord::notifyOthers()
{
  Peer *f = finger->next;
  QVariantMap msg;
  msg.insert("Notify", QVariant(1));
  msg.insert("IPAddress", QVariant(NetSocket::myIP.toIPv4Address()));
  msg.insert("Port", QVariant(NetSocket::myPort));
  msg.insert("Name", QVariant(Peer::myName));
  while (f != finger)
  {
    Communicator::comm->sendVariantMap(&msg, f);
    f = f->next;
  }
}

void Chord::syncPeer(QVariantMap *map)
{
  if (map->contains("IPAddress") && map->contains("Port") &&
      map->contains("Name"))
  {
    qDebug() << "Syncing finger list";
    Peer *p = new Peer(QHostAddress(map->value("IPAddress").toUInt()),
                       map->value("Port").toUInt());
    if (map->value("Name").toULongLong() != p->name)
      qWarning() << "ERROR CAN'T VERIFY NAME";
    addPeerToFinger(p);
  }
  else
  {
    qDebug() << "Couldn't read the sync message?";
  }
}

void Chord::addPeerToFinger(Peer *p)
{
  if (p->name < smallest->name)
  {
    qDebug() << "Adding smallest name to the list";
    p->prev = smallest->prev;
    p->next = smallest;
    smallest->prev->next = p;
    smallest->prev = p;
    smallest = p;
  }
  else
  {
    Peer *f = smallest->next;
    bool set = false;
    while (f != smallest)
    {
      if (p->name < f->name)
      {
        p->prev = f->prev;
        p->next = f;
        f->prev->next = p;
        f->prev = p;
        set = true;
        break;
      }
      f = f->next;
    }
    if (!set)
    {
      qDebug() << "Adding biggest name to the list";
      p->prev = f->prev;
      p->next = f;
      f->prev->next = p;
      f->prev = p;
    }
  }
  printFinger();
}

void Chord::printFinger()
{
  qDebug() << QString::number(finger->prev->name, 16) << "<- Repeat for loop reference\n|\nv";
  qDebug() << QString::number(finger->name, 16) << "<- Finger\n|\nv";
  Peer *f = finger->next;
  while (f != finger)
  {
    qDebug() << QString::number(f->name, 16) << "\n|\nv";
    f = f->next;
  }
}
