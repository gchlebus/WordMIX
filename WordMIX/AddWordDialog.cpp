// Local includes
#include "AddWordDialog.h"
// Qt includes
#include <QSqlQuery>
#include <QMessageBox>
#include<QFile>

AddWordDialog::AddWordDialog(int m, QWidget *parent /* = NULL */)
  : QDialog(parent)
  , mode(m)
{
  setupUi(this);

  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

  if (mode == 0) {
    QFile file("./template.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;
    defLineEdit->setPlainText(file.readAll());
  }
}

AddWordDialog::~AddWordDialog()
{

}

void AddWordDialog::accept()
{
  QSqlQuery query("SELECT COUNT(*) AS ilosc FROM wordbook WHERE word = '" + wordLineEdit->text() + "'");
  query.next();

  if (wordLineEdit->text().isEmpty()) {
    done(QDialog::Rejected);
    return;
  }

  if (mode == 1) { 
    done(QDialog::Accepted);
    return;
  }

  if (query.value(0).toInt() == 0) {
    done(QDialog::Accepted);
    return;
  }
  
  int btn = QMessageBox::question(this, "Uwaga!", "Podane słowo już istnieje w słowniku."
    "Czy na pewno chcesz je dodać?", QMessageBox::Ok, QMessageBox::Cancel); 
    
  if (btn == QMessageBox::Ok) done(QDialog::Accepted);
  else done(QDialog::Rejected);
}

