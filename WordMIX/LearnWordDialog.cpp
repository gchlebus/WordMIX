// Local includes
#include "LearnWordDialog.h"
// Global includes
#include <time.h>
// Qt includes
#include <QSqlQuery>
#include <QDate>
#include <QKeyEvent>

//-------------------------------------------------------------------------------
//! Constructor.
//-------------------------------------------------------------------------------
LearnWordDialog::LearnWordDialog(int num, QWidget *parent /* = NULL */)
  : QDialog(parent, Qt::WindowMinimizeButtonHint)
  , count(num)
  , currentIdx(0)
{
  setupUi(this);

  createLists();
  createActions();
  installEventFilter(this);
  // set up dialog for first word
  next();  
}

//-------------------------------------------------------------------------------
//! Destructor.
//-------------------------------------------------------------------------------
LearnWordDialog::~LearnWordDialog()
{

}

//-------------------------------------------------------------------------------
//! Event filter.
//-------------------------------------------------------------------------------
bool LearnWordDialog::eventFilter(QObject *object, QEvent *e)
{
  if (e->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
    int key = keyEvent->key();

    if (key == Qt::Key_Return) {
      check();
      return true;
    }
    else if (key == Qt::Key_D) {
      next();
      return true;
    }
    else if (key == Qt::Key_A) {
      previous();
      return true;
    }
  }
  return false;
}

//-------------------------------------------------------------------------------
//! Creates actions.
//-------------------------------------------------------------------------------
void LearnWordDialog::createActions()
{
  connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
  connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
  connect(checkButton, SIGNAL(clicked()), this, SLOT(check()));
}
//-------------------------------------------------------------------------------
//! Goes to next word.
//-------------------------------------------------------------------------------
void LearnWordDialog::next()
{
  // increment counter
  if (++currentIdx > count) currentIdx = 1;
  // update counterLabel
  char buf[50];
  sprintf(buf, "%d/%d", currentIdx, count);
  counterLabel->setText(buf);

  wordLabel->setText(words[currentIdx-1]);
  wordDefinition->clear();
}

//-------------------------------------------------------------------------------
//! Goes to previous word.
//-------------------------------------------------------------------------------
void LearnWordDialog::previous()
{
  // decrement counter
  if (--currentIdx < 1) currentIdx = count;
  // update counterLabel
  char buf[50];
  sprintf(buf, "%d/%d", currentIdx, count);
  counterLabel->setText(buf);

  wordLabel->setText(words[currentIdx-1]);
  wordDefinition->clear();
}

//-------------------------------------------------------------------------------
//! Displays definition for currently displayed word.
//-------------------------------------------------------------------------------
void LearnWordDialog::check()
{
  wordDefinition->setHtml(definitions[currentIdx-1]);
}

//-------------------------------------------------------------------------------
//! Creates lists with words and their definitions to be used during learning.
//-------------------------------------------------------------------------------
void LearnWordDialog::createLists()
{
  QSqlQuery query("SELECT COUNT(*) AS ilosc FROM wordbook");
  query.next();
  int wordsCount = query.value(0).toInt();
  if (wordsCount < count) count = wordsCount;

  QStringList wordsTmp, definitionsTmp;
  query.exec("SELECT word, definition FROM wordbook");
  while (query.next()) {
    wordsTmp.append(query.value(0).toString());
    definitionsTmp.append(query.value(1).toString());
  }

  int day = QDate::currentDate().day()-1;
  int month = QDate::currentDate().month();
  srand(day * month);

  int num;
  QVector<int> nums;
  for (int i = 0; i < count; ++i) {
    bool present;
    do {
      present = false;
      num = (rand() % wordsCount) + 1;
      for (int j = 0; j < nums.size(); ++j) {
        if (num == nums[j]) present = true;
      }
    } while (present);
    nums.append(num);
    words.append(wordsTmp[num]);
    definitions.append(definitionsTmp[num]);
  }
}