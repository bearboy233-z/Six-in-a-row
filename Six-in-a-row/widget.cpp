#include "widget.h"
#include "./ui_widget.h"

#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <math.h>
#include <cstdlib>
#include <QThread>

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
    this->setAttribute(Qt::WA_TranslucentBackground,true);//半透明

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

    //绘制棋子
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

    //虚化背景
    if (game->gameStatus!=PLAYING)
    {
        painter.setBrush(QColor(0,0,0,100));
        painter.drawRect(0,0,size().width(),size().height());
    }

    //绘制模式选择界面
    if (game->gameStatus==unknownStatus)
    {
        //对话框
        //背景
        painter.setBrush(QColor(255,255,255,233));
        painter.setPen(QColor(255,255,255,233));
        painter.drawRect(size().width()*0.1,size().height()*0.2,size().width()*0.8,size().height()*0.5);
        //文本
        painter.setPen(QColor(50,50,50,255));
        painter.setFont(QFont("宋体",30,87));
        painter.drawText(size().width()*0.5-100,size().height()*0.35,"请选择模式");
        //选择按钮
        painter.setBrush(QColor(200,200,200,255));
        painter.setPen(QColor(180,180,180,255));
        painter.drawRect(size().width()*0.5-200,size().height()*0.45,160,80);
        painter.drawRect(size().width()*0.5+40,size().height()*0.45,160,80);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Times New Roman",20,75));
        painter.drawText(size().width()*0.5-200+50,size().height()*0.45+45,"P v P");
        painter.drawText(size().width()*0.5+40+50,size().height()*0.45+45,"P v E");
        //阴影
        painter.setPen(QPen(QColor(100,100,100,200),3));
        painter.drawLine(size().width()*0.5-200,size().height()*0.45+80,size().width()*0.5-200+160-1,size().height()*0.45+80);
        painter.drawLine(size().width()*0.5-200+160,size().height()*0.45,size().width()*0.5-200+160,size().height()*0.45+80-1);
        painter.drawLine(size().width()*0.5+40,size().height()*0.45+80,size().width()*0.5+40+160-1,size().height()*0.45+80);
        painter.drawLine(size().width()*0.5+40+160,size().height()*0.45,size().width()*0.5+40+160,size().height()*0.45+80-1);

        //按钮选择提示
        if (mousePosPvP&&(!mouseClickPvP))
        {
            painter.setBrush(QColor(190,190,190,255));
            painter.setPen(QColor(180,180,180,255));
            painter.drawRect(size().width()*0.5-200,size().height()*0.45,160,80);
            painter.setPen(Qt::black);
            painter.setFont(QFont("Times New Roman",20,75));
            painter.drawText(size().width()*0.5-200+52,size().height()*0.45+47,"P v P");
            painter.setPen(QPen(QColor(100,100,100,200),2));
            painter.drawLine(size().width()*0.5-200+1,size().height()*0.45+80,size().width()*0.5-200+160-1,size().height()*0.45+80);
            painter.drawLine(size().width()*0.5-200+160,size().height()*0.45+1,size().width()*0.5-200+160,size().height()*0.45+80-1);
            painter.setPen(QPen(QColor(100,100,100,200),1));
            painter.drawLine(size().width()*0.5-200,size().height()*0.45,size().width()*0.5-200+160-1,size().height()*0.45);
            painter.drawLine(size().width()*0.5-200,size().height()*0.45,size().width()*0.5-200,size().height()*0.45+80-1);
        }
        if (mousePosPvE&&(!mouseClickPvE))
        {
            painter.setBrush(QColor(190,190,190,255));
            painter.setPen(QColor(180,180,180,255));
            painter.drawRect(size().width()*0.5+40,size().height()*0.45,160,80);
            painter.setPen(Qt::black);
            painter.setFont(QFont("Times New Roman",20,75));
            painter.drawText(size().width()*0.5+40+52,size().height()*0.45+47,"P v E");
            painter.setPen(QPen(QColor(100,100,100,200),2));
            painter.drawLine(size().width()*0.5+40+1,size().height()*0.45+80,size().width()*0.5+40+160-1,size().height()*0.45+80);
            painter.drawLine(size().width()*0.5+40+160,size().height()*0.45+1,size().width()*0.5+40+160,size().height()*0.45+80-1);
            painter.setPen(QPen(QColor(100,100,100,200),1));
            painter.drawLine(size().width()*0.5+40,size().height()*0.45,size().width()*0.5+40+160-1,size().height()*0.45);
            painter.drawLine(size().width()*0.5+40,size().height()*0.45,size().width()*0.5+40,size().height()*0.45+80-1);
        }

        //按钮点击提示
        if (mousePosPvP&&mouseClickPvP)
        {
            painter.setBrush(QColor(200,200,200,255));
            painter.setPen(QColor(180,180,180,255));
            painter.drawRect(size().width()*0.5-200,size().height()*0.45,160,80);
            painter.setPen(Qt::black);
            painter.setFont(QFont("Times New Roman",20,75));
            painter.drawText(size().width()*0.5-200+55,size().height()*0.45+50,"P v P");
            painter.setPen(QPen(QColor(100,100,100,200),3));
            painter.drawLine(size().width()*0.5-200,size().height()*0.45,size().width()*0.5-200+160-1,size().height()*0.45);
            painter.drawLine(size().width()*0.5-200,size().height()*0.45,size().width()*0.5-200,size().height()*0.45+80-1);
        }
        if (mousePosPvE&&mouseClickPvE)
        {
            painter.setBrush(QColor(200,200,200,255));
            painter.setPen(QColor(180,180,180,255));
            painter.drawRect(size().width()*0.5+40,size().height()*0.45,160,80);
            painter.setPen(Qt::black);
            painter.setFont(QFont("Times New Roman",20,75));
            painter.drawText(size().width()*0.5+40+55,size().height()*0.45+50,"P v E");
            painter.setPen(QPen(QColor(100,100,100,200),3));
            painter.drawLine(size().width()*0.5+40,size().height()*0.45,size().width()*0.5+40+160-1,size().height()*0.45);
            painter.drawLine(size().width()*0.5+40,size().height()*0.45,size().width()*0.5+40,size().height()*0.45+80-1);
        }
    }

    //绘制鼠标选择提示标记
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

}

//鼠标选择
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->x();
    int y=event->y();

    //模式选择
    if (game->gameStatus==unknownStatus)
    {
        mousePosPvP=false;
        mousePosPvE=false;
        if (x>size().width()*0.5-200&&x<size().width()*0.5-200+160&&
                y>size().height()*0.45&&y<size().height()*0.45+80)
            mousePosPvP=true;
        if (x>size().width()*0.5+40&&x<size().width()*0.5+40+160&&
                y>size().height()*0.45&&y<size().height()*0.45+80)
            mousePosPvE=true;
        update();
    }


    //落子模糊判定
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
//释放鼠标
void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    //模式选择
    if (game->gameStatus==unknownStatus)
    {
        if (mousePosPvP) initPvPGame();
        if (mousePosPvE) initPvEGame();
        mouseClickPvP=false;
        mouseClickPvE=false;
    }

    //落子
    if (game->gameStatus==PLAYING&&rightMousePos)
    {
    if (game->gameMode==PvE&&game->AITurn) chessByAI();
        else chessByPerson();

    //gg
    if (game->gameStatus==BLACKWIN||game->gameStatus==WHITEWIN||game->gameStatus==DRAW)
    {
        if (game->gameStatus==DRAW) game->gameWindows->msg_Draw();
            else game->gameWindows->msg_End(game->gameStatus,game->gameMode,game->AITurn);
        initGame();
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
}
//压下鼠标
void Widget::mousePressEvent(QMouseEvent *event)
{
    //模式选择
    if (game->gameStatus==unknownStatus)
    {
        if (mousePosPvP)
        {
            mouseClickPvP=true;
            update();
        }
        if (mousePosPvE)
        {
            mouseClickPvE=true;
            update();
        }
    }
}

void Widget::initGame()
{
    delete game;
    game=new GameModel;
    game->chessNum=0;
    srand(time(0));
    game->gameStatus=unknownStatus;
    game->gameMode=unknownMode;
    mouseClickPvP=false;
    mouseClickPvE=false;
    update();
}

void Widget::initPvPGame()
{
    mousePosPvP=false;
    game_mode=PvP;
    game->gameStatus=PLAYING;
    game->AITurn=false;
    game->startGame(game_mode);
    update();
}

void Widget::initPvEGame()//pve未启用
{
    mousePosPvE=false;
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
    if (game->gameMode==PvE) game->AITurn=true;
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
