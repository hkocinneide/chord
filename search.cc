#include "search.hh"
#include "fileshare.hh"

Search *Search::search = NULL;

Search::Search(QListWidget *list)
{
  search = this;
  searchList = list;
}

void Search::searchFiles(QString term)
{
  QMap<QString, quint64> *files = FileShare::share->sharedFiles;
  QMap<QString, quint64>::iterator i;
  for (i = files->begin(); i != files->end(); i++)
  {
    if (i.key().contains(term, Qt::CaseInsensitive))
    {
      new QListWidgetItem(i.key(), searchList);
    }
  }
}
