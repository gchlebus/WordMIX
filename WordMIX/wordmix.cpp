// Local includes
#include "wordmix.h"
#include "AddWordDialog.h"
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
// WinAPI include
#include <Windows.h>

//-------------------------------------------------------------------------------
WordMIXDialog::WordMIXDialog(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
    , selectedItem(NULL)
{
  setupUi(this);

  QStringList headers;
  headers << "ID" << "Word";
  resultsTreeWidget->setHeaderLabels(headers);
  resultsTreeWidget->setColumnHidden(0, true);
  resultsTreeWidget->header()->hide();
  
  // Create actions
  addAction = new QAction("Dodaj", this);
  addAction->setShortcut(tr("Ctrl+D"));
  connect(addAction, SIGNAL(triggered()), this, SLOT(addWord()));
  editAction = new QAction("Edytuj", this);
  editAction->setShortcut(tr("Ctrl+E"));
  connect(editAction, SIGNAL(triggered()), this, SLOT(editWord()));
  deleteAction = new QAction("Usuń", this);
  connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteWord()));
  keyboardLayoutAction = new QAction("ChangeLayout", this);
  keyboardLayoutAction->setShortcut(tr("Ctrl+W"));
  connect(keyboardLayoutAction, SIGNAL(triggered()), this, SLOT(changeLayout()));
  connect(searchLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(updateTreeWidget()));

  QWidget::addAction(keyboardLayoutAction);
  resultsTreeWidget->addAction(addAction);
  resultsTreeWidget->addAction(editAction);
  resultsTreeWidget->addAction(deleteAction);
  resultsTreeWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
  
  connectToDatabase();

  definitionTextBrowser->installEventFilter(
    new DefBrowserEventFilter(searchLineEdit, resultsTreeWidget, this));
  resultsTreeWidget->installEventFilter(
    new ResultsWidgetEventFilter(searchLineEdit, this));
  searchLineEdit->installEventFilter(
    new SearchLineEventFilter(resultsTreeWidget, this));

  updateTreeWidget();

  QTime dieTime = QTime::currentTime().addSecs(1);
  while(QTime::currentTime() < dieTime) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  }
}

//-------------------------------------------------------------------------------
WordMIXDialog::~WordMIXDialog()
{
  
}

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
void WordMIXDialog::deleteWord()
{
  if (resultsTreeWidget->selectedItems().empty()) return;
  QTreeWidgetItem *item = resultsTreeWidget->selectedItems().at(0);
  QString id = item->text(0);

  QSqlQuery query("DELETE FROM wordbook WHERE id = " + id);

  updateTreeWidget();
}

//-------------------------------------------------------------------------------
void WordMIXDialog::changeLayout()
{
  ActivateKeyboardLayout((HKL)HKL_NEXT, KLF_SETFORPROCESS);
}

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
  QSqlQuery query("SELECT id, word FROM wordbook WHERE word LIKE '" + searchLineEdit->text() + "%' ORDER BY word");
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
int WordMIXDialog::getOccurencesCount(const QString &s)
{
  QSqlQuery query("SELECT COUNT(*) AS ilosc FROM wordbook WHERE word LIKE '%" + s + "%'");
  query.next();
  return query.value(0).toInt();
}
