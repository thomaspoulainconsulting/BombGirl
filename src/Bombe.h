#ifndef DEF_BOMBE
#define DEF_BOMBE
#include "Bomberman.h"
#include "Item.h"
#include "ImgAnim.h"
#include "AniSprite.hpp"

class Bombe : public Item
{
private:
    int mPortee;
    int tab[4];
    static int mrandom;
    sf::Clock timer, timerGlisse;
    sf::Image image, imageExplose, imageExplose1;
    sf::Sprite sprite, spriteExplose, spriteExplose1;
    int bbm;
    bool destru[4]; // Indique si un bloc doit etre cassé par la bombe
    bool exploser; // Signifie que la bombe doit etre détruite
    int mIdentifiant;
    ImgAnim mAnimBombe;
    AniSprite mAniSprite;

public:
    Bombe(int x,int y, int portee, int bbm, int id_bombe);
    int collision(int x, int y);
    void glisserBombe(int direction, int index);
    void explose();
    void afficher(int i);
    void larguerItem(int x, int y);
    int getProba(int j, int repetition_proba1=0, int indice_tab1=0, int repetition_proba2=0, int indice_tab2=0, int repetition_proba3=0, int indice_tab3=0);
    void destruction(int index); // Pour détruire les blocs cassables en fin de vie

    int getPortee();
    sf::Sprite getSprite();
    int getX();
    int getY();
    int getIMod(int &i);

    void setX(int x);
    void setY(int y);
};

#endif
