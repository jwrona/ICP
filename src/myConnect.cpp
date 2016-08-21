#include "myConnect.h"

#include <QtNetwork>
#include <QDataStream>
#include <QObject>

#include <iostream>
#include <cstdlib>

//konstruktor
myConnect::myConnect()
{
    tcpServer = new QTcpServer();
    tcpSocket = new QTcpSocket();

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));
}


//klient
void myConnect::connectToHost(QString host, quint16 port )
{
    QHostAddress serverAddr( host );
    //pokud bylo spojeni, ukoncime ho
    tcpSocket->abort();
    tcpSocket->connectToHost(host, port);

    if (!tcpSocket->waitForConnected(60000))
    {
        std::cerr << "Nepovedlo se pripojit k protihraci!\n";
        exit(2);
    }
}

//start servru
void myConnect::startServer()
{
    //poslouchame
    if (!tcpServer->listen())
    {
        std::cerr << "Nepovedlo se vytvorit hru!\n";
        exit(2);
        tcpServer->close();
        return;
    }

    QString ipAddr;
    QList<QHostAddress> ipAddrList = QNetworkInterface::allAddresses();

    //pouzijeme prvni ne-lokalni adresu
    for (int i = 0; i < ipAddrList.size(); ++i)
    {
        if (ipAddrList.at(i) != QHostAddress::LocalHost &&
            ipAddrList.at(i).toIPv4Address())
        {
            ipAddr = ipAddrList.at(i).toString();
            break;
        }
    }

    //pokud zadna nebyla nalezena, pouzije se localhost
    if (ipAddr.isEmpty())
        ipAddr = QHostAddress(QHostAddress::LocalHost).toString();

	emit serverCreated(ipAddr, tcpServer->serverPort());

    std::cout << "IP adresa pro pripojeni: " << ipAddr.toUtf8().constData() << "\n";
    std::cout << "PORT pro pripojeni: " << tcpServer->serverPort() << "\n\n";
    std::cout << "Cekam na pripojeni protihrace ...\n";

    //cyklime, dokud nedojde k navazani spojeni
    while(1)
    {
        if(tcpServer->waitForNewConnection(60000))
            break;
    }
}


//prijimani dat
QString myConnect::readData()
{
    //pocet bytu, ktere se budou cist
    int bytesAvail = 0;
    recvData.clear();

    while(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        if(tcpSocket->waitForReadyRead(10000))
        {
            bytesAvail = tcpSocket->bytesAvailable();
            break;
        }
    }

    if (bytesAvail > 0)
    {
        int cnt = 0;
        bool endOfStream = false;

        while (cnt < bytesAvail && (!endOfStream))
        {
            char ch;
            int bytesRead = tcpSocket->read(&ch, sizeof( ch ));

            if (bytesRead == sizeof( ch ))
            {
                cnt++;
                recvData.append( ch );
            }

            else
            {
                endOfStream = true;
            }
        }
    }

    else
    {
        std::cerr << "Chyba v komunikaci - nepovedlo se precit data!\n";
        exit(2);
    }

    isWrite = true;
    return recvData;
}

//zapsani dat
void myConnect::writeData(QString sendText)
{
    //kontrola, zda je spojeni v poradku
    if(tcpSocket->state() != QAbstractSocket::ConnectedState)
    {
        std::cerr << "Chyba v komunikaci - komunikace odpojena!\n";
        exit(2);
    }

    tcpSocket->write( sendText.toAscii() );

    if(! tcpSocket->waitForBytesWritten())
    {
        std::cerr << "Chyba v komunikaci - nepovedlo se odeslat data!\n";
        exit(2);
    }

    isWrite = false;
}


//destruktor
myConnect::~myConnect()
{
    tcpSocket->abort();
    tcpServer->close();
    delete tcpSocket;
    tcpSocket = nullptr;
    delete tcpServer;
    tcpServer = nullptr;
}
