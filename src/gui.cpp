#include "gui.h"

extern myConnect network;

MainWindow::MainWindow(QWidget* qparent):
    QMainWindow(qparent),
    desk(new Desk(deskDim))
{
    //deska pod sachovnici
    const int fieldSize = 70;
    setObjectName("MainWindow");
    setWindowTitle(QString::fromUtf8("Dáma"));
    QFrame* deskFrame = new QFrame(this);
    deskFrame->setObjectName("deskFrame");
    deskFrame->setGeometry(QRect(0, 20, fieldSize * deskDim,
                                 fieldSize * deskDim));
    deskFrame->setStyleSheet("background-color:white;");

    //sachovnice
    for (int posY = (deskDim - 1), r = 0; posY >= 0; posY--, r++)
    {
        for (int c = 0; c < deskDim; c++)
        {
            if (((r % 2) && (c % 2)) || (!(r % 2) && !(c % 2)))
            {
                DeskBox* box = new DeskBox(&desk->getPositionAt(c, r), deskFrame);
                box->setGeometry(QRect(c * fieldSize, posY * fieldSize, fieldSize, fieldSize));
                box->setStyleSheet("background-color:black; color:white;");
            }
        }
    }

    //create menus and their actions
    createActions();
    createMenus();
    //status bar
    statusBar()->showMessage("Nova hra");
    resize(700, 700);
    //nefunkcni sitova hra
    /*
    connect(&network, SIGNAL(serverCreated(QString, quint16)), this,
            SLOT(newServer(QString, quint16)));
    connect(&network, SIGNAL(connectionAccepted()), this,
            SLOT(connected()));
    */
}

MainWindow::~MainWindow()
{
    delete game;
    delete desk;
}

void MainWindow::createMenus()
{
    QMenuBar* menu = menuBar();
    QMenu* game = menu->addMenu("Hra");
    game->addAction(newAct);
    game->addAction(saveActXml);
    game->addAction(saveActTxt);
    game->addAction(loadAct);
    game->addAction(quitAct);
    QMenu* netw = menu->addMenu("Sitova hra");
    netw->addAction(newNetworkAct);
    netw->addAction(connNetworkAct);
    QMenu* options = menu->addMenu("Moznosti");
    options->addAction(choosePlayer1Act);
    options->addAction(choosePlayer2Act);
}

void MainWindow::createActions()
{
    newAct = new QAction("Nova", this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip("Vytvori novou hru");
    connect(newAct, SIGNAL(triggered()), this, SLOT(newGame()));
    saveActXml = new QAction("Uloz XML", this);
    saveActXml->setShortcuts(QKeySequence::Save);
    saveActXml->setStatusTip("Ulozi soucasnou hru");
    connect(saveActXml, SIGNAL(triggered()), this, SLOT(saveMyGameXml()));
    saveActTxt = new QAction("Uloz TXT", this);
    saveActTxt->setStatusTip("Ulozi soucasnou hru");
    connect(saveActTxt, SIGNAL(triggered()), this, SLOT(saveMyGameTxt()));
    loadAct = new QAction("Nacti", this);
    loadAct->setShortcuts(QKeySequence::Open);
    loadAct->setStatusTip("Nacte ulozenou hru");
    connect(loadAct, SIGNAL(triggered()), this, SLOT(loadGame()));
    quitAct = new QAction("Ukonci", this);
    quitAct->setShortcuts(QKeySequence::Quit);
    quitAct->setStatusTip("Ukonci hru");
    connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
    choosePlayer1Act = new QAction("Volba bileho hrace", this);
    choosePlayer1Act->setStatusTip("Zvoli bileho hrace");
    connect(choosePlayer1Act, SIGNAL(triggered()), this, SLOT(choosePlayer1()));
    choosePlayer2Act = new QAction("Volba cerneho hrace", this);
    choosePlayer2Act->setStatusTip("Zvoli cerneho hrace");
    connect(choosePlayer2Act, SIGNAL(triggered()), this, SLOT(choosePlayer2()));
    newNetworkAct = new QAction("Vytvorit", this);
    newNetworkAct->setStatusTip("Vytvori novou sitovou hru");
    connect(newNetworkAct, SIGNAL(triggered()), this, SLOT(newNetwork()));
    connNetworkAct = new QAction("Pripojit", this);
    connNetworkAct->setStatusTip("Pripoji se k jinemu hraci");
    connect(connNetworkAct, SIGNAL(triggered()), this, SLOT(connNetwork()));
}

//slots
void MainWindow::newGame()
{
    delete game;
    game = nullptr;
    desk->removeFigures();
    game = new Game(desk, player1, player2, IPadd, port);
    redrawFigures();
    player = game->getPlayerOnTurn();
    whitesTurn = true;
    loadAct->setEnabled(true);

    if (player1 == Game::HUMAN)
        gameStatus = FIGQUERY;

    else if (player1 == Game::NETWORKMASTER)
        gameStatus = FIGQUERY;

    else
    {
        gameStatus = FOREIGN;
        game->playRound();
        redrawFigures();
        switchTurn();
    }
}

void MainWindow::saveMyGameXml()
{
    if(savingGame.saveIntoFile(true) != 0)
        statusBar()->showMessage("Nepovedlo se otevrit soubor pro zapis");

    statusBar()->showMessage("Ulozen soubor hra[ nejvyssi cislo].xml");
}

void MainWindow::saveMyGameTxt()
{
    if(savingGame.saveIntoFile(false) != 0)
        statusBar()->showMessage("Nepovedlo se otevrit soubor pro zapis");

    statusBar()->showMessage("Ulozen soubor hra[nejvyssi cislo].txt");
}

void MainWindow::loadGame()
{   
    newGame();
    QDir dir;
    QStringList filters;
    filters << "*.txt" << "*.xml";
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    
    
    
    loading = new QDialog(this); //or QMainWindow, or your custom class
    loading->show();
    edit = new QTextEdit(loading);
    edit->setGeometry(QRect(QPoint(0, 0), QSize(300, 300)));
    edit->setReadOnly(true);

    for (int i = 0; i < list.size(); ++i)
    {
        QFileInfo fileInfo = list.at(i);
        edit->append(fileInfo.fileName().toUtf8().constData());
    }

    edit->show();
    fileLoad = new QLineEdit(loading);
    fileLoad->setGeometry(QRect(QPoint(350, 100), QSize(300, 30)));
    fileLoad->show();
    loadButt = new QPushButton(tr("Nacti soubor"), loading);
    loadButt->show();
    loadButt->setGeometry(QRect(QPoint(350, 150),
                                QSize(100, 50)));
    connect(loadButt, SIGNAL(clicked()), this, SLOT(setLoadFile()));
    //parseFile("hra586.txt");
}

void MainWindow::choosePlayer1()
{
    QStringList items;
    QString human("Clovek");
    QString cpu("Pocitac");
    items << human << cpu;
    bool ok;
    QString item = QInputDialog::getItem(this, "Bily hrac", "Vyber:", items, 0,
                                         false, &ok);

    if (ok && !item.isEmpty())
    {
        if (item == human)
            player1 = Game::HUMAN;

        else if (item == cpu)
            player1 = Game::CPU;
    }
}

void MainWindow::choosePlayer2()
{
    QStringList items;
    QString human("Clovek");
    QString cpu("Pocitac");
    items << human << cpu;
    bool ok;
    QString item = QInputDialog::getItem(this, "Cerny hrac", "Vyber:", items, 0,
                                         false, &ok);

    if (ok && !item.isEmpty())
    {
        if (item == human)
            player2 = Game::HUMAN;

        else if (item == cpu)
            player2 = Game::CPU;
    }
}

void MainWindow::newNetwork()
{
    /*
        player1 = Game::NETWORKMASTER;
        player2 = Game::NETWORKRECEIVER;
        statusBar()->showMessage("Cekam na pripojeni hrace");
        newGame();
    */
    statusBar()->showMessage("Bohuzel implementovano pouze v konzolove aplikaci");
}

void MainWindow::connNetwork()
{
    /*
        bool ok;
        QString text = QInputDialog::getText(this, "IP adresa", "IP adresa",
                                             QLineEdit::Normal, "127.0.0.1", &ok);

        if (ok && !text.isEmpty())
        {
            IPadd = text.toUtf8().constData();
            port = QInputDialog::getInt(this, "Port", "Port", 0, 0, 65535);
            player1 = Game::NETWORKRECEIVER;
            player2 = Game::NETWORKSLAVE;
            newGame();
        }
    */
    statusBar()->showMessage("Bohuzel implementovano pouze v konzolove aplikaci");
}

void MainWindow::newServer(QString IP, quint16 port)
{
//nefunkcni sitova hra
    /*
        //std::cout << IP.toStdString() << '\t' << port << std::endl;
        QMessageBox msgBox;
        msgBox.setText("Server uspesne vytvoren!");
        msgBox.setInformativeText("IP adresa: " + IP + "\nPort: " + QString::number(
                                      port) + "\n\nPripojte vzdaleneho hrace a potom potvrdte.");
        msgBox.exec();
    */
}

void MainWindow::connected()
{
//nefunkcni sitova hra
    /*
        statusBar()->showMessage("Pripojeno!");
        gameStatus = FIGQUERY;
    */
}

void MainWindow::setLoadFile()
{
    tmpLoadingFile = fileLoad->text();
    loading->close();
    delete edit;
    delete fileLoad;
    delete loadButt;
    delete loading;
    parseFile(tmpLoadingFile.toStdString());
}

void MainWindow::parseFile(std::string fileName)
{
    myLoadGame.loadParseFile(fileName);
    int numRow, numCol;
    Figure* f;
    bool multiJump = false;
    //opet aby jsme nenacitali
    loadAct->setEnabled(false);

    for (unsigned int i = 0; i < myLoadGame.moves.size();  i++)
    {
        //std::cout << "Element " << i << ": " << (myLoadGame.moves.at(i))[0] << std::endl;
        numCol = myLoadGame.moves.at(i)[1] - '0';
        numRow =  myLoadGame.moves.at(i)[0] - '0';
        savingGame.oldPos = QString::number(numRow) + QString::number(numCol);
        numCol--;
        numRow--;
        f = nullptr;
        //std::cout << numCol << " " << numRow << " ";
        Position* from = &(desk->getPositionAt(numCol, numRow));
        f = from->getFigure();
        numCol = myLoadGame.moves.at(i)[3] - '0';
        numRow =  myLoadGame.moves.at(i)[2] - '0';
        savingGame.newPos = QString::number(numRow) + QString::number(numCol);
        numCol--;
        numRow--;
        //ulozeni tahu
        savingGame.insertMove(whitesTurn);
        //std::cout << numCol << " " << numRow << "\n";
        Position* to = &(desk->getPositionAt(numCol, numRow));

        //std::cout << "Kdo tahne: " << whitesTurn << " " << game->canJumpWhite() << "\n";
        if ((whitesTurn && game->canJumpWhite()) || (!whitesTurn
                && game->canJumpBlack()))
        {
            //std::cout << "Musi se skakat";
            //hrac ma moznost skoku
            if (f->canJump())
            {
                //std::cout << "Jsem ve skokou";
                //zvolena figurka muze skakat
                if (!f->jump(to))
                {
                    //zvolena figurka muze skakat, ale neskoci
                }
                else if (f->canJump())
                {
                    //predchozi skok uspel a je moznost dalsiho skoku
                    multiJump = true;
                }
            }

            else
            {
                //zvolena figurka vubec nemuze skakat
            }
        }
        else if (!f->move(to))
        {
            //std::cout << "Nemohu se zde pohnout ";
            //hrac nema moznost skoku ale tah na zvolenou pozici neni mozny
        }

        //tah (ci skok) na novou pozici probehl uspesne
        game->pawnToQueen();

        if(!multiJump)
            whitesTurn = !whitesTurn;

        redrawFigures();
    }
}

void MainWindow::redrawFigures()
{
    QFrame* deskFrame = findChild<QFrame*>("deskFrame");
    QObjectList list = deskFrame->children();

    for(QObjectList::iterator it = list.begin(); it != list.end(); ++it)
    {
        DeskBox* box = qobject_cast<DeskBox*>(*it);
        int c = box->getColumn(), r = box->getRow();

        if (desk->getFigureAt(c, r) != nullptr)
        {
            box->setText(QString::fromUtf8(desk->getFigureAt(c, r)->getSymbol()));
            box->setAlignment(Qt::AlignCenter);
            QFont figureFont("AnyStyle", 40);
            box->setFont(figureFont);
            box->setStyleSheet("background-color:black; color:white;");
        }

        else
            box->clear();
    }
}

bool MainWindow::figClick(Position* p)
{
    Q_ASSERT(p != nullptr);
    Figure* f = p->getFigure();

    if (f == nullptr || ((whitesTurn && !f->isWhite()) || (!whitesTurn
                         && f->isWhite())))
    {
        statusBar()->showMessage("Toto neni vase figurka!");
        return false;
    }

    chosenFig = f;
    gameStatus = POSQUERY;
    redrawFigures();
    //figurka by zde mela byt vzdy zvolena
    Q_ASSERT(chosenFig != nullptr);
    //zde budu ukladat pocatecni souradnici
    savingGame.oldPos = QString::number(p->getRow() + 1) + QString::number(
                            p->getColumn() + 1);
    //statusBar()->showMessage(QString::number(p->getRow()));
    return true;
}

void MainWindow::posClick(Position* p)
{
    Q_ASSERT(p != nullptr);
    //zde je to proto, ze nemuzu nacitat
    loadAct->setEnabled(false);

    //zde budu ukladat konecnou souradnici
    if ((whitesTurn && game->canJumpWhite()) || (!whitesTurn
            && game->canJumpBlack()))
    {
        //hrac ma moznost skoku
        if (chosenFig->canJump())
        {
            //zvolena figurka muze skakat
            if (!chosenFig->jump(p))
            {
                //zvolena figurka muze skakat, ale neskoci
                //UPOZORNENI: SKAKEJ
                gameStatus = POSQUERY;
                return;
            }

            else if (chosenFig->canJump())
            {
                //predchozi skok uspel a je moznost dalsiho skoku
                statusBar()->showMessage("Vicenasobny skok!");
                redrawFigures();
                chosenBox->setStyleSheet("background-color:black; color:red;");
                gameStatus = POSQUERY;
                return;
            }
        }

        else
        {
            //zvolena figurka vubec nemuze skakat
            statusBar()->showMessage("Tah nelze provest, jina figurka ma moznost skoku!");
            gameStatus = FIGQUERY;
            return;
        }
    }

    else if (!chosenFig->move(p))
    {
        //hrac nema moznost skoku ale tah na zvolenou pozici neni mozny
        statusBar()->showMessage("Tento tah neni mozny!");
        gameStatus = FIGQUERY;
        return;
    }

    //zde budu ukladat pocatecni souradnici
    savingGame.newPos = QString::number(p->getRow() + 1) + QString::number(
                            p->getColumn() + 1);
    Figure* helpFigure = p->getFigure();
    savingGame.insertMove(helpFigure->isWhite());
    game->pawnToQueen();
    redrawFigures();
    switchTurn();
}

void MainWindow::switchTurn()
{
    if (game->finished())
    {
        QMessageBox msgBox;

        if (game->isWinnerWhite())
            msgBox.setText("Vitezem je bily!");

        else
            msgBox.setText("Vitezem je cerny!");

        msgBox.exec();
        gameStatus = FINISHED;
        return;
    }

    game->nextTurn();
    player = game->getPlayerOnTurn();
    whitesTurn = !whitesTurn;

    //pokud je na tahu hrac typu clovek
    if ((whitesTurn && (player1 == Game::HUMAN)) || (!whitesTurn
            && (player2 == Game::HUMAN)))
    {
        gameStatus = FIGQUERY;
    }

    else
    {
        gameStatus = FOREIGN;
        game->playRound();
        savingGame.oldPos = QString::fromStdString(game->oldPos);
        savingGame.newPos = QString::fromStdString(game->newPos);
        savingGame.insertMove(whitesTurn);
        redrawFigures();
        switchTurn();
    }
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if ((gameStatus != FINISHED) && (event->button() == Qt::LeftButton))
    {
        DeskBox* box = qobject_cast<DeskBox*>(childAt(event->pos()));

        if (box != nullptr)
        {
            chosenBox = box;

            switch (gameStatus)
            {
                case FIGQUERY:
                    if (figClick(box->getPosition()))
                        chosenBox->setStyleSheet("background-color:black; color:red;");

                    break;

                case POSQUERY:
                    posClick(box->getPosition());
                    break;

                default:
                    break;
            }
        }
    }
}

DeskBox::DeskBox(Position* p, QWidget* qparent):
    QLabel(qparent),
    pos(p)
{
}
