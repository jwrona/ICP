#ifndef BASIS_H
#define BASIS_H

#include <vector>
#include <cassert>
#include <string>

#include <QtGlobal>

class Position;
class Figure;

/*!\brief Třída představující hrací desku.
 *
 * Čtvercová hrací deska o zvolené velikosti,
 * obsahuje příslušný počet instancí třídy
 * Position, každá představuje jedno políčko.
 */
class Desk
{
    int dim; //!< Rozměr desky (strana čtverce)
    Position** posArr; //!< 2D Pole pozic
public:
    /* Zakazani copy konstruktoru a operatoru prirazeni
     * kvuli 2D poli na heapu
     */
    Desk(const Desk&) = delete;
    Desk& operator=(const Desk&) = delete;

    /*! \brief Konstruktor.
     *
     * Alokuje paměť pro dvourozměrné pole pozic a inicializuje
     * tyto objekty příslušnými hodnotami sloupec a řádek.
     *
     * \param[in] dimension rozměr hrací desky.
     */
    Desk(int dimension);

    /*! \brief Ostraní všechny figurky z desky.
     *
     *  Všem pozicím na hrací desce odebere
     *  ukazatel na jejich figurku.
     */
    void removeFigures();

    /*! \brief Vrací referenci na pozici.
     *
     * Podle vstupních argumentů vrací referenci na instanci
     * třídy Position. Pokud požadovaná pozice není validní
     * (je mimo rozsah desky), je vyvolána výjimka bad_position.
     *
     * \param[in] c index sloupce.
     * \param[in] r index řádku.
     * \return Reference na zadanou pozici
     */
    Position& getPositionAt(int c, int r) const;

    /*! \brief Vrací ukazatel na figurku.
     *
     * Podle vstupních argumentů vrací ukazatel na figurku umístěnou
     * na pozici, která je identifikovaná sloupcem c a řádkem r.
     * Pokud zadaná pozice není validní (je mimo rozsah desky) nebo
     * na pozici žádná figurka není, vrací NULL.
     *
     * \param[in] c index sloupce.
     * \param[in] r index řádku.
     * \return Ukazatel na figurku na zadané pozici nebo NULL
     */
    Figure* getFigureAt(int c, int r) const;

    /*! \brief Vrací rozměr desky.
     */
    int getDim() const
    {
        return dim;
    };

    /*! \brief Destruktor.
     *
     * Uvolňuje paměť alokovanou v konstruktoru pro 2D pole pozic.
     */
    ~Desk();
};

/*!\brief Třída představující pozici na hrací desce.
 *
 * Pozice zná své umístění dle indexů sloupec a řádek,
 * ví, zda na pozici leží figurka a zná hrací desku,
 * na které je pozice umístěna.
 */
class Position
{
    int column, row; //!< Sloupec a řádek na desce
    Figure* figure; //!< Ukazatel na figurku ležící na pozici
    Desk* desk; //!< Ukazatel na desku

public:

    /*!\brief Defaultní konstruktor.
     */
    Position() = default;

    /*! \brief Inicializace pozice.
     *
     * Metoda inicializuje pozici na správný řádek a sloupec na
     * hrací desce.
     * Je třeba volat nad každým vytvořeným políčkem.
     *
     * \param[in] c index sloupce.
     * \param[in] r index řádku.
     */
    void init(Desk&, int c, int r);


    /*! \brief Vrací následující pozici.
     *
     *  Vrací pozici o dC sloupců a dR řádků
     *  od současné pozice.
     *
     * \param[in] dC rozdíl sloupců.
     * \param[in] dR rozdíl řádků.
     * \return Ukazatel na novou pozici
     */
    Position* nextPosition(int dC, int dR) const;

    /*! \brief True, pokud jsou pozice ve stejném sloupci.
     */
    bool sameColumn(Position) const;

    /*! \brief True, pokud jsou pozice na stejném řádku.
     */
    bool sameRow(Position) const;

    /*! \brief Vrací aktuální sloupec pozice.
     */
    int getColumn() const
    {
        return column;
    };

    /*! \brief Vrací aktuální řádek pozice.
     */
    int getRow() const
    {
        return row;
    };

    /*! \brief Vrací ukazatel na figurku.
     *
     *  Vrací ukazatel na figurku, která na pozici leží.
     *  Pokud na pozici žádná figurka neleží, vrací
     *  nullptr.
     *
     * \return Ukazatel na novou figurku
     */
    Figure* getFigure()const;

    /*! \breif Umístí na pozici figurku.
     *
     *  Ukazaletem na třídu Figure předaným parametrem
     *  se přepíše současný ukazatel na figurku dané
     *  pozice.
     *
     *  \param[in] f ukazatel na Figure.
     *  \return Původní figurka na pozici, nebo nullptr.
     */
    const Figure* putFigure(Figure* f);

    /*! \breif Odstraní figurku z pozice.
     *
     *  \return Původní figurka na pozici, nebo nullptr.
     */
    Figure* removeFigure();

    /*! \breif Vrací pozici mezi dvěmi pozicemi.
     *
     *  Metoda vrací první pozici na které leží figurka
     *  mezi současnou pozicí a pozicí předanou parametrem.
     *  Pokud mezi těmito pozicemi žádná figurka neleží,
     *  návratová hodnota je nullptr.
     *  Aktuální pozice a ta předaná parametrem musí vždy
     *  ležet na uhlopříčce, jinak nastává kritická chyba.
     *
     *  \param[in] p ukazatel na pozici..
     *  \return Pozice s figurkou mezi pozicemi, nebo nullptr.
     */
    Position* betweenPosition(const Position* p);

    /*! \breif True, pokud se jedná o poslední řádek.
     *
     *  Dle toho, jaká na pozici leží figurka
     *  (bílá nebo černá) vyhonotí, zda se jedná
     *  o její poslední řádek, při jehož dosažení
     *  se pěšec mění v dámu.
     *
     *  \return True pokud se jedná o poslední řádek, jinak false
     */
    bool lastRow() const;
};


/*! \breif Operátor == pro dva objekty třídy Position.
 *
 *  \return True pokud pozice leží na stejném řádku i sloupci, jinak false
 */
inline bool operator==(const Position& lhs, const Position& rhs)
{
    if ((lhs.getColumn() == rhs.getColumn()) && (lhs.getRow() == rhs.getRow()))
        return true;

    else
        return false;
}

/*! \brief Abstraktní řída reprezentující figurku.
 *
 * Třída představující hrací figurku.
 * Obsahuje ukazatel na objekt typu Position,
 * figurka tedy zná svou pozici. Pokud je tento ukazatel nullptr,
 * figurka není umístěna na žádné pozici.
 */
class Figure
{
protected:
    Position* position; //!< ukazatel na pozici.
    const bool color; //!< barva figurky, true = bílá, false = černá

    /*! \brief Konstruktor.
     *
     * Konstruktor imicializuje proměnné position a color.
     * Při nezadání argumentu color je figurka považována za bílou.
     *
     * \param[in] p ukazatel na instanci třídy position.
     * \param[in] col barva figurky, true = bíla, false = černá.
     */
    Figure(Position& p, bool col = true);

public:
    /*! \brief Vrací pozici figurky.
     *
     * Metoda vrací aktuální pozici figurky.
     * \return Pointer na aktuální pozici figurky
     */
    Position* getPosition() const;

    /*! \brief True, pokud je figurka na pozici předané argumetem.
     *
     * Metoda vrací true, pokud je aktuální pozice figurky stejná
     * jako pozice zadaná v jediném argumentu. Jinak vrací false.
     * \param[in] p pozice pro porovnání s pozicí figurky.
     * \return True pokud se pozice shodují, jinak false
     */
    bool isAtPosition(const Position p) const;

    /*! \brief Přesun figurky na jinou pozici.
     *
     * Pokud je pozice validní a pokud je přesun legální,
     * přesune figurku na pozici předanou argumentem a vrátí true.
     * Jinak pozice zůstává stejná a vrací falsek.
     * Využívá virtuální metodu canMove().
     * \param[in] p pozice na kterou by měla být figurka přesunuta.
     * \return True pokud byl přesun úspěšný, jinak false
     */
    bool move(Position* p);

    /*! \brief True pokud je barva figurky bílá.
     *
     *  Metoda vrací true, pokud je barva figurky true, tedy bílá,
     *  jinak je barva figurky černá a vrací false.
     *  \return True při barvě figurky bíla, jinak false
     */
    bool isWhite() const
    {
        return color;
    };

    /*! \brief True pokud je přesun možný.
     *
     *  Vrací true, pokud je přesun figurky kamkoliv možný,
     *  jinak false.
     *  Přesun se nekoná.
     *  \return True pokud je přesun možný, false pokud není
     */
    bool canMove()const;

    /*! \brief True pokud je přesun na pozici možný.
     *
     *  Vrací true, pokud je přesun figurky
     *  na pozici předanou argumentem možný, jinak false.
     *  Přesun se nekoná.
     *  \param[in] p pozice na kterou se testuje možnost přesunu.
     *  \return True pokud je přesun možný, false pokud není
     */
    bool canMove(const Position* p)const;

    /*! \brief Určení všech táhnutelných pozic.
     *
     *  Vytvoří vektor pozic, na které je figurka
     *  schopná se dostat jedním legálním krokem.
     *  \return Vektor legálních táhnutelných pozic
     */
    virtual std::vector<Position*> movePositions() const = 0;

    /*! \brief True pokud je skok možný.
     *
     *  Vrací true, pokud je skok figurky kamkoliv možný,
     *  jinak false.
     *  Skok se nekoná.
     *  \return True pokud je skok možný, false pokud není
     */
    bool canJump() const;

    /*! \brief True pokud je skok na pozici možný.
     *
     *  Vrací true, pokud je skok figurky na určenou pozici
     *  možný, jinak false.
     *  Skok se nekoná.
     *  \param[in] p pozice na kterou se testuje možnost skoku.
     *  \return True pokud je skok možný, false pokud není
     */
    bool canJump(const Position* p) const;

    /*! \brief Určení všech skákatelných pozic.
     *
     *  Vytvoří vektor pozic, na které je figurka
     *  schopná se dostat jedním legálním skokem.
     *  \return Vektor legálních skákatelných pozic
     */
    virtual std::vector<Position*> jumpPositions() const = 0;

    /*! \brief Provede skok a vrací true při úspěchu.
     *
     *  Vrací true, pokud je skok figurky na určenou pozici
     *  možný, jinak false. Skok je vykonán.
     *  \param[in] p pozice na kterou je požadéván skok.
     *  \return True pokud byl skok úspěšný, false pokud nebyl
     */
    bool jump(Position* p);

    /*! \brief Vrátí symbol figurky.
     *
     *  Dle typu figurky vrací buď symbol pěšce
     *  nebo dámy, bílý nebo černý. Tyto symboly
     *  jsou obsaženy v kódování UNICODE.
     */
    virtual const char* getSymbol() const = 0;

    /*! \brief True pokud je možné pěšce transformovat na dámu.
     */
    virtual bool transformToQueen() const
    {
        return false;
    }

    /*! \brief Odstraní figurce pozici.
     */
    const Position* removePosition();

    /*! \brief True, pokud je figurka stejné barvy.
     *
     *  \param[in] ukazatel na figurku
     *  \return True, pokud je aktuální figurka a figurka předaná\
     *  parametrem stejné barvy, jinak false
     */
    bool isFriend(Figure* f) const
    {
        assert(f != nullptr);
        return (color == f->color);
    };

    /*! \brief True, pokud je figurka jiné barvy.
     *
     *  \param[in] ukazatel na figurku
     *  \return True, pokud je aktuální figurka a figurka předaná\
     *  parametrem jiné barvy, jinak false
     */
    bool isEnemy(Figure* f) const
    {
        assert(f != nullptr);
        return (color != f->color);
    };

    /*! \brief Destruktor.
     *
     * Destruktor není implementován.
     */
    virtual ~Figure() { };
};

/*! \brief Třída představující pěšce.
 *
 * Třída odvozená z třídy abstraktní třídy Figure,
 * představuje jednoho pěšce.
 */
class Pawn: public Figure
{
    int forward; //!< Směr vpřed (dle barvy pěšce)
public:
    //using Figure::Figure; /* inheriting constructors, GCC 4.8 */

    /*! \brief Konstruktor.
     *
     * Jediný konstruktor, invokuje konstruktor rodičovské třídy
     * Figure.
     */
    Pawn(Position& p, bool col = true);

    /*! \brief Popis v bázové třídě Figure.
     */
    virtual std::vector<Position*> movePositions() const;
    /*! \brief Popis v bázové třídě Figure.
     */
    virtual std::vector<Position*> jumpPositions() const;
    /*! \brief Popis v bázové třídě Figure.
     */
    virtual const char* getSymbol() const;
    /*! \brief Popis v bázové třídě Figure.
     */
    virtual bool transformToQueen() const;

    /*! \brief Destruktor.
     *
     * Destruktor není implementován.
     */
    ~Pawn() { };
};


/*! \brief Třída představující dámu.
 *
 * Třída odvozená z třídy abstraktní třídy Figure,
 * představuje jednu dámu.
 */
class Queen: public Figure
{
public:
    //using Figure::Figure; /* inheriting constructors, GCC 4.8 */

    /*! \brief Konstruktor.
     *
     * Jediný konstruktor, invokuje konstruktor rodičovské třídy
     * Figure.
     */
    Queen(Position&, bool col = true);

    /*! \brief Popis v bázové třídě Figure.
     */
    virtual std::vector<Position*> movePositions() const;
    /*! \brief Popis v bázové třídě Figure.
     */
    virtual std::vector<Position*> jumpPositions() const;
    /*! \brief Popis v bázové třídě Figure.
     */
    virtual const char* getSymbol() const;

    /*! \brief Destruktor.
     *
     * Destruktor není implementován.
     */
    ~Queen() { };
};

class Player;


/*! \brief Třída představující hru.
 *
 *  Game zná hrací desku a hráče, včetně jejich typů.
 */
class Game
{
    Desk* desk;
    std::vector<Figure*> white;
    std::vector<Figure*> black;
    Player* whitePlayer = nullptr;
    Player* blackPlayer = nullptr;
    Player* playerOnTurn = nullptr;

    int rowsOfFigures;
    bool whitesTurn = true;
    bool whiteWon = false;

    std::string hostIP;
    quint16 hostPort;

public:
    enum PlayerType
    {
        HUMAN,
        CPU,
        NETWORKMASTER,
        NETWORKSLAVE,
        NETWORKRECEIVER,
    };

    enum GameType
    {
        SMALL,
        CLASSIC
    };

    /*! \brief Konstruktor.
     */
    Game(Desk* , PlayerType, PlayerType, std::string = "127.0.0.1", quint16 = 0,
         GameType = CLASSIC);
    /*! \brief True pokud může bílý skákat.
     */
    bool canJumpWhite() const;
    /*! \brief True pokud může černý skákat.
     */
    bool canJumpBlack() const;

    /*! \brief Vrací vektor hýbatelných figur bílého.
     */
    std::vector<Figure*> movableWhiteFigures() const;
    /*! \brief Vrací vektor hýbatelných figur černého.
     */
    std::vector<Figure*> movableBlackFigures() const;
    /*! \brief Vrací vektor skákatelných figur bílého.
     */
    std::vector<Figure*> jumpableWhiteFigures() const;
    /*! \brief Vrací vektor skákatelných figur černého.
     */
    std::vector<Figure*> jumpableBlackFigures() const;

    /*! \brief Transformuje adekvátní pěšce na dámy.
     */
    void pawnToQueen();

    /*! \brief True, pokud je hra u konce.
     */
    bool finished();

    /*! \brief True, pokud vyhrál bílý hráč.
     */
    bool isWinnerWhite() const
    {
        return whiteWon;
    }

    /*! \brief Vrací hráče na tahu.
     */
    const Player* getPlayerOnTurn() const;

    /*! \brief Zajistí přepnutí hry na další kolo.
     */
    void nextTurn();

    /*! \brief Zajistí odehrání jednoho kola jednoho hráče.
     */
    void playRound();

    /*! \brief Vrací IP adresu pro síťovou hru.
     */
    std::string getIP()
    {
        if (hostIP.empty())
            exit(2);

        return hostIP;
    }

    /*! \brief Vrací port pro síťovou hru.
     */
    quint16 getPort()
    {
        if (hostPort == 0)
            exit(2);

        return hostPort;
    }

    std::string oldPos;
    std::string newPos;
    ~Game();
};

#endif //BASIS_H
