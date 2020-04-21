#include "menu.h"
#include "ui_menu.h"
#include <QStyleFactory>
#include <QMessageBox>
#include <QLabel>


Menu::Menu(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Menu)
{




    ui->setupUi(this);
    ui->comboBox->addItem("Easy");
    ui->comboBox->addItem("Medium");
    ui->comboBox->addItem("Hard");
    ui->comboBox->addItem("Beast Mode");
    for (int i = 0;i < ui->comboBox->count(); i++){
        ui->comboBox->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }

   ui->pushButton->setStyleSheet("background-color: rgb(221, 217, 42); color: rgb(45, 42, 50); font-weight: bold; ");
   ui->comboBox->setStyleSheet("background-color: rgb(221, 217, 42); color: rgb(45, 42, 50); font-weight: bold; ");
   //this->setStyleSheet("background-color: rgb(45, 42, 50);");

}

Menu::~Menu()
{
    delete ui;
}

// Open SudokuGrid window

void Menu::on_pushButton_clicked()
{
    this->hide();
    game_window = new GameWindow(this);
    game_window->showFullScreen();

}



