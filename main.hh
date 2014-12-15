#ifndef CHORD_MAIN_HH
#define CHORD_MAIN_HH

#include <QString>
#include <QByteArray>

extern QString quint64ToHex(quint64 num);
extern quint64 Sha1Mod64(QByteArray *data);

#endif // CHORD_MAIN_HH
