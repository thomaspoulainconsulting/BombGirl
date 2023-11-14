#include "SuperClass.h"
#include "Menu.h"
#include "Partie.h"
#include "Aide.h"
#include "fonctions.h"
#include "Chat.hpp"

using namespace std;
using namespace sf;


int main()
{
    RenderWindow app(VideoMode(HAUTEUR_MENU*DEPLACEMENT+GESTION_ITEM_MENU, LARGEUR_MENU*DEPLACEMENT+GESTION_BBM_MENU, 32), "BombGirl");
    app.SetFramerateLimit(60);

    sf::View view = app.GetDefaultView(); //on cr�er une vue pour redimensionner l'affichage si on met en plein �cran
    app.SetView(view);

    BombermanManager::Get().RegisterApp(&app); // on envoit l'adresse de l'app au manager

    Sons son;
    son.chargerSonsMenu();
    son.Jouer("Intro");
    BombermanManager::Get().RegisterSound(&son);




    //On cr�er et enregistre le client et on d�finie les 2 threads
    Client *test = new Client;
    BombermanManager::Get().RegisterClient(test);


    string pseudo=lecture("profil.txt"); // On r�cup�re le nom de biloute dans le fichier
    BombermanManager::Get().RegisterPseudo(pseudo);

    sf::Thread thread_serveur(&Serveur_Complet);
    sf::Thread thread_client(&Client_Complet);


    bool afficher=true;
    int menu=0;
    /*
        0 : menu
        1 : cr�er
        2 : rejoindre
        3 : changer nom
        4 : aide
        5 : quitter
        7 : remontrer le menu
    */
    menu = launcher(view);

    BombermanManager::Get().RegisterDebut(false);



    while(afficher)
    {
        switch(menu)
        {
        case 0:
            //if(!BombermanManager::Get().GetSound()->IsPlaying()) BombermanManager::Get().GetSound()->Jouer("Intro");
            menu = afficherMenu(view);
            break;
        case 2:{
            string ipserveur(""); //Par D�fault
            ipserveur=entrerIPServeur(view, ipserveur);
            if(ipserveur == "") menu = 0;
            else {

                //on v�rifie si l'ip est bonne
                if(!is_valid_ip(ipserveur.c_str()))
                {
                    //on affiche "erreur : ip invalide"
                    afficher_info("error", "Adresse ip invalide");
                    BombermanManager::Get().setShowError(true);

                }else {
                    BombermanManager::Get().GetSound()->Stop();
                    BombermanManager::Get().RegistrerIpServeur(ipserveur);

                    /*On se d�fini comme client*/
                    BombermanManager::Get().setServer(false);


                    //thread_chat_client.Launch();
                    thread_client.Launch();
                    menu=partie(view, false);

                    }

                //On vide tout et on arr�te le client
                BombermanManager::Get().DeleteAll();
                thread_client.Terminate();
                }
            }
            break;
        case 1: {
            menu = creer_partie(view);
            }
            break;
        case 3:
            pseudo = changerNom(view, pseudo);
            BombermanManager::Get().RegisterPseudo(pseudo);
            menu = 0;
            break;
        case 4:
            menu = aide(view);
            break;
        case 5:
            afficher=false;
            thread_serveur.Terminate();
            thread_client.Terminate();
            break;
        case 7:
            /*On se d�fini comme serveur*/
            BombermanManager::Get().setServer(true);
            thread_serveur.Launch();
            BombermanManager::Get().GetSound()->Stop();

            //lancement de la partie
            menu=partie(view, true);

            //on vide tout et on arr�te le serveur
            BombermanManager::Get().DeleteAll();
            thread_serveur.Terminate();
            break;
        default:
            menu=0;
            break;
        }
    }


    return EXIT_SUCCESS;
}
