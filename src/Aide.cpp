#include "SuperClass.h"
#include "fonctions.h"
#include "Aide.h"
#include <SFML/Graphics.hpp>

const int NB_ELT=22;
const int NB_IMAGE=5;

using namespace std;
using namespace sf;

int aide(View &view)
{

    sf::String element[NB_ELT];
    Font police;
    string nomPolice="police/Balkeno.ttf";
    Image image;
    Sprite sprite;
    Image img_fond;
    Sprite sprite_fond;
    chargerImageFromMemory(img_fond, sprite_fond, "ressources/fond.jpg", false, false, true);

    bool afficher = true;
    int hauteur = BombermanManager::Get().GetApp()->GetHeight();
    int largeur = BombermanManager::Get().GetApp()->GetWidth();
    int nb_menu = hauteur / 50;  // Hauteur d'1 ligne=20px, hauteur d'un espace = 20px + 10 px pour espacer les images
    int positionImage[NB_ELT];
    string nomImage[NB_ELT];


    for(int i=0;i<NB_ELT; i++)
        nomImage[i]="";

    // Nom des images que l'on stock afin de charger ensuite les images
    nomImage[4] = "ressources/echap.png";
    nomImage[8] = "item/PorteeMoins.png";
    nomImage[9] = "item/PorteePlus.png";
    nomImage[10] = "item/PorteeMax.png";
    nomImage[11] = "item/VitesseMoins.png";
    nomImage[12] = "item/VitessePlus.png";
    nomImage[13] = "item/NbBombePlus.png";
    nomImage[14] = "item/ProtectOwnBomb.png";
    nomImage[15] = "item/Coeur.png";

    nomImage[19] = "decor/BlocNonDestructible.png";
    nomImage[20] = "decor/BlocDestructible.png";
    nomImage[21] = "decor/SolGlace.png";


    // Chargement de tout les textes a afficher
    chargerFontFromMemory(police, element[0], nomPolice, "TOUCHES", 30);
    chargerFontFromMemory(police, element[1], nomPolice, "", 20);
    chargerFontFromMemory(police, element[2], nomPolice, "Déplacer le personnage", 20);
    chargerFontFromMemory(police, element[3], nomPolice, "Poser une bombe", 20);
    chargerFontFromMemory(police, element[4], nomPolice, "Retourner au menu précédent / afficher le menu de jeu", 20);
    chargerFontFromMemory(police, element[5], nomPolice, "", 20);
    chargerFontFromMemory(police, element[6], nomPolice, "OBJETS", 30);
    chargerFontFromMemory(police, element[7], nomPolice, "", 20);
    chargerFontFromMemory(police, element[8], nomPolice, "Diminue la portée des bombes de 1", 20);
    chargerFontFromMemory(police, element[9], nomPolice, "Augmente la portée des bombes de 1", 20);
    chargerFontFromMemory(police, element[10], nomPolice, "Augmente la portée des bombes au max", 20);
    chargerFontFromMemory(police, element[11], nomPolice, "Diminue la vitesse de déplacement", 20);
    chargerFontFromMemory(police, element[12], nomPolice, "Augmente la vitesse de déplacement", 20);
    chargerFontFromMemory(police, element[13], nomPolice, "Permet de porter une bombe de plus", 20);
    chargerFontFromMemory(police, element[14], nomPolice, "Protège de ses propres bombes pendant 20 secondes", 20);
    chargerFontFromMemory(police, element[15], nomPolice, "Permet de survivre à une explosion, usage unique", 20);
    chargerFontFromMemory(police, element[16], nomPolice, "", 20);
    chargerFontFromMemory(police, element[17], nomPolice, "DECORS", 30);
    chargerFontFromMemory(police, element[18], nomPolice, "", 20);
    chargerFontFromMemory(police, element[19], nomPolice, "Bloc indestructible, résiste aussi aux rayons gamma", 20);
    chargerFontFromMemory(police, element[20], nomPolice, "Bloc destructible qui peut parfois cacher un objet", 20);
    chargerFontFromMemory(police, element[21], nomPolice, "Sol gelé, laissez vous glisser !", 20);


    for(int i=0; i<NB_ELT; i++) // on positionne les elements
    {
        element[i].SetColor(Color(220,140,80));
        element[i].SetPosition(60 , (((hauteur/(nb_menu+1))*(i+1)) - element[i].GetRect().GetHeight()));
        positionImage[i] = element[i].GetPosition().y - 10;
    }
    // X > 40px pour l'image + 20px pour ne pas coller
    // Y calculé selon nombre d'elements et hauteur de la fenetre

    /// TITRES
    // Couleur special pour les titres et centrage
    element[0].SetColor(Color(255,255,255));
    element[6].SetColor(Color(255,255,255));
    element[17].SetColor(Color(255,255,255));
    element[0].SetPosition((largeur-element[0].GetRect().GetWidth())/2 , (hauteur/(nb_menu+1)) - element[0].GetRect().GetHeight());
    element[6].SetPosition((largeur-element[6].GetRect().GetWidth())/2 , (((hauteur/(nb_menu+1))*(7)) - element[6].GetRect().GetHeight()));
    element[17].SetPosition((largeur-element[17].GetRect().GetWidth())/2 , (((hauteur/(nb_menu+1))*(18)) - element[17].GetRect().GetHeight()));


    while(afficher)
    {
        BombermanManager::Get().GetApp()->Clear();
        BombermanManager::Get().GetApp()->Draw(sprite_fond);


        for(int i=0; i<NB_ELT; i++){ // Affichage des textes si ils sont dans l'écran
            if(element[i].GetRect().GetHeight() >= 0 && element[i].GetRect().GetHeight() <= hauteur)
                BombermanManager::Get().GetApp()->Draw(element[i]);
            if(nomImage[i]!="" && positionImage[i] >= 0 && positionImage[i]<= hauteur){ // affichage des images si elles sont dans l'écran
                chargerImageFromMemory(image, sprite, nomImage[i], false, true, false, "Ressources.dat");
                sprite.SetPosition(10, positionImage[i]);
                BombermanManager::Get().GetApp()->Draw(sprite);
            }
        }

        Event event;
        while(BombermanManager::Get().GetApp()->GetEvent(event))
        {
            if (event.Type == Event::Closed)
            {
                return 6; // Quitte le programme
            }
            if (event.Type == Event::Resized) // REDIMENTIONNEMENT
            {
                hauteur = event.Size.Height;
                largeur = event.Size.Width;
                nb_menu = hauteur / 50;
                view.SetFromRect(FloatRect(0, 0,largeur, hauteur));
                for(int i=0; i<NB_ELT; i++){
                    element[i].SetPosition(60 , (((hauteur/(nb_menu+1))*(i+1)) - element[i].GetRect().GetHeight()));
                    positionImage[i] = element[i].GetPosition().y - 10;
                }
            }
            if (event.Type == Event::KeyPressed && event.Key.Code == Key::Escape)
                afficher = false;
        }


        const Input& input = BombermanManager::Get().GetApp()->GetInput();
        if(input.IsKeyDown(Key::Up))
        {
            if ((element[0].GetPosition().y - 1)<0)  // Remonter l'affichage donc descendre les menus
            {
                for(int i=0; i<NB_ELT; i++)
                {
                    element[i].Move(0, hauteur/(nb_menu+1));
                    positionImage[i] = element[i].GetPosition().y - 10;
                }
                Sleep(0.075f);
            }
        }
        if(input.IsKeyDown(Key::Down))
        {   // obligé d'ajouter des pixels pour avoir le dernier texte
            if((element[NB_ELT-1].GetPosition().y + 25)>hauteur) // Faire descendre l'affichage donc remonter les menus
                for(int i=0; i<NB_ELT; i++)
                {
                    element[i].Move(0, -(hauteur/(nb_menu+1)));
                    positionImage[i] = element[i].GetPosition().y - 10;
                }
            Sleep(0.075f);
        }
        BombermanManager::Get().GetApp()->Draw(sprite);
        BombermanManager::Get().GetApp()->Display();

    }
    return 0;
}
