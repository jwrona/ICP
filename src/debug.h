#include "basis.h"

#include <iostream>
#include <iomanip>
using namespace std;

void printDesk(const Desk& d)
{
    for (int r = d.getDim() - 1; r >= 0; r--)
    {
        cout << setw(2) << r << " |";

        for (int c = 0; c < d.getDim(); c++)
        {
            Position pos = d.getPositionAt(c, r);
            cout << setw(2);

            if (pos.getFigure() == nullptr)
            {
                if ((r % 2) && (c % 2)) // pri lichem radku a sloupci
                    cout << '#';

                else if (!(r % 2) && !(c % 2)) //a sudem radku a sloupci
                    cout << '#';

                else
                    cout << ' '; //jinak
            }

            else
            {
                cout << setw(4) << pos.getFigure()->getSymbol();
            }
        }

        cout << " |" << endl;
    }

    cout << "    ";

    for (char i = 'a'; i < (d.getDim() + 'a'); i++)
        cout << setw(2) << i;

    cout << endl;
}


void printPosition(const Position& p)
{
    cout << "Col: " << p.getColumn() << "\tRow: " << p.getRow() << "\tAddr: " << &p
         << endl;
}

void printPositions(Desk& desk)
{
    for (int c = 0; c < desk.getDim(); c++)
    {
        for (int r = 0; r < desk.getDim(); r++)
        {
            printPosition(desk.getPositionAt(c, r));
        }
    }
}

void printFigure(const Figure* f)
{
    cout << "\tWhite: " << boolalpha << f->isWhite() << endl;
}

void printFigures(const Desk& desk)
{
    for (int c = 0; c < desk.getDim(); c++)
    {
        for (int r = 0; r < desk.getDim(); r++)
        {
            cout << "Col: " << c << "\tRow: " << r;

            if (desk.getFigureAt(c, r) == nullptr)
                cout << "\tNo figure" << endl;

            else
                printFigure(desk.getFigureAt(c, r));
        }
    }
}
