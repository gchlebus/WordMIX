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
  //! Constructor.
  LearnWordDialog(int num, QWidget *parent = NULL);
  //! Destructor.
  ~LearnWordDialog();

protected:
  //! Event filter.
  bool eventFilter(QObject *object, QEvent *e);

private slots:
  //! Creates actions.
  void createActions();
  //! Goes to next word.
  void next();
  //! Goes to previous word.
  void previous();
  //! Displays definition for currently displayed word.
  void check();

private:
  //! Creates lists with words and their definitions to be used during learning.
  void createLists();

  QStringList words;
  QStringList definitions;
  int currentIdx;
  int count;

};

#endif // __LearnWordDialog_H
