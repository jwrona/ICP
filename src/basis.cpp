#include "basis.h"
#include "player.h"
#include "myException.h"

#include <iostream>
#include <cstdlib>

/* class Desk */
Desk::Desk(int dimension): dim(dimension), posArr(nullptr)  //constructor
{
    if (dim < 2)
    {
        std::cerr << "Too small desk" << std::endl;
        exit(2);
    }

    try
    {
        //allocation
        posArr = new Position*[dim];

        for (int i = 0; i < dim; i++)
            posArr[i] = new Position[dim];
    }

    catch(const std::bad_alloc& e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    //initialization
    for (int c = 0; c < dim; c++)
    {
        for (int r = 0; r < dim; r++)
            posArr[c][r].init(*this, c, r);
    }
}

void Desk::removeFigures()
{
    for (int c = 0; c < dim; c++)
    {
        for (int r = 0; r < dim; r++)
            posArr[c][r].removeFigure();
    }
}

Position& Desk::getPositionAt(int c, int r) const
{
    if ((c >= 0) && (c < dim) && (r >= 0) && (r < dim))
        return posArr[c][r];

    else
    {
        std::cerr << "Desk::getPositionAt" << std::endl;
        throw bad_position();
    }
}

Figure* Desk::getFigureAt(int c, int r) const
{
    return getPositionAt(c, r).getFigure();
}

Desk::~Desk()
{
    for (int i = 0; i < dim; i++)
        delete[] posArr[i];

    delete[] posArr;
}


/* class Position */
void Position::init(Desk& d, int c, int r)  //initialization
{
    if ((c >= 0) && (c < d.getDim()) && (r >= 0)
        && (r < d.getDim()))
    {
        column = c;
        row = r;
        desk = &d;
        figure = nullptr;
    }

    else
        std::cerr << "Position::init - out of desk" << std::endl;
}

Position* Position::nextPosition(int dC,
                                 int dR) const   //return new position of class Position form Desk
{
    int newCol = column + dC;
    int newRow = row + dR;

    if (((newCol >= 0) && (newCol < desk->getDim())) &&
        ((newRow >= 0) && (newRow < desk->getDim())))
        return &desk->getPositionAt(newCol, newRow);

    return nullptr;
}

bool Position::sameRow(Position p)
const   //return true if actual position is on the same row as p
{
    if (p.row == row)
        return true;

    else
        return false;
}

bool Position::sameColumn(Position p)
const   //return true if actual position is on the same column as p
{
    if (p.column == column)
        return true;

    else
        return true;
}

Figure* Position::getFigure() const
{
    return figure;
}

const Figure* Position::putFigure(Figure* f)
{
    Figure* remove_figure = figure;
    figure = f;
    return remove_figure;
}

Figure* Position::removeFigure()
{
    Figure* remove_figure = figure;
    figure = nullptr;
    return remove_figure;
}

Position* Position::betweenPosition(const Position* p)
{
    //pozice by nemely byt stejne
    assert(this != p);
    //pozice musi lezet na uhlopricce
    int dCol = abs(column - p->column);
    int dRow = abs(row - p->row);
    assert(dCol == dRow);
    const int left = -1, down = -1;
    const int right = 1, up = 1;
    Position* actual = this;

    if (row > p->row)
    {
        //p je od soucasne pozice dolu (nize)
        if (column > p->column)
        {
            //p je na uhlopricce dole vlevo
            do
            {
                actual = actual->nextPosition(left, down);
            }
            while ((actual->getFigure() == nullptr) && (actual != p));
        }

        else
        {
            //p je na uhlopricce dole vpravo
            do
            {
                actual = actual->nextPosition(right, down);
            }
            while ((actual->getFigure() == nullptr) && (actual != p));
        }
    }

    else
    {
        //p je od soucasne pozice nahoru (vyse)
        if (column > p->column)
        {
            //p je na uhlopricce nahore vlevo
            do
            {
                actual = actual->nextPosition(left, up);
            }
            while ((actual->getFigure() == nullptr) && (actual != p));
        }

        else
        {
            //p je na uhlopricce nahore vpravo
            do
            {
                actual = actual->nextPosition(right, up);
            }
            while ((actual->getFigure() == nullptr) && (actual != p));
        }
    }

    if (actual == p) //pokud mezi pozicemi neni zadna figurka
        return nullptr;

    return actual; //pozice figurky mezi pozicemi
}

bool Position::lastRow() const
{
    if (figure->isWhite()) //bila figurky
        return (row == (desk->getDim() - 1));

    else //cerna figurky
        return (row == 0);

    return false;
}

/* class Figure */
Figure::Figure(Position& p, bool col): position(&p), color(col)  //constructor
{
    if (p.getFigure())
    {
        std::cerr << "Initializing figure into non-empty position" << std::endl;
        exit(2); //tohle prijde nahradit
    }

    else
        p.putFigure(this);
}

Position* Figure::getPosition() const  //return figure position
{
    return position;
}

bool Figure::isAtPosition(const Position p)
const  //true if figure is at position p
{
    return (*position == p);
}

bool Figure::move(Position* p)  //true if successfull move at position p
{
    if (canMove(p))
    {
        //na novem umisteni nesmi byt figurka
        assert(p->getFigure() == nullptr);
        //odstraneni figurky z puvodniho policka +
        //pridani odstranene figurky do noveho policka
        p->putFigure(position->removeFigure());
        //zmena pointeru na pozici aktualni figurky
        position = p;
        //adresa figurky v position musi byt shodna
        //s adresou tohoto objektu (figurky)
        assert(this == position->getFigure());
        return true;
    }

    return false;
}

bool Figure::canMove() const
{
    //vrati true, pokud vektor tahnutelnych pozic neni prazdny
    return (!movePositions().empty());
}

bool Figure::canMove(const Position* p) const
{
    std::vector<Position*> posVec = movePositions();

    for(std::vector<Position*>::const_iterator it = posVec.begin();
        it != posVec.end(); ++it)
    {
        if ((*it) == p)
            return true;
    }

    return false;
}

bool Figure::canJump() const
{
    //vrati true, pokud vektor skakatelnych pozic neni prazdny
    return (!jumpPositions().empty());
}

bool Figure::canJump(const Position* p) const
{
    std::vector<Position*> posVec = jumpPositions();

    for(std::vector<Position*>::const_iterator it = posVec.begin();
        it != posVec.end(); ++it)
    {
        if ((*it) == p)
            return true;
    }

    return false;
}

bool Figure::jump(Position* p)
{
    if (canJump(p))
    {
        //na novem umisteni nesmi byt figurka
        assert(p->getFigure() == nullptr);
        Position* between = position->betweenPosition(p);
        assert(isEnemy(between->getFigure()));
        //odstraneni preskakovane figurky a
        //vymazani atributu pozice preskakovane figurky
        between->removeFigure()->removePosition();
        //odstraneni figurky z puvodniho policka +
        //pridani odstranene figurky do noveho policka
        p->putFigure(position->removeFigure());
        //zmena pointeru na pozici aktualni figurky
        position = p;
        return true;
    }

    return false;
}

const Position* Figure::removePosition()
{
    const Position* remove_position = position;
    position = nullptr;
    return remove_position;
}

/* class Pawn */
Pawn::Pawn(Position& p, bool col): Figure(p,
            col)  //invoke base class constructor
{
    if (color) //white pawn
        forward = 1;

    else //black pawn
        forward = -1;
}

std::vector<Position*> Pawn::movePositions() const
{
    std::vector<Position*> posVec;
    Position* left = position->nextPosition(-1, forward);

    //policko neni mimo desku a zaroven je prazdne
    if ((left != nullptr) && (left->getFigure() == nullptr))
        posVec.push_back(left);

    Position* right = position->nextPosition(1, forward);

    //policko neni mimo desku a zaroven je prazdne
    if ((right != nullptr) && (right->getFigure() == nullptr))
        posVec.push_back(right);

    return posVec;
}

std::vector<Position*> Pawn::jumpPositions() const
{
    //TODO: nejaka optimalizace
    std::vector<Position*> posVec;
    Position* left = position->nextPosition(-1, forward);

    if ((left != nullptr) && (left->getFigure() != nullptr)
        && (isEnemy(left->getFigure())))
    {
        //pokud o policko vlevo dopredu lezi souperova figurka
        Position* behind = left->nextPosition(-1, forward);

        if ((behind != nullptr) && (behind->getFigure() == nullptr))
            posVec.push_back(behind);
    }

    Position* right = position->nextPosition(1, forward);

    if ((right != nullptr) && (right->getFigure() != nullptr)
        && (isEnemy(right->getFigure())))
    {
        //pokud o policko vpravo dopredu lezi souperova figurka
        Position* behind = right->nextPosition(1, forward);

        if ((behind != nullptr) && (behind->getFigure() == nullptr))
            posVec.push_back(behind);
    }

    return posVec;
}

const char* Pawn::getSymbol() const
{
    if (color) //if white
        return "\u265F";

    else
        return "\u2659";
}

bool Pawn::transformToQueen() const
{
    return (position != nullptr && (position->lastRow()));
}

/* class Queen */
Queen::Queen(Position& p, bool col): Figure(p,
            col)  //invoke base class constructor
{
}

std::vector<Position*> Queen::movePositions() const
{
    std::vector<Position*> posVec;

    //prvni pruchod: horizontal = -1 -> doleva
    //druhy pruchod: horizontal = 1  -> doprava
    for (int horizontal = -1; horizontal <= 1; horizontal += 2)
    {
        //prvni pruchod: vertical = -1 -> dolu
        //druhy pruchod: vertical = 1 -> nahoru
        for (int vertical = -1; vertical <= 1; vertical += 2)
        {
            Position* nextPos = position->nextPosition(horizontal, vertical);

            //dokud nejsme mimo desku nebo na pozici nelezi figurka
            while ((nextPos != nullptr) && (nextPos->getFigure() == nullptr))
            {
                //pridani do vektoru legalnich tahu
                posVec.push_back(nextPos);
                //posun do uhlopricneho smeru
                nextPos = nextPos->nextPosition(horizontal, vertical);
            }
        }
    }

    return posVec;
}

std::vector<Position*> Queen::jumpPositions() const
{
    std::vector<Position*> posVec;

    //prvni pruchod: horizontal = -1 -> doleva
    //druhy pruchod: horizontal = 1  -> doprava
    for (int horizontal = -1; horizontal <= 1; horizontal += 2)
    {
        //prvni pruchod: vertical = -1 -> dolu
        //druhy pruchod: vertical = 1 -> nahoru
        for (int vertical = -1; vertical <= 1; vertical += 2)
        {
            Position* nextPos = position;

            do
            {
                //posun do uhlopricneho smeru
                nextPos = nextPos->nextPosition(horizontal, vertical);
                //dokud nejsme mimo desku nebo na pozici nelezi figurka
            }
            while ((nextPos != nullptr) && (nextPos->getFigure() == nullptr));

            //na pozici je souperova figurka
            if ((nextPos != nullptr) && (isEnemy(nextPos->getFigure())))
            {
                //pozice za souperovou figurkou
                Position* behind = nextPos->nextPosition(horizontal, vertical);

                //vsechny pozice za figurkou, dokud jsou prazdne
                while ((behind != nullptr) && (behind->getFigure() == nullptr))
                {
                    posVec.push_back(behind);
                    behind = behind->nextPosition(horizontal, vertical);
                }
            }
        }
    }

    return posVec;
}

const char* Queen::getSymbol() const
{
    if (color) //if white
        return "\u265B"; //black queen unicode symbol

    else //if black
        return "\u2655"; //white queen unicode symbol
}

/* class Game */
Game::Game(Desk* d, PlayerType pl1, PlayerType pl2, std::string IP, quint16 port, GameType gameType):
    desk(d),
    hostIP(IP),
    hostPort(port)
{
    switch (gameType)
    {
        case SMALL:
            rowsOfFigures = 2;
            break;

        case CLASSIC:
            rowsOfFigures = 3;
            break;

        default:
            rowsOfFigures = 3;
            break;
    }

    switch (pl1)
    {
        case HUMAN:
            whitePlayer = new HumanPlayer(this, true);
            break;

        case CPU:
            whitePlayer = new CPUPlayer(this, true);
            break;

        case NETWORKMASTER:
            whitePlayer = new NetworkMaster(this, true);
            break;

        case NETWORKSLAVE:
            whitePlayer = new NetworkSlave(this, true);
            break;

        case NETWORKRECEIVER:
            whitePlayer = new NetworkReceiver(this, true);
            break;

        default:
            whitePlayer = nullptr;
            break;
    }

    switch (pl2)
    {
        case HUMAN:
            blackPlayer = new HumanPlayer(this, false);
            break;

        case CPU:
            blackPlayer = new CPUPlayer(this, false);
            break;

        case NETWORKMASTER:
            blackPlayer = new NetworkMaster(this, false);
            break;

        case NETWORKSLAVE:
            blackPlayer = new NetworkSlave(this, false);
            break;

        case NETWORKRECEIVER:
            blackPlayer = new NetworkReceiver(this, false);
            break;

        default:
            blackPlayer = nullptr;
            break;
    }

    playerOnTurn = whitePlayer; //zacina bily

    if ((rowsOfFigures << 1) >= desk->getDim())
    {
        std::cerr << "Too much rows of figures" << std::endl;
        exit(2);
    }

    for (int r = 0; r < rowsOfFigures; r++) //bile figurky
    {
        for (int c = 0; c < desk->getDim(); c++)
        {
            //TODO: kontrola bezchybne alokace
            if ((r % 2) && (c % 2)) // pri lichem radku a sloupci
                white.push_back(new Pawn(desk->getPositionAt(c, r), true));

            if (!(r % 2) && !(c % 2)) //a sudem radku a sloupci
                white.push_back(new Pawn(desk->getPositionAt(c, r), true));
        }
    }

    for (int r = (desk->getDim() - rowsOfFigures); r < desk->getDim();
         r++) //cerne figurky
    {
        for (int c = 0; c < desk->getDim(); c++)
        {
            if ((r % 2) && (c % 2)) // pri lichem radku a sloupci
                black.push_back(new Pawn(desk->getPositionAt(c, r), false));

            if (!(r % 2) && !(c % 2)) //a sudem radku a sloupci
                black.push_back(new Pawn(desk->getPositionAt(c, r), false));
        }
    }
}

bool Game::canJumpWhite() const
{
    //pozor! ne vsechny figurky ve vektoru musi byt na desce
    for(std::vector<Figure*>::const_iterator it = white.begin(); it != white.end();
        ++it)
    {
        //pokud figurka lezi na desce (ma pozici) a muze skakat
        if (((*it)->getPosition() != nullptr) && (*it)->canJump())
            return true;
    }

    return false;
}

bool Game::canJumpBlack() const
{
    //pozor! ne vsechny figurky ve vektoru musi byt na desce
    for(std::vector<Figure*>::const_iterator it = black.begin(); it != black.end();
        ++it)
    {
        //pokud figurka lezi na desce (ma pozici) a muze skakat
        if (((*it)->getPosition() != nullptr) && (*it)->canJump())
            return true;
    }

    return false;
}

std::vector<Figure*> Game::movableWhiteFigures() const
{
    std::vector<Figure*> figVec;

    for(std::vector<Figure*>::const_iterator it = white.begin(); it != white.end();
        ++it)
    {
        if (((*it)->getPosition() != nullptr) && (*it)->canMove())
            figVec.push_back(*it);
    }

    return figVec;
}

std::vector<Figure*> Game::movableBlackFigures() const
{
    std::vector<Figure*> figVec;

    for(std::vector<Figure*>::const_iterator it = black.begin(); it != black.end();
        ++it)
    {
        if (((*it)->getPosition() != nullptr) && (*it)->canMove())
            figVec.push_back(*it);
    }

    return figVec;
}

std::vector<Figure*> Game::jumpableWhiteFigures() const
{
    std::vector<Figure*> figVec;

    for(std::vector<Figure*>::const_iterator it = white.begin(); it != white.end();
        ++it)
    {
        if (((*it)->getPosition() != nullptr) && (*it)->canJump())
            figVec.push_back(*it);
    }

    return figVec;
}

std::vector<Figure*> Game::jumpableBlackFigures() const
{
    std::vector<Figure*> figVec;

    for(std::vector<Figure*>::const_iterator it = black.begin(); it != black.end();
        ++it)
    {
        if (((*it)->getPosition() != nullptr) && (*it)->canJump())
            figVec.push_back(*it);
    }

    return figVec;
}

void Game::pawnToQueen()
{
    for(std::vector<Figure*>::iterator it = white.begin(); it != white.end(); ++it)
    {
        //bily pesec je na vrchnim radku
        if ((*it)->transformToQueen())
        {
            Position* p = (*it)->getPosition();
            //odstraneni pesce z desky a
            //vymazani atributu pozice pesce
            p->removeFigure()->removePosition();
            //alokace nove figurky, damy
            //pridani do vektoru bilych figurek
            white.push_back(new Queen(*p, true));
        }
    }

    for(std::vector<Figure*>::iterator it = black.begin(); it != black.end(); ++it)
    {
        //cerny pesec je na vrchnim radku
        if ((*it)->transformToQueen())
        {
            Position* p = (*it)->getPosition();
            //odstraneni pesce z desky a
            //vymazani atributu pozice pesce
            p->removeFigure()->removePosition();
            //alokace nove figurky, damy
            //pridani do vektoru cernych figurek
            black.push_back(new Queen(*p, false));
        }
    }
}

bool Game::finished()
{
    bool whiteCanMove = false;
    bool blackCanMove = false;

    for(std::vector<Figure*>::const_iterator it = white.begin(); it != white.end();
        ++it)
    {
        if (((*it)->getPosition() != nullptr) && ((*it)->canMove() || (*it)->canJump()))
        {
            whiteCanMove = true;
            break;
        }
    }

    for(std::vector<Figure*>::const_iterator it = black.begin(); it != black.end();
        ++it)
    {
        if (((*it)->getPosition() != nullptr) && ((*it)->canMove() || (*it)->canJump()))
        {
            blackCanMove = true;
            break;
        }
    }

    if (whiteCanMove && blackCanMove)
        return false;

    else if (whiteCanMove)
        whiteWon = true;

    else if (blackCanMove)
        whiteWon = false;

    return true;
}

const Player* Game::getPlayerOnTurn() const
{
    return playerOnTurn;
}

void Game::nextTurn()
{
    whitesTurn = !whitesTurn;
    if (whitesTurn)
        playerOnTurn = whitePlayer;
    else
        playerOnTurn = blackPlayer;
}

void Game::playRound()
{
    bool multiJump = false;
    Figure* f = nullptr;
    Position *oldP = nullptr;
    Position* p;

    while (true)
    {
        //tah
        if (!multiJump)
        {
            //pokud se zrovna neprovadi vicenasobny skok, provadi se vyber figurky
            f = playerOnTurn->queryFrom(desk)->getFigure();
            
            oldP = f->getPosition();
            oldPos = std::to_string((oldP->getRow() + 1)) + std::to_string(
                            (oldP->getColumn() + 1));
                            
            if (f == nullptr || ((whitesTurn && !f->isWhite()) || (!whitesTurn
                                 && f->isWhite())))
                continue;
        }else
         {
           oldPos = std::to_string((p->getRow() + 1)) + std::to_string(
                            p->getColumn() + 1);
         }

        //figurka by zde mela byt vzdy zvolena
        assert(f != nullptr);
        p = playerOnTurn->queryTo(desk, f);
        
        newPos = std::to_string((p->getRow() + 1)) + std::to_string(
                            p->getColumn() + 1);
        
        //pozice by zde mela byt vzdy zvolena
        assert(p != nullptr);

        if ((whitesTurn && canJumpWhite()) || (!whitesTurn && canJumpBlack()))
        {
            //hrac ma moznost skoku
            if (f->canJump())
            {
                //zvolena figurka muze skakat
                if (!f->jump(p))
                {
                    //zvolena figurka muze skakat, ale neskoci
                    continue;
                }

                else if (f->canJump())
                {
                    //predchozi skok uspel a je moznost dalsiho skoku
                    multiJump = true;
                    continue;
                }
            }

            else
            {
                //zvolena figurka vubec nemuze skakat
                continue;
            }
        }

        else if (!f->move(p))
        {
            //hrac nema moznost skoku ale tah na zvolenou pozici neni mozny
            continue;
        }

        //tah (ci skok) na novou pozici probehl uspesne
	pawnToQueen();
        break; //konec tahu jednoho hrace
    }
}

Game::~Game()
{
    for(std::vector<Figure*>::iterator it = white.begin(); it != white.end(); ++it)
        delete *it;

    for(std::vector<Figure*>::iterator it = black.begin(); it != black.end(); ++it)
        delete *it;

    delete whitePlayer;
    delete blackPlayer;
}
