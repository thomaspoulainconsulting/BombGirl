#ifndef RESEAU
#define RESEAU
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

class Reseau
{
public:
    Reseau();
    void lancerServeur();
    void lancerClient();
    void ecouteBroadCast();
    void envoiBroadCast();


private:
    std::vector<std::string> AddrClients;
    std::string AddrServeur;

};

#endif
