#include "player.h"

std::random_device rd;
std::mt19937_64 rndGen(rd());

myConnect network;

Player::Player(Game* g, bool isWhite):
    game(g),
    color(isWhite)
{
}

Player::~Player()
{
}

HumanPlayer::HumanPlayer(Game* g, bool isWhite): Player(g, isWhite)
{
}

Position* HumanPlayer::queryFrom(const Desk* d) const
{
    std::cout << "Zadejte startovní souřadnice (např. a0):" << std::endl;
    char tmpCol;
    int row;
    std::cin >> tmpCol >> row;
    int col = tmpCol - 'a';
    return &(d->getPositionAt(col, row));
}

Position* HumanPlayer::queryTo(const Desk* d, Figure*) const
{
    std::cout << "Zadejte cílové souřadnice (např. b1):" << std::endl;
    char tmpCol;
    int row;
    std::cin >> tmpCol >> row;
    int col = tmpCol - 'a';
    return &(d->getPositionAt(col, row));
}

CPUPlayer::CPUPlayer(Game* g, bool isWhite): Player(g, isWhite)
{
}

Position* CPUPlayer::queryFrom(const Desk*) const
{
    std::vector<Figure*> figVec;

    if (color) //tah bileho
    {
        if (game->canJumpWhite()) //bily ma moznost skoku
            figVec = game->jumpableWhiteFigures();

        else //bily nema moznost skoku
            figVec = game->movableWhiteFigures();
    }

    else //tah cerneho
    {
        if (game->canJumpBlack()) //cerny ma moznost skoku
            figVec = game->jumpableBlackFigures();

        else //cerny nema moznost skoku
            figVec = game->movableBlackFigures();
    }

    std::uniform_int_distribution<unsigned int> dist(0, figVec.size() - 1);
    return figVec[dist(rndGen)]->getPosition();
}

Position* CPUPlayer::queryTo(const Desk*, Figure* f) const
{
    std::vector<Position*> posVec;

    if (f->canJump()) //figurka muze skakat
        posVec = f->jumpPositions();

    else //figurka nemuze skakat
        posVec = f->movePositions();

    std::uniform_int_distribution<unsigned int> dist(0, posVec.size() - 1);
    return posVec[dist(rndGen)];
}

NetworkMaster::NetworkMaster(Game* g, bool isWhite): HumanPlayer(g, isWhite)
{
    network.startServer();
}

Position* NetworkMaster::queryFrom(const Desk* d) const
{
    Position* from = HumanPlayer::queryFrom(d);
    QString sendMsg;
    sendMsg.append(QString::number(from->getColumn()));
    sendMsg.append(QString::number(from->getRow()));
    network.writeData(sendMsg);
    return from;
}

Position* NetworkMaster::queryTo(const Desk* d, Figure* f) const
{
    Position* to = HumanPlayer::queryTo(d, f);

    QString sendMsg;
    sendMsg.append(QString::number(to->getColumn()));
    sendMsg.append(QString::number(to->getRow()));
    network.writeData(sendMsg);
    return to;
}

NetworkSlave::NetworkSlave(Game* g, bool isWhite): HumanPlayer(g, isWhite)
{
    network.connectToHost(QString::fromStdString(g->getIP()), g->getPort());
}

Position* NetworkSlave::queryFrom(const Desk* d) const
{
    Position* from = HumanPlayer::queryFrom(d);
    QString sendMsg;
    sendMsg.append(QString::number(from->getColumn()));
    sendMsg.append(QString::number(from->getRow()));
    network.writeData(sendMsg);
    return from;
}

Position* NetworkSlave::queryTo(const Desk* d, Figure* f) const
{
    Position* to = HumanPlayer::queryTo(d, f);

    QString sendMsg;
    sendMsg.append(QString::number(to->getColumn()));
    sendMsg.append(QString::number(to->getRow()));
    network.writeData(sendMsg);
    return to;
}

NetworkReceiver::NetworkReceiver(Game* g, bool isWhite): Player(g, isWhite)
{
}

Position* NetworkReceiver::queryFrom(const Desk* d) const
{
    //cteni dat
    QString recvData = network.readData();
    int col = recvData[0].digitValue();
    int row = recvData[1].digitValue();
    return &d->getPositionAt(col, row);
}

Position* NetworkReceiver::queryTo(const Desk* d, Figure*) const
{
    return queryFrom(d);
}
