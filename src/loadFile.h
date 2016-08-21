#ifndef LOADFILE_H
 #define LOADFILE_H
 
  #include <QDomDocument>
  #include <QtXml>
  #include "QString"
 
  /*!\brief Třída načtení a zpracování uložené hry v souboru.
  *
  * Třída zajišťuje kontrolu typu načítaného souboru, následně ho
  * zpracuje a jednotlivé kroky jsou uloženy.
  */
 
 class loadFile
  {
    public:
     
     /*! \brief Provede kontrolu typů souborů a volá příslušný parser
     *
     * Metoda podporuje soubory *.txt nebo *.xml. Soubor musí být uložený
     * ve stejném adresářovém prostoru jak program.
     * 
     * \param[in] Nazev vstupniho souboru
     */ 
     void loadParseFile(std::string);
     
     
    /*! \brief Provede zpracování xml souboru
     *
     * Metoda zpracuje uložený soubor příslušné hry a jednotlivé kroky 
     * uloží do vektoru stringů.
     * 
     * \param[in] Nazev vstupniho souboru
     */ 
     void parseXML(QString);
     
     /*! \brief Provede zpracování txt souboru
     *
     * Metoda zpracuje uložený soubor příslušné hry a jednotlivé kroky 
     * uloží do vektoru stringů.
     * 
     * \param[in] Nazev vstupniho souboru
     */
     void parseTxt(std::string);
     
     
     //tahy jsou ukladany ve tvaru a4b5, kde a4 je pocatecni souradnice
     //a b5 je cilova souradnice 
     std::vector <std::string> moves;  //!< seznam jednotlivych tahu    
    
     
   
  };




#endif
