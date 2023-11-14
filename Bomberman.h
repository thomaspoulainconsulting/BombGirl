#ifndef DEF_BOMBERMAN
#define DEF_BOMBERMAN
#include <SFML/Graphics.hpp>
#include "Damier.h"
#include "ImgAnim.h"

class Bomberman
{
private:
    int mX;
    int mY;
    float mVitesse;
    int mNbBombe;
    int mPorteeBombe;
    int mIndexVector;
    int mCoordBombePosee[2];
    int mVie;
    int indexItemBombeDontBombByOwnBomb;
    bool mVisible;
    bool mTimerActive;
    bool mDontBombByOwnBomb;
    bool OnGlace;
    bool TP;
    bool mMort;
    int DirectionGlissade;
    int mDirection;
    sf::String mNom, mMenuGaucheNBBombe, mMenuGauchePortee;
    sf::Image image;
    sf::Sprite sprite;
    sf::Font font;
    sf::Clock timer, timerPose, timerDontBombByOwnBomb;
    std::vector<int> mImageItem;
    ImgAnim mAnimBomberman;
    std::string mNomString;



public:
    Bomberman(std::string nom);
    bool collisionDecor(int direction); // Si deplacement
    bool collisionBombe(int direction);
    void collisionItem(int direction);
    bool collisionGlace(int direction);
    bool collisionTeleporteur(int direction);


    void deplacer(float x,float y);
    void afficher();

    void changerVisibilite(bool visible);
    void changerVitesse(float vitesse);
    void use_item(std::string nomItem);
    void mort();

    void AnimStop();


    void setBombePosee(bool valeur);
    void setCoord(int x, int y);
    void resetIndexItemBombeDontBombByOwnBomb();

    int getX();
    int getY();
    int getCouleur();
    int getNbBombe();
    float getVitesse();
    int getPortee();
    bool getVisible();
    bool getBombePosee();
    bool getDontBombByOwnBomb();
    int getIndexVector();
    int getDirection();
    sf::Sprite getSprite();
    sf::Image getImage();
    sf::Font getFont();
    std::string getNom();
    void setFrame(int animrow, int frame);

    void poserBombe();
    bool poserBombe(int x, int y, int id, int portee);
    void incrementeNbBombe();
    void decrementeNbBombe();
    void SetMort(bool mmort);


};

#endif
