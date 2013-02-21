#ifndef __EventFilters_H 
#define __EventFilters_H

// Qt includes
#include <QObject>
#include <QLineEdit>
#include <QTreeWidget>
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>

//-------------------------------------------------------------------------------
// Definition Browser Event Filter
//-------------------------------------------------------------------------------
class DefBrowserEventFilter : 
  public QObject
{
  Q_OBJECT

public:

  DefBrowserEventFilter(QLineEdit *lineEdit, QTreeWidget *treeWidget,
    QObject *parent = 0)
    : QObject(parent), _lineEdit(lineEdit), _treeWidget(treeWidget) 
  {

  }

protected:

  bool eventFilter(QObject *object, QEvent *e) 
  {
    if (e->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
      int key = keyEvent->key();
      int modif = keyEvent->modifiers();

      // if pressed letters or backspace set focus to search line edit
      if ( (key >= Qt::Key_A && key <= Qt::Key_Z && modif == Qt::NoModifier) ||
        key == Qt::Key_Backspace) {
          QApplication::sendEvent(_lineEdit, e);
          _lineEdit->setFocus();
		  return true;
      }

      // if key down or key up then set focus to results tree
      if (key == Qt::Key_Up || key == Qt::Key_Down) {
        _treeWidget->setFocus();
		return true;
      }
      
    }
    return false;
  }

private:

  QLineEdit *_lineEdit;
  QTreeWidget *_treeWidget;

};

//-------------------------------------------------------------------------------
// Results Tree Widget Event Filter
//-------------------------------------------------------------------------------
class ResultsWidgetEventFilter
  : public QObject
{
  Q_OBJECT

public:

  ResultsWidgetEventFilter(QLineEdit *lineEdit, QObject *parent = 0)
    : QObject(parent), _lineEdit(lineEdit)
  {

  }

protected:

  bool eventFilter(QObject *object, QEvent *e) 
  {
    if (e->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
      int key = keyEvent->key();
      int modif = keyEvent->modifiers();

      // if pressed letters or backspace set focus to search line edit
      if ( (key >= Qt::Key_A && key <= Qt::Key_Z && modif == Qt::NoModifier) ||
        key == Qt::Key_Backspace) {
          QApplication::sendEvent(_lineEdit, e);
          _lineEdit->setFocus();
      }
    }
    return false;
  }

private:

  QLineEdit *_lineEdit;

};

//-------------------------------------------------------------------------------
// Search Line Edit Event Filter
//-------------------------------------------------------------------------------
class SearchLineEventFilter
  : public QObject
{
  Q_OBJECT

public:

  SearchLineEventFilter(QTreeWidget *treeWidget, QObject *parent = 0)
    : QObject(parent), _treeWidget(treeWidget)
  {

  }

protected:

  bool eventFilter(QObject *object, QEvent *e) 
  {
    if (e->type() == QEvent::KeyPress) {
      QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
      int key = keyEvent->key();
      
      // if key down or key up then set focus to results tree
      if (key == Qt::Key_Up || key == Qt::Key_Down) {
        _treeWidget->setFocus();
        return true;
      }
      
    }
    return false;
  }

private:

  QTreeWidget *_treeWidget;

};

#endif // __EventFilters_H