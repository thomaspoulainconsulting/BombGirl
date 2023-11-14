#include <iostream>
#include <cassert>
#include "fonctions.h"
#include "Damier.h"
#include "SuperClass.h"
#include "ImageManager.hpp"
#include "cDAT.h"

using namespace std;
using namespace sf;

Damier::Damier() {}
Damier::Damier(string nomfichier) : menuLoaded(false)
{
    charger_damier(nomfichier);

    //On charge l'image à partir du dat s'il existe, sinon à partir des ressources par defaut
    if(ImageManager::Get().ExistImageOnDat(nomfichier, "images/Sol1.png"))
        chargerImageFromMemory(mImgSol1, mSprite, "images/Sol1.png", false, true, false, nomfichier);
    else chargerImageFromMemory(mImgSol1, mSprite, "decor/Sol1.png", false, true, true);


    if(ImageManager::Get().ExistImageOnDat(nomfichier, "images/Sol2.png"))
        chargerImageFromMemory(mImgSol2, mSprite, "images/Sol2.png", false, true, false, nomfichier);
    else chargerImageFromMemory(mImgSol2, mSprite, "decor/Sol2.png", false, true, true);


    if(ImageManager::Get().ExistImageOnDat(nomfichier, "images/BlocNonDestructible.png")) {
        chargerImageFromMemory(mImgBlocIndestructible, mSprite, "images/BlocNonDestructible.png", false, true, false, nomfichier);
    }
    else chargerImageFromMemory(mImgBlocIndestructible, mSprite, "decor/BlocNonDestructible.png", false, true, true);

    if(ImageManager::Get().ExistImageOnDat(nomfichier, "images/BlocDestructible.png"))
        chargerImageFromMemory(mImgBlocDestructible, mSprite, "images/BlocDestructible.png", false, true, false, nomfichier);
    else chargerImageFromMemory(mImgBlocDestructible, mSprite, "decor/BlocDestructible.png", false, true, true);


    chargerImageFromMemory(mImgMenu, mSprite, "ressources/MenuItem.png", false, true, true);

    //on charge en fct de ce qu'il y a véritablement dans la map
    if(IsSolGele)
        chargerImageFromMemory(mImgSolGlace, mSprite, "decor/SolGlace.png", false, true, true);

    if(mNbTeleporteur > 0)
        chargerImageFromMemory(mImgTeleporteur1, mSprite, "decor/Teleporteur1.png", false, true, true);
    if(mNbTeleporteur > 2)
        chargerImageFromMemory(mImgTeleporteur2, mSprite, "decor/Teleporteur2.png", false, true, true);
    if(mNbTeleporteur > 4)
        chargerImageFromMemory(mImgTeleporteur3, mSprite, "decor/Teleporteur3.png", false, true, true);
    if(mNbTeleporteur > 6)
        chargerImageFromMemory(mImgTeleporteur4, mSprite, "decor/Teleporteur4.png", false, true, true);


    //BombermanManager::Get().GetApp()->SetSize(DEPLACEMENT*mLongueur+GESTION_ITEM_MENU, DEPLACEMENT*mLongueur+GESTION_BBM_MENU);

}

void Damier::chargerMenu()
{
    mSprite.SetPosition(40,40);
    mSprite.SetImage(mImgMenu);

    for(int i=0;i<mHauteur;i++)
    {
        mSprite.SetPosition(0,GESTION_BBM_MENU+40*i);
        BombermanManager::Get().GetApp()->Draw(mSprite);
    }
}

void Damier::afficher()
{
    chargerMenu(); //on réaffiche à chaque fois le fond car dans le main on efface l'écran avec app.Clear();


    int mod=0;

    for(int j=0; j<mHauteur; j++)
    {
        for(int i=0; i<mLongueur; i++)
        {

            mSprite.SetPosition(i*DEPLACEMENT+GESTION_ITEM_MENU,j*DEPLACEMENT+GESTION_BBM_MENU);

            // Le damier de couleur par terre
            if(mod == 1)
            {
                mSprite.SetImage(mImgSol1);
                mod = 0;
            }
            else
            {
                mSprite.SetImage(mImgSol2);
                mod = 1;
            }

            switch(this->mTab_damier[j][i])
            {
            case 1 : // Les blocs du décor
                mSprite.SetImage(mImgBlocIndestructible);
                break;
            case 2 :
                mSprite.SetImage(mImgBlocDestructible); // les blocs destructibles
                break;
            case 3 :
                mSprite.SetImage(mImgSolGlace); // le sol glacé
                break;
            case 4 :
                for(int g=0;g<mNbTeleporteur;g++)
                {
                    if(mCoordTeleporteur[g][0] == i && mCoordTeleporteur[g][1] == j)
                    {
                        if(g >= 0 && g < 2) mSprite.SetImage(mImgTeleporteur1);
                        else if(g >= 2 && g < 4) mSprite.SetImage(mImgTeleporteur2);
                        else if(g >= 4 && g < 6) mSprite.SetImage(mImgTeleporteur3);
                        else if(g >= 6 && g < 8) mSprite.SetImage(mImgTeleporteur4);
                    }
                }
                break;
            }

             BombermanManager::Get().GetApp()->Draw(mSprite);

        }
        if(mLongueur % 2 == 0 && mod == 1 && j>0) mod=0;
        else if(mLongueur % 2 == 0 && mod == 0 && j>0) mod=1;

    }

}

void Damier::setValeurCase(int x, int y, int newValeur)
{
    this->mTab_damier[y][x]=newValeur;
}



int Damier::getValeurCase(int x, int y)
{
    if(x < (int)this->mTab_damier.size() && y < (int)this->mTab_damier.size())
        return this->mTab_damier[y][x];
    else return -1;
}

vector<int> Damier::getValeurCoord()
{
    assert(mCoordBomberman.size() > 0 && "Erreur plus de coord disponible");
    vector<int> aretourner = mCoordBomberman[0];
    mCoordBomberman.erase(mCoordBomberman.begin());
    return aretourner;
}

vector<int> Damier::getValeurCoordTeleporteur(int index)
{
    assert((int)mCoordTeleporteur.size() > index && "Erreur d'index coord téléporteur");
    return mCoordTeleporteur[index];
}

void Damier::charger_damier(string nomfichier)
{

     //Initialisation lecture du .dat
    cDAT read_test;
    char* buffer;
    read_test.Read(nomfichier);


    buffer = read_test.GetFile("map.txt");
    if(buffer == NULL)
    {
        cout << "Erreur ouverture map.txt";
        exit(-1);
    }
    else
    {
        int i=0;
        int intcastbufint;
        string bufint;


        while (buffer[i] != '\r') { mCreateur+=buffer[i];i++; } i++;
        while (buffer[i] != '\r') { mNomMap+=buffer[i];i++; } i++;
        while (buffer[i] != '\r') { mDateCreation+=buffer[i];i++; } i++;
        while (buffer[i] != '\r') { bufint+=buffer[i];i++; } i++;
        mLongueur = atoi(bufint.c_str()); bufint="";
        while (buffer[i] != '\r') { bufint+=buffer[i];i++; } i++;
        mHauteur = atoi(bufint.c_str()); bufint="";
        while (buffer[i] != '\r') { bufint+=buffer[i];i++; } i++;
        mNbJoueur = atoi(bufint.c_str()); bufint="";


        vector<vector<int> > tab_coord(mNbJoueur, vector<int>(2, 0) );
        vector<vector<int> > tab_damier(mHauteur, vector<int>(mLongueur, 0) );
        this->mTab_damier=tab_damier;
        this->mCoordBomberman=tab_coord;


        //Lecture des coordonnées des bombermans
        for(int j=0;j < mNbJoueur;j++)
           {
                while(buffer[i] != ' ') { bufint+=buffer[i]; i++; } i++;
                intcastbufint = atoi(bufint.c_str());
                mCoordBomberman[j][0]=intcastbufint;
                bufint="";


               while(buffer[i] != '\r') { bufint+=buffer[i]; i++; } i++;
                intcastbufint = atoi(bufint.c_str());
                mCoordBomberman[j][1]=intcastbufint;
                bufint="";
            }


        //Lecture nombre téléporteur
        while (buffer[i] != '\r') { bufint+=buffer[i];i++; } i++;
        mNbTeleporteur = atoi(bufint.c_str()); bufint="";

        if(mNbTeleporteur > 0)
        {
            vector<vector<int> > tab_coord_tp(mNbTeleporteur, vector<int>(2, 0) );
            mCoordTeleporteur=tab_coord_tp;





            //Lecture des coords des téléporteurs
            for(int j=0;j<mNbTeleporteur;j++) // on met fois 2 parce que chaque téléporteur est relié à un autre
            {
                while(buffer[i] != ' ') { bufint+=buffer[i]; i++; } i++;
                intcastbufint = atoi(bufint.c_str());
                if(intcastbufint != 0) mCoordTeleporteur[j][0]=intcastbufint;
                bufint="";


               while(buffer[i] != '\r') { bufint+=buffer[i]; i++; } i++;
                intcastbufint = atoi(bufint.c_str());
                if(intcastbufint != 0) mCoordTeleporteur[j][1]=intcastbufint;
                bufint="";
            }

        }


        //Lecture des éléments de la map
        int j=0;
        int g=0;
        while(g < mLongueur){
            while(buffer[i] != '\r' )
            {
                int test;
                bufint = buffer[i];
                test = atoi(bufint.c_str());
                mTab_damier[j][g]=test;
                if(!IsSolGele && test == 3) IsSolGele=true;

                i++;j++;
            }
            i++;g++;j=0;
        }




    }
}

int Damier::getLongueur() {  return mLongueur;}
int Damier::getHauteur() {  return mHauteur;}
int Damier::getNbJoueurs() {  return mNbJoueur;}
int Damier::getNbCoordRestant() {  return mCoordBomberman.size();}
std::string Damier::getNomFichierDat() { return mNomFichierDat; }
std::string Damier::getNomMap() { return mNomMap; }
std::string Damier::getCreateur() { return mCreateur; }
std::string Damier::getDateCreation() { return mDateCreation; }
int Damier::getNbCoordTeleporteur() { return (mCoordTeleporteur.size()); }
