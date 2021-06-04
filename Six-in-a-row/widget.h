#ifndef WIDGET_H

#include "GameModel.h"

#include <QWidget>
#include <QMessageBox>
#include <QKeySequence>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QString>

#define WIDGET_H

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;

    GameModel *game;
    GameMode game_mode=unknownMode;

    //鼠标
    int mousePosRow=-1,mousePosCol=-1;//鼠标所在行列
    int aiChessRow=-1,aiChessCol=-1;//AI下棋所在行列
    int tipChessRow=-1,tipChessCol=-1;//提示下棋所在行列
    bool rightMousePos=false;//是否是合法的选中位置
    bool mousePosPvP=false,mousePosPvE=false;//鼠标在按钮上
    bool mouseClickPvP=false,mouseClickPvE=false;//鼠标点击按钮
    bool iftip=false;//需要提示

    //绘制
    void paintEvent(QPaintEvent *event);

    //鼠标位置
    void mouseMoveEvent(QMouseEvent *event);

    //鼠标点击
    void mouseReleaseEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    //落子
    void chessByPerson();
    void chessByAI();

    //初始化
    void initGame();
    void initPvPGame();
    void initPvEGame();

    //菜单栏
    bool initMenuBar();
    bool initGameMenu(QMenuBar* mb);
    bool initActionMenu(QMenuBar* mb);
    bool makeAction(QAction *&action,QString text,int key);

    //槽函数
private slots:
    //菜单栏动作
    void newGameAction();
    void saveGameAction();
    void loadGameAction();
    void undoAction();
    void tipAction();
};
#endif // WIDGET_H
