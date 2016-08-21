#include "saveGame.h"
#include <ctime>
#include <time.h>
#include <iostream>
#include <QFile>



saveGame::saveGame()
{ 
  saveInTxt = "";
  
  //vytvori rodicovsky element
  root = document.createElement("game");
  document.appendChild(root);
  writeLine = false;
  counter = 1;
  
}
     
void saveGame::insertMove(bool player)
{ 
  //XML
  
  //vytvorime novy tah
  QDomElement move = document.createElement("move");
  
  //pridame, kdo ho provedl 
  move.setAttribute("color", QString::number(player));
  
  //stara pozice
  move.setAttribute("OldPos", oldPos);

  //nova pozice
  move.setAttribute("NewPos", newPos);
  root.appendChild(move);
  
  //txt
  
  //zda vypisujeme cislo radku
  if(!writeLine)
  {
    saveMov = QString::number(counter);
    saveMov += ". ";
  }  
  
  //ulozime do promenne stary, novy tah
  saveMov += oldPos; 
  saveMov += "-";
  saveMov += newPos;
  //saveInTxt += newPos;
  
  //pokud bude potreba novy radek
  if(writeLine)
  {
    saveMov += "\n";
    saveInTxt += saveMov;
    writeLine = false;
    counter++;
  }else
   {
     saveMov += " ";
     writeLine = true;
   }
  
   //oldPos = newPos = "";
}

int saveGame::saveIntoFile( bool fileOrXml)
{  
  unsigned long countFile = 0;
  QString tmpName = ""; 
  //budeme generovat xml  
  if(fileOrXml)
  { 
    
    while(1)
    { 
      tmpName = "hra"+ QString::number(countFile) +".xml";
      QFile fileHelp(tmpName);
      if(!fileHelp.open(QIODevice::ReadOnly | QIODevice::Text))
      {
       //std::cerr << "Nepovedlo se otevrit soubor pro zapis \n"; 
       break;
      }else
       {
         fileHelp.close();
       }
       countFile++;
    }
    
    QFile file(tmpName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
     //std::cerr << "Nepovedlo se otevrit soubor pro zapis \n"; 
     return -1;
    }

    QTextStream stream (&file);
    stream << document.toString();
    file.close();
  }else
   {
     while(1)
    { 
      tmpName = "hra"+ QString::number(countFile) +".txt";
      QFile fileHelp(tmpName);
      if(!fileHelp.open(QIODevice::ReadOnly | QIODevice::Text))
      {
       //std::cerr << "Nepovedlo se otevrit soubor pro zapis \n"; 
       break;
      }else
       {
         fileHelp.close();
       }
       countFile++;
    }
    
    QFile file(tmpName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
     //std::cerr << "Nepovedlo se otevrit soubor pro zapis \n"; 
     return -1;
    }

    QTextStream stream (&file);
    stream << saveInTxt;
    file.close();
   } 
  return 0;
}


QString saveGame::getTime()
{ 
  time_t rawtime;
  time ( &rawtime );
  return ctime (&rawtime);
}

