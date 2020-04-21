#ifndef SUDOKU_WIDGET_H
#define SUDOKU_WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QSignalMapper;
class QLineEdit;
class QPushButton;
class QProgressBar;
QT_END_NAMESPACE




class SudokuWidget : public QWidget
{
    Q_OBJECT;

public:
    SudokuWidget( QWidget * parent = nullptr );
    // ~SudokuWidget();

signals:

   // void clicked();
   // void test(int x);

private slots:
    void onMapped( int rowColId );
    void on_Solve_clicked();
    void on_Reset_clicked();
    void on_Finish_clicked();

protected:
    bool eventFilter( QObject * pObj, QEvent * pEvent );

private:

    //QProgressBar *progressBar;

    QVector < QLineEdit *> tiles;
    void change_color (int y, int x, int color);
    QSignalMapper *mapper;
    QVector <QVector < QLineEdit* > > game_data;

    QPushButton *Solve;
    QVector <QVector < QLineEdit* > > solve_data;
    bool Solve_Board( QVector< QVector <  QLineEdit* > > &board );
    bool empty_cell( QVector< QVector <  QLineEdit* > >, int &y, int &x );


    QVector <QPair <int, int>> random_pos_value;
    void random_position(int digits, QVector < QPair <int, int> > &pos_value);

    // int mapRowAndColumnToTileIndex( int row, int column ) const;
    //bool mapTileIndexToRowAndColumn( int tileIndex,  int & row, int  & column ) const;

    QVector <QVector < QLineEdit* > > finished_board_data;
    bool is_legal_board();

    bool is_legal_row(int value, int y,  QVector< QVector <  QLineEdit* > > &tiles1);
    bool is_legal_colum(int value, int x, QVector< QVector <  QLineEdit* > > &tiles1);

    int box_position(int y, int x);
    bool is_legal_box(int value, int y,int x, QVector <QVector <  QLineEdit* > > &tiles1);
};


#endif // WIDGET_H
