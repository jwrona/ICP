#include <iostream>
#include <QFile>
#include "loadFile.h"
#include <string>
#include <QString>
#include <fstream>


void loadFile::loadParseFile(std::string fileName)
 { 
   //kontrola typu souboru
   if(fileName.substr(fileName.find_last_of(".") + 1) == "txt")
   { 
     parseTxt(fileName);
     
   }else if (fileName.substr(fileName.find_last_of(".") + 1) == "xml")
    {
      parseXML(QString::fromStdString(fileName));
    }else
      {
        std::cerr << "Spatny typ souboru";
        return;
      }
  
  
  
    
 }
 
void loadFile::parseXML(QString fileName)
 {
   QDomDocument document;
   
   QFile file(fileName);
   
   //otevreme prislusny xml soubor
   if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
   {
    
     std::cerr << "Nepovedlo se otevrit soubor \n";  
     return;
   }else
    { 
     //nacteme XML dokument 
     if(!document.setContent(&file))
     {
       std::cerr << "Nepovedlo se nacist XML dokument \n";
       return;
     }
     
     //zavreme souboru    
     file.close();
    
     //root element
     QDomElement root = document.firstChildElement();
     
     //nacteme vsechny tahy
     QDomNodeList items = root.elementsByTagName("move");
     
     QDomNode itemNode;
     
     //slouzi pro spojeni tahu
     std::string help;
     
     //prochazime jednotlive tahy
     for(int i = 0; i < items.count(); i++)
     {  
        itemNode = items.at(i);
        if(itemNode.isElement())
        { 
          QDomElement itemElement = itemNode.toElement();
          //spojime jednotlive tahy
          help = itemElement.attribute("OldPos").toStdString() +
                 itemElement.attribute("NewPos").toStdString();
                 
          //ulozime do vectoru       
          moves.push_back(help);
        }
        
     }
    } 
    
  return;
 }

void loadFile::parseTxt(std::string fileName)
 {
   
   std::ifstream infile(fileName);
   std::string cMove, firstPlayer, secondPlayer;
   //nacteni vsech radku souboru
   while (infile >> cMove >> firstPlayer >> secondPlayer)
   { 
     //nahrazeni pomlcky
     firstPlayer.replace(2, 1,"");
     moves.push_back(firstPlayer);
     secondPlayer.replace(2, 1,"");
     moves.push_back(secondPlayer);
   }
  
  /*for (unsigned int i = 0; i< moves.size();  i++) {
        std::cout << "Element " << i << ": " << (moves.at(i))[0] << std::endl;
  }*/
  
    
 }
