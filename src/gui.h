#ifndef GUI_H
#define GUI_H

#include "player.h"
#include "saveGame.h"
#include "loadFile.h"

#include <QMainWindow>
#include <QFrame>
#include <QLabel>
#include <QMouseEvent>
#include <QStatusBar>
#include <QMessageBox>
#include <QMenuBar>
#include <QInputDialog>
#include <iostream>
#include <QDir>
#include <QDialog>
#include <QPushButton>
 #include <QTextEdit>
 #include <QLineEdit>

class DeskBox;

class MainWindow: public QMainWindow
{
    Q_OBJECT

public: //methods
    MainWindow(QWidget* = nullptr);
    ~MainWindow();

private:
    void createActions();
    void createMenus();
    void redrawFigures();
    bool figClick(Position*);
    void posClick(Position*);
    void switchTurn();

protected:
    void mousePressEvent(QMouseEvent* event);

private slots:
    void newGame();
    void saveMyGameXml();
    void saveMyGameTxt();
    void loadGame();
    void choosePlayer1();
    void choosePlayer2();
    void newNetwork();
    void connNetwork();
    void newServer(QString IP, quint16 port);
    void connected();
    void parseFile(std::string);
    void setLoadFile();

private: //variables
    const int deskDim = 8;
    Desk* desk;
    Game* game = nullptr;
    saveGame savingGame;
    loadFile myLoadGame;
    QString tmpLoadingFile;
    QDialog *loading;
    QPushButton *loadButt;
    QLineEdit* fileLoad;
    QTextEdit* edit;
    
    
    const Player* player = nullptr;
    Figure* chosenFig = nullptr;
    DeskBox* chosenBox = nullptr;
    bool whitesTurn = true; //bily hrac zacina

    std::string IPadd ="127.0.0.1";
    quint16 port = 0;

    enum GameStatus
    {
        FINISHED,
	FIGQUERY,
	POSQUERY,
	MULTIJUMP,
	FOREIGN,
    } gameStatus = FINISHED;

    Game::PlayerType player1 = Game::HUMAN;
    Game::PlayerType player2 = Game::CPU;

    QAction* newAct = nullptr;
    QAction* saveActXml = nullptr;
    QAction* saveActTxt = nullptr;
    QAction* loadAct = nullptr;
    QAction* quitAct = nullptr;
    QAction* newNetworkAct = nullptr;
    QAction* connNetworkAct = nullptr;
    QAction* choosePlayer1Act = nullptr;
    QAction* choosePlayer2Act = nullptr;
};
/*
*/
class DeskBox: public QLabel
{
    Q_OBJECT

public:
    DeskBox(Position*, QWidget* = nullptr);
    ~DeskBox() { };

    Position* getPosition() const
    {
        return pos;
    }

    int getColumn() const
    {
        return pos->getColumn();
    }
    int getRow() const
    {
        return pos->getRow();
    }

protected:

private:
    Position* pos;
};

#endif // GUI_H
