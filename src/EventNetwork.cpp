#include "EventNetwork.h"
#include "fonctions.h"
#include "SuperClass.h"
#include <iostream>

using namespace sf;
using namespace std;

void EventNetwork::addEvent(sf::Int32 typepacket, sf::Int32 identifiant, sf::Int32 etat, sf::Int32 x, sf::Int32 y, sf::Int32 etat2, std::string pseudo, std::string message)
{
    Packet event;

    if(typepacket == CHAT)
        event << typepacket << identifiant << etat << x << y << etat2 << pseudo << message;
    else
        event << typepacket << identifiant << etat << x << y << etat2 << "" << "";

    if(typepacket != MOUVEMENT)
    cout << typepacket << identifiant << etat << x << y << etat2 << "" << "" << endl;


    BombermanManager::Get().MutexLock();
    mEvent.push_back(event);
    BombermanManager::Get().MutexUnLock();
}


void EventNetwork::removeAllEvent() { mEvent.clear();}

int EventNetwork::getNbEvent() { return mEvent.size(); }
Packet EventNetwork::getEventAt(int indice) {  return mEvent.at(indice); }

