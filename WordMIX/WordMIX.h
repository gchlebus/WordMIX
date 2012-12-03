#ifndef __WordMIX_H
#define __WordMIX_H

// UI include
#include "ui_wordmix.h"
//Qt includes
#include <QtGui/QMainWindow>

// Forward declarations
class QLabel;

class WordMIXDialog 
  : public QMainWindow
  , public Ui::WordMIXClass
{
    Q_OBJECT

public:
  WordMIXDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
  ~WordMIXDialog();

private slots:
  void on_resultsTreeWidget_itemSelectionChanged();
  void addWord();
  void editWord();
  void deleteWord();
  void changeLayout();
  void updateTreeWidget();

private:
  void connectToDatabase();
  int getOccurencesCount(const QString &s);
  
  QAction *keyboardLayoutAction;
  QAction *addAction;
  QAction *editAction;
  QAction *deleteAction;
  QLabel *statusLabel;
  QTreeWidgetItem *selectedItem;
};

#endif // __WordMIX_H
