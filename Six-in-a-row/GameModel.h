#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "BaseParameter.h"
#include "GameWindows.h"

//gamemodel
class GameModel
{
    public:
        GameModel(){}

        //弹窗
        GameWindows *gameWindows;

        //游戏模式
        GameMode gameMode=unknownMode;

        //游戏状态
        GameStatus gameStatus=unknownStatus;

        //下棋方
        PlayerTurn playerTurn=blackturn;

        //棋盘落子情况
        ChessPiece chessPiece[LINE_NUM][LINE_NUM]={null};

        //<ai>轮到ai
        bool AITurn=false;

        //<ai>落子权值
        int chessValue[LINE_NUM][LINE_NUM]={0};

        //落子总数
        int chessNum=0;

        //启动游戏
        void startGame(GameMode mode);

        //落子
        void move_in_chess(int row,int col);

        //判断输赢
        void judge(int row,int col);

        //<ai>ai下棋
        void AIchess(int *row,int *col);
        void getvalue();

        //计算两点间距离
        static double len(int x1,int y1,int x2,int y2);
};

#endif // GAMEMODEL_H
