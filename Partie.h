#ifndef DEF_PARTIE
#define DEF_PARTIE
#include <dirent.h>
#include <SFML/Graphics.hpp>
#include "cDAT.h"

int listerMap();
int getNbMap();

int creer_partie(sf::View &view);
int listerFichierDAT(std::string dir, std::vector<std::string> &files);
void rafraichir(std::vector<std::string> &files);

int partie(sf::View &view, bool server);

#endif
