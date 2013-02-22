#ifndef __WordMIX_H
#define __WordMIX_H

// UI include
#include "ui_wordmix.h"
//Qt includes
#include <QtGui/QMainWindow>

// Forward declarations
class QLabel;
class LearnWordDialog;

class WordMIXDialog 
  : public QMainWindow
  , public Ui::WordMIXClass
{
    Q_OBJECT

public:

  //! Constructor.
  WordMIXDialog(QWidget *parent = 0, Qt::WFlags flags = 0);
  //! Destructor.
  ~WordMIXDialog();

private slots:
  //! Tree widget selection changed slot.
  void on_resultsTreeWidget_itemSelectionChanged();
  //! Definition text browser selection.
  void on_definitionTextBrowser_selectionChanged();
  //! Add word action slot.
  void addWord();
  //! Edit word action slot.
  void editWord();
  //! Delete word action slot.
  void deleteWord();
  //! Learn words action slot.
  void learnWords();
  //! Change keyboard layout action slot.
  void changeLayout();
  //! Updates results tree.
  void updateTreeWidget();

private:
  //! Sets up result tree.
  void setupResultsTree();
  //! Creates actions.
  void createActions();
  //! Creates menus.
  void createMenus();
  //! Connects to database (if database doesn't exist, creates one).
  void connectToDatabase();
  //! Installs event filters.
  void installEventFilters();
  //! Gets number of results that matches text in searchLineEdit.
  int getOccurencesCount(const QString &s);
  //! Delay in seconds.
  void delay(int sec);
  
  QAction *keyboardLayoutAction;
  QAction *addAction;
  QAction *editAction;
  QAction *deleteAction;
  QAction *learnAction;
  QAction *exitAction;
  QLabel *statusLabel;
  QTreeWidgetItem *selectedItem;
  LearnWordDialog *learnWordDialog;
};

#endif // __WordMIX_H
