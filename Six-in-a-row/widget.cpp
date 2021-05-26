#include "widget.h"
#include "./ui_widget.h"

#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <math.h>
#include <cstdlib>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //鼠标追踪
    setMouseTracking(true);

    //窗口大小
    setFixedSize(EDGE*2+BLOCK_SIZE*BLOCK_NUM,EDGE*2+BLOCK_SIZE*BLOCK_NUM);

    //初始化
    initGame();

}

Widget::~Widget()
{
    delete ui;
}

//绘制
void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);//调用QPainter绘图
    painter.setRenderHint(QPainter::Antialiasing,true);//抗锯齿

    //绘制棋盘

    //设置棋盘背景
    QPixmap background("Recouses/chessboard.jpg");
    painter.drawPixmap(this->rect(),background);

    //绘制网格线
    painter.setPen(QPen(Qt::black,2));
    for (int i=0;i<LINE_NUM;i++)
    {
        //竖线
        painter.drawLine(EDGE+BLOCK_SIZE*i,EDGE,EDGE+BLOCK_SIZE*i,size().height()-EDGE);
        //横线
        painter.drawLine(EDGE,EDGE+BLOCK_SIZE*i,size().width()-EDGE,EDGE+BLOCK_SIZE*i);
    }

    //绘制鼠标点击提示标记
    if (game->gameStatus==PLAYING)
    {
    if (rightMousePos)
    {
        if (game->playerTurn==blackturn) painter.setPen(QPen(Qt::black,1.5));
            else painter.setPen(QPen(Qt::darkGray,1.5));
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE+MARK_CORNER,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MARK_CORNER);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE-MARK_CORNER,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MARK_CORNER);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE+MARK_CORNER,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE-MARK_CORNER);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE-MARK_CORNER,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE-MARK_CORNER);
    }
    }

    //绘制棋子
    if (game->gameStatus!=unknownStatus)
    {
    for (int i=0;i<LINE_NUM;i++)
    for (int j=0;j<LINE_NUM;j++)
    {
        if (game->chessPiece[i][j]==black){
            QPixmap black_chess("Recouses/blackchess.png");
            painter.drawPixmap(EDGE+j*BLOCK_SIZE-15,EDGE+i*BLOCK_SIZE-15,black_chess);
        }
        if (game->chessPiece[i][j]==white){
            QPixmap white_chess("Recouses/whitechess.png");
            painter.drawPixmap(EDGE+j*BLOCK_SIZE-15,EDGE+i*BLOCK_SIZE-15,white_chess);
        }
    }
    }
}

//鼠标点击模糊判定
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->x();
    int y=event->y();

    if (game->gameStatus==PLAYING)
    {
    if (x>EDGE-BLOCK_SIZE*0.5&&x<size().width()-EDGE+BLOCK_SIZE*0.5&&
            y>EDGE-BLOCK_SIZE*0.5&&y<size().height()-EDGE+BLOCK_SIZE*0.5)
    {
        int col=(x-EDGE)/BLOCK_SIZE;
        int row=(y-EDGE)/BLOCK_SIZE;
        rightMousePos=false;

        if (GameModel::len(x,y,EDGE+col*BLOCK_SIZE,EDGE+row*BLOCK_SIZE)<=POS_OFFSET)
        {
            mousePosCol=col;
            mousePosRow=row;
            if (game->chessPiece[mousePosRow][mousePosCol]==0)
                rightMousePos=true;
        }

        if (GameModel::len(x,y,EDGE+(col+1)*BLOCK_SIZE,EDGE+row*BLOCK_SIZE)<=POS_OFFSET)
        {
            mousePosCol=col+1;
            mousePosRow=row;
            if (game->chessPiece[mousePosRow][mousePosCol]==0)
                rightMousePos=true;
        }

        if (GameModel::len(x,y,EDGE+col*BLOCK_SIZE,EDGE+(row+1)*BLOCK_SIZE)<=POS_OFFSET)
        {
            mousePosCol=col;
            mousePosRow=row+1;
            if (game->chessPiece[mousePosRow][mousePosCol]==0)
                rightMousePos=true;
        }

        if (GameModel::len(x,y,EDGE+(col+1)*BLOCK_SIZE,EDGE+(row+1)*BLOCK_SIZE)<=POS_OFFSET)
        {
            mousePosCol=col+1;
            mousePosRow=row+1;
            if (game->chessPiece[mousePosRow][mousePosCol]==0)
                rightMousePos=true;
        }
    }
    update();//重绘
    }
}

//鼠标点击
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (!rightMousePos) return;
    if (game->gameMode==PvE&&game->AITurn) chessByAI();
        else chessByPerson();

    //gg
    if (game->gameStatus==BLACKWIN||game->gameStatus==WHITEWIN||game->gameStatus==DRAW)
    {
        if (game->gameStatus==DRAW) game->gameWindows->msg_Draw();
            else game->gameWindows->msg_End(game->gameStatus,game->gameMode,game->AITurn);
    }

    rightMousePos=false;

    //换手
    if (game->playerTurn==blackturn) game->playerTurn=whiteturn;
        else game->playerTurn=blackturn;
    if (game->gameMode==PvE)
    {
        if (game->AITurn) game->AITurn=false;
            else game->AITurn=true;
    }
}

void Widget::initGame()
{
    game=new GameModel;
    game->chessNum=0;
    srand(time(0));
    initPvPGame();//pve未启用，暂时默认pvp
}

void Widget::initPvPGame()
{
    game_mode=PvP;
    game->gameStatus=PLAYING;
    game->AITurn=false;
    game->startGame(game_mode);
    update();
}

void Widget::initPvEGame()
{
    game_mode=PvE;
    game->gameStatus=PLAYING;
    game->AITurn=false;
    if (rand()%2) game->AITurn=true;
    game->startGame(game_mode);
    update();
}

void Widget::chessByPerson()
{
    if (rightMousePos&&mousePosRow>=0&&mousePosRow<LINE_NUM
            &&mousePosCol>=0&&mousePosCol<LINE_NUM)
        game->move_in_chess(mousePosRow,mousePosCol);
    update();
}

void Widget::chessByAI()
{
    //模拟ai思考时间
    clock_t now=clock();
    while (clock()-now<AI_THINK_TIME);

    game->AIchess(&mousePosRow,&mousePosCol);
    game->move_in_chess(mousePosRow,mousePosCol);
    update();
    game->AITurn=false;
}
