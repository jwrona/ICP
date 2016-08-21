#ifndef MYCONNECT_H
#define MYCONNECT_H


#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QNetworkSession>


/*!\brief Třída zajišťující konzolovou komunikaci.
*
* Třída je použita jak na straně hráče, který vytváří hru (server),
* tak i na straně hráče, který se připojuje (klienta).
*/
class myConnect: public QObject
{
    Q_OBJECT

public:
    bool isWrite; //!< Zda data bude zapisovat nebo bude číst


    /*! \brief Konstruktor.
    *
    * Alokuje paměť pro klienta a server
    */
    myConnect();


    /*! \brief Připojení klienta k servru.
    *
    *  Provede připojení klienta k servru.
    *
    * \param[in] ip adresa hosta, ke kterému se připojuje
    * \param[in] port, na ktery se pripojuje
    */
    void connectToHost(QString, quint16);


    /*! \brief Vytvoření servru.
    *
    *  Dojde k vytvoření servru, který poslouchá na localní stanici a
    *  čeká na připojení klienta.
    */
    void startServer();


    /*! \brief Odesílání dat přes soket.
    *
    *  Zkontroluje, zda nedošlo k přerušení spojení a provede odesílání
    *  dat.
    *
    * \param[in] text, který se odesílá
    */
    void writeData(QString);


    /*! \brief Čtení dat přes soket.
    *
    *  Zkontroluje, zda nedošlo k přerušení spojení a čte příchozí data.
    *
    * \return vrací přijatý text
    */
    QString readData();


    /*! \brief Destruktor
    *
    *  Uvolní alokovanou paměť.
    */
    ~myConnect();



private:
    QTcpSocket* tcpSocket; //!< Socket, přes který se komunikuje
    QString recvData;  //!< Pro uložení příchozího textu
    QTcpServer* tcpServer; //!< Server, který přijímá spojení

signals:
    void serverCreated(QString IP, quint16 port);
    void connectionAccepted();

private slots:
    void acceptConnection()
    {
        //prijmeme spojeni
        tcpSocket = tcpServer->nextPendingConnection();
	emit connectionAccepted();
    }
};

#endif  //MYCONNECT_H

