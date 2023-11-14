#ifndef DEF_ITEM
#define DEF_ITEM
#include <SFML/Graphics.hpp>

std::string getCorrespondance(int identifiant);

enum {ITEM_BOMBE, ITEM_BOMBE_PLUS, ITEM_PORTEE_PLUS, ITEM_PORTEE_MOINS, ITEM_PORTEE_MAX, ITEM_VITESSE_PLUS, ITEM_VITESSE_MOINS, ITEM_COEUR, ITEM_PROTECTION};

class Item
{
protected:
    int mX;
    int mY;
    int mType;
    sf::Image image;
    sf::Sprite sprite;

public:
    Item(int x, int y, int icone, bool transparence=false);

    void afficher();

    int getX();
    int getY();
    int getType();

};


#endif
