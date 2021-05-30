#include "GameModel.h"

#include <math.h>
#include <cstdlib>

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
    if (!(row>=0&&row<LINE_NUM&&col>=0&&col<LINE_NUM)) return;
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
        if (row+i<0||row+i+5>=LINE_NUM) continue;
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
        if (col+i<0||col+i+5>=LINE_NUM) continue;
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
        if (row+i<0||row+i+5>=LINE_NUM) continue;
        if (col+i<0||col+i+5>=LINE_NUM) continue;
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
        if (row+i<0||row+i+5>=LINE_NUM) continue;
        if (col-i-5<0||col-i>=LINE_NUM) continue;
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
    if (chessNum==0){
        *row=9;
        *col=9;
        return;
    }
    getvalue();

    int max=-1;
    int num=0;
    int maxrow[200]={0},maxcol[200]={0};
    for (int i=0;i<LINE_NUM;i++)
    for (int j=0;j<LINE_NUM;j++)
    {
        if (chessValue[i][j]==max){
            maxrow[num]=i;
            maxcol[num]=j;
            num++;
            continue;
        }
        if (chessValue[i][j]>max){
            max=chessValue[i][j];
            maxrow[0]=i;
            maxcol[0]=j;
            num=1;
        }
    }
    srand(time(0));
    num=rand()%num;
    *row=maxrow[num];
    *col=maxcol[num];
}

void GameModel::getvalue()
{
    for (int i=0;i<LINE_NUM;i++)
    for (int j=0;j<LINE_NUM;j++)
    {
        if (chessPiece[i][j]==null)
        {
            int tempvalue=0;
            ChessPiece present=null,opposite=null;
            if (playerTurn==blackturn) present=black;
                else present=white;
            if (present==black) opposite=white;
                else opposite=black;

            {
            int l=1,r=1;
            for (;l<6;l++)
                if (i-l<0||chessPiece[i-l][j]==opposite) break;
            l--;
            for (;r<6;r++)
                if (i+r==LINE_NUM||chessPiece[i+r][j]==opposite) break;
            r--;
            if (l+r+1<6) tempvalue+=0;
            else {
                int k,h;
                for (k=i-1;k>=i-l;k--)
                    if (chessPiece[k][j]==null) break;
                k++;
                for (h=i+1;h<=i+r;h++)
                    if (chessPiece[h][j]==null) break;
                h--;
                if (h-k+1>=6) tempvalue+=1919810;
                if (h-k+1==5&&k>i-l&&h<i+r) tempvalue+=1919810;
                if (h-k+1==5&&(k==i-l||h==i+r)) tempvalue+=10086;
                if (h-k+1==4){
                    if (k==i-l||h==i+r) tempvalue+=10;
                    else if (k-1>i-l||h+1<i+r) tempvalue+=500;
                        else tempvalue+=200;
                }
                if (h-k+1==3){
                    if (k==i-l||h==i+r) tempvalue+=4;
                    else if ((k-2>i-l&&chessPiece[k-3][j]==present&&chessPiece[k-2][j]==present)||
                             (h+2<i+r&&chessPiece[h+3][j]==present&&chessPiece[h+2][j]==present))
                        tempvalue+=190;
                        else tempvalue+=15;
                }
                if (h-k+1==2){
                    if (k==i-l||h==i+r) tempvalue+=1;
                    else if ((k-3>i-l&&chessPiece[k-4][j]==present&&chessPiece[k-3][j]==present&&chessPiece[k-2][j]==present)||
                             (h+3<i+r&&chessPiece[h+4][j]==present&&chessPiece[h+3][j]==present&&chessPiece[h+2][j]==present))
                        tempvalue+=90;
                        else tempvalue+=2;
                }
            }
            }
            {
            int l=1,r=1;
            for (;l<6;l++)
                if (i-l<0||chessPiece[i-l][j]==present) break;
            l--;
            for (;r<6;r++)
                if (i+r==LINE_NUM||chessPiece[i+r][j]==present) break;
            r--;
            if (l+r+1<6) tempvalue+=0;
            else {
                int k,h;
                for (k=i-1;k>=i-l;k--)
                    if (chessPiece[k][j]==null) break;
                k++;
                for (h=i+1;h<=i+r;h++)
                    if (chessPiece[h][j]==null) break;
                h--;
                if (h-k+1>=6) tempvalue+=114514;
                if (h-k+1==5&&k>i-l&&h<i+r) tempvalue+=114514;
                if (h-k+1==5&&(k==i-l||h==i+r)) tempvalue+=10000;
                if (h-k+1==4){
                    if (k==i-l||h==i+r) tempvalue+=20;
                    else if (k-1>i-l||h+1<i+r) tempvalue+=1000;
                        else tempvalue+=180;
                }
                if (h-k+1==3){
                    if (k==i-l||h==i+r) tempvalue+=2;
                    else if ((k-2>i-l&&chessPiece[k-3][j]==opposite&&chessPiece[k-2][j]==opposite)||
                             (h+2<i+r&&chessPiece[h+3][j]==opposite&&chessPiece[h+2][j]==opposite))
                        tempvalue+=200;
                        else tempvalue+=10;
                }
                if (h-k+1==2){
                    if (k==i-l||h==i+r) tempvalue+=0;
                    else if ((k-3>i-l&&chessPiece[k-4][j]==opposite&&chessPiece[k-3][j]==opposite&&chessPiece[k-2][j]==opposite)||
                             (h+3<i+r&&chessPiece[h+4][j]==opposite&&chessPiece[h+3][j]==opposite&&chessPiece[h+2][j]==opposite))
                        tempvalue+=50;
                        else tempvalue+=1;
                }
            }
            }

            {
            int l=1,r=1;
            for (;l<6;l++)
                if (j-l<0||chessPiece[i][j-l]==opposite) break;
            l--;
            for (;r<6;r++)
                if (j+r==LINE_NUM||chessPiece[i][j+r]==opposite) break;
            r--;
            if (l+r+1<6) tempvalue+=0;
            else {
                int k,h;
                for (k=j-1;k>=j-l;k--)
                    if (chessPiece[i][k]==null) break;
                k++;
                for (h=j+1;h<=j+r;h++)
                    if (chessPiece[i][h]==null) break;
                h--;
                if (h-k+1>=6) tempvalue+=1919810;
                if (h-k+1==5&&k>j-l&&h<j+r) tempvalue+=1919810;
                if (h-k+1==5&&(k==j-l||h==j+r)) tempvalue+=10086;
                if (h-k+1==4){
                    if (k==j-l||h==j+r) tempvalue+=10;
                    else if (k-1>j-l||h+1<j+r) tempvalue+=500;
                        else tempvalue+=200;
                }
                if (h-k+1==3){
                    if (k==j-l||h==j+r) tempvalue+=4;
                    else if ((k-2>j-l&&chessPiece[i][k-3]==present&&chessPiece[i][k-2]==present)||
                             (h+2<j+r&&chessPiece[i][h+3]==present&&chessPiece[i][h+2]==present))
                        tempvalue+=190;
                        else tempvalue+=15;
                }
                if (h-k+1==2){
                    if (k==j-l||h==j+r) tempvalue+=1;
                    else if ((k-3>j-l&&chessPiece[i][k-4]==present&&chessPiece[i][k-3]==present&&chessPiece[i][k-2]==present)||
                             (h+3<j+r&&chessPiece[i][h+4]==present&&chessPiece[i][h+3]==present&&chessPiece[i][h+2]==present))
                        tempvalue+=90;
                        else tempvalue+=2;
                }
            }
            }
            {
            int l=1,r=1;
            for (;l<6;l++)
                if (j-l<0||chessPiece[i][j-l]==present) break;
            l--;
            for (;r<6;r++)
                if (j+r==LINE_NUM||chessPiece[i][j+r]==present) break;
            r--;
            if (l+r+1<6) tempvalue+=0;
            else {
                int k,h;
                for (k=j-1;k>=j-l;k--)
                    if (chessPiece[i][k]==null) break;
                k++;
                for (h=j+1;h<=j+r;h++)
                    if (chessPiece[i][h]==null) break;
                h--;
                if (h-k+1>=6) tempvalue+=114514;
                if (h-k+1==5&&k>j-l&&h<j+r) tempvalue+=114514;
                if (h-k+1==5&&(k==j-l||h==j+r)) tempvalue+=10000;
                if (h-k+1==4){
                    if (k==j-l||h==j+r) tempvalue+=20;
                    else if (k-1>j-l||h+1<j+r) tempvalue+=1000;
                        else tempvalue+=180;
                }
                if (h-k+1==3){
                    if (k==j-l||h==j+r) tempvalue+=2;
                    else if ((k-2>j-l&&chessPiece[i][k-3]==opposite&&chessPiece[i][k-2]==opposite)||
                             (h+2<j+r&&chessPiece[i][h+3]==opposite&&chessPiece[i][h+2]==opposite))
                        tempvalue+=200;
                        else tempvalue+=10;
                }
                if (h-k+1==2){
                    if (k==j-l||h==j+r) tempvalue+=0;
                    else if ((k-3>j-l&&chessPiece[i][k-4]==opposite&&chessPiece[i][k-3]==opposite&&chessPiece[i][k-2]==opposite)||
                             (h+3<j+r&&chessPiece[i][h+4]==opposite&&chessPiece[i][h+3]==opposite&&chessPiece[i][h+2]==opposite))
                        tempvalue+=50;
                        else tempvalue+=1;
                }
            }
            }

            {
            int l=1,r=1;
            for (;l<6;l++)
                if (i-l<0||j-l<0||chessPiece[i-l][j-l]==opposite) break;
            l--;
            for (;r<6;r++)
                if (i+r==LINE_NUM||j+r==LINE_NUM||chessPiece[i+r][j+r]==opposite) break;
            r--;
            if (l+r+1<6) tempvalue+=0;
            else {
                int k,h;
                for (k=i-1;k>=i-l;k--)
                    if (chessPiece[k][j-i+k]==null) break;
                k++;
                for (h=i+1;h<=i+r;h++)
                    if (chessPiece[h][j-i+h]==null) break;
                h--;
                if (h-k+1>=6) tempvalue+=1919810;
                if (h-k+1==5&&k>i-l&&h<i+r) tempvalue+=1919810;
                if (h-k+1==5&&(k==i-l||h==i+r)) tempvalue+=10086;
                if (h-k+1==4){
                    if (k==i-l||h==i+r) tempvalue+=10;
                    else if (k-1>i-l||h+1<i+r) tempvalue+=500;
                        else tempvalue+=200;
                }
                if (h-k+1==3){
                    if (k==i-l||h==i+r) tempvalue+=4;
                    else if ((k-2>i-l&&chessPiece[k-3][j-i+k-3]==present&&chessPiece[k-2][j-i+k-2]==present)||
                             (h+2<i+r&&chessPiece[h+3][j-i+h+3]==present&&chessPiece[h+2][j-i+h+2]==present))
                        tempvalue+=190;
                        else tempvalue+=15;
                }
                if (h-k+1==2){
                    if (k==i-l||h==i+r) tempvalue+=1;
                    else if ((k-3>i-l&&chessPiece[k-4][j-i+k-4]==present&&chessPiece[k-3][j-i+k-3]==present&&chessPiece[k-2][j-i+k-2]==present)||
                             (h+3<i+r&&chessPiece[h+4][j-i+h+4]==present&&chessPiece[h+3][j-i+h+3]==present&&chessPiece[h+2][j-i+h+2]==present))
                        tempvalue+=90;
                        else tempvalue+=2;
                }
            }
            }
            {
            int l=1,r=1;
            for (;l<6;l++)
                if (i-l<0||j-l<0||chessPiece[i-l][j-l]==present) break;
            l--;
            for (;r<6;r++)
                if (i+r==LINE_NUM||j+r==LINE_NUM||chessPiece[i+r][j+r]==present) break;
            r--;
            if (l+r+1<6) tempvalue+=0;
            else {
                int k,h;
                for (k=i-1;k>=i-l;k--)
                    if (chessPiece[k][j-i+k]==null) break;
                k++;
                for (h=i+1;h<=i+r;h++)
                    if (chessPiece[h][j-i+h]==null) break;
                h--;
                if (h-k+1>=6) tempvalue+=114514;
                if (h-k+1==5&&k>i-l&&h<i+r) tempvalue+=114514;
                if (h-k+1==5&&(k==i-l||h==i+r)) tempvalue+=10000;
                if (h-k+1==4){
                    if (k==i-l||h==i+r) tempvalue+=20;
                    else if (k-1>i-l||h+1<i+r) tempvalue+=1000;
                        else tempvalue+=180;
                }
                if (h-k+1==3){
                    if (k==i-l||h==i+r) tempvalue+=2;
                    else if ((k-2>i-l&&chessPiece[k-3][j-i+k-3]==opposite&&chessPiece[k-2][j-i+k-2]==opposite)||
                             (h+2<i+r&&chessPiece[h+3][j-i+h+3]==opposite&&chessPiece[h+2][j-i+h+2]==opposite))
                        tempvalue+=200;
                        else tempvalue+=10;
                }
                if (h-k+1==2){
                    if (k==i-l||h==i+r) tempvalue+=0;
                    else if ((k-3>i-l&&chessPiece[k-4][j-i+k-4]==opposite&&chessPiece[k-3][j-i+k-3]==opposite&&chessPiece[k-2][j-i+k-2]==opposite)||
                             (h+3<i+r&&chessPiece[h+4][j-i+h+4]==opposite&&chessPiece[h+3][j-i+h+3]==opposite&&chessPiece[h+2][j-i+h+2]==opposite))
                        tempvalue+=50;
                        else tempvalue+=1;
                }
            }
            }

            {
            int l=1,r=1;
            for (;l<6;l++)
                if (i-l<0||j+l==LINE_NUM||chessPiece[i-l][j+l]==opposite) break;
            l--;
            for (;r<6;r++)
                if (i+r==LINE_NUM||j-r>0||chessPiece[i+r][j-r]==opposite) break;
            r--;
            if (l+r+1<6) tempvalue+=0;
            else {
                int k,h;
                for (k=i-1;k>=i-l;k--)
                    if (chessPiece[k][j+i-k]==null) break;
                k++;
                for (h=i+1;h<=i+r;h++)
                    if (chessPiece[h][j+i-h]==null) break;
                h--;
                if (h-k+1>=6) tempvalue+=1919810;
                if (h-k+1==5&&k>i-l&&h<i+r) tempvalue+=1919810;
                if (h-k+1==5&&(k==i-l||h==i+r)) tempvalue+=10086;
                if (h-k+1==4){
                    if (k==i-l||h==i+r) tempvalue+=10;
                    else if (k-1>i-l||h+1<i+r) tempvalue+=500;
                        else tempvalue+=200;
                }
                if (h-k+1==3){
                    if (k==i-l||h==i+r) tempvalue+=4;
                    else if ((k-2>i-l&&chessPiece[k-3][j+i-k+3]==present&&chessPiece[k-2][j+i-k+2]==present)||
                             (h+2<i+r&&chessPiece[h+3][j+i-h-3]==present&&chessPiece[h+2][j+i-h-2]==present))
                        tempvalue+=190;
                        else tempvalue+=15;
                }
                if (h-k+1==2){
                    if (k==i-l||h==i+r) tempvalue+=1;
                    else if ((k-3>i-l&&chessPiece[k-4][j+i-k+4]==present&&chessPiece[k-3][j+i-k+3]==present&&chessPiece[k-2][j+i-k+2]==present)||
                             (h+3<i+r&&chessPiece[h+4][j+i-h-4]==present&&chessPiece[h+3][j+i-h-3]==present&&chessPiece[h+2][j+i-h-2]==present))
                        tempvalue+=90;
                        else tempvalue+=2;
                }
            }
            }
            {
            int l=1,r=1;
            for (;l<6;l++)
                if (i-l<0||j+l==LINE_NUM||chessPiece[i-l][j+l]==present) break;
            l--;
            for (;r<6;r++)
                if (i+r==LINE_NUM||j-r<0||chessPiece[i+r][j-r]==present) break;
            r--;
            if (l+r+1<6) tempvalue+=0;
            else {
                int k,h;
                for (k=i-1;k>=i-l;k--)
                    if (chessPiece[k][j+i-k]==null) break;
                k++;
                for (h=i+1;h<=i+r;h++)
                    if (chessPiece[h][j+i-h]==null) break;
                h--;
                if (h-k+1>=6) tempvalue+=114514;
                if (h-k+1==5&&k>i-l&&h<i+r) tempvalue+=114514;
                if (h-k+1==5&&(k==i-l||h==i+r)) tempvalue+=10000;
                if (h-k+1==4){
                    if (k==i-l||h==i+r) tempvalue+=20;
                    else if (k-1>i-l||h+1<i+r) tempvalue+=1000;
                        else tempvalue+=180;
                }
                if (h-k+1==3){
                    if (k==i-l||h==i+r) tempvalue+=2;
                    else if ((k-2>i-l&&chessPiece[k-3][j+i-k+3]==opposite&&chessPiece[k-2][j+i-k+2]==opposite)||
                             (h+2<i+r&&chessPiece[h+3][j+i-h-3]==opposite&&chessPiece[h+2][j+i-h-2]==opposite))
                        tempvalue+=200;
                        else tempvalue+=10;
                }
                if (h-k+1==2){
                    if (k==i-l||h==i+r) tempvalue+=0;
                    else if ((k-3>i-l&&chessPiece[k-4][j+i-k+4]==opposite&&chessPiece[k-3][j+i-k+3]==opposite&&chessPiece[k-2][j+i-k+2]==opposite)||
                             (h+3<i+r&&chessPiece[h+4][j+i-h-4]==opposite&&chessPiece[h+3][j+i-h-3]==opposite&&chessPiece[h+2][j+i-h-2]==opposite))
                        tempvalue+=50;
                        else tempvalue+=1;
                }
            }
            }

            chessValue[i][j]=tempvalue;
        }
        else chessValue[i][j]=-1;
    }
}
