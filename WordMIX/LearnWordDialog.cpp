// Local includes
#include "LearnWordDialog.h"
// Qt includes
#include <QSqlQuery>
#include <QDate>

LearnWordDialog::LearnWordDialog(int num, QWidget *parent /* = NULL */)
  : QDialog(parent)
  , count(num)
  , currentIdx(1)
{
  setupUi(this);

  createLists();
  char buf[50];
  sprintf(buf, "%d/%d", currentIdx, count);
  counterLabel->setText(buf);
  wordLabel->setText(words[currentIdx-1]);
  wordDefinition->clear();

  connect(nextButton, SIGNAL(clicked()), this, SLOT(next()));
  connect(previousButton, SIGNAL(clicked()), this, SLOT(previous()));
  connect(checkButton, SIGNAL(clicked()), this, SLOT(check()));
}

LearnWordDialog::~LearnWordDialog()
{

}

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

void LearnWordDialog::check()
{
  wordDefinition->setHtml(definitions[currentIdx-1]);
}

void LearnWordDialog::createLists()
{
  QSqlQuery query("SELECT word, definition FROM wordbook");
  int counter = 0;
  while (query.next() && counter++ < count) {
    words.append(query.value(0).toString());
    definitions.append(query.value(1).toString());
  }
  count = counter - 1;
}