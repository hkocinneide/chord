#ifndef CHORD_CHORDDIALOG_HH
#define CHORD_CHORDDIALOG_HH

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>

#include "textentrybox.hh"

class ChordDialog : public QDialog
{
	Q_OBJECT

public:
	ChordDialog();

  QListWidget *searchList;

public slots:
	void gotReturnPressed();
  void gotReturnPressedConnection();

private:
	QTextEdit *textview;
	QLineEdit *textline;
  TextEntryBox *newConnection;
  TextEntryBox *searchBox;
};

#endif // CHORD_CHORDDIALOG_HH
