// Local includes
#include "wordmix.h"
#include "AddWordDialog.h"
#include "LearnWordDialog.h"
#include "EventFilters.h"
// Qt includes
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QLabel>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDir>
#include <QTextStream>
#include <QFile>
#include <QTextCodec>
#include <QTime>

//-------------------------------------------------------------------------------
//! Constructor.
//------------------------------------------------------------------------------- 
WordMIXDialog::WordMIXDialog(QWidget *parent, QFlag flags)
    : QMainWindow(parent, flags)
    , selectedItem(NULL)
    , learnWordDialog(NULL)
{
  setupUi(this);
  setupResultsTree();
  createActions();
  createMenus();
  connectToDatabase();
  installEventFilters();
  updateTreeWidget();
  delay(1);
}

//-------------------------------------------------------------------------------
//! Destructor.
//-------------------------------------------------------------------------------
WordMIXDialog::~WordMIXDialog()
{
  
}

//-------------------------------------------------------------------------------
//! Tree widget selection changed slot.
//-------------------------------------------------------------------------------
void WordMIXDialog::on_resultsTreeWidget_itemSelectionChanged()
{
  if (resultsTreeWidget->selectedItems().empty()) {
    selectedItem = NULL;
    return;
  }

  selectedItem = resultsTreeWidget->selectedItems().at(0);
  QString id = selectedItem->text(0);
  QSqlQuery query("SELECT definition FROM wordbook WHERE id = " + id);
  query.next();
  definitionTextBrowser->setHtml(query.value(0).toString());
}

//-------------------------------------------------------------------------------
//! Definition text browser selection.
//-------------------------------------------------------------------------------
void WordMIXDialog::on_definitionTextBrowser_selectionChanged()
{
	QString text = definitionTextBrowser->textCursor().selectedText();
	if (text.length() > 0 &&
		text.split(' ').count() == 1 && text.split('\n').count() == 1) {
			searchLineEdit->setText(text);
	}
}

//-------------------------------------------------------------------------------
//! Add word action slot.
//-------------------------------------------------------------------------------
void WordMIXDialog::addWord()
{
  AddWordDialog dialog(0, this);
  if (dialog.exec()) {
    QSqlQuery query;
    query.prepare("INSERT INTO wordbook (word, definition, date_added, last_modified) "
                  "VALUES(:word, :definition, datetime('now'), datetime('now'))");
    query.bindValue(":word", dialog.wordLineEdit->text());
    query.bindValue(":definition", dialog.defLineEdit->toPlainText());
    if (!query.exec()) {
      QMessageBox::warning(this, "Błąd", query.lastError().text());
      return;
    }
  }
  
  updateTreeWidget();
}

//-------------------------------------------------------------------------------
//! Edit word action slot.
//-------------------------------------------------------------------------------
void WordMIXDialog::editWord()
{
  if (resultsTreeWidget->selectedItems().empty()) return;

  QTreeWidgetItem *item = resultsTreeWidget->selectedItems().at(0);
  QString id = item->text(0);

  QSqlQuery query("SELECT word, definition FROM wordbook WHERE id = " + id);
  query.next();

  AddWordDialog dialog(1, this);
  dialog.wordLineEdit->setText(query.value(0).toString());
  dialog.defLineEdit->setPlainText(query.value(1).toString());
  
  if (dialog.exec()) {
    QSqlQuery updateQuery("UPDATE wordbook SET word = '" + dialog.wordLineEdit->text() + 
      "', definition = '" + dialog.defLineEdit->toPlainText() + "', last_modified = datetime('now') WHERE id = " + id);
  }
  updateTreeWidget();
}

//-------------------------------------------------------------------------------
//! Delete word action slot.
//-------------------------------------------------------------------------------
void WordMIXDialog::deleteWord()
{
  if (resultsTreeWidget->selectedItems().empty()) return;
  QTreeWidgetItem *item = resultsTreeWidget->selectedItems().at(0);
  QString id = item->text(0);

  QSqlQuery query("DELETE FROM wordbook WHERE id = " + id);

  updateTreeWidget();
}

//-------------------------------------------------------------------------------
//! Learn words action slot.
//-------------------------------------------------------------------------------
void WordMIXDialog::learnWords()
{
  if (!learnWordDialog) {
    learnWordDialog = new LearnWordDialog(20, this);
  }
  learnWordDialog->show();
  learnWordDialog->activateWindow();
}

//-------------------------------------------------------------------------------
//! Change keyboard layout action slot.
//-------------------------------------------------------------------------------
void WordMIXDialog::changeLayout()
{
  //  ActivateKeyboardLayout((HKL)HKL_NEXT, KLF_SETFORPROCESS);
}

//-------------------------------------------------------------------------------
//! Updates results tree.
//-------------------------------------------------------------------------------
void WordMIXDialog::updateTreeWidget()
{
  char buf[10];
  sprintf(buf, "Wyniki: %d", getOccurencesCount(searchLineEdit->text()));
  resultsCountLabel->setText(buf);

  // Store ID of previously selected item, so the selection is preserved if possible
  QString id;
  if (selectedItem) id = selectedItem->text(0);

  // Clear tree widget and repopulate
  resultsTreeWidget->clear();
  QList<QTreeWidgetItem *> items;
  QSqlQuery query("SELECT id, word FROM wordbook WHERE word LIKE '" + searchLineEdit->text() + "%' ORDER BY word COLLATE NOCASE");
  while (query.next()) {
    QStringList entry;
    entry << query.value(0).toString() << query.value(1).toString();
    items.append(new QTreeWidgetItem((QTreeWidgetItem *)NULL, entry));
  }
  resultsTreeWidget->insertTopLevelItems(0, items);

  // Try to find in current tree widget previously selected item
  // if not possible then select first item in list
  QList<QTreeWidgetItem *> itemList = resultsTreeWidget->findItems(id, Qt::MatchExactly);
  if (!itemList.isEmpty()) selectedItem = itemList.at(0);
  else if (!items.isEmpty()) selectedItem = items.at(0);
  if (selectedItem) resultsTreeWidget->setCurrentItem(selectedItem);
}

//-------------------------------------------------------------------------------
//! Sets up result tree.
//-------------------------------------------------------------------------------
void WordMIXDialog::setupResultsTree()
{
  QStringList headers;
  headers << "ID" << "Word";
  resultsTreeWidget->setHeaderLabels(headers);
  resultsTreeWidget->setColumnHidden(0, true);
  resultsTreeWidget->header()->hide();
  resultsTreeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
}

//-------------------------------------------------------------------------------
//! Creates actions.
//-------------------------------------------------------------------------------
void WordMIXDialog::createActions()
{
  // Create actions
  addAction = new QAction(tr("&Dodaj"), this);
  addAction->setIcon(QIcon(":/WordMIX/add.png"));
  addAction->setShortcut(tr("Ctrl+D"));
  addAction->setStatusTip(tr("Dodaj słowo do słownika."));
  connect(addAction, SIGNAL(triggered()), this, SLOT(addWord()));

  editAction = new QAction(tr("&Edytuj"), this);
  editAction->setIcon(QIcon(":/WordMIX/edit.png"));
  editAction->setShortcut(tr("Ctrl+E"));
  editAction->setStatusTip(tr("Edytuj zaznaczone słowo."));
  connect(editAction, SIGNAL(triggered()), this, SLOT(editWord()));

  deleteAction = new QAction(tr("&Usuń"), this);
  deleteAction->setIcon(QIcon(":/WordMIX/delete.png"));
  deleteAction->setStatusTip(tr("Usuń zaznaczone słowo."));
  connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteWord()));

  learnAction = new QAction(tr("&Ucz się"), this);
  learnAction->setIcon(QIcon(":/WordMIX/learn.png"));
  learnAction->setShortcut(tr("Ctrl+L"));
  learnAction->setStatusTip(tr("Przejdź do nauki słówek."));
  connect(learnAction, SIGNAL(triggered()), this, SLOT(learnWords()));

  /*
  keyboardLayoutAction = new QAction(tr("Układ &klawiatury"), this);
  keyboardLayoutAction->setIcon(QIcon(":/WordMIX/keyboard.png"));
  keyboardLayoutAction->setShortcut(tr("Ctrl+W"));
  keyboardLayoutAction->setStatusTip(tr("Zmień język wejściowy klawiatury."));
  connect(keyboardLayoutAction, SIGNAL(triggered()), this, SLOT(changeLayout()));
  */
  
  connect(searchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateTreeWidget()));

  exitAction = new QAction(tr("&Zakończ"), this);
  exitAction->setIcon(QIcon(":/WordMIX/exit.png"));
  exitAction->setShortcut(tr("Ctrl+Q"));
  exitAction->setStatusTip(tr("Wyjdź z programu."));
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

//-------------------------------------------------------------------------------
//! Creates menus.
//-------------------------------------------------------------------------------
void WordMIXDialog::createMenus()
{
  menuFile->addAction(learnAction);
  menuFile->addAction(keyboardLayoutAction);
  menuFile->addSeparator();
  menuFile->addAction(exitAction);

  menuEdit->addAction(addAction);
  menuEdit->addAction(editAction);
  menuEdit->addAction(deleteAction);

  resultsTreeWidget->addAction(addAction);
  resultsTreeWidget->addAction(editAction);
  resultsTreeWidget->addAction(deleteAction);
}

//-------------------------------------------------------------------------------
//! Connects to database (if database doesn't exist, creates one).
//-------------------------------------------------------------------------------
void WordMIXDialog::connectToDatabase()
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  
  QDir dir("./db");
  if (!dir.exists()) dir.mkpath(".");
  db.setDatabaseName("./db/dictionary.db");
  
  if (!db.open()) {
    QMessageBox::warning(this, "Błąd", db.lastError().text());
    return;
  }

  QSqlQuery query("CREATE TABLE IF NOT EXISTS wordbook ( "
    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    "word TEXT, definition TEXT, date_added TEXT, last_modified TEXT)");
}

//-------------------------------------------------------------------------------
//! Installs event filters.
//-------------------------------------------------------------------------------
void WordMIXDialog::installEventFilters()
{
  definitionTextBrowser->installEventFilter(
    new DefBrowserEventFilter(searchLineEdit, resultsTreeWidget, this));
  resultsTreeWidget->installEventFilter(
    new ResultsWidgetEventFilter(searchLineEdit, this));
  searchLineEdit->installEventFilter(
    new SearchLineEventFilter(resultsTreeWidget, this));
}

//-------------------------------------------------------------------------------
//! Gets number of results that matches text in searchLineEdit.
//-------------------------------------------------------------------------------
int WordMIXDialog::getOccurencesCount(const QString &s)
{
  QSqlQuery query("SELECT COUNT(*) AS ilosc FROM wordbook WHERE word LIKE '" + s + "%'");
  query.next();
  return query.value(0).toInt();
}

//-------------------------------------------------------------------------------
//! Delay in seconds.
//-------------------------------------------------------------------------------
void WordMIXDialog::delay(int sec)
{
  QTime dieTime = QTime::currentTime().addSecs(sec);
  while(QTime::currentTime() < dieTime) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  }
}
