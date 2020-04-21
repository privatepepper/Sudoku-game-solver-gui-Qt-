#include "gamewindow.h"
#include "sudoku_widget.h"
#include <QLabel>
#include <QFrame>



GameWindow::GameWindow( QWidget *parent )
    : QMainWindow( parent )
{
    SudokuWidget *pSudokuWidget = new SudokuWidget( this );
    setCentralWidget(pSudokuWidget);
    QLabel *label = new QLabel(this);
    label->setText("<html><head/><body><p><span style= font-size:48pt;>Sudoku</span></p></body></html>");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("font-weight: bold; color: rgb(234, 225, 81) ");
    this->setStyleSheet("background-color: rgb(45, 42, 50);");
    //Here is how to change position:
    label->setGeometry(500,0,450,81);

}
