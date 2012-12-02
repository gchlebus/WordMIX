#ifndef __AddWordDialog_H
#define __AddWordDialog_H

// ui include
#include "ui_AddWord.h"
// Qt includes
#include <QDialog>

class AddWordDialog
  : public QDialog
  , public Ui_AddWord
{
  Q_OBJECT

public:
  AddWordDialog(int m, QWidget *parent = NULL);
  ~AddWordDialog();
  void accept();

private slots:
  void changeLayout();

private:
  QAction *keyboardLayoutAction;
  int mode; // 0 - add word
            // 1 - edit word
};



#endif // __AddWordDialog_H
