#include "sudoku_widget.h"
#include "qlineedit_clickable.h"

#include <QGridLayout>
#include <QFrame>
#include <QLineEdit>
#include <QIntValidator>
#include <QMessageBox>
#include <QPushButton>
#include <QRandomGenerator>
#include <QDateTime>
#include <QSet>
#include <iostream>
#include <QSignalMapper>
#include <QMainWindow>
#include <QResizeEvent>
#include <QVBoxLayout>
#include <QLayout>
#include <QFont>
#include <QTimer>
#include <QProgressBar>


SudokuWidget::SudokuWidget(QWidget * parent)
    : QWidget(parent) {

    mapper = new QSignalMapper( this );

    QIntValidator *pValidator = new QIntValidator( this );  // user can input only 1 - 9
    pValidator->setRange( 1, 9 ); // 1 - 9

    // Sudoku big square
    QGridLayout  *pMainLayout = new QGridLayout( this );
    pMainLayout->setSpacing(0);
    pMainLayout->setAlignment(Qt::AlignCenter);

    // 3 x 3 array of smaller squares
    QVector< QGridLayout * > bigGrids;
    for ( int y = 0; y < 3; y++ ){
        for ( int x = 0; x < 3; x++ ){
            QGridLayout *pInnerGrid = new QGridLayout( this );

            pInnerGrid->setAlignment(Qt::AlignHCenter);
            pInnerGrid->setSpacing(0);
            pInnerGrid->setMargin(0);

            pMainLayout->addLayout( pInnerGrid, y, x );
            bigGrids.push_back( pInnerGrid );
        }
    }


    tiles.resize(81);
    solve_data.resize(81);
    // adds tiles into smaller squares
    for (int y = 0; y < 9; y++){
        for (int x = 0; x < 9; x++){
            int bigGridIndex = box_position( y, x);
            QGridLayout *pInnerGrid = bigGrids[bigGridIndex];

            // square coordinates in the box
            int grid_y = y % 3;
            int grid_x = x % 3;

            QLineEdit *tile = new QLineEdit( this );
            tile->installEventFilter( this );

            int rowColId = x + y * 10;
            mapper->setMapping( tile, rowColId );
            connect( tile, SIGNAL( textEdited(QString) ), mapper, SLOT( map() ) );

            // tile customization
            tile->setValidator(pValidator); // input 1 - 9
            tile->setMaxLength(1);
            tile->setFixedSize(30,30);
            tile->setStyleSheet("QLineEdit{ border-width: 1.5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168); }");
            tile->setAlignment(Qt::AlignCenter);
            tile->setFrame(QFrame::Box);
            pInnerGrid->addWidget( tile, grid_y, grid_x);

            // inedex of tile in tiles array
            int tileIndex = y * 9 + x;
            tiles [tileIndex] = tile;
        }
    }

    connect( mapper, SIGNAL( mapped( int ) ), this, SLOT( onMapped( int ) ) );


    // Generate random digits with solvable solution ( 10 digs )
    random_position(10, random_pos_value);
    for (int i = 0;i < 10; i++){
        tiles[random_pos_value[i].first]->setText(QString::number(random_pos_value[i].second));
        tiles[random_pos_value[i].first]->setReadOnly(true);
    }

    // solve button
    Solve = new QPushButton( "Solve", this);
    Solve->setStyleSheet("background-color: rgb(221, 217, 42); font-weight: bold;");
    connect( Solve, SIGNAL(clicked()), this, SLOT(on_Solve_clicked()) );
    pMainLayout->addWidget(Solve, 5, 0);

    // clear board button
    Solve = new QPushButton( "Generate", this);
    Solve->setStyleSheet("background-color: rgb(221, 217, 42); font-weight: bold;");
    connect( Solve, SIGNAL(clicked()), this, SLOT(on_Reset_clicked()) );
    pMainLayout->addWidget(Solve, 5, 1);

    // Finish Button
    Solve = new QPushButton( "Finish", this);
    Solve->setStyleSheet("background-color: rgb(221, 217, 42); font-weight: bold;");
    connect( Solve, SIGNAL(clicked()), this, SLOT(on_Finish_clicked()) );
    pMainLayout->addWidget(Solve, 5, 2);

    // Progress bar

   // progressBar = new QProgressBar(this);
   // progressBar->setGeometry(600, 100, 300, 130);
    //connect(tiles,SIGNAL(test(int)), bar, SLOT)

    setLayout( pMainLayout );

}



//void SudokuWidget::mousePressEvent(QMouseEvent *event)
//{
//    emit clicked();
//}

void SudokuWidget::onMapped( int rowColId )
{
   // progressBar->rese;
    double count = 0;
    for (int pos = 0; pos < 81; pos++){
        if (tiles[pos]->text() != "" || tiles[pos]->text() != " " || tiles[pos]->text() != "0"){
            count++;
        }
    }

    //emit test(count);

    int pos = 0;
    game_data.resize(81);
    for (int y = 0; y < 9; y++){
        for(int x = 0; x < 9; x++){
            game_data[y].push_back(tiles[pos]);
            pos++;
        }
    }
    int y = rowColId / 10;
    int x = rowColId % 10;


    QLineEdit *tile1 = tiles[ y * 9 + x ];

    int tile_value = tile1->text().toInt();
    if (!is_legal_box(tile_value, y, x, game_data ) || !is_legal_row(tile_value, y, game_data) || !is_legal_colum(tile_value, x, game_data)){
        //tiles[y * 9 + x]->setStyleSheet("QLineEdit { color: red }");
        change_color(y, x, 1);
        this->hide();
        this->show();
    }
    if (is_legal_box(tile_value, y, x, game_data ) && is_legal_row(tile_value, y, game_data) && is_legal_colum(tile_value, x, game_data)){
        change_color(y, x, 0);
        this->hide();
        this->show();
    }
}




void SudokuWidget::on_Solve_clicked()
{

    int pos = 0;
    for ( int y = 0; y < 9; y++ ){
        for ( int x = 0; x < 9; x++ ){
            solve_data[y].push_back(tiles[pos]);
            pos++;

        }
    }
    if (Solve_Board(solve_data)){
        pos = 0;
        for ( int y = 0; y < 9; y++ ){
            for ( int x = 0; x < 9; x++ ){
                QString s = solve_data[y][x]->text();
                tiles[pos]->setText(solve_data[y][x]->text());
                tiles[pos]->setReadOnly(true);
                pos++;
            }
        }
        this->hide();
        this->show();
    }
}

void SudokuWidget::on_Reset_clicked()
{
    //QMessageBox::about(this,"works","works");
    for (int i = 0;i < 81; i++){
        tiles[i]->setText("");
        tiles[i]->setReadOnly(false);
    }
    random_pos_value.resize(0);
    random_position(10, random_pos_value);
    for (int i = 0;i < 10; i++){
        tiles[random_pos_value[i].first]->setText(QString::number(random_pos_value[i].second));
        tiles[random_pos_value[i].first]->setReadOnly(true);
    }
    this->hide();
    this->show();
}

void SudokuWidget::on_Finish_clicked()
{
    int count = 0;
    for (int i = 0; i < 81; i++){
        if (tiles[i]->text() == "" || tiles[i]->text() == "0" || tiles[i]->text() == " "){
            QMessageBox::about(this, "Title", "Your board is not finished");
            count++;
            break;
        }
    }
    if (count == 0){
        if (is_legal_board()){
            // QMessageBox::StandardButton reply = QMessageBox::question(this, "Title", "You won!!", QMessageBox::Yes | QMessageBox::No);
            QMessageBox msgBox;
            msgBox.setText(tr("You won!!!, Click reset to reset your board or Quit to exit the game"));
            QAbstractButton* quit = msgBox.addButton(tr("Quit"), QMessageBox::YesRole);
            QAbstractButton* pButtonYes = msgBox.addButton(tr("Reset"), QMessageBox::YesRole);

            msgBox.exec();

            if (msgBox.clickedButton() == pButtonYes){
               on_Reset_clicked();
               QMessageBox::about(this,"","Your board is reseted");
            }
            if (msgBox.clickedButton() == quit){
                this->close();
            }
        } else {
            QMessageBox::about(this, "Title", "You lose!!");
        }

    }

}

bool SudokuWidget::eventFilter(QObject *pObj, QEvent *pEvent)
{

    if ( pEvent->type() == QEvent::FocusIn )
    {
        for (int y = 0; y < 9; y++){
            for (int x = 0; x < 9; x++){
                change_color(y, x, 0);
            }
        }
        QLineEdit * pEdit = qobject_cast< QLineEdit * >( pObj );
        if ( pEdit )
        {
            //QMessageBox::about(this,"t","test");
            int pos = 0;
            for (int i = 0 ; i < 81; i++){
                if (pEdit == tiles[i]){
                    pos = i;
                    break;
                }
            }
            int y = pos / 9;
            int x = pos % 9;
            for (int i = 0;i < 9; i++ ){
                tiles[(y * 9) + (i % 9)]->setStyleSheet("background-color: rgb(234, 225, 81);");
                tiles[(i * 9) + (x % 9)]->setStyleSheet("background-color: rgb(234, 225, 81);");
            }
            this->hide();
            this->show();
            // Now look up the line edit in your QVector, determine its row and column, and highlight them
        }
    }
    return QWidget::eventFilter( pObj, pEvent );  // Let normal processing continue
}


bool SudokuWidget::Solve_Board(QVector< QVector <  QLineEdit* > > &board)
{
    int y, x;
    if ( !empty_cell( board, y, x ) ){
        return true;
    }
    for ( int num = 1; num <= 9; num++){
        board[y][x]->setText(QString::number(num));
        if(is_legal_box( num, y, x, board) && is_legal_row( num, y, board) && is_legal_colum( num, x, board)){
            if (Solve_Board(board)){
                return  true;
            }
            board[y][x]->setText("");
        }
    }
    board[y][x]->setText("");
    return false;
}

bool SudokuWidget::empty_cell(QVector< QVector <  QLineEdit* > > board, int &y, int &x)
{
    for (y = 0; y < 9; y++)
        for (x = 0; x < 9; x++)
            if (board[y][x]->text() == "0" || board[y][x]->text() == "" || board[y][x]->text() == " ") {
                return true;
            }
    return false;
}





bool SudokuWidget::is_legal_row(int value, int y,  QVector< QVector <  QLineEdit* > > &tiles1)
{
    int count = 0;
    for (int i = 0;i < 9; i++){
        if (QString::number(value) == tiles1[y][i]->text()){
            count++;
        }
    }
    if (count == 1){
        return true;
    }
    return false;
}

bool SudokuWidget::is_legal_colum(int value, int x, QVector<QVector<  QLineEdit* > > &tiles1)
{
    int count = 0;
    for (int i = 0;i < 9; i++){
        if (QString::number(value) == tiles1[i][x]->text()){
            count++;
        }
    }
    if (count == 1){
        return true;
    }
    return false;
}

void SudokuWidget::random_position(int digits, QVector <QPair <int, int> > &random_pos_value)
{

    QVector < QVector < QLineEdit* > > random_boards(81);
    for (int y1 = 0;y1 < 9; y1++){
        for (int x1 = 0;x1 < 9; x1++){
            QLineEdit *line_edit = new QLineEdit("0");
            random_boards[y1].push_back(line_edit);
        }
    }
    qsrand(QDateTime::currentMSecsSinceEpoch() / 1000);
    int count = 0;
    int value;
    while (true){
        int position = (qrand() % 80) + 0;
        int y = position / 9;
        int x = position % 9;
        if (random_boards[y][x]->text() == "0"){
            while (true){
                value = (qrand() % 9) + 1;
                random_boards[y][x]->setText(QString::number(value));
                if (is_legal_box(value, y, x, random_boards) && is_legal_colum(value, x, random_boards) && is_legal_row(value, y, random_boards)){
                    break;
                }
                random_boards[y][x]->setText("0");
            }
            QPair <int, int> values = {position,value};
            random_pos_value.push_back(values);
            count++;
            if (count == digits){
                break;
            }
        }
    }
}

bool SudokuWidget::is_legal_board()
{
    int pos = 0;
    finished_board_data.resize(81);
    for (int y = 0; y < 9; y++){
        for (int x = 0; x < 9; x++){
            if (tiles[pos]->text() == "0" || tiles[pos]->text() == "" || tiles[pos]->text() == " "){
                return false;
            }
            finished_board_data[y].push_back(tiles[pos]);
            pos++;
        }
    }
    for (int x = 0; x < 9; x++){
        if(!is_legal_colum(finished_board_data[0][x]->text().toInt(), x, finished_board_data)){
            return  false;
        }
    }
    for (int y = 0; y < 9; y++){
        if(!is_legal_row(finished_board_data[y][0]->text().toInt(), y, finished_board_data)){
            return false;
        }
    }
    for (int y = 0; y <= 6; y += 3){
        for (int x = 0; x <= 6; x += 3){
            if (!is_legal_box(finished_board_data[y][x]->text().toInt(), y, x, finished_board_data)){
                return  false;
            }
        }
    }
    return true;
}


int SudokuWidget::box_position(int y, int x)
{
    return 3*(y/3) + (x/3);
}

bool SudokuWidget::is_legal_box(int value, int y1, int x1,  QVector<QVector< QLineEdit* > > &tiles1)
{
    //box
    int count = 0;
    int square_position = box_position(y1, x1);
    QVector <QString> values;
    for (int y = (square_position / 3) * 3; y < ((square_position / 3) * 3) + 3;y++){
        for (int x = (square_position % 3) * 3; x < ((square_position % 3) * 3) + 3;x++){
            if (tiles1[y][x]->text() == QString::number(value)){
                count++;
            }
        }

    }
    if (count == 1){
        return true;
    }
    return false;
}

void SudokuWidget::change_color(int y, int x, int color)
{
    if (color == 0){

        if (y == 0){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-top: 5px; border-style: solid; border-color: rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }

        if (x == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }

        if (y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }

        if ((x == 1 && y == 1) || (x == 1 && y == 7)){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px;  border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if ((y == 1 && x == 3) || (y == 7 && x == 3) || (y == 7 && x == 6 ) || (y == 1 && x == 6 )){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px;  border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if ((y == 1 && x == 4) || (y == 7 && x == 4) || (y == 7 && x == 7 ) || (y == 1 && x == 7 )){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px;  border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if ((y == 3 && x == 1) || (y == 6 && x == 1) || ((y == 6 || y == 3) && (x == 3 || x == 4 || x == 6 || x == 7))){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px;  border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if ((y == 4 && (x == 1 || x == 3 || x == 4 || x == 6 || x == 7))){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px;  border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if (x == 0){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-left: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246); background-color: rgb(221, 217, 42); }");
        }
        if (x == 0 && y == 0){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-top: 5px; border-left: 5px; border-style: solid; border-color: rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246); background-color: rgb(221, 217, 42); }");
        }
        if (x == 8 && y == 0){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-top: 5px; border-right: 5px; border-style: solid; border-color: rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if (x == 8 && y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(250, 253, 246); background-color: rgb(221, 217, 42); }");
        }
        if (x == 0 && y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-left: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246); background-color: rgb(221, 217, 42); }");
        }
        if (x == 5 || x == 2){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if (y == 2 || y == 5){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if (x == 2 && y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if (x == 5 && y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if ( ( x == 2 || x == 5 || x == 8 ) && y == 5){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168); background-color: rgb(221, 217, 42);}");
        }
        if ( ( x == 2 || x == 5 || x == 8 ) && y == 2){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
        if (x == 0 && (y == 2 || y == 5)){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-left: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246); background-color: rgb(221, 217, 42); }");
        }
        if ( y == 0 && ( x == 2 || x == 5 ) ){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-top: 5px; border-right: 5px; border-style: solid; border-color: rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168); background-color: rgb(221, 217, 42); }");
        }
    } else {

        if (y == 0){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-top: 5px; border-style: solid; border-color: rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168); color: red }");
        }

        if (x == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168); color: red }");
        }
        if (y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246) rgb(238, 239, 168); color: red }");
        }
        if (x == 0){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-left: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246); color: red }");
        }
        if (x == 0 && y == 0){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-top: 5px; border-left: 5px; border-style: solid; border-color: rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246); color: red }");
        }
        if (x == 8 && y == 0){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-top: 5px; border-right: 5px; border-style: solid; border-color: rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168); color: red }");
        }
        if (x == 8 && y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(250, 253, 246); color: red }");
        }
        if (x == 0 && y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-left: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246); color: red }");
        }
        if (x == 5 || x == 2){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168); color: red  }");
        }
        if (y == 2 || y == 5){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246) rgb(238, 239, 168); color: red  }");
        }
        if (x == 2 && y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168); color: red  }");
        }
        if (x == 5 && y == 8){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168); color: red  }");
        }
        if ( ( x == 2 || x == 5 || x == 8 ) && y == 5){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168); color: red  }");
        }
        if ( ( x == 2 || x == 5 || x == 8 ) && y == 2){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-right: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168); color: red  }");
        }
        if (x == 0 && (y == 2 || y == 5)){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-bottom: 5px; border-left: 5px; border-style: solid; border-color: rgb(238, 239, 168) rgb(238, 239, 168) rgb(250, 253, 246) rgb(250, 253, 246); color: red  }");
        }
        if ( y == 0 && ( x == 2 || x == 5 ) ){
            tiles[y * 9 + x]->setStyleSheet("QLineEdit{ border-width: 1.5px; border-top: 5px; border-right: 5px; border-style: solid; border-color: rgb(250, 253, 246) rgb(250, 253, 246) rgb(238, 239, 168) rgb(238, 239, 168); color: red  }");
        }

    }

}


