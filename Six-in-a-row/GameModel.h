#ifndef GAMEMODEL_H
#define GAMEMODEL_H

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
    WIN,
    LOSE
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

const int AI_THINK_TIME=1000;                   //ai模拟思考时间

//gamemodel
class GameModel
{
    public:
        GameModel(){};

        //游戏模式
        GameMode gameMode;

        //游戏状态
        GameStatus gameStatus;

        //下棋方
        PlayerTurn playerTurn;

        //棋盘落子情况
        ChessPiece chessPiece[LINE_NUM-1][LINE_NUM-1];

        //<ai>轮到ai
        bool AITurn;

        //<ai>落子权值
        int chessValue[LINE_NUM-1][LINE_NUM-1];

        //计算两点间距离
        static double len(int x1,int y1,int x2,int y2);

        //启动游戏
        void startGame(GameMode mode);

        //落子
        void move_in_chess(int row,int col);
};

#endif // GAMEMODEL_H
