#include "widget.h"
#include "./ui_widget.h"

#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <math.h>
#include <cstdlib>
#include <QThread>
#include <windows.h>
#include <Mmsystem.h>
#include <QMenu>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    //鼠标追踪
    setMouseTracking(true);

    //窗口大小
    setFixedSize(EDGE*2+BLOCK_SIZE*BLOCK_NUM,EDGE*2+BLOCK_SIZE*BLOCK_NUM+MENU_HEIGHT);

    game=new GameModel;
    //初始化
    initMenuBar();
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
    QPixmap background("Resources/chessboard.jpg");
    painter.drawPixmap(this->rect(),background);
    painter.setBrush(QColor(255,255,255,255));
    painter.setPen(QPen(QColor(255,255,255,255),1));
    painter.drawRect(0,0,size().width(),MENU_HEIGHT+2);

    //绘制网格线
    painter.setPen(QPen(Qt::black,2));
    for (int i=0;i<LINE_NUM;i++)
    {
        //竖线
        painter.drawLine(EDGE+BLOCK_SIZE*i,EDGE+MENU_HEIGHT,EDGE+BLOCK_SIZE*i,size().height()-EDGE);
        //横线
        painter.drawLine(EDGE,EDGE+BLOCK_SIZE*i+MENU_HEIGHT,size().width()-EDGE,EDGE+BLOCK_SIZE*i+MENU_HEIGHT);
    }

    //绘制棋子
    for (int i=0;i<LINE_NUM;i++)
    for (int j=0;j<LINE_NUM;j++)
    {
        if (game->chessPiece[i][j]==black){
            QPixmap black_chess("Resources/blackchess.png");
            painter.drawPixmap(EDGE+j*BLOCK_SIZE-15,EDGE+i*BLOCK_SIZE-15+MENU_HEIGHT,black_chess);
        }
        if (game->chessPiece[i][j]==white){
            QPixmap white_chess("Resources/whitechess.png");
            painter.drawPixmap(EDGE+j*BLOCK_SIZE-15,EDGE+i*BLOCK_SIZE-15+MENU_HEIGHT,white_chess);
        }
    }

    //虚化背景
    if (game->gameStatus!=PLAYING)
    {
        painter.setBrush(QColor(0,0,0,100));
        painter.drawRect(0,MENU_HEIGHT+3,size().width(),size().height());
    }

    //绘制模式选择界面
    if (game->gameStatus==unknownStatus)
    {
        //对话框
        //背景
        painter.setBrush(QColor(255,255,255,233));
        painter.setPen(QColor(255,255,255,233));
        painter.drawRect(size().width()*0.1,size().height()*0.2+MENU_HEIGHT,size().width()*0.8,size().height()*0.5+MENU_HEIGHT);
        //文本
        painter.setPen(QColor(50,50,50,255));
        painter.setFont(QFont("宋体",30,87));
        painter.drawText(size().width()*0.5-100,size().height()*0.35+MENU_HEIGHT,"请选择模式");
        //选择按钮
        painter.setBrush(QColor(200,200,200,255));
        painter.setPen(QColor(180,180,180,255));
        painter.drawRect(size().width()*0.5-200,size().height()*0.45+MENU_HEIGHT,160,80);
        painter.drawRect(size().width()*0.5+40,size().height()*0.45+MENU_HEIGHT,160,80);
        painter.setPen(Qt::black);
        painter.setFont(QFont("Times New Roman",20,75));
        painter.drawText(size().width()*0.5-200+50,size().height()*0.45+45+MENU_HEIGHT,"P v P");
        painter.drawText(size().width()*0.5+40+50,size().height()*0.45+45+MENU_HEIGHT,"P v E");
        //阴影
        painter.setPen(QPen(QColor(100,100,100,200),3));
        painter.drawLine(size().width()*0.5-200,size().height()*0.45+80+MENU_HEIGHT,size().width()*0.5-200+160-1,size().height()*0.45+80+MENU_HEIGHT);
        painter.drawLine(size().width()*0.5-200+160,size().height()*0.45+MENU_HEIGHT,size().width()*0.5-200+160,size().height()*0.45+80-1+MENU_HEIGHT);
        painter.drawLine(size().width()*0.5+40,size().height()*0.45+80+MENU_HEIGHT,size().width()*0.5+40+160-1,size().height()*0.45+80+MENU_HEIGHT);
        painter.drawLine(size().width()*0.5+40+160,size().height()*0.45+MENU_HEIGHT,size().width()*0.5+40+160,size().height()*0.45+80-1+MENU_HEIGHT);

        //按钮选择提示
        if (mousePosPvP&&(!mouseClickPvP))
        {
            painter.setBrush(QColor(190,190,190,255));
            painter.setPen(QColor(180,180,180,255));
            painter.drawRect(size().width()*0.5-200,size().height()*0.45+MENU_HEIGHT,160,80);
            painter.setPen(Qt::black);
            painter.setFont(QFont("Times New Roman",20,75));
            painter.drawText(size().width()*0.5-200+52,size().height()*0.45+47+MENU_HEIGHT,"P v P");
            painter.setPen(QPen(QColor(100,100,100,200),2));
            painter.drawLine(size().width()*0.5-200+1,size().height()*0.45+80+MENU_HEIGHT,size().width()*0.5-200+160-1,size().height()*0.45+80+MENU_HEIGHT);
            painter.drawLine(size().width()*0.5-200+160,size().height()*0.45+1+MENU_HEIGHT,size().width()*0.5-200+160,size().height()*0.45+80-1+MENU_HEIGHT);
            painter.setPen(QPen(QColor(100,100,100,200),1));
            painter.drawLine(size().width()*0.5-200,size().height()*0.45+MENU_HEIGHT,size().width()*0.5-200+160-1,size().height()*0.45+MENU_HEIGHT);
            painter.drawLine(size().width()*0.5-200,size().height()*0.45+MENU_HEIGHT,size().width()*0.5-200,size().height()*0.45+80-1+MENU_HEIGHT);
        }
        if (mousePosPvE&&(!mouseClickPvE))
        {
            painter.setBrush(QColor(190,190,190,255));
            painter.setPen(QColor(180,180,180,255));
            painter.drawRect(size().width()*0.5+40,size().height()*0.45+MENU_HEIGHT,160,80);
            painter.setPen(Qt::black);
            painter.setFont(QFont("Times New Roman",20,75));
            painter.drawText(size().width()*0.5+40+52,size().height()*0.45+47+MENU_HEIGHT,"P v E");
            painter.setPen(QPen(QColor(100,100,100,200),2));
            painter.drawLine(size().width()*0.5+40+1,size().height()*0.45+80+MENU_HEIGHT,size().width()*0.5+40+160-1,size().height()*0.45+80+MENU_HEIGHT);
            painter.drawLine(size().width()*0.5+40+160,size().height()*0.45+1+MENU_HEIGHT,size().width()*0.5+40+160,size().height()*0.45+80-1+MENU_HEIGHT);
            painter.setPen(QPen(QColor(100,100,100,200),1));
            painter.drawLine(size().width()*0.5+40,size().height()*0.45+MENU_HEIGHT,size().width()*0.5+40+160-1,size().height()*0.45+MENU_HEIGHT);
            painter.drawLine(size().width()*0.5+40,size().height()*0.45+MENU_HEIGHT,size().width()*0.5+40,size().height()*0.45+80-1+MENU_HEIGHT);
        }

        //按钮点击提示
        if (mousePosPvP&&mouseClickPvP)
        {
            painter.setBrush(QColor(200,200,200,255));
            painter.setPen(QColor(180,180,180,255));
            painter.drawRect(size().width()*0.5-200,size().height()*0.45+MENU_HEIGHT,160,80);
            painter.setPen(Qt::black);
            painter.setFont(QFont("Times New Roman",20,75));
            painter.drawText(size().width()*0.5-200+55,size().height()*0.45+50+MENU_HEIGHT,"P v P");
            painter.setPen(QPen(QColor(100,100,100,200),3));
            painter.drawLine(size().width()*0.5-200,size().height()*0.45+MENU_HEIGHT,size().width()*0.5-200+160-1,size().height()*0.45+MENU_HEIGHT);
            painter.drawLine(size().width()*0.5-200,size().height()*0.45+MENU_HEIGHT,size().width()*0.5-200,size().height()*0.45+80-1+MENU_HEIGHT);
        }
        if (mousePosPvE&&mouseClickPvE)
        {
            painter.setBrush(QColor(200,200,200,255));
            painter.setPen(QColor(180,180,180,255));
            painter.drawRect(size().width()*0.5+40,size().height()*0.45+MENU_HEIGHT,160,80);
            painter.setPen(Qt::black);
            painter.setFont(QFont("Times New Roman",20,75));
            painter.drawText(size().width()*0.5+40+55,size().height()*0.45+50+MENU_HEIGHT,"P v E");
            painter.setPen(QPen(QColor(100,100,100,200),3));
            painter.drawLine(size().width()*0.5+40,size().height()*0.45+MENU_HEIGHT,size().width()*0.5+40+160-1,size().height()*0.45+MENU_HEIGHT);
            painter.drawLine(size().width()*0.5+40,size().height()*0.45+MENU_HEIGHT,size().width()*0.5+40,size().height()*0.45+80-1+MENU_HEIGHT);
        }
    }

    //绘制鼠标选择提示标记
    if (game->gameStatus==PLAYING)
    {
    if (rightMousePos)
    {
        if (game->playerTurn==blackturn) painter.setPen(QPen(Qt::black,1.5));
            else painter.setPen(QPen(Qt::darkGray,1.5));
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MENU_HEIGHT,EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE+MARK_CORNER,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MENU_HEIGHT);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MENU_HEIGHT,EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MARK_CORNER+MENU_HEIGHT);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MENU_HEIGHT,EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE-MARK_CORNER,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MENU_HEIGHT);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MENU_HEIGHT,EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE-MARK_SIZE+MARK_CORNER+MENU_HEIGHT);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE+MENU_HEIGHT,EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE+MARK_CORNER,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE+MENU_HEIGHT);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE+MENU_HEIGHT,EDGE+mousePosCol*BLOCK_SIZE-MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE-MARK_CORNER+MENU_HEIGHT);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE+MENU_HEIGHT,EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE-MARK_CORNER,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE+MENU_HEIGHT);
        painter.drawLine(EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE+MENU_HEIGHT,EDGE+mousePosCol*BLOCK_SIZE+MARK_SIZE,EDGE+mousePosRow*BLOCK_SIZE+MARK_SIZE-MARK_CORNER+MENU_HEIGHT);
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
                y>size().height()*0.45+MENU_HEIGHT&&y<size().height()*0.45+80+MENU_HEIGHT)
            mousePosPvP=true;
        if (x>size().width()*0.5+40&&x<size().width()*0.5+40+160&&
                y>size().height()*0.45+MENU_HEIGHT&&y<size().height()*0.45+80+MENU_HEIGHT)
            mousePosPvE=true;
        update();
    }


    //落子模糊判定
    if (game->gameStatus==PLAYING)
    {
    if (x>EDGE-BLOCK_SIZE*0.5&&x<size().width()-EDGE+BLOCK_SIZE*0.5&&
            y>EDGE-BLOCK_SIZE*0.5+MENU_HEIGHT&&y<size().height()-EDGE+BLOCK_SIZE*0.5)
    {
        int col=(x-EDGE)/BLOCK_SIZE;
        int row=(y-EDGE-MENU_HEIGHT)/BLOCK_SIZE;
        rightMousePos=false;

        if (GameModel::len(x,y,EDGE+col*BLOCK_SIZE,EDGE+row*BLOCK_SIZE+MENU_HEIGHT)<=POS_OFFSET)
        {
            mousePosCol=col;
            mousePosRow=row;
            if (game->chessPiece[mousePosRow][mousePosCol]==0)
                rightMousePos=true;
        }

        if (GameModel::len(x,y,EDGE+(col+1)*BLOCK_SIZE,EDGE+row*BLOCK_SIZE+MENU_HEIGHT)<=POS_OFFSET)
        {
            mousePosCol=col+1;
            mousePosRow=row;
            if (game->chessPiece[mousePosRow][mousePosCol]==0)
                rightMousePos=true;
        }

        if (GameModel::len(x,y,EDGE+col*BLOCK_SIZE,EDGE+(row+1)*BLOCK_SIZE+MENU_HEIGHT)<=POS_OFFSET)
        {
            mousePosCol=col;
            mousePosRow=row+1;
            if (game->chessPiece[mousePosRow][mousePosCol]==0)
                rightMousePos=true;
        }

        if (GameModel::len(x,y,EDGE+(col+1)*BLOCK_SIZE,EDGE+(row+1)*BLOCK_SIZE+MENU_HEIGHT)<=POS_OFFSET)
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
    if (!game->AITurn) chessByPerson();

    //gg
    if (game->gameStatus==BLACKWIN||game->gameStatus==WHITEWIN||game->gameStatus==DRAW)
    {
        PlaySound(TEXT("Resources/end.wav"),NULL,SND_FILENAME | SND_ASYNC);//音频
        if (game->gameStatus==DRAW) game->gameWindows->msg_Draw();
            else game->gameWindows->msg_End(game->gameStatus,game->gameMode,game->AITurn);
        initGame();
    }

    rightMousePos=false;

    //换手
    if (game->playerTurn==blackturn) game->playerTurn=whiteturn;
        else game->playerTurn=blackturn;
    if (game->gameMode==PvE){
        game->AITurn=true;
        chessByAI();
        //gg
        if (game->gameStatus==BLACKWIN||game->gameStatus==WHITEWIN||game->gameStatus==DRAW)
        {
            PlaySound(TEXT("Resources/end.wav"),NULL,SND_FILENAME | SND_ASYNC);//音频
            if (game->gameStatus==DRAW) game->gameWindows->msg_Draw();
                else game->gameWindows->msg_End(game->gameStatus,game->gameMode,game->AITurn);
            initGame();
        }
        if (game->playerTurn==blackturn) game->playerTurn=whiteturn;
            else game->playerTurn=blackturn;
        game->AITurn=false;
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
    PlaySound(TEXT("Resources/click-botton.wav"),NULL,SND_FILENAME | SND_ASYNC);//音频
    mousePosPvP=false;
    game_mode=PvP;
    game->gameStatus=PLAYING;
    game->AITurn=false;
    game->startGame(game_mode);
    update();
}

void Widget::initPvEGame()//pve未启用
{
    PlaySound(TEXT("Resources/click-botton.wav"),NULL,SND_FILENAME | SND_ASYNC);//音频
    mousePosPvE=false;
    game_mode=PvE;
    game->gameStatus=PLAYING;
    game->AITurn=false;
    if (rand()%2) game->AITurn=true;
    game->startGame(game_mode);
    update();
    if (game->AITurn){
        chessByAI();
        game->AITurn=false;
        game->playerTurn=whiteturn;
    }
}

void Widget::chessByPerson()
{
    if (rightMousePos&&mousePosRow>=0&&mousePosRow<LINE_NUM
            &&mousePosCol>=0&&mousePosCol<LINE_NUM)
        game->move_in_chess(mousePosRow,mousePosCol);
    update();//画面
    PlaySound(TEXT("Resources/move-in-chess.wav"),NULL,SND_FILENAME | SND_ASYNC);//音频
}

void Widget::chessByAI()
{
    int aiChessRow=-1,aiChessCol=-1;
    game->AIchess(&aiChessRow,&aiChessCol);
    game->move_in_chess(aiChessRow,aiChessCol);
    update();//画面
    PlaySound(TEXT("Resources/move-in-chess.wav"),NULL,SND_FILENAME | SND_ASYNC);//音频
}

bool Widget::initMenuBar()
{
    bool ret=true;
    QMenuBar *mb=new QMenuBar(this);
    ret=ret&&initGameMenu(mb);
    ret=ret&&initActionMenu(mb);
    return ret;
}

bool Widget::initGameMenu(QMenuBar *mb)
{
    QMenu *menu=new QMenu("游戏(&G)");
    bool ret=(menu!=NULL);
        if (ret)
        {
            QAction *action=NULL;
            ret=ret&&makeAction(action,"新游戏(&N)",Qt::CTRL+Qt::Key_N);
            if (ret)
            {
                connect(action,&QAction::triggered,this,&Widget::newGameAction);
                menu->addAction(action);    // add Action item to Menu
            }
            menu->addSeparator();
            ret=ret&&makeAction(action,"保存游戏(&S)",Qt::CTRL+Qt::Key_S);
            if (ret)
            {
                connect(action,&QAction::triggered,this,&Widget::saveGameAction);
                menu->addAction(action);    // add Action item to Menu
            }
            ret=ret&&makeAction(action,"载入游戏(&L)",Qt::CTRL+Qt::Key_L);
            if (ret)
            {
                connect(action,&QAction::triggered,this,&Widget::loadGameAction);
                menu->addAction(action);    // add Action item to Menu
            }
        }
        if (ret)
        {
            mb->addMenu(menu);    // add Menu add to application Menu Bar
        }
        else {
            delete menu;
        }
        return ret;
}

bool Widget::makeAction(QAction *&action,QString text,int key)
{
    bool ret=true;
    action=new QAction(text,NULL);
    if (action!=NULL)
    {
        action->setShortcut(QKeySequence(key));
    }
    else
    {
        ret=false;
    }
    return ret;
}

bool Widget::initActionMenu(QMenuBar* mb)
{
    QMenu *menu=new QMenu("操作(&A)");
    bool ret=(menu!=NULL);
    if (ret)
    {
        QAction *action=NULL;
        ret=ret&&makeAction(action,"悔棋(&U)",Qt::CTRL+Qt::Key_Z);
        if(ret)
        {
            connect(action,&QAction::triggered,this,&Widget::undoAction);
            menu->addAction(action);
        }
        ret=ret&&makeAction(action,"提示(&T)",Qt::CTRL+Qt::Key_T);
        if (ret)
        {
            connect(action,&QAction::triggered,this,&Widget::tipAction);
            menu->addAction(action);
        }
    }
    if (ret)
    {
        mb->addMenu(menu);
    }
    else
    {
        delete menu;
    }
    return ret;
}

void Widget::newGameAction()
{
    initGame();
}

void Widget::saveGameAction()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptSave);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("保存游戏"));
    //设置默认文件路径
    fileDialog->setDirectory("./Saves");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Save(*.sav)"));
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);

    string path;
    if (fileDialog->exec())
    {
        QString filename=fileDialog->selectedFiles()[0];
        path=filename.toStdString();
    }

    if (path=="") return;
    ofstream outfile(path,ios::out | ios::trunc);
    outfile<<game->gameMode<<endl;
    outfile<<game->gameStatus<<endl;
    outfile<<game->playerTurn<<endl;
    outfile<<game->AITurn<<endl;
    outfile<<game->chessNum<<endl;
    for (int i=0;i<LINE_NUM;i++)
    {
        for (int j=0;j<LINE_NUM-1;j++)
            outfile<<game->chessPiece[i][j]<<' ';
        outfile<<game->chessPiece[i][LINE_NUM-1]<<endl;
    }
    outfile.close();
}

void Widget::loadGameAction()
{
    //定义文件对话框类
    QFileDialog *fileDialog = new QFileDialog(this);
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);
    //定义文件对话框标题
    fileDialog->setWindowTitle(tr("载入游戏"));
    //设置默认文件路径
    fileDialog->setDirectory("./Saves");
    //设置文件过滤器
    fileDialog->setNameFilter(tr("Save(*.sav)"));
    //设置视图模式
    fileDialog->setViewMode(QFileDialog::Detail);

    string path="";
    if (fileDialog->exec())
    {
        QString filename=fileDialog->selectedFiles()[0];
        path=filename.toStdString();
    }

        if (path=="") return;
        ifstream infile(path,ios::in);
        if (!infile) {
            QMessageBox::warning(this,tr("错误"),tr("打开文件失败"));
            return;
        }
        int temp=0;
        if (!(infile>>temp)) return;
        switch (temp)
        {
        case 0 : game->gameMode=unknownMode; break;
        case 1 : game->gameMode=PvP; break;
        case 2 : game->gameMode=PvE; break;
        default: game->gameMode=unknownMode; break;
        }
        infile>>temp;
        switch (temp)
        {
        case 0 : game->gameStatus=unknownStatus; break;
        case 1 : game->gameStatus=PLAYING; break;
        case 2 : game->gameStatus=BLACKWIN; break;
        case 3 : game->gameStatus=WHITEWIN; break;
        case 4 : game->gameStatus=DRAW; break;
        default: game->gameStatus=unknownStatus; break;
        }
        infile>>temp;
        switch (temp)
        {
        case 0 : game->playerTurn=blackturn; break;
        case 1 : game->playerTurn=whiteturn; break;
        default: game->playerTurn=blackturn; break;
        }
        infile>>game->AITurn;
        infile>>game->chessNum;
        for (int i=0;i<LINE_NUM;i++)
        {
            for (int j=0;j<LINE_NUM;j++)
            {
                infile>>temp;
                switch (temp)
                {
                case 0 : game->chessPiece[i][j]=null; break;
                case 1 : game->chessPiece[i][j]=black; break;
                case 2 : game->chessPiece[i][j]=white; break;
                default: game->chessPiece[i][j]=null; break;
                }
            }
        }
        infile.close();
}

void Widget::undoAction()
{
    if (game->gameStatus!=PLAYING)
    {
        QMessageBox::warning(this,tr("错误"),tr("无法悔棋"));
        return;
    }
    if (game->chessNum==game->lastNum||(game->gameMode==PvP&&game->chessNum==0)||(game->gameMode==PvE&&game->chessNum<=1))
    {
        QMessageBox::warning(this,tr("错误"),tr("无法继续悔棋"));
        return;
    }
    if (game->gameMode==PvP)
    {
        if (game->playerTurn==blackturn) game->playerTurn=whiteturn;
            else game->playerTurn=blackturn;
        game->chessNum=game->lastNum;
        for (int i=0;i<LINE_NUM;i++)
        for (int j=0;j<LINE_NUM;j++)
            game->chessPiece[i][j]=game->lastPiece[i][j];
        update();
        return;
    }
    if (game->gameMode==PvE)
    {
        game->chessNum=game->lastNum;
        for (int i=0;i<LINE_NUM;i++)
        for (int j=0;j<LINE_NUM;j++)
            game->chessPiece[i][j]=game->lastPiece[i][j];
        update();
        return;
    }
}

void Widget::tipAction()
{
    //
}
