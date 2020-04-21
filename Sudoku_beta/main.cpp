#include "menu.h"

#include <QApplication>
#include <QLabel>
#include <QLayout>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Menu w;
    w.setWindowTitle("Sudoku");
    w.setStyleSheet("background-color: rgb(45, 42, 50);");

    QLabel *label = new QLabel(&w);
    label->setText("<html><head/><body><p><span style= font-size:48pt;>Sudoku</span></p></body></html>");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-weight: bold; color: rgb(234, 225, 81) ");
    label->setGeometry(166,0,450,81);

    w.show();
    return a.exec();
}
