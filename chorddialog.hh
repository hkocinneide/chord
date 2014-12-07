#ifndef CHORD_CHORDDIALOG_HH
#define CHORD_CHORDDIALOG_HH

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>

class ChordDialog : public QDialog
{
	Q_OBJECT

public:
	ChordDialog();

public slots:
	void gotReturnPressed();

private:
	QTextEdit *textview;
	QLineEdit *textline;
};

#endif // CHORD_CHORDDIALOG_HH
