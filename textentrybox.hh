#ifndef CHORD_TEXTENTRYBOX_HH
#define CHORD_TEXTENTRYBOX_HH

#include <QTextEdit>
#include <QKeyEvent>

class TextEntryBox : public QTextEdit
{
  Q_OBJECT

public:
  TextEntryBox(QWidget *parent = 0);

protected:
  void keyPressEvent(QKeyEvent *e);

signals:
  void returnPressed();
};

#endif // CHORD_TEXTENTRYBOX_HH
