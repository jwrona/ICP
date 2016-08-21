#ifndef SAVEGAME_H
 #define SAVEGAME_H
 
  #include <QDomDocument>
  #include <QtXml>
  #include "QString"
 
  /*!\brief Třída zajišťující ukládání hry.
  *
  * Tříd zajišťuje postupné ukládání tahů do a také výsledné zapsání do 
  * souboru typu XML nebo txt.
  */
 
 class saveGame
  {
    public:
     
     /*! \brief Konstruktor.
     *
     * Nastaví počítadlo tahů na 1, vynuluje řetězec pro ukládání tahů,
     * vytvoří nový rodičovský element pro XML soubor, nastavení proměnnou
     * writeLine na false 
     * 
     */
     saveGame();
     
     
     /*! \brief Provede uložení příslušného tahu.
     *
     * Ukládá postupně jednotlivé tahy do XML, tak i do textové podoby.
     * 
     * \param[in] pokud táhl bílý, je argument true, pokud černý je false
     * \param[in] počáteční pozice figurky
     * \param[in] pozice, na kterou se táhlo
     */
     void insertMove(bool);
     
     
     /*! \brief Provede uložení tahů do souboru
     *
     * Vytvoří soubor *.XML nebo *.txt a provede zápis uložených tahů
     * 
     * \param[in] pokud je true, zapisuje se do XML, pokud false, tak do txt
     */ 
     int saveIntoFile(bool);
     
     //vraci aktualni cas
     QString getTime();
     
      
     QDomDocument document; //!< hlavní dokument XML
     QDomElement root; //!< rodičovský element v XML
     QString saveInTxt; //!< seznam tahů, slouží pro uložení do textového souboru
     QString saveMov; //!< pomocna pro uloženi do txt
     bool writeLine; //!< zda vypisujeme nový řádek
     QString oldPos;
     QString newPos;
     
   private:
      int counter; //!< čítač jednotlivých tahů
     
   
  };




#endif
