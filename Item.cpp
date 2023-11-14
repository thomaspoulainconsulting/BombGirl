#include <iostream>
#include "Item.h"
#include "SuperClass.h"
#include "fonctions.h"
#include "ImgAnim.h"

using namespace std;
using namespace sf;

Item::Item(int x, int y, int type, bool transparence) : mX(x), mY(y), mType(type)
{
    if(type == ITEM_BOMBE_PLUS)
        chargerImageFromMemory(image, sprite, "item/NbBombePlus.png", transparence, false, true);
    else if(type == ITEM_PORTEE_PLUS)
        chargerImageFromMemory(image, sprite, "item/PorteePlus.png", transparence, false, true);
    else if(type == ITEM_PORTEE_MOINS)
        chargerImageFromMemory(image, sprite, "item/PorteeMoins.png", transparence, false, true);
    else if(type == ITEM_PORTEE_MAX)
        chargerImageFromMemory(image, sprite, "item/PorteeMax.png", transparence, false, true);
    else if(type == ITEM_VITESSE_PLUS)
        chargerImageFromMemory(image, sprite, "item/VitessePlus.png", transparence, false, true);
    else if(type == ITEM_VITESSE_MOINS)
        chargerImageFromMemory(image, sprite, "item/VitesseMoins.png", transparence, false, true);
    else if(type == ITEM_COEUR)
        chargerImageFromMemory(image, sprite, "item/Coeur.png", transparence, false, true);
    else if(type == ITEM_PROTECTION)
        chargerImageFromMemory(image, sprite, "item/ProtectOwnBomb.png", transparence, false, true);


    sprite.SetPosition(mX*DEPLACEMENT+GESTION_ITEM_MENU, mY*DEPLACEMENT+GESTION_BBM_MENU);


}



void Item::afficher()
{
    sprite.SetImage(image);
    BombermanManager::Get().GetApp()->Draw(sprite);
}


int Item::getX() { return mX; }
int Item::getY() { return mY; }
int Item::getType() { return mType; }

string getCorrespondance(int identifiant)
{
    if(identifiant == ITEM_BOMBE_PLUS)
        return "item/NbBombePlus.png";
    else if(identifiant == ITEM_COEUR)
        return "item/Coeur.png";
    else if(identifiant == ITEM_PORTEE_MAX)
        return "item/PorteeMax.png";
    else if(identifiant == ITEM_PORTEE_MOINS)
        return "item/PorteeMoins.png";
    else if(identifiant == ITEM_PORTEE_PLUS)
        return "item/PorteePlus.png";
    else if(identifiant == ITEM_PROTECTION)
        return "item/ProtectOwnBomb.png";
    else if(identifiant == ITEM_VITESSE_MOINS)
        return "item/VitesseMoins.png";
    else if(identifiant == ITEM_VITESSE_PLUS)
        return "item/VitessePlus.png";
    else return "";
}
