#ifndef GAMEWINDOWS_H

#include "BaseParameter.h"

#include <QMessageBox>

#define GAMEWINDOWS_H

class GameWindows
{
    public:
    GameWindows(){}

    //弹窗
    void msg_Draw();
    void msg_End(GameStatus status,GameMode mode,bool aiturn);
};

#endif // GAMEWINDOWS_H
