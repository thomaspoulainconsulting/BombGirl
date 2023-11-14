#include <iostream>
#include <sstream>
#include <SFML/Network.hpp>
#include "fonctions.h"
#include "SuperClass.h"

const float TEMPS_AVANT_EFFACAGE=1.0;
const float TPS_POSEBOMBE=0.5;
const float SECONDES = 20;

using namespace std;
using namespace sf;

enum {HAUT, BAS, GAUCHE, DROITE};


Bomberman::Bomberman(string nom) : mVitesse(1), mNbBombe(2), mPorteeBombe(1), mVie(0), indexItemBombeDontBombByOwnBomb(-1), mVisible(true), mTimerActive(false), mDontBombByOwnBomb(false), OnGlace(false), TP(false), mMort(false), mDirection(BAS)
{
    mNomString=nom;
    chargerFontFromMemory(font, mNom, "police/Balkeno.ttf", nom);
    chargerFontFromMemory(font, mMenuGaucheNBBombe, "police/Balkeno.ttf", "", 16);
    chargerFontFromMemory(font, mMenuGauchePortee, "police/Balkeno.ttf", "", 16);

    mIndexVector=BombermanManager::Get().RegisterBomberman(this);


    if(BombermanManager::Get().GetDamier().getNbCoordRestant() == 0) //si y a plus de coordonnées disponible pour la map on efface du vecteur le bbm
        BombermanManager::Get().EraseBomberman(mIndexVector);
    else
    {
        vector<int> coord=BombermanManager::Get().GetDamier().getValeurCoord();
        mX=coord[0]*LARGEUR_BBM+GESTION_ITEM_MENU+VIDE_BBM;
        mY=coord[1]*LARGEUR_BBM+GESTION_BBM_MENU+HAUTEUR_BBM/2;
    }

    image = Image(DEPLACEMENT,DEPLACEMENT*2);


    ostringstream oss;
    string nomBBMimage = "Bomberman";

    oss << "ressources/" << nomBBMimage << mIndexVector << ".png";
    chargerImageFromMemory(image, sprite, oss.str(), true, true, true); //on charge le bon sprite en fct° de son arrivée dans le jeu


    ImgAnim test(image, 4, 4); // Crée l'animation avec 4 lignes(directions) et 4 colonnes(animations)
    mAnimBomberman=test;
    mAnimBomberman.setFrame(1);
    mAnimBomberman.setAnimRow(2); // Met le personnage vers le bas

    //pour gérer la collision bbm/bombe
    mCoordBombePosee[0]=0;
    mCoordBombePosee[1]=0;

    //on ajoute les deux items au menu de gauche
    mImageItem.push_back(ITEM_BOMBE_PLUS);
    mImageItem.push_back(ITEM_PORTEE_PLUS);

    sprite.SetPosition(mX-VIDE_BBM,mY-HAUTEUR_BBM);
}


// GESTION DEPLACEMENTS
bool Bomberman::collisionDecor(int direction)  // Si déplacement
{
    /*
    0 = haut
    1 = bas
    2 = gauche
    3 = droite
    */
    switch(direction) // Ecart d'1 px obligatoire pour que BBM puisse se décrocher du mur !
    {
    case 0 : // tester X+1 et son X+COLLISION_BMM-1 pour Y-COLLISION_BMM
        if((BombermanManager::Get().GetDamier().getValeurCase((mX+1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)==0) && (BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)==0))
            return false;
        break;
    case 1: // tester X+1 et son X+COLLISION_BMM-1 pour Y
        if((BombermanManager::Get().GetDamier().getValeurCase((mX+1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-GESTION_BBM_MENU)/DEPLACEMENT)==0 ) && (BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-GESTION_BBM_MENU)/DEPLACEMENT)==0))
            return false;
        break;
    case 2: // tester son Y-1 et son Y-COLLISION_BBM+1 pour X
        if((BombermanManager::Get().GetDamier().getValeurCase((mX-GESTION_ITEM_MENU)/DEPLACEMENT, (mY-1-GESTION_BBM_MENU)/DEPLACEMENT)==0 ) && (BombermanManager::Get().GetDamier().getValeurCase((mX-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-COLLISION_BBM+1-GESTION_BBM_MENU)/DEPLACEMENT)==0))
            return false;
        break;
    case 3: // tester son Y-1 et son Y-COLLISION_BBM+1 pour X+COLLISION_BBM
        if((BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-1-GESTION_BBM_MENU)/DEPLACEMENT)==0) && (BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-GESTION_ITEM_MENU)/DEPLACEMENT, (mY-COLLISION_BBM+1-GESTION_BBM_MENU)/DEPLACEMENT)==0))
            return false;
        break;
    }

    return true;

}

bool Bomberman::collisionGlace(int direction)
{
    switch(direction) // Ecart d'1 px obligatoire pour que BBM puisse se décrocher du mur !
    {
    case 0 : // tester X+1 et son X+COLLISION_BMM-1 pour Y-COLLISION_BMM
        if((BombermanManager::Get().GetDamier().getValeurCase((mX+1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)==3) && (BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)==3))
            return false;
        break;
    case 1: // tester X+1 et son X+COLLISION_BMM-1 pour Y
        if((BombermanManager::Get().GetDamier().getValeurCase((mX+1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-GESTION_BBM_MENU)/DEPLACEMENT)==3) && (BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-GESTION_BBM_MENU)/DEPLACEMENT)==3))
            return false;
        break;
    case 2: // tester son Y-1 et son Y-COLLISION_BBM+1 pour X
        if((BombermanManager::Get().GetDamier().getValeurCase((mX-GESTION_ITEM_MENU)/DEPLACEMENT, (mY-1-GESTION_BBM_MENU)/DEPLACEMENT)==3) && (BombermanManager::Get().GetDamier().getValeurCase((mX-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-COLLISION_BBM+1-GESTION_BBM_MENU)/DEPLACEMENT)==3))
            return false;
        break;
    case 3: // tester son Y-1 et son Y-COLLISION_BBM+1 pour X+COLLISION_BBM
        if((BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-1-GESTION_BBM_MENU)/DEPLACEMENT)==3) && (BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-GESTION_ITEM_MENU)/DEPLACEMENT, (mY-COLLISION_BBM+1-GESTION_BBM_MENU)/DEPLACEMENT)==3))
            return false;
        break;
    }

    return true;
}


bool Bomberman::collisionTeleporteur(int direction)
{
    /*
    0 = haut
    1 = bas
    2 = gauche
    3 = droite
    */
    switch(direction) // Ecart d'1 px obligatoire pour que BBM puisse se décrocher du mur !
    {
    case 0 : // tester X+1 et son X+COLLISION_BMM-1 pour Y-COLLISION_BMM
        if((BombermanManager::Get().GetDamier().getValeurCase((mX+1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)==4) && (BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)==4))
            return false;
        break;
    case 1: // tester X+1 et son X+COLLISION_BMM-1 pour Y
        if((BombermanManager::Get().GetDamier().getValeurCase((mX+1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-GESTION_BBM_MENU)/DEPLACEMENT)==4) && (BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-1-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-GESTION_BBM_MENU)/DEPLACEMENT)==4))
            return false;
        break;
    case 2: // tester son Y-1 et son Y-COLLISION_BBM+1 pour X
        if((BombermanManager::Get().GetDamier().getValeurCase((mX-GESTION_ITEM_MENU-1)/DEPLACEMENT, (mY-1-GESTION_BBM_MENU)/DEPLACEMENT)==4) && (BombermanManager::Get().GetDamier().getValeurCase((mX-GESTION_ITEM_MENU-1)/(DEPLACEMENT), (mY-COLLISION_BBM+1-GESTION_BBM_MENU)/DEPLACEMENT)==4))
            return false;
        break;
    case 3: // tester son Y-1 et son Y-COLLISION_BBM+1 pour X+COLLISION_BBM
        if((BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-GESTION_ITEM_MENU)/(DEPLACEMENT), (mY-1-GESTION_BBM_MENU)/DEPLACEMENT)==4) && (BombermanManager::Get().GetDamier().getValeurCase((mX+COLLISION_BBM-GESTION_ITEM_MENU)/DEPLACEMENT, (mY-COLLISION_BBM+1-GESTION_BBM_MENU)/DEPLACEMENT)==4))
            return false;
        break;
    }

    return true;
}



void Bomberman::deplacer(float x,float y)
{
    if(!mMort)
    {
        int newX, newY;
        newX=this->mX+x;
        newY=this->mY+y;


        if(!mTimerActive)
        {

            // Si deplacement en X
            if((newX-this->mX)!=0)
            {
                if(newX-this->mX>0)  // Si deplacement vers la droite
                {
                    mDirection=DROITE;

                    collisionItem(3);
                    if(!collisionGlace(3) && !OnGlace)
                    {
                        OnGlace=true;
                        DirectionGlissade=3;
                        TP=false;
                    }
                    else
                    {
                        if(!collisionTeleporteur(3) && !TP) //si on est sur un téléporteur et pas vers un mur
                        {
                            //on boucle sur tous les téléporteurs du damier
                            for(int i=0; i<BombermanManager::Get().GetDamier().getNbCoordTeleporteur(); i++)
                            {

                                vector<int> test = BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i);

                                if(ceil((mX-GESTION_ITEM_MENU-VIDE_BBM*2)/DEPLACEMENT+1) == test[0] && ceil((mY-GESTION_BBM_MENU-HAUTEUR_BBM)/DEPLACEMENT+1) == (test[1]))
                                {
                                    if(i % 2 == 0)
                                    {
                                        test = BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i+1);
                                        mX=test[0]*DEPLACEMENT+GESTION_ITEM_MENU+VIDE_BBM;
                                        mY=test[1]*DEPLACEMENT+GESTION_BBM_MENU+HAUTEUR_BBM/2;
                                    }
                                    else
                                    {
                                        test=BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i-1);
                                        mX=test[0]*DEPLACEMENT+GESTION_ITEM_MENU+VIDE_BBM;
                                        mY=test[1]*DEPLACEMENT+GESTION_BBM_MENU+HAUTEUR_BBM/2;
                                    }
                                    break;
                                }

                            }

                            mX++;
                        }
                        else
                        {

                            if(TP) mX++;
                            while(!collisionDecor(3)&&newX!=this->mX&&!collisionBombe(3) && !OnGlace)
                            {
                                mX++;
                                TP=false;
                                mAnimBomberman.setAnimRow(3);
                                mAnimBomberman.play();
                                //BombermanManager::Get().GetApp()->Draw(mAnimBomberman);
                            }

                        }
                    }


                }
                else  // si deplacement vers la gauche
                {
                    mDirection=GAUCHE;

                    collisionItem(2);
                    if(!collisionGlace(2) && !OnGlace)
                    {
                        OnGlace=true;
                        DirectionGlissade=2;
                        TP=false;
                    }
                    else
                    {
                        if(!collisionTeleporteur(2) && !TP) //si on est sur un téléporteur et qu'on va pas vers un mur
                        {
                            //on boucle sur tous les téléporteurs du damier
                            //si on tombe sur celui où on est
                            //on regarde si j==0 ou j==1
                            for(int i=0; i<BombermanManager::Get().GetDamier().getNbCoordTeleporteur(); i++)
                            {

                                vector<int> test = BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i);


                                if(ceil((mX-GESTION_ITEM_MENU-LARGEUR_BBM)/DEPLACEMENT) == test[0] && ceil((mY-GESTION_BBM_MENU-HAUTEUR_BBM)/DEPLACEMENT+1) == (test[1]))
                                {

                                    if(i % 2 == 0)
                                    {
                                        test = BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i+1);
                                        mX=test[0]*DEPLACEMENT+GESTION_ITEM_MENU+VIDE_BBM;
                                        mY=test[1]*DEPLACEMENT+GESTION_BBM_MENU+HAUTEUR_BBM/2;
                                    }
                                    else
                                    {
                                        test=BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i-1);
                                        mX=test[0]*DEPLACEMENT+GESTION_ITEM_MENU+VIDE_BBM;
                                        mY=test[1]*DEPLACEMENT+GESTION_BBM_MENU+HAUTEUR_BBM/2;
                                    }
                                    TP=true;
                                    break;
                                }

                            }
                            mX--;

                        }
                        else
                        {

                            if(TP) mX--;

                            while((!collisionDecor(2))&&newX!=this->mX&&!collisionBombe(2) && !OnGlace)
                            {
                                mX--;
                                TP=false;
                                mAnimBomberman.setAnimRow(1);
                                mAnimBomberman.play();
                                //BombermanManager::Get().GetApp()->Draw(mAnimBomberman);
                            }

                        }

                    }

                }
            }
            else   // Si deplacement en Y
            {
                if(newY-this->mY>0)  // Si deplacement vers le bas
                {
                    mDirection=BAS;

                    collisionItem(1);
                    if(!collisionGlace(1) && !OnGlace)
                    {
                        OnGlace=true;
                        DirectionGlissade=1;
                        TP=false; //si on est sur la glace et qu'on prend un téléporteur ça bug sans ça car le TP reste à true
                    }
                    else
                    {
                        if(!collisionTeleporteur(1) && !TP) //si on est sur un téléporteur
                        {
                            //on boucle sur tous les téléporteurs du damier
                            //si on tombe sur celui où on est
                            //on regarde si j==0 ou j==1
                            for(int i=0; i<BombermanManager::Get().GetDamier().getNbCoordTeleporteur(); i++)
                            {

                                vector<int> test = BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i);

                                if(ceil((mX-GESTION_ITEM_MENU-VIDE_BBM*2)/DEPLACEMENT) == test[0] && ceil((mY-GESTION_BBM_MENU-HAUTEUR_BBM)/DEPLACEMENT+1) == (test[1]))
                                {

                                    if(i % 2 == 0)
                                    {
                                        test = BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i+1);
                                        mX=test[0]*DEPLACEMENT+GESTION_ITEM_MENU+VIDE_BBM;
                                        mY=test[1]*DEPLACEMENT+GESTION_BBM_MENU+38; // on met 38 au lieu de DEPLACEMENT car bug super chelou
                                    }
                                    else
                                    {
                                        test=BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i-1);
                                        mX=test[0]*DEPLACEMENT+GESTION_ITEM_MENU+VIDE_BBM;
                                        mY=test[1]*DEPLACEMENT+GESTION_BBM_MENU+38; // on met 38 au lieu de DEPLACEMENT car bug super chelou
                                    }

                                    TP=true;
                                    break;
                                }
                            }

                            mY++;

                        }
                        else
                        {
                            if(TP) mY++;
                            while(!collisionDecor(1)&&newY!=this->mY&&!collisionBombe(1) && !OnGlace)
                            {
                                mY++;
                                TP=false;
                                mAnimBomberman.setAnimRow(2);
                                mAnimBomberman.play();
                                //BombermanManager::Get().GetApp()->Draw(mAnimBomberman);
                            }
                        }

                    }

                }
                else  // Vers le haut
                {
                    mDirection=HAUT;

                    collisionItem(0);
                    if(!collisionGlace(0) && !OnGlace)
                    {
                        OnGlace=true;
                        DirectionGlissade=0;
                        TP=false;
                    }
                    else
                    {
                        if(!collisionTeleporteur(0) && !TP) //si on est sur un téléporteur
                        {
                            //on boucle sur tous les téléporteurs du damier
                            //si on tombe sur celui où on est
                            //on regarde si j==0 ou j==1
                            for(int i=0; i<BombermanManager::Get().GetDamier().getNbCoordTeleporteur(); i++)
                            {

                                vector<int> test = BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i);

                                if(ceil((mX-GESTION_ITEM_MENU-VIDE_BBM*2)/DEPLACEMENT) == test[0] && ceil((mY-GESTION_BBM_MENU-HAUTEUR_BBM)/DEPLACEMENT) == (test[1]))
                                {
                                    if(i % 2 == 0)
                                    {
                                        test = BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i+1);
                                        mX=test[0]*DEPLACEMENT+GESTION_ITEM_MENU+VIDE_BBM;
                                        mY=test[1]*DEPLACEMENT+GESTION_BBM_MENU+HAUTEUR_BBM/2;
                                    }
                                    else
                                    {
                                        test=BombermanManager::Get().GetDamier().getValeurCoordTeleporteur(i-1);
                                        mX=test[0]*DEPLACEMENT+GESTION_ITEM_MENU+VIDE_BBM;
                                        mY=test[1]*DEPLACEMENT+GESTION_BBM_MENU+HAUTEUR_BBM/2;
                                    }
                                    TP=true;
                                    break;
                                }
                            }
                            mY--;
                        }
                        else
                        {
                            if(TP) mY--;

                            while(!collisionDecor(0)&&newY!=this->mY&&!collisionBombe(0) && !OnGlace)
                            {
                                mY--;
                                TP=false;
                                mAnimBomberman.setAnimRow(0);
                                mAnimBomberman.play();
                                //BombermanManager::Get().GetApp()->Draw(mAnimBomberman);
                            }
                        }

                    }

                }
            }

            /*ON CONSTRUIT LE PACKET A ENVOYER*/
            Packet deplacement;
            deplacement << true << BombermanManager::Get().getIdentifiant() << false << 0 << mAnimBomberman.animRow() << mAnimBomberman.currentFrame() << mX << mY << 0 << 0 << -1 << -1 << 0 << 0;
            //BombermanManager::Get().GetEvent()->addEvent(deplacement);
            BombermanManager::Get().GetEvent()->addEvent(MOUVEMENT, mIndexVector, mAnimBomberman.animRow(), mX, mY, mAnimBomberman.currentFrame());



        }
    }
}

bool Bomberman::collisionBombe(int direction)
{
    //on parcourt toutes les bombes présent dans la superclass
    switch(direction)
    {
    case 0 :
        // Si il se déplace sur la bombe qu'il vient de poser on le laisse partir
        if(mCoordBombePosee[1] == (int)((mY-GESTION_BBM_MENU-COLLISION_BBM+1)/DEPLACEMENT) && (mCoordBombePosee[0] == (int)((mX+1-GESTION_ITEM_MENU)/DEPLACEMENT) || mCoordBombePosee[0] == (int)((mX-1-GESTION_ITEM_MENU+COLLISION_BBM)/DEPLACEMENT)))
            return false;
        // Sinon on efface les coordonnées de cette bombe
        mCoordBombePosee[0]=0;
        mCoordBombePosee[1]=0;
        // Et on test la collision avec toutes les bombes du damier
        for(int i=0; i<BombermanManager::Get().getNbBombes(); i++)
        {
            if((int)BombermanManager::Get().GetBombe(i).getY() == (int)((mY-GESTION_BBM_MENU-COLLISION_BBM+1)/DEPLACEMENT) && ((int)BombermanManager::Get().GetBombe(i).getX() == (int)((mX+1-GESTION_ITEM_MENU)/DEPLACEMENT) || (int)BombermanManager::Get().GetBombe(i).getX() == (int)((mX-1-GESTION_ITEM_MENU+COLLISION_BBM)/DEPLACEMENT)))
            {
                BombermanManager::Get().GetBombe(i).glisserBombe(0, i); // On essaye de déplacer la bombe vers le haut si collision
                return true;
            }
        }
        break;
    case 1:
        if(mCoordBombePosee[1] == (int)((mY-GESTION_BBM_MENU-1)/DEPLACEMENT) && (mCoordBombePosee[0] == (int)((mX+1-GESTION_ITEM_MENU)/DEPLACEMENT) || mCoordBombePosee[0] == (int)((mX+1-GESTION_ITEM_MENU+COLLISION_BBM)/DEPLACEMENT)))
            return false;
        mCoordBombePosee[0]=0;
        mCoordBombePosee[1]=0;
        for(int i=0; i<BombermanManager::Get().getNbBombes(); i++)
        {
            if((int)BombermanManager::Get().GetBombe(i).getY() == (int)((mY-GESTION_BBM_MENU-1)/DEPLACEMENT) && ((int)BombermanManager::Get().GetBombe(i).getX() == (int)((mX+1-GESTION_ITEM_MENU)/DEPLACEMENT) || (int)BombermanManager::Get().GetBombe(i).getX() == (int)((mX+1-GESTION_ITEM_MENU+COLLISION_BBM)/DEPLACEMENT)))
            {
                BombermanManager::Get().GetBombe(i).glisserBombe(1, i); // On essaye de déplacer la bombe vers le bas si collision
                return true;
            }
        }
        break;
    case 2: // Tester X-1 et X+COLLISION_BBM-1 pour Y-1
        if(mCoordBombePosee[0] == (int)((mX-GESTION_ITEM_MENU+1)/DEPLACEMENT) && (mCoordBombePosee[1] == (int)((mY-1-GESTION_BBM_MENU)/DEPLACEMENT) || mCoordBombePosee[1] == (int)((mY+1-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)))
            return false;
        mCoordBombePosee[0]=0;
        mCoordBombePosee[1]=0;
        for(int i=0; i<BombermanManager::Get().getNbBombes(); i++)
        {
            if((int)BombermanManager::Get().GetBombe(i).getX() == (int)((mX-GESTION_ITEM_MENU+1)/DEPLACEMENT) && ((int)BombermanManager::Get().GetBombe(i).getY() == (int)((mY-1-GESTION_BBM_MENU)/DEPLACEMENT) || (int)BombermanManager::Get().GetBombe(i).getY() == (int)((mY+1-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)))
            {
                BombermanManager::Get().GetBombe(i).glisserBombe(2, i); // On essaye de déplacer la bombe vers la gauche si collision
                return true;
            }
        }
        break;
    case 3: // tester son X+1 et son X-COLLISION_BBM-1 pour Y-1
        if(mCoordBombePosee[0] == (int)((mX-GESTION_ITEM_MENU-1+COLLISION_BBM)/DEPLACEMENT) && (mCoordBombePosee[1] == (int)((mY-1-GESTION_BBM_MENU)/DEPLACEMENT) || mCoordBombePosee[1] == (int)((mY+1-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)))
            return false;
        mCoordBombePosee[0]=0;
        mCoordBombePosee[1]=0;
        for(int i=0; i<BombermanManager::Get().getNbBombes(); i++)
        {
            if((int)BombermanManager::Get().GetBombe(i).getX() == (int)((mX-GESTION_ITEM_MENU-1+COLLISION_BBM)/DEPLACEMENT) && ((int)BombermanManager::Get().GetBombe(i).getY() == (int)((mY-1-GESTION_BBM_MENU)/DEPLACEMENT) || (int)BombermanManager::Get().GetBombe(i).getY() == (int)((mY+1-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)))
            {
                BombermanManager::Get().GetBombe(i).glisserBombe(3, i); // On essaye de déplacer la bombe vers la droite si collision
                return true;
            }
        }
        break;
    }

    return false;
}

void Bomberman::collisionItem(int direction)
{
    /*
        0 = haut
        1 = bas
        2 = gauche
        3 = droite
        */
    bool touche=false; //pour éviter la répétition de code
    int i=0;


    switch(direction)
    {
    case 0:
        for(i=0; i<BombermanManager::Get().getNbItem(); i++)
        {
            if(BombermanManager::Get().GetItem(i).getY() == (int)((mY-GESTION_BBM_MENU-COLLISION_BBM+1)/DEPLACEMENT) && (BombermanManager::Get().GetItem(i).getX() == (int)((mX+1-GESTION_ITEM_MENU)/DEPLACEMENT) || BombermanManager::Get().GetItem(i).getX() == (int)((mX-1-GESTION_ITEM_MENU+COLLISION_BBM)/DEPLACEMENT)))
            {
                touche=true;
                break; //on peut touché qu'un seul item donc une fois qu'on l'a touché on sort de la boucle

            }
        }
        break;
    case 1:
        for(i=0; i<BombermanManager::Get().getNbItem(); i++)
        {
            if(BombermanManager::Get().GetItem(i).getY() == (int)((mY-GESTION_BBM_MENU-1)/DEPLACEMENT) && (BombermanManager::Get().GetItem(i).getX() == (int)((mX+1-GESTION_ITEM_MENU)/DEPLACEMENT) || BombermanManager::Get().GetItem(i).getX() == (int)((mX-1-GESTION_ITEM_MENU+COLLISION_BBM)/DEPLACEMENT)))
            {
                touche=true;
                break;

            }
        }
        break;
    case 2:
        for(i=0; i<BombermanManager::Get().getNbItem(); i++)
        {
            if(BombermanManager::Get().GetItem(i).getX() == (int)((mX-GESTION_ITEM_MENU+1)/DEPLACEMENT) && (BombermanManager::Get().GetItem(i).getY() == (int)((mY-1-GESTION_BBM_MENU)/DEPLACEMENT) || BombermanManager::Get().GetItem(i).getY() == (int)((mY+1-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)))
            {
                touche=true;
                break;

            }
        }
        break;
    case 3:
        for(i=0; i<BombermanManager::Get().getNbItem(); i++)
        {
            if(BombermanManager::Get().GetItem(i).getX() == (int)((mX-GESTION_ITEM_MENU-1+COLLISION_BBM)/DEPLACEMENT) && (BombermanManager::Get().GetItem(i).getY() == (int)((mY-1-GESTION_BBM_MENU)/DEPLACEMENT) || BombermanManager::Get().GetItem(i).getY() == (int)((mY+1-COLLISION_BBM-GESTION_BBM_MENU)/DEPLACEMENT)))
            {
                touche=true;
                break;

            }
        }
        break;
    }


    if(touche)
    {
        int type=BombermanManager::Get().GetItem(i).getType();

        if(type == ITEM_BOMBE_PLUS) mNbBombe++;
        if(type == ITEM_PORTEE_PLUS)
        {
            if(!(mPorteeBombe+1 >= (BombermanManager::Get().GetDamier().getLongueur()-2))) mPorteeBombe++;
        }
        if(type == ITEM_VITESSE_MOINS)
        {
            if(mVitesse >= 1)
            {
                if(mVitesse == 1) mVitesse=0.7;
                else mVitesse=1;

                if(mVitesse != 1)
                {
                    mImageItem.push_back(type);
                }
                else
                {
                    //on retire l'image du vector
                    for(int i=0; i<(int)mImageItem.size(); i++)
                    {
                        if(mImageItem.at(i) == ITEM_VITESSE_PLUS)
                        {
                            mImageItem.erase(mImageItem.begin()+i);
                            break;
                        }

                    }
                    indexItemBombeDontBombByOwnBomb=-1; //On reset l'index sinon le chronomètre du ProtectBomb est décalé
                }
            }
        }
        if(type == ITEM_VITESSE_PLUS)
        {
            if(mVitesse < 1.7)
            {
                if(mVitesse == 1)  mVitesse=1.7;
                else mVitesse=1;

                if(mVitesse != 1)
                {
                    mImageItem.push_back(type);
                }
                else
                {
                    //on retire l'image de l'autre image du vector
                    for(int i=0; i<(int)mImageItem.size(); i++)
                    {
                        if(mImageItem.at(i) == ITEM_VITESSE_MOINS)
                        {
                            mImageItem.erase(mImageItem.begin()+i);
                            break;
                        }

                    }
                    indexItemBombeDontBombByOwnBomb=-1; //On reset l'index sinon le chronomètre du ProtectBomb est décalé
                }
            }
        }
        if(type == ITEM_PORTEE_MOINS)
        {
            if(mPorteeBombe-1 > 0) mPorteeBombe--;
            if(mImageItem.at(1) == ITEM_PORTEE_MAX) mImageItem.at(1)=ITEM_PORTEE_PLUS;
        }
        if(type == ITEM_COEUR)
        {
            if(mVie == 0)
            {
                mVie++;
                mImageItem.push_back(type);
            }
            indexItemBombeDontBombByOwnBomb=-1; //On reset l'index sinon le chronomètre du ProtectBomb est décalé
        }
        if(type == ITEM_PORTEE_MAX)
        {
            mPorteeBombe = (BombermanManager::Get().GetDamier().getLongueur()-3); //on fait -2 car on a deux blocs indestructibles du décor et -1 car le bomberman prend une case
            mImageItem.at(1)=type;
        }
        if(type == ITEM_PROTECTION)
        {
            if(mDontBombByOwnBomb != true)
            {
                mDontBombByOwnBomb=true;
                timerDontBombByOwnBomb.Reset();
                mImageItem.push_back(type);
            }
        }


        BombermanManager::Get().EraseItem(i);

        /*ON INDIQUE L'EVENEMENT*/
        BombermanManager::Get().GetEvent()->addEvent(ITEM, type, PRIS, mX, mY, i);




    }

}


void Bomberman::afficher()
{
    if(!mMort)
    {
        //affichage du nom du bonhomme
        mNom.SetPosition(Vector2f(mX, mY-HAUTEUR_BBM-4));
        BombermanManager::Get().GetApp()->Draw(mNom);


        /*début affichage du menu gauche*/
        //affichage des images dans le vecteur d'imageItem
        //si l'identifiant de ce bomberman == l'identifiant du joueur on affiche
        if(mIndexVector == BombermanManager::Get().getIdentifiant()) {

        Sprite spriteItem;
        Image imageItem;
        for(int i=0; i<(int)mImageItem.size(); i++)
        {
            chargerImageFromMemory(imageItem, spriteItem, getCorrespondance(mImageItem.at(i)), false, false, true);

            spriteItem.SetPosition(0,GESTION_BBM_MENU+DEPLACEMENT*i);
            spriteItem.SetX(10);
            BombermanManager::Get().GetApp()->Draw(spriteItem);
        }




        ostringstream oss;
        string test = "x ";
        oss << test << mNbBombe;
        mMenuGaucheNBBombe.SetText("");
        BombermanManager::Get().GetApp()->Draw(mMenuGaucheNBBombe);
        mMenuGaucheNBBombe.SetText(oss.str());
        mMenuGaucheNBBombe.SetColor(Color(255, 255, 255, 255));
        mMenuGaucheNBBombe.SetPosition(Vector2f(25,GESTION_BBM_MENU+20)); // 25 : décalage du x // 20 décalage du y
        BombermanManager::Get().GetApp()->Draw(mMenuGaucheNBBombe);


        oss.str("");
        oss << test << mPorteeBombe;
        mMenuGauchePortee.SetText("");
        BombermanManager::Get().GetApp()->Draw(mMenuGauchePortee);
        mMenuGauchePortee.SetText(oss.str());
        mMenuGauchePortee.SetColor(Color(255, 255, 255, 255));
        mMenuGauchePortee.SetPosition(Vector2f(25,GESTION_BBM_MENU+DEPLACEMENT+20));
        BombermanManager::Get().GetApp()->Draw(mMenuGauchePortee);


        if(mDontBombByOwnBomb)
        {
            if(SECONDES-(int)timerDontBombByOwnBomb.GetElapsedTime() >= 0)
            {

                oss.str("");
                oss << SECONDES-(int)timerDontBombByOwnBomb.GetElapsedTime();
                String timerItemBombe;
                chargerFontFromMemory(font, timerItemBombe, "police/Balkeno.ttf", "", 16);
                timerItemBombe.SetText(oss.str());

                if(indexItemBombeDontBombByOwnBomb == -1)
                {
                    for(int i=0; i<(int)mImageItem.size(); i++)
                    {
                        if(mImageItem.at(i) == ITEM_PROTECTION)
                        {
                            timerItemBombe.SetColor(Color(255, 255, 255, 255));
                            timerItemBombe.SetPosition(Vector2f(25,GESTION_BBM_MENU+DEPLACEMENT*i+20));
                            BombermanManager::Get().GetApp()->Draw(timerItemBombe);
                            indexItemBombeDontBombByOwnBomb=i;
                            break;
                        }
                    }

                }
                else
                {
                    timerItemBombe.SetColor(Color(255, 255, 255, 255));
                    timerItemBombe.SetPosition(Vector2f(25,GESTION_BBM_MENU+DEPLACEMENT*indexItemBombeDontBombByOwnBomb+20));
                    BombermanManager::Get().GetApp()->Draw(timerItemBombe);
                }

            }
            else
            {
                mImageItem.erase(mImageItem.begin()+indexItemBombeDontBombByOwnBomb);
                mDontBombByOwnBomb=false;
            }

        }
        }//fin du si identifiant
        /*fin affichage menu gauche*/


        //Gestion de la mort
        if(mTimerActive)
        {
            if(mVie == 0) //si on a qu'une vie on crève
            {
                if(timer.GetElapsedTime() > TEMPS_AVANT_EFFACAGE)
                {
                    mMort=true;
                    mTimerActive=false;

                    /*ON INDIQUE L'EVENEMENT*/
                    Packet item;
                    item << true << BombermanManager::Get().getIdentifiant() << true << 0 << 0 << 0 << 0 << 0 << 0 << 0 << -1 << -1 << 0 << 0;
                    //BombermanManager::Get().GetEvent()->addEvent(item);
                    BombermanManager::Get().GetEvent()->addEvent(ITEM, mIndexVector, PRIS, mX, mY);


                }
            }
            else //si on a plus d'une vie, on diminue le nombre et on remet l'image normal de bbm
            {
                if(timer.GetElapsedTime() > TEMPS_AVANT_EFFACAGE)
                {
                    mVie=0;
                    //on parcourt la liste des imageItemMenuGauche pour effacer le coeur
                    for(int i=0; i<(int)mImageItem.size(); i++)
                    {
                        if(mImageItem.at(i) == ITEM_COEUR) mImageItem.erase(mImageItem.begin()+i);
                    }

                    ostringstream oss;
                    string nomBBMimage = "ressources/Bomberman";
                    oss << nomBBMimage << mIndexVector << ".png";

                    chargerImageFromMemory(image, sprite, oss.str(), true, true, true); // on remet l'ancien sprite du bbm
                    mTimerActive=false;
                }
            }
        }


        if(OnGlace) //Si on est sur la glace on glisse
        {
            if(DirectionGlissade == 3 && !collisionGlace(DirectionGlissade))
            {
                mX+=2;
                sprite.SetPosition(mX-VIDE_BBM, mY-HAUTEUR_BBM);
            }
            else if(DirectionGlissade == 2 && !collisionGlace(DirectionGlissade))
            {
                mX-=2;
                sprite.SetPosition(mX-VIDE_BBM, mY-HAUTEUR_BBM);
            }
            else if(DirectionGlissade == 1 && !collisionGlace(DirectionGlissade))
            {
                mY+=2;
                sprite.SetPosition(mX-VIDE_BBM, mY-HAUTEUR_BBM);
            }
            else if(DirectionGlissade == 0 && !collisionGlace(DirectionGlissade))
            {
                mY-=2;
                sprite.SetPosition(mX-VIDE_BBM, mY-HAUTEUR_BBM);
            }
            else
            {
                switch(DirectionGlissade)
                {
                case 3 :
                    mX+=LARGEUR_BBM-VIDE_BBM;
                    if(collisionDecor(3)) mX-=LARGEUR_BBM-VIDE_BBM; // si on va tomber sur un bloc du décor on se stop
                    break;
                case 2 :
                    mX-=LARGEUR_BBM-VIDE_BBM;
                    if(collisionDecor(2)) mX+=LARGEUR_BBM-VIDE_BBM;
                    break;
                case 1 :
                    mY+=COLLISION_BBM;
                    if(collisionDecor(1)) mY-=COLLISION_BBM;
                    break;
                case 0 :
                    mY-=COLLISION_BBM;
                    if(collisionDecor(0)) mY+=COLLISION_BBM;
                    break;
                }

                sprite.SetPosition(mX-VIDE_BBM, mY-HAUTEUR_BBM);
                OnGlace=false;
            }

            /*ON CONSTRUIT LE PACKET A ENVOYER*/
            Packet deplacement;
            deplacement << true << BombermanManager::Get().getIdentifiant() << false << 0 << mAnimBomberman.animRow() << mAnimBomberman.currentFrame() << mX << mY << 0 << 0 << -1 << -1 << 0 << 0;
            //BombermanManager::Get().GetEvent()->addEvent(deplacement);
            BombermanManager::Get().GetEvent()->addEvent(MOUVEMENT, mIndexVector, mAnimBomberman.animRow(), mX, mY, mAnimBomberman.currentFrame());

        }

        mAnimBomberman.SetPosition(mX-VIDE_BBM, mY-HAUTEUR_BBM);
        BombermanManager::Get().GetApp()->Draw(mAnimBomberman);
    }
}

void Bomberman::AnimStop()
{
    mAnimBomberman.stop();
}

void Bomberman::mort()
{
    /*On se met en position haut, pour bien afficher le sprite de mort...*/
    mAnimBomberman.setAnimRow(0);
    mAnimBomberman.play();
    BombermanManager::Get().GetApp()->Draw(mAnimBomberman);

    ostringstream oss;
    string nomBBMimage = "ressources/Bomberman_burn.png";
    oss << nomBBMimage;

    chargerImageFromMemory(image, sprite, oss.str(), true, true, true);
    timer.Reset();
    mTimerActive=true;

    BombermanManager::Get().GetEvent()->addEvent(ETAT_JOUEUR, mIndexVector, MORT);
}


// GESTION BOMBES
void Bomberman::poserBombe()
{
    int bombeX, bombeY;
    bombeX = mX-GESTION_ITEM_MENU+COLLISION_BBM/2;
    bombeY = mY-GESTION_BBM_MENU-COLLISION_BBM/2;


    if(mNbBombe>0 && timerPose.GetElapsedTime()>=TPS_POSEBOMBE && BombermanManager::Get().GetDamier().getValeurCase(bombeX/DEPLACEMENT,bombeY/DEPLACEMENT == 0))
    {
        BombermanManager::Get().RegisterBombe(bombeX/DEPLACEMENT, bombeY/DEPLACEMENT, (int)mPorteeBombe, mIndexVector);

        mCoordBombePosee[0]=bombeX/DEPLACEMENT;
        mCoordBombePosee[1]=bombeY/DEPLACEMENT;

        BombermanManager::Get().GetBomberman(mIndexVector)->decrementeNbBombe();
        timerPose.Reset();

        BombermanManager::Get().GetEvent()->addEvent(BOMBE, mIndexVector, DEPOSE, bombeX, bombeY, mPorteeBombe);
    }
}

bool Bomberman::poserBombe(int x, int y, int identifiant, int portee)
{
    if(mNbBombe>0 && timerPose.GetElapsedTime()>=TPS_POSEBOMBE && BombermanManager::Get().GetDamier().getValeurCase(x/DEPLACEMENT,y/DEPLACEMENT == 0))
    {
        BombermanManager::Get().RegisterBombe(x/DEPLACEMENT, y/DEPLACEMENT, portee, identifiant);
        BombermanManager::Get().GetBomberman(identifiant)->decrementeNbBombe();
        return true;
    }
    else
    {
        cout << "Err: Pose Bombe"<<endl;
        return false;
    }
}



//SETTEUR
void Bomberman::changerVisibilite(bool visible)
{
    this->mVisible = visible;
}
void Bomberman::changerVitesse(float vitesse)
{
    this->mVitesse=vitesse;
}

//GETTEUR
void Bomberman::incrementeNbBombe()
{
    mNbBombe++;
}
void Bomberman::decrementeNbBombe()
{
    mNbBombe--;
}
int Bomberman::getX()
{
    return this->mX;
}
int Bomberman::getY()
{
    return this->mY;
}
float Bomberman::getVitesse()
{
    return this->mVitesse;
}
int Bomberman::getNbBombe()
{
    return this->mNbBombe;
}
int Bomberman::getPortee()
{
    return this->mPorteeBombe;
}
bool Bomberman::getVisible()
{
    return this->mVisible;
}
Sprite Bomberman::getSprite()
{
    return this->sprite;
}
Font Bomberman::getFont()
{
    return this->font;
}
Image Bomberman::getImage()
{
    return this->image;
}
string Bomberman::getNom()
{
    return mNomString;
}
bool Bomberman::getDontBombByOwnBomb()
{
    return mDontBombByOwnBomb;
}

void Bomberman::resetIndexItemBombeDontBombByOwnBomb()
{
    indexItemBombeDontBombByOwnBomb=-1;
}

int Bomberman::getIndexVector()
{
    return mIndexVector;
}

void Bomberman::setCoord(int x, int y)
{
    mX=x;
    mY=y;
}

void Bomberman::setFrame(int animrow, int frame)
{
    mAnimBomberman.setAnimRow(animrow);
    mAnimBomberman.setFrame(frame);
    mAnimBomberman.play();
    BombermanManager::Get().GetApp()->Draw(mAnimBomberman);
}

void Bomberman::SetMort(bool mmort)
{
    mMort=mmort;
    //si mort on met le perso en dehors de l'écran
    if(mmort) {
        mAnimBomberman.SetPosition(-DEPLACEMENT*2, 0);
        sprite.SetPosition(-DEPLACEMENT*2, 0);
    }
}

int Bomberman::getDirection()
{
    return mDirection;
}
