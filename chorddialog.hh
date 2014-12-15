#ifndef CHORD_CHORDDIALOG_HH
#define CHORD_CHORDDIALOG_HH

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QListWidget>

#include "textentrybox.hh"
#include "communicator.hh"
#include "chord.hh"

class ChordDialog : public QDialog
{
	Q_OBJECT

public:
	ChordDialog();

  static ChordDialog *dialog;
  Communicator *communicator;
  QListWidget *searchList;

  void initChord(QString connection);

public slots:
	void gotReturnPressed();
  void gotReturnPressedConnection();
  void gotReturnPressedSearch();
  void gotItemActivated(QListWidgetItem *item);

private:
	QTextEdit *textview;
	QLineEdit *textline;
  TextEntryBox *newConnection;
  TextEntryBox *searchBox;
};

#endif // CHORD_CHORDDIALOG_HH
