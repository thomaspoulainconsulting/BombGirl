#ifndef DEF_MENU
#define DEF_MENU
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>


void modifie(int &x, int value);
void ecriture(std::string texte, std::string chemin);
std::string lecture(std::string chemin);
std::string changerNom(sf::View &view, std::string pseudo);
int launcher(sf::View &view);
int afficherMenu(sf::View &view);
std::string entrerIPServeur(sf::View &view, std::string ipserveur);
void afficher_info(std::string type, std::string text);



#endif
