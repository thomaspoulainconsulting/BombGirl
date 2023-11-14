#ifndef CLIENT
#define CLIENT
#include <SFML/Network.hpp>

class Client
{
private:
    sf::IPAddress mAddrServeur;
    bool mAttenteDeConnexion;
public:
    Client(sf::IPAddress serveur="");
    bool getAttenteDeConnexion();
    void setAttenteDeConnexion(bool value);
};

//fonctions externes � la classe car ex�cut� dans un thread
void Client_Complet(void* UserData);

#endif
