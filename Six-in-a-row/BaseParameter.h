#ifndef BASEPARAMETER_H
#define BASEPARAMETER_H

//游戏类型
enum GameMode
{
    unknownMode,
    PvP,
    PvE
};

//游戏状态
enum GameStatus
{
    unknownStatus,
    PLAYING,
    BLACKWIN,
    WHITEWIN,
    DRAW
};

//下棋方
enum PlayerTurn
{
    blackturn,
    whiteturn
};

//落子情况
enum ChessPiece
{
    null,
    black,
    white
};

//棋盘参数
const int BLOCK_NUM=18;                         //棋盘格子数
const int LINE_NUM=19;                          //棋盘线条数
const int EDGE=30;                              //棋盘边界宽度
const int BLOCK_SIZE=40;                        //棋盘格子大小
const int CHESS_RADIUS=15;                      //棋子半径
const int POS_OFFSET=BLOCK_SIZE*0.35;           //鼠标点击模糊判定上限
const int MARK_SIZE=BLOCK_SIZE*0.4;             //鼠标点击提示标记大小
const int MARK_CORNER=MARK_SIZE/4;              //鼠标点击提示角标大小
const int MENU_HEIGHT=19;                       //菜单栏高度

#endif // BASEPARAMETER_H
