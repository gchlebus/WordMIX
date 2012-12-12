#ifndef __LearnWordDialog_H
#define __LearnWordDialog_H

// Ui include
#include "ui_LearnWord.h"
// Qt includes
#include <QDialog>

class LearnWordDialog
  : public QDialog
  , public Ui_LearnWords
{
  Q_OBJECT

public:
  LearnWordDialog(int num, QWidget *parent = NULL);
  ~LearnWordDialog();

protected:
  bool eventFilter(QObject *object, QEvent *e);

private slots:
  void next();
  void previous();
  void check();

private:
  void createLists();
  int getRandomNumber();
  QStringList words;
  QStringList definitions;
  int currentIdx;
  int count;

};

#endif // __LearnWordDialog_H
