#include <iostream>
#include <string>
#include "Chat.hpp"
#include "SuperClass.h"
#include "fonctions.h"


using namespace sf;
using namespace std;

const int PORT_CHAT_TCP = 61586;

Chat::Chat()
{

}

void Chat::addText()
{

    if(mTextEnCours == "" || mTextEnCours == "\n")
        return;

    if(mText.size() >= 57)
    {
        mPseudos.erase(mPseudos.begin());
        mText.erase(mText.begin());
    }


    mPseudos.push_back(BombermanManager::Get().GetPseudo());
    mText.push_back(mTextEnCours);

    BombermanManager::Get().GetEvent()->addEvent(CHAT, BombermanManager::Get().getIdentifiant(), -1, -1, -1, -1, BombermanManager::Get().GetPseudo(), mTextEnCours);

}

void Chat::addText(std::string pseudo, std::string texte)
{
    if(mText.size() >= 57)
    {
        mPseudos.erase(mPseudos.begin());
        mText.erase(mText.begin());
    }

    mPseudos.push_back(pseudo);
    mText.push_back(texte);
}

void Chat::afficher()
{
    Font police;
    Shape barre_ecriture;
    sf::String textaafficher, pseudo_affiche_en_bas;

    chargerFontFromMemory(police, mStringEnCours, "police/Balkeno.ttf", "", 12);
    chargerFontFromMemory(police, textaafficher, "police/Balkeno.ttf", "", 12);
    chargerFontFromMemory(police, pseudo_affiche_en_bas, "police/Balkeno.ttf", "", 12);

    barre_ecriture.SetOutlineWidth(2);
    barre_ecriture.EnableFill(false);
    barre_ecriture.AddPoint(Vector2f(75, 660), Color(255, 255, 255), Color(255, 255, 255));
    barre_ecriture.AddPoint(Vector2f(575, 660), Color(255, 255, 255), Color(255, 255, 255));
    barre_ecriture.AddPoint(Vector2f(575, 676), Color(255, 255, 255), Color(255, 255, 255));
    barre_ecriture.AddPoint(Vector2f(75, 676), Color(255, 255, 255), Color(255, 255, 255));

    BombermanManager::Get().GetApp()->Draw(barre_ecriture);

    //texte écrit en cours
    mStringEnCours.SetText(mTextEnCours);
    mStringEnCours.SetPosition(78, 660);
    BombermanManager::Get().GetApp()->Draw(mStringEnCours);

    //pseudo
    pseudo_affiche_en_bas.SetText(BombermanManager::Get().GetPseudo());
    pseudo_affiche_en_bas.SetPosition(10, 660);
    pseudo_affiche_en_bas.SetColor(getColorFromPseudo(BombermanManager::Get().GetPseudo()));
    BombermanManager::Get().GetApp()->Draw(pseudo_affiche_en_bas);


    /*on parcours le vector et on affiche*/
    for(int i=0;i<(int)mText.size();i++)
    {
        textaafficher.SetText(mPseudos.at(i));
        textaafficher.SetPosition(60, 70+(i+1)*10);
        textaafficher.SetColor(getColorFromPseudo(mPseudos.at(i)));
        BombermanManager::Get().GetApp()->Draw(textaafficher);

        int longueurpseudo = textaafficher.GetSize();
        textaafficher.SetText(" : "+mText.at(i)+"\n");
        textaafficher.SetPosition(60+longueurpseudo*3+3, 70+(i+1)*10);
        textaafficher.SetColor(Color(255, 255, 255));
        BombermanManager::Get().GetApp()->Draw(textaafficher);
    }

}


void Chat::updateTextEnCours(char text)
{
    mTextEnCours+=static_cast<char>(text);
}

void Chat::eraseTextEnCours()
{
    mTextEnCours="";
}

void Chat::retourArriere()
{
    mTextEnCours=mTextEnCours.substr(0, mTextEnCours.size()-1);
}

int Chat::getSizeTextEnCours() { return mTextEnCours.size(); }



Color Chat::getColorFromPseudo(std::string pseudo)
{
    string rouge(pseudo+"rouge");
    string vert(pseudo+"vert");
    string bleu(pseudo+"bleu");

    int somme_rouge=0, somme_vert=0, somme_bleu=0;

    for(int i=0;i<(int)rouge.size();i++)
        somme_rouge+=(int)rouge.at(i);

    for(int i=0;i<(int)vert.size();i++)
        somme_vert+=(int)vert.at(i);

    for(int i=0;i<(int)bleu.size();i++)
        somme_bleu+=(int)bleu.at(i);

    return Color(somme_rouge%255, somme_vert%255, somme_bleu%255);
}

/*void Chat::addClient(sf::SocketTCP addr){ mClients.push_back(addr);}
int Chat::getNbClients() { return mClients.size(); }
sf::SocketTCP Chat::getClient(int indice){return mClients.at(indice);}*/
