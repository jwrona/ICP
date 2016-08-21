#ifndef PLAYER_H
#define PLAYER_H

#include "basis.h"
#include "myConnect.h"

#include <random>
#include <string>
#include <vector>
#include <iostream>

/*! \brief Abstraktní třída představující jednoho hráče.
 *
 * Hráč zná hru, ve které hraje a barvu figurek,
 * se kterými hraje.
 */
class Player
{
protected:
    Game* game;
    bool color;

    /*! \breif Konstruktor.
     */
    Player(Game*, bool);
public:
    
    /*! \brief Zjistí a vrátí pozici, odkud táhnout.
     *
     *  Dle typu objektu tato metoda zjistí ze kterého
     *  pole chce hráč táhnout, zkontroluje správnost
     *  a vrátí ukazatel na tuto pozici.
     */
    virtual Position* queryFrom(const Desk*) const = 0;
    
    /*! \brief Zjistí a vrátí pozici, na kterou táhnout.
     *
     *  Dle typu objektu tato metoda zjistí na které
     *  pole chce hráč táhnout, zkontroluje správnost
     *  a vrátí ukazatel na tuto pozici.
     */
    virtual Position* queryTo(const Desk*, Figure*) const = 0;

    /*! \brief Dekstruktor.
     */
    virtual ~Player();
};

/*! \breif Třída představující hráče člověka.
 *
 *  Bázová třída je obecný hráč Player.
 *  Popis metod hledejte u třídy Player.
 */
class HumanPlayer: public Player
{
public:
    HumanPlayer(Game*, bool);
    virtual Position* queryFrom(const Desk*) const;
    virtual Position* queryTo(const Desk*, Figure*) const;
    virtual ~HumanPlayer() = default;
};


/*! \breif Třída představující hráče pana Počítače.
 *
 *  Bázová třída je obecný hráč Player.
 *  Popis metod hledejte u třídy Player.
 */
class CPUPlayer: public Player
{
public:
    CPUPlayer(Game*, bool);
    virtual Position* queryFrom(const Desk*) const;
    virtual Position* queryTo(const Desk*, Figure*) const;
    virtual ~CPUPlayer() = default;
};

/*! \breif Třída představující hráče, který tvoří síťovou hru.
 *
 *  Bázová třída je hráč HumanPlayer.
 *  Popis metod hledejte u třídy Player.
 */
class NetworkMaster: public HumanPlayer
{
public:
    NetworkMaster(Game*, bool);
    virtual Position* queryFrom(const Desk*) const;
    virtual Position* queryTo(const Desk*, Figure*) const;
    virtual ~NetworkMaster() = default;

};

/*! \breif Třída představující hráče, který se připojuje k existující síťové hře.
 *
 *  Bázová třída je hráč HumanPlayer.
 *  Popis metod hledejte u třídy Player.
 */
class NetworkSlave: public HumanPlayer
{
public:
    NetworkSlave(Game*, bool);
    virtual Position* queryFrom(const Desk*) const;
    virtual Position* queryTo(const Desk*, Figure*) const;
    virtual ~NetworkSlave() = default;

};

/*! \breif Třída představující vzdáleného hráče.
 *
 *  Bázová třída je obecný hráč Player.
 *  Popis metod hledejte u třídy Player.
 */
class NetworkReceiver: public Player
{
public:
    NetworkReceiver(Game*, bool);
    virtual Position* queryFrom(const Desk*) const;
    virtual Position* queryTo(const Desk*, Figure*) const;
    virtual ~NetworkReceiver() = default;

};

#endif //PLAYER_H
