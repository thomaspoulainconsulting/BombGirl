#ifndef DEF_EVENT_NETWORK
#define DEF_EVENT_NETWORK
#include <vector>
#include <SFML/Network.hpp>
#include "fonctions.h"



class EventNetwork{
private:
    std::vector<sf::Packet> mEvent;

public:
    void addEvent(sf::Int32 typepacket, sf::Int32 identifiant=VIDE, sf::Int32 etat=VIDE, sf::Int32 x=VIDE, sf::Int32 y=VIDE, sf::Int32 etat2=VIDE, std::string pseudo="", std::string message="");
    void removeAllEvent();

    int getNbEvent();
    sf::Packet getEventAt(int indice);


};


#endif

