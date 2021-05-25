#include "GameModel.h"

#include <math.h>

double GameModel::len(int x1,int y1,int x2,int y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

void GameModel::startGame(GameMode mode)
{
    gameMode=mode;
    playerTurn=blackturn;

    //初始化棋盘
    for (int i=0;i<LINE_NUM;i++)
    for (int j=0;j<LINE_NUM;j++)
    {
        chessPiece[i][j]=null;
        if (gameMode==PvE) chessValue[i][j]=0;
    }
}

void GameModel::move_in_chess(int row, int col)
{
    chessNum++;
    if (playerTurn==blackturn) chessPiece[row][col]=black;
    if (playerTurn==whiteturn) chessPiece[row][col]=white;
    judge(row,col);
}

void GameModel::judge(int row, int col)
{
    bool win=false;
    for (int i=-5;i<=0;i++)
    {
        bool cont=true;
        for (int j=0;j<6;j++)
            if (chessPiece[row+i+j][col]!=chessPiece[row][col]){
                cont=false;
                break;
            }
        if (cont){
            win=true;
            break;
        }
    }
    for (int i=-5;i<=0;i++)
    {
        bool cont=true;
        for (int j=0;j<6;j++)
            if (chessPiece[row][col+i+j]!=chessPiece[row][col]){
                cont=false;
                break;
            }
        if (cont){
            win=true;
            break;
        }
    }
    for (int i=-5;i<=0;i++)
    {
        bool cont=true;
        for (int j=0;j<6;j++)
            if (chessPiece[row+i+j][col+i+j]!=chessPiece[row][col]){
                cont=false;
                break;
            }
        if (cont){
            win=true;
            break;
        }
    }
    for (int i=-5;i<=0;i++)
    {
        bool cont=true;
        for (int j=0;j<6;j++)
            if (chessPiece[row+i+j][col-i-j]!=chessPiece[row][col]){
                cont=false;
                break;
            }
        if (cont){
            win=true;
            break;
        }
    }

    //胜利
    if (win)
    {
        if (chessPiece[row][col]==black) gameStatus=BLACKWIN;
        if (chessPiece[row][col]==white) gameStatus=WHITEWIN;
        return;
    }

    //和局
    if ((!win)&&chessNum==LINE_NUM*LINE_NUM){
        gameStatus=DRAW;
        return;
    }
}

void GameModel::AIchess(int *row, int *col)
{
    //待完成
}
