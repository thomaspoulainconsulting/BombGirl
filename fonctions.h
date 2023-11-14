#ifndef DEF_FONCTIONS
#define DEF_FONCTIONS
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>

const int DEPLACEMENT=40.0; // TAILLE EN PX DES CASES
const int HAUTEUR_BBM =80; // HAUTEUR DU SPRITE BBM
const int LARGEUR_BBM= 40; // LARGEUR DU SPRITE BBM
const int VIDE_BBM =5;  // VIDE DE CHAQUE COTE DU BBM
const int COLLISION_BBM =30; // TAILLE PRISE PAR BBM SUR LE DAMIER POUR LES COLLISIONS

const int GESTION_BBM_MENU =80; // EN HAUT
const int GESTION_ITEM_MENU= 50; //A GAUCHE

const int HAUTEUR_MENU =15;
const int LARGEUR_MENU =15;

/*CONSTANTES RESEAU*/
const sf::Int32 MOUVEMENT = 0;
const sf::Int32 BOMBE = 1;
const sf::Int32 ITEM = 2;
const sf::Int32 ETAT_JOUEUR = 3;
const sf::Int32 GAMEOVER = 5;
const sf::Int32 CHAT = 7;
const sf::Int32 DEBUT_PARTIE=8;
const sf::Int32 MORT_JOUEUR_AVANT_PARTIE=9;

const sf::Int32 VIDE = -1;


const sf::Int32 REQUETE = 0;
const sf::Int32 REPONSE = 1;
const sf::Int32 DEPLACER = 2;

const sf::Int32 DEPOSE = 0;
const sf::Int32 PRIS = 1;

const sf::Int32 MORT = 0;
const sf::Int32 VIVANT = 1;

const std::string REPERTOIRE_MAP="map/";

void chargerImageFromMemory(sf::Image &image, sf::Sprite &sprite, std::string nomImage, bool transparence=false, bool setsmooth=false, bool ressources=false, std::string nomMap="", bool alpha=false, bool memeNomMaisImageDifferente=false);
void chargerFontFromMemory(sf::Font &font, sf::String &mNom, std::string nomFont, std::string nomTexte, int taille=14);
bool is_valid_ip(const char *ip_str);

#endif
