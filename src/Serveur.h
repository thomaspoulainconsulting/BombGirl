#ifndef SERVEUR
#define SERVEUR
#include <SFML/Network.hpp>

class Serveur
{
private:
    std::vector<sf::SocketTCP> mClients;
    int mNbJoueurs;
    int mMaxNbJoueurs;
    std::string mNomMap;
public:
    Serveur(int nbjoueur, int nbmaxjoueur, std::string nommap);

    void incrementeNbJoueurs();
    void decrementeNbJoueurs();
    void addClient(sf::SocketTCP addr);

    sf::SocketTCP getClient(int indice);
    void eraseClient(int indice);
    std::string getNomMap();
    int getNbMaxJoueurs();
    int getNbJoueurs();

    void deleteClient();
};

void Serveur_Complet(void* UserData);
#endif
