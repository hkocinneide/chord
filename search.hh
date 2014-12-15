#ifndef CHORD_SEARCH_HH
#define CHORD_SEARCH_HH

#include <QListWidget>

class Search : public QObject
{
  Q_OBJECT

public:
  Search(QListWidget *list);

  static Search *search;

  QListWidget *searchList;

  void searchFiles(QString query);
};

#endif // CHORD_SEARCH_HH
