#ifndef DEF_DAMIER
#define DEF_DAMIER
#include <vector>
#include <SFML/Graphics.hpp>

class Damier
{
private:
    std::string mNomMap;
    std::string mCreateur;
    std::string mDateCreation;
    std::string mNomFichierDat;
    int mLongueur;
    int mHauteur;
    int mNbJoueur;
    int mNbTeleporteur;
    bool menuLoaded;
    bool IsSolGele;
    std::vector<std::vector<int> > mCoordBomberman;
    std::vector<std::vector<int> > mTab_damier;
    std::vector<std::vector<int> > mCoordTeleporteur;
    sf::Image mImgSol1, mImgSol2, mImgBlocIndestructible, mImgBlocDestructible, mImgMenu, mImgSolGlace;
    sf::Image mImgTeleporteur1, mImgTeleporteur2, mImgTeleporteur3, mImgTeleporteur4;
    sf::Sprite mSprite;

public:
    Damier();
    Damier(std::string nomfichier);
    void chargerDamier(int nom_damier);
    void afficher();
    void charger_damier(std::string nomfichier);
    void chargerMenu();
    void setValeurCase(int x, int y, int newValeur);
    int getValeurCase(int x, int y);
    std::vector<int> getValeurCoord();
    std::vector<int> getValeurCoordTeleporteur(int index);


    int getLongueur();
    int getHauteur();
    int getNbCoordRestant();
    int getNbJoueurs();
    int getNbCoordTeleporteur();
    std::string getNomFichierDat();
    std::string getNomMap();
    std::string getDateCreation();
    std::string getCreateur();



};

#endif
