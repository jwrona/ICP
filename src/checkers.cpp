#include "basis.h"
#include "player.h"
#include "myException.h"
#include "debug.h"
#include "gui.h"
#include "myConnect.h"

#include <cstdlib>
#include <iostream>
#include <cassert>
#include <random>
#include <QApplication>
#include <QtGlobal>

int setGameType()
{
    int gameNumb;
    std::cout << "***** HRA DAMA *****\n\n";
    std::cout << "1. Vytvorit sitovou hru\n";
    std::cout << "2. Pripojit se k hostovi\n";
    std::cout << "3. Proti umele inteligenci\n";
    std::cout << "4. Proti cloveku\n";
    std::cout << "5. Konec\n\n";
    std::cout << "Vyberte pozadovany typ hry: ";
    cin >> gameNumb;
    return gameNumb;
}

int main(int argc, char* argv[]) try
{
#ifdef Q_WS_X11
    bool useGUI = getenv("DISPLAY") != nullptr;
#else
    bool useGUI = true;
#endif
    useGUI = false;

    if (useGUI)
    {
        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        return app.exec();
    }

    else
    {
        Game::PlayerType player1, player2;
	std::string IP;
	quint16 port = 0;

        switch(setGameType())
        {
            case 1:
	        player1 = Game::NETWORKMASTER;
	        player2 = Game::NETWORKRECEIVER;
                break;

            case 2:
	        std::cout << "Zadej IP:" << std::endl;
		std::cin >> IP;
	        std::cout << "Zadej port:" << std::endl;
		std::cin >> port;

	        player1 = Game::NETWORKRECEIVER;
	        player2 = Game::NETWORKSLAVE;
                break;

            case 3:
	        player1 = Game::HUMAN;
	        player2 = Game::CPU;
                break;

            case 4:
	        player1 = Game::HUMAN;
	        player2 = Game::HUMAN;
                break;

            case 5:
                return EXIT_SUCCESS;
                break;
        }

        const int dimension = 8;
        Desk desk(dimension);
        Game game(&desk, player1, player2, IP, port);
        const Player* player = nullptr;
        bool whitesTurn = true; //bily hrac zacina

        while (!game.finished())
        {
            bool multiJump = false;
            Figure* f = nullptr;
            player = game.getPlayerOnTurn();

            while (true)
            {
                //tah
                printDesk(desk); //vykresleni desky do konzole

                if (!multiJump)
                {
                    //pokud se zrovna neprovadi vicenasobny skok, provadi se vyber figurky
		    if (whitesTurn)
                        std::cout << "Táhne bílý" << std::endl;
		    else
                        std::cout << "Táhne černý" << std::endl;

                    f = player->queryFrom(&desk)->getFigure();

                    if (f == nullptr || ((whitesTurn && !f->isWhite()) || (!whitesTurn
                                         && f->isWhite())))
                    {
                        std::cout << "Na této pozici není Vaše figurka!" << std::endl;
                        continue;
                    }
                }

                //figurka by zde mela byt vzdy zvolena
                assert(f != nullptr);
                Position* p = player->queryTo(&desk, f);
                //pozice by zde mela byt vzdy zvolena
                assert(p != nullptr);

                if ((whitesTurn && game.canJumpWhite()) || (!whitesTurn && game.canJumpBlack()))
                {
                    //hrac ma moznost skoku
                    if (f->canJump())
                    {
                        //zvolena figurka muze skakat
                        if (!f->jump(p))
                        {
                            //zvolena figurka muze skakat, ale neskoci
                            std::cout << "Tah na zvolenou pozici není možný, musíte skákat!" <<
                                      std::endl;
                            continue;
                        }

                        else if (f->canJump())
                        {
                            //predchozi skok uspel a je moznost dalsiho skoku
                            std::cout << "Vícenásobný skok!" << std::endl;
                            multiJump = true;
                            continue;
                        }
                    }

                    else
                    {
                        //zvolena figurka vubec nemuze skakat
                        std::cout << "Tah se zvolenou figurkou není možný, jiná může skákat!" <<
                                  std::endl;
                        continue;
                    }
                }

                else if (!f->move(p))
                {
                    //hrac nema moznost skoku ale tah na zvolenou pozici neni mozny
                    std::cout << "Tah na zvolenou pozici není možný!" << std::endl;
                    continue;
                }

                //tah (ci skok) na novou pozici probehl uspesne
                break; //konec tahu jednoho hrace
            }

            game.pawnToQueen();
            game.nextTurn();
            whitesTurn = !whitesTurn;
        }

        printDesk(desk); //zaverecne vykresleni desky do konzole

        if (game.isWinnerWhite())
            std::cout << "Vítěz je bílý!" << std::endl;

        else
            std::cout << "Vítěz je černý!" << std::endl;

        return EXIT_SUCCESS;
    }
}

catch (...)
{
    std::cerr << "Exception after main()" << std::endl;
    return EXIT_FAILURE;
}
