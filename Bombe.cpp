#include <string>
#include <iostream>
#include <SFML/Audio.hpp>
#include "Bombe.h"
#include "Bomberman.h"
#include "SuperClass.h"
#include "fonctions.h"
#include "Son.h"

const int TPS_EXPLOSION=2; // Temps avant que la bombe explose
const int DUREE_EXPLOSION=1; // Temps de durée de l'explosion

/* CONST PROBABILITE */
const float PROBA_ITEM_NORMAUX = 0.4; // Probabilité pour les items généraux
const float PROBA_ITEM_RARE = 0.2; // Probabilité pour les items rares
const float PROBA_ITEM_TRES_RARE = 0.1; // Probabilité pour les items très rares
const float PROBA_ITEM_NEPASAVOIR = 0.3; // Probabilité de ne pas avoir d'item

// VALEURS A CHANGER
const int NB_ITEM_NORMAUX = 3;
const int NB_ITEM_RARE = 2;
const int NB_ITEM_TRES_RARE = 3;
const int NB_ITEM_NEPASAVOIR = NB_ITEM_NORMAUX+NB_ITEM_RARE+NB_ITEM_TRES_RARE;

const int RANGE_TOTAL = (int)(PROBA_ITEM_NEPASAVOIR*NB_ITEM_NEPASAVOIR*NB_ITEM_NEPASAVOIR*2) + (int)(PROBA_ITEM_NORMAUX*NB_ITEM_NORMAUX*NB_ITEM_NEPASAVOIR*2*NB_ITEM_NORMAUX) + (int)(PROBA_ITEM_RARE*NB_ITEM_RARE*NB_ITEM_NEPASAVOIR*2*NB_ITEM_RARE) + (int)(PROBA_ITEM_TRES_RARE*NB_ITEM_TRES_RARE*NB_ITEM_NEPASAVOIR*2*NB_ITEM_TRES_RARE);


using namespace std;
using namespace sf;

int Bombe::mrandom=-1;



Bombe::Bombe(int x,int y, int portee, int bbg, int id_bombe) :  Item(x, y, ITEM_BOMBE, false), mPortee(portee), bbm(bbg), exploser(false), mIdentifiant(id_bombe)
{
    chargerImageFromMemory(imageExplose, spriteExplose, "ressources/Explose.png", true, true, true);
    chargerImageFromMemory(imageExplose1, spriteExplose1, "decor/BlocDestructible1.png", true, true, true);
    sprite.SetPosition(mX*DEPLACEMENT+GESTION_ITEM_MENU, mY*DEPLACEMENT+GESTION_BBM_MENU);
    timer.Reset();

    tab[0]=(int)(PROBA_ITEM_NEPASAVOIR*NB_ITEM_NEPASAVOIR*NB_ITEM_NEPASAVOIR*2);
    tab[1]=(int)(PROBA_ITEM_NORMAUX*NB_ITEM_NORMAUX*NB_ITEM_NEPASAVOIR*2);
    tab[2]=(int)(PROBA_ITEM_RARE*NB_ITEM_RARE*NB_ITEM_NEPASAVOIR*2);
    tab[3]=(int)(PROBA_ITEM_TRES_RARE*NB_ITEM_TRES_RARE*NB_ITEM_NEPASAVOIR*2);

    for(int i=0;i<4;i++)
        destru[i]=false;


    chargerImageFromMemory(image, sprite, "item/bombe_anim.png", false, true, true);

    AniSprite test(image, 40, 40);
    mAniSprite=test;
    mAniSprite.SetLoopSpeed(10);
    mAniSprite.Play();
    mAniSprite.SetPosition(x*DEPLACEMENT+GESTION_ITEM_MENU, y*DEPLACEMENT+GESTION_BBM_MENU);


}

int Bombe::collision(int x, int y)
{
    x=x/DEPLACEMENT;
    y=y/DEPLACEMENT;


    int valeur_case=BombermanManager::Get().GetDamier().getValeurCase(x,y);

    if(valeur_case == 0 || valeur_case==3)
    {
        for(int i=0;i<BombermanManager::Get().getNbBomberman();i++){ // Y de BBM devient Y+DEPLACEMENT
                // On test le X+COLLISION_BBM et Y-COLLISION_BMM
                if(x == (int)((BombermanManager::Get().GetBomberman(i)->getX()+COLLISION_BBM-GESTION_ITEM_MENU-1)/DEPLACEMENT) && y == (int)((BombermanManager::Get().GetBomberman(i)->getY()+DEPLACEMENT-COLLISION_BBM-GESTION_BBM_MENU+1)/DEPLACEMENT-1))
                    {
                        if(BombermanManager::Get().GetBomberman(i)->getDontBombByOwnBomb() == false)
                            BombermanManager::Get().GetBomberman(i)->mort(); //on check la mort que si on est client

                    }
                else
                    // On test le X et Y de BBM
                    if(x == (int)((BombermanManager::Get().GetBomberman(i)->getX()-GESTION_ITEM_MENU+1)/DEPLACEMENT) && y == (int)((BombermanManager::Get().GetBomberman(i)->getY()+DEPLACEMENT-GESTION_BBM_MENU-1)/DEPLACEMENT-1))
                        {
                            if(BombermanManager::Get().GetBomberman(i)->getDontBombByOwnBomb() == false)
                                BombermanManager::Get().GetBomberman(i)->mort();
                        }// On rajoute 1 px de marge pour que bbm ne meurt pas si il est caché derriere un bloc

        }
        return 0;
    }
    else if(valeur_case==2){
        return 2;
    }
    else return 1;

}

/*
EXPLICATIONS :
par ex pour la première condition :
la fonction getNbItemProbaNormaux(i) renvoit i+1 tout en modifiant la valeur réelle du i passé par référence
ça permet de ne pas à avoir de variable à modifier dès qu'on rajoute un item

on test d'habord la borne supérieur de l'intervale car si on rentre pas dans la première condition,
la valeur du i ne sera pas modifié car pas d'appel à getNbItemProbaNormaux(i)
et ça faussera les valeurs des intervalles dans les conditions suivantes

explications de getProba
getProba(i, 1, 1, 1, 2, 2, 3)
on boucle de tab[i]
on répète l'indice 1, 1 fois
on répète l'indice 2, 1 fois
on répète l'indice 3, 2 fois

*/
void Bombe::larguerItem(int x, int y)
{
    int random;
    int i=0;
    srand ( time(NULL) ); //initialisation de la graine


    random=rand() % RANGE_TOTAL + 1;

    if(mrandom != random)
    {
        mrandom=random;
    }
    else
    {
        while(mrandom == random)
        {
            random=rand() % RANGE_TOTAL + 1;
        }
        mrandom=random;
    }



    //PAS D'ITEM
    if(mrandom < getProba(getIMod(i)) && mrandom > getProba(i-1))
    {}
    //ITEMS NORMAUX
    else if(mrandom < getProba(getIMod(i)) && mrandom >= getProba(i-1))
    {
        BombermanManager::Get().GetEvent()->addEvent(ITEM, ITEM_BOMBE_PLUS, DEPOSE, x,y);
        BombermanManager::Get().RegisterItem(Item(x, y, ITEM_BOMBE_PLUS));
    }
    else if(mrandom < getProba(i,1,1) && mrandom >= getProba(i))
    {
        BombermanManager::Get().GetEvent()->addEvent(ITEM, ITEM_PORTEE_PLUS, DEPOSE, x,y);
        BombermanManager::Get().RegisterItem(Item(x, y, ITEM_PORTEE_PLUS));
    }
    else if(mrandom < getProba(i,2,1) && mrandom >= getProba(i))
    {
        BombermanManager::Get().GetEvent()->addEvent(ITEM, ITEM_PORTEE_MOINS, DEPOSE, x,y);
        BombermanManager::Get().RegisterItem(Item(x, y, ITEM_PORTEE_MOINS));
    }
    // ITEMS RARES
    else if(mrandom < getProba(getIMod(i), 2, 1) && mrandom >= getProba(i-1, 2, 1) )
    {
        BombermanManager::Get().GetEvent()->addEvent(ITEM, ITEM_VITESSE_PLUS, DEPOSE, x,y);
        BombermanManager::Get().RegisterItem(Item(x, y, ITEM_VITESSE_PLUS));
    }
    else if(mrandom < getProba(i, 2, 1, 1, 2) && mrandom >= getProba(i,2, 1) )
    {
        BombermanManager::Get().GetEvent()->addEvent(ITEM, ITEM_VITESSE_MOINS, DEPOSE, x,y);
        BombermanManager::Get().RegisterItem(Item(x, y, ITEM_VITESSE_MOINS));
    }
    //ITEMS TRES RARES
    else if(mrandom <= getProba(i, 2, 1, 1, 2, 1, 3) && mrandom >= getProba(i, 2, 1, 1, 2) )
    {
        BombermanManager::Get().GetEvent()->addEvent(ITEM, ITEM_COEUR, DEPOSE, x,y);
        BombermanManager::Get().RegisterItem(Item(x, y, ITEM_COEUR));
    }
    else if(mrandom <= getProba(i, 2, 1, 1, 2, 2, 3) && mrandom >= getProba(i, 2, 1, 1, 2, 1, 3) )
    {
        BombermanManager::Get().GetEvent()->addEvent(ITEM, ITEM_PORTEE_MAX, DEPOSE, x,y);
        BombermanManager::Get().RegisterItem(Item(x, y, ITEM_PORTEE_MAX));
    }
    else if(mrandom <= getProba(i, 2, 1, 1, 2, 3, 3) && mrandom >= getProba(i, 2, 1, 1, 2, 2, 3) )
    {
        BombermanManager::Get().GetEvent()->addEvent(ITEM, ITEM_PROTECTION, DEPOSE, x,y);
        BombermanManager::Get().RegisterItem(Item(x, y, ITEM_PROTECTION));
    }

}

int Bombe::getProba(int j, int repetition_proba1, int indice_tab1, int repetition_proba2, int indice_tab2, int repetition_proba3, int indice_tab3)
{
    int retour=0;

    for(int i=0;i<j;i++)
    {
        retour += tab[i];
    }

    for(int g=0;g<repetition_proba1;g++)
    {
        retour += tab[indice_tab1];
    }
    for(int g=0;g<repetition_proba2;g++)
    {
        retour += tab[indice_tab2];
    }
    for(int g=0;g<repetition_proba3;g++)
    {
        retour += tab[indice_tab3];
    }
    return retour;
}

int Bombe::getIMod(int &i)
{
    i++;
    int j=i;
    return j;
}


void Bombe::glisserBombe(int direction, int index){

/*
    0: haut
    1: bas
    2: gauche
    3: droite
*/
    int newX=0,newY=0;

    if(!exploser && timerGlisse.GetElapsedTime()>0.7){
        switch(direction){ // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< AJOUTER COLLISION BBM POUR LE MULTI ! >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            case 0:
                if(BombermanManager::Get().GetDamier().getValeurCase(mX,mY-1) == 0) // Si pas de collision avec le décor
                    newY=-1;
            break;
            case 1:
                if(BombermanManager::Get().GetDamier().getValeurCase(mX,mY+1) == 0)
                    newY=1;
            break;
            case 2:
                if(BombermanManager::Get().GetDamier().getValeurCase(mX-1,mY) == 0)
                    newX=-1;
            break;
            case 3:
                if(BombermanManager::Get().GetDamier().getValeurCase(mX+1,mY) == 0)
                    newX=1;

            break;
        }
        if(mX!=0 || mY != 0){ // Si la bombe doit bouger
            bool ok=true;
            for(int i=0; i<BombermanManager::Get().getNbBombes();i++){ // On test si aucune autre bombe n'est déjà sur la case avant de la bouger
                if(BombermanManager::Get().GetBombe(i).mX==(newX+mX) && BombermanManager::Get().GetBombe(i).mY==(newY+mY))
                    if(i!=index){
                        ok=false;
                        break;
                    }
            }
            if(ok){
                mX+=newX;
                mY+=newY;
                timerGlisse.Reset();
                sprite.Move(newX*DEPLACEMENT, newY*DEPLACEMENT);


                BombermanManager::Get().GetEvent()->addEvent(BOMBE, mIdentifiant, DEPLACER, mX, mY);
            }
        }
    }
}



void Bombe::explose(){
    int i=1, res, X=mX*DEPLACEMENT, Y=mY*DEPLACEMENT;
    spriteExplose.SetImage(imageExplose);

    collision(X,Y); // Pour tuer un bbm sur la bombe

    spriteExplose.SetRotation(0);

    // vers le haut
    while(i<=mPortee){
        res=collision(X,Y-(DEPLACEMENT*i));
        if(res==1) // Si indestructible on arrete
            break;

        if(i >= mPortee && res == 2)
        {
            //a remplacer par une animation
            spriteExplose1.SetImage(imageExplose1);
            spriteExplose1.SetPosition(X+GESTION_ITEM_MENU,Y-(DEPLACEMENT*i)+GESTION_BBM_MENU);
            BombermanManager::Get().GetApp()->Draw(spriteExplose1);
        }
        else {
        spriteExplose.SetPosition(X+GESTION_ITEM_MENU, Y-(DEPLACEMENT*i)+GESTION_BBM_MENU);
        BombermanManager::Get().GetApp()->Draw(spriteExplose);
        }

        if(res==2){ // Si cassable on arrete apres avoir afficher l'explosion et on modifie la variable
            destru[0]=true;
            break;
        }
        i++;
    }

    // vers le bas
    i=1;
    while(i<=mPortee){
        res=collision(X,Y+(DEPLACEMENT*i));
        if(res==1)
            break;

        if(i >= mPortee && res == 2)
        {
            //a remplacer par une animation
            spriteExplose1.SetImage(imageExplose1);
            spriteExplose1.SetPosition(X+GESTION_ITEM_MENU,Y+(DEPLACEMENT*i)+GESTION_BBM_MENU);
            BombermanManager::Get().GetApp()->Draw(spriteExplose1);
        }
        else {
            spriteExplose.SetPosition(X+GESTION_ITEM_MENU,Y+(DEPLACEMENT*i)+GESTION_BBM_MENU);
            BombermanManager::Get().GetApp()->Draw(spriteExplose);
        }
        if(res==2){
            destru[1]=true;
            break;
        }
        i++;
    }

    spriteExplose.SetRotation(90);
    // vers la gauche
    i=1;
    while(i<=mPortee){
        res=collision(X-(DEPLACEMENT*i),Y);
        if(res==1)
            break;

        if(i >= mPortee && res == 2)
        {
            //a remplacer par une animation
            spriteExplose1.SetImage(imageExplose1);
            spriteExplose1.SetPosition(X-(DEPLACEMENT*i)+GESTION_ITEM_MENU,Y+GESTION_BBM_MENU);
            BombermanManager::Get().GetApp()->Draw(spriteExplose1);
        }else {
            spriteExplose.SetPosition(X-(DEPLACEMENT*i)+GESTION_ITEM_MENU,Y+DEPLACEMENT+GESTION_BBM_MENU); // on ajoute DEPLACEMENT pour corriger la rotation
            BombermanManager::Get().GetApp()->Draw(spriteExplose);
        }

        if(res==2){
            destru[2]=true;
            break;
        }
        i++;
    }

    // vers la droite
    i=1;
    while(i<=mPortee){
        res=collision(X+(DEPLACEMENT*i),Y);
        if(res==1)
            break;


        if(i >= mPortee && res == 2)
        {
            //a remplacer par une animation
            spriteExplose1.SetImage(imageExplose1);
            spriteExplose1.SetPosition(X+(DEPLACEMENT*i)+GESTION_ITEM_MENU,Y+GESTION_BBM_MENU);
            BombermanManager::Get().GetApp()->Draw(spriteExplose1);
        }else {
        spriteExplose.SetPosition(X+(DEPLACEMENT*i)+GESTION_ITEM_MENU,Y+DEPLACEMENT+GESTION_BBM_MENU);
        BombermanManager::Get().GetApp()->Draw(spriteExplose);
        }

        if(res==2){
            destru[3]=true;
            break;
        }
        i++;
    }
}

void Bombe::afficher(int index)
{
    if(timer.GetElapsedTime() > TPS_EXPLOSION+DUREE_EXPLOSION) // Si fin de l'explosion on supprime la bombe
    {
        exploser=true;
        BombermanManager::Get().GetBomberman(bbm)->incrementeNbBombe();
        this->destruction(index);
    }
    else
    {
        if(timer.GetElapsedTime() > TPS_EXPLOSION-0.02 && timer.GetElapsedTime() < TPS_EXPLOSION-0.01) // juste avant d'exploser visuellement
            BombermanManager::Get().GetSound()->Jouer("Explosion");

        if((timer.GetElapsedTime()) > TPS_EXPLOSION){ // Si explosion on l'affiche
            chargerImageFromMemory(image, sprite, "ressources/ExploseCentre.png", true, true, true); // On modifie la bombe par explosion au centre
            sprite.SetImage(image);
            BombermanManager::Get().GetApp()->Draw(sprite);
            this->explose();
        }else
        {
            mAniSprite.SetImage(image);
            mAniSprite.Update();
            BombermanManager::Get().GetApp()->Draw(mAniSprite);
        }


    }
}

void Bombe::destruction(int index){
    /*
        mX, mY = Cases
        x,y = Cases
        larguerItem = Cases
        X,Y = pixels
        collision = pixels
    */

    int res,i=1, x=mX, y=mY;
    int X=mX*DEPLACEMENT, Y=mY*DEPLACEMENT;

    if(destru[0]){ // Si bloc a détruire en haut
        while(i<=mPortee){
            res=collision(X,Y-(DEPLACEMENT*i));
            y=mY-i;
            if(res==1) // Si indestructible on arrete
                break;
            if(res==2){ // Si cassable on arrete apres avoir afficher detruit le bloc
                BombermanManager::Get().GetDamier().setValeurCase(x,y,0);
                if(BombermanManager::Get().IsServer()) larguerItem(x, y); //On largue un item
                break;
            }
            i++;
        }
    }

    if(destru[1]){ // vers le bas
        i=1;
        while(i<=mPortee){
            res=collision(X,Y+(DEPLACEMENT*i));
            y=mY+i;
            if(res==1)
                break;
            if(res==2){
                BombermanManager::Get().GetDamier().setValeurCase(x,y,0);
                if(BombermanManager::Get().IsServer()) larguerItem(x, y);
                break;
            }
            i++;
        }
    }

    y=mY; // On réinitialise y pour les cotés car on l'a bidouillé

    // vers la gauche

    if(destru[2]){
        i=1;
        while(i<=mPortee){
            res=collision(X-(DEPLACEMENT*i),Y);
            x=mX-i;
            if(res==1)
                break;
            if(res==2){
                BombermanManager::Get().GetDamier().setValeurCase(x,y,0);
                if(BombermanManager::Get().IsServer()) larguerItem(x, y);
            }
            i++;
        }
    }

    if(destru[3]){
        // vers la droite
        i=1;
        while(i<=mPortee){
            res=collision(X+(DEPLACEMENT*i),Y);
            x=mX+i;
            if(res==1)
                break;
            if(res==2){
                BombermanManager::Get().GetDamier().setValeurCase(x,y,0);
                if(BombermanManager::Get().IsServer()) larguerItem(x, y);
            }
            i++;
        }
    }
    if(exploser)
        BombermanManager::Get().EraseBombe(index);
}

int Bombe::getPortee()  { return mPortee; }
Sprite Bombe::getSprite()  { return sprite; }
int Bombe::getX()  { return mX; }
int Bombe::getY()  { return mY; }
void Bombe::setX(int x) { mX=x; }
void Bombe::setY(int y) { mY=y; }
