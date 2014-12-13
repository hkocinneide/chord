#include <QGridLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDebug>

#include "chorddialog.hh"
#include "peer.hh"

ChordDialog *ChordDialog::dialog = NULL;

ChordDialog::ChordDialog()
{
  dialog = this;

	setWindowTitle("Chord");

	textview = new QTextEdit(this);
	textview->setReadOnly(true);
	textline = new QLineEdit(this);

  // Add yourself to the network
  QGroupBox *groupBoxConnect = new QGroupBox(tr("Join a Network"));

  newConnection = new TextEntryBox();
  newConnection->setMaximumHeight(35);

  QVBoxLayout *l = new QVBoxLayout();
  l->addWidget(newConnection);
  groupBoxConnect->setLayout(l);

  // Search Box
  QGroupBox *searchBoxOutline = new QGroupBox(tr("Search for files"));
  searchBox = new TextEntryBox();
  searchBox->setMaximumHeight(35);
  QVBoxLayout *m = new QVBoxLayout();
  m->addWidget(searchBox);
  searchBoxOutline->setLayout(m);

  searchList = new QListWidget(this);


  // Setting the layout

	// QVBoxLayout *layout = new QVBoxLayout();
  QGridLayout *layout = new QGridLayout();
  // First Column - Search
  layout->addWidget(searchBoxOutline, 0, 0);
  layout->addWidget(searchList, 1, 0);
  // Second Column - 
	layout->addWidget(textview, 0, 1);
	layout->addWidget(textline, 1, 1);
  // Third Column
  layout->addWidget(groupBoxConnect, 0, 2);
	setLayout(layout);

	connect(textline, SIGNAL(returnPressed()),
          this, SLOT(gotReturnPressed()));
  connect(newConnection, SIGNAL(returnPressed()),
          this, SLOT(gotReturnPressedConnection()));
  connect(searchBox, SIGNAL(returnPressed()),
          this, SLOT(gotReturnPressedSearch()));
}

///////////
// SLOTS //
///////////

void ChordDialog::gotReturnPressed()
{
	textview->append(textline->text());

	textline->clear();
}

void ChordDialog::gotReturnPressedConnection()
{
  if (Chord::chord == NULL)
  {
    QString connection = newConnection->toPlainText();
    if (connection != "New")
    {
      Peer *p = Peer::fromString(connection);
      if (!p)
      {
        newConnection->clear();
        return;
      }
      QVariantMap msg;
      msg.insert("Init", QVariant(1));
      qDebug() << "Sending Init message to" << p->IPAddress << p->port;
      Communicator::comm->sendVariantMap(&msg, p);
    }
    else
    {
      new Chord();
    }
  }
  newConnection->clear();
}

void ChordDialog::gotReturnPressedSearch()
{
  QString query = searchBox->toPlainText();
  // Do s.t.
  searchBox->clear();
}
