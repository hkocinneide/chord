#include <QVBoxLayout>

#include "chorddialog.hh"

ChordDialog::ChordDialog()
{
	setWindowTitle("Chord");

	textview = new QTextEdit(this);
	textview->setReadOnly(true);
	textline = new QLineEdit(this);

	QVBoxLayout *layout = new QVBoxLayout();
	layout->addWidget(textview);
	layout->addWidget(textline);
	setLayout(layout);

	connect(textline, SIGNAL(returnPressed()),
		this, SLOT(gotReturnPressed()));
}

void ChordDialog::gotReturnPressed()
{
	textview->append(textline->text());

	textline->clear();
}

