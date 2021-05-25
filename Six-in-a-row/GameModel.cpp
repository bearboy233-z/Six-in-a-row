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
    if (playerTurn==blackturn) chessPiece[row][col]=black;
    if (playerTurn==whiteturn) chessPiece[row][col]=white;
}
