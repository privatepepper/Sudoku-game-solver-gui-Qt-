#ifndef QLINEEDIT_CLICKABLE_H
#define QLINEEDIT_CLICKABLE_H

#include <QWidget>
#include <Qt>
#include <QLineEdit>
#include <QVector>

class QLineEdit_Clickable : public QLineEdit
{
    Q_OBJECT
public:
    explicit QLineEdit_Clickable(QWidget *parent = nullptr);


signals:
    void HilightMe(int y, int x);


protected:

    void focusInEvent(QFocusEvent *e);
   // int y;
   // int x;


};

#endif // QLINEEDIT_CLICKABLE_H
