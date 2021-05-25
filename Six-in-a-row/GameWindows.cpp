#include <GameWindows.h>

#include <QMessageBox>
#include <QPixmap>
#include <QIcon>

void GameWindows::msg_End(GameStatus status, GameMode mode, bool aiturn)
{
    QMessageBox msgBox;
    if (status==BLACKWIN){
        QPixmap icon("Recouses/blackchess.png");
        msgBox.setIconPixmap(icon);
    }
    else {
        QPixmap icon("Recouses/whitechess.png");
        msgBox.setIconPixmap(icon);
    }
    if (mode==PvP){
        if (status==BLACKWIN) msgBox.setText("Black Win!");
        if (status==WHITEWIN) msgBox.setText("White Win!");
        msgBox.setButtonText(QMessageBox::Ok,"好耶！");
        msgBox.setWindowTitle("提示");
    }
    else {
        if (aiturn){
            msgBox.setText("You Lose!");
            msgBox.setButtonText(QMessageBox::Ok,"可惜");
            msgBox.setWindowTitle("失败");
        }
        else {
            msgBox.setText("You Win!");
            msgBox.setButtonText(QMessageBox::Ok,"好耶！");
            msgBox.setWindowTitle("胜利");
        }
    }
    msgBox.exec();
}

void GameWindows::msg_Draw()
{
    QMessageBox msgBox;
    msgBox.setText("Draw!");
    msgBox.setButtonText(QMessageBox::Ok,"好耶！");
    msgBox.setWindowTitle("和局");
    QPixmap icon("Recouses/icon.png");//未加入图标
    msgBox.setIconPixmap(icon);
    msgBox.exec();
}
