#include <sstream>
#include <SFML/System.hpp>
#include "fonctions.h"
#include "Partie.h"
#include "SuperClass.h"
#include "Menu.h"
#include "Client.h"
#include "Chat.hpp"

using namespace sf;
using namespace std;

int creer_partie(View &view)
{
    //on affiche avec un ascenceur si besoin les map présent dans le dossier map avec le screenshot de la carte
    string dir = string("map");
    vector<string> files = vector<string>();

    listerFichierDAT("map",files);

    int ecart=25; // ecart entre les noms des maps
    int choix=0;
    bool afficher=true;
    bool MapValide=true;
    String text, nomMap[files.size()], details;
    Image img_fond;
    Sprite sprite_fond;
    chargerImageFromMemory(img_fond, sprite_fond, "ressources/fond.jpg", false, false, true);

    vector<string> MAPs;
    Font font_aria, font_antipasto;
    Image Imgcontour;
    Sprite Spritecontour_horizontal, Spritecontour_vertical;

    chargerFontFromMemory(font_aria, text, "police/Balkeno.ttf", "BombGirl", 60);
    text.SetColor(Color(143, 155, 173));
    text.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-text.GetRect().GetWidth())/2, 20);

    chargerFontFromMemory(font_antipasto, details, "police/Blox2.ttf", "", 18);
    details.SetStyle(String::Bold);

    chargerImageFromMemory(Imgcontour, Spritecontour_horizontal, "ressources/Contour_CreerPartie.png", false, true, true);
    chargerImageFromMemory(Imgcontour, Spritecontour_vertical, "ressources/Contour_CreerPartie.png", false, true, true);
    Spritecontour_vertical.SetRotation(90);

    for (unsigned int i = 0; i < files.size(); i++)
    {
        MAPs.push_back(files[i]);
        chargerFontFromMemory(font_antipasto, nomMap[i], "police/Blox2.ttf", files[i], 18);
        nomMap[i].SetPosition(25,125+ecart*i);
        nomMap[i].SetStyle(String::Bold);
        //nomMap[i].SetColor(Color(200,40,40));
        nomMap[i].SetColor(Color(12,46,92));
    }


    while(afficher)
    {
        Event event;
        while(BombermanManager::Get().GetApp()->GetEvent(event))
        {

            BombermanManager::Get().GetApp()->Clear();
            BombermanManager::Get().GetApp()->Draw(sprite_fond);



            for(int i=0; i<LARGEUR_MENU; i++) //MENU HORIZONTALE HAUT
            {
                Spritecontour_horizontal.SetPosition(20+40*i,120);
                BombermanManager::Get().GetApp()->Draw(Spritecontour_horizontal);
            }
            for(int i=0; i<LARGEUR_MENU; i++) //MENU HORIZONTALE BAS
            {
                Spritecontour_horizontal.SetPosition(20+40*i,640);
                BombermanManager::Get().GetApp()->Draw(Spritecontour_horizontal);
            }
            for(int i=0; i<HAUTEUR_MENU-2; i++) //MENU VERTICAL GAUCHE
            {
                Spritecontour_vertical.SetPosition(20,160+40*i);
                BombermanManager::Get().GetApp()->Draw(Spritecontour_vertical);
            }
            for(int i=0; i<HAUTEUR_MENU-2; i++) //MENU VERTICAL CENTRALE
            {
                Spritecontour_vertical.SetPosition(320,160+40*i);
                BombermanManager::Get().GetApp()->Draw(Spritecontour_vertical);
            }
            for(int i=0; i<HAUTEUR_MENU-2; i++) //MENU VERTICAL DROITE
            {
                Spritecontour_vertical.SetPosition(620,160+40*i);
                BombermanManager::Get().GetApp()->Draw(Spritecontour_vertical);
            }

            BombermanManager::Get().GetApp()->Draw(text); //Logo BombGirl



            const Input& input = BombermanManager::Get().GetApp()->GetInput();
            //nomMap[choix].SetColor(Color(200,40,40)); //on remet l'ancienne couleure
            nomMap[choix].SetColor(Color(12,46,92)); //on remet l'ancienne couleure

            Image image;

            if(input.IsKeyDown(Key::Up)){
                if(choix > 0)
                    choix--;
                else
                    choix=(int)MAPs.size()-1;
                if(nomMap[choix].GetPosition().y < 125){
                    for(int i=0;i<(int)files.size(); i++)
                        nomMap[i].Move(0,ecart);
                }

            }
            if(input.IsKeyDown(Key::Down)){
                if(choix < (int)MAPs.size()-1)
                    choix++;
                else
                    choix=0;
                if(nomMap[choix].GetPosition().y>620){
                    for(int i=0;i<(int)files.size(); i++)
                        nomMap[i].Move(0,-ecart);
                }

            }
            if(input.IsKeyDown(Key::Escape)) return -1;
            if(event.Type == Event::Closed) exit(EXIT_SUCCESS);
            if(event.Type == Event::Resized){

            }



            nomMap[choix].SetColor(Color(255,255,255)); // On change la couleur de la selection

            for(int i=0; i<(int)MAPs.size(); i++) //Affichage du menu
            {
                if(nomMap[i].GetPosition().y>=125 && nomMap[i].GetPosition().y <  620){
                    String test2=nomMap[i];
                    string test=nomMap[i].GetText();
                    test2.SetText(test.substr(0, test.size()-4));
                    BombermanManager::Get().GetApp()->Draw(test2);
                }
            }


            //on charge les détails de la map à droite
            cDAT read_test;
            char* buffer=NULL;
            read_test.Read(REPERTOIRE_MAP+MAPs[choix]);

            buffer = read_test.GetFile("map.txt");
            if(buffer == NULL)
            {
                MapValide=false;
                details.SetText("Erreur d'ouverture de "+MAPs[choix]);
                details.SetPosition(340,150);
                BombermanManager::Get().GetApp()->Draw(details);
            }
            else
            {
                MapValide=true;
                int i=0;
                string bufint;
                string Createur, NomMap, DateCreation, test, Joueurs;
                Sprite sprite;


                while (buffer[i] != '\r') { Createur+=buffer[i];i++; } i++;
                while (buffer[i] != '\r') { NomMap+=buffer[i];i++; } i++;
                while (buffer[i] != '\r') { DateCreation+=buffer[i];i++; } i++;
                while (buffer[i] != '\r') { bufint+=buffer[i];i++; } i++;
                test = atoi(bufint.c_str()); bufint="";
                while (buffer[i] != '\r') { bufint+=buffer[i];i++; } i++;
                test = atoi(bufint.c_str()); bufint="";
                while (buffer[i] != '\r') { bufint+=buffer[i];i++; } i++;
                Joueurs = atoi(bufint.c_str());

                chargerImageFromMemory(image, sprite, "images/screenshot.png", false, false, false, REPERTOIRE_MAP+MAPs[choix],false, true);


                details.SetText("Créateur : "+Createur);
                details.SetPosition(340,100+25);
                BombermanManager::Get().GetApp()->Draw(details);
                details.SetText("Nom : "+NomMap);
                details.SetPosition(340,100+25*2);
                BombermanManager::Get().GetApp()->Draw(details);
                details.SetText("Date : "+DateCreation);
                details.SetPosition(340,100+25*3);
                BombermanManager::Get().GetApp()->Draw(details);
                details.SetText("Joueurs : "+bufint);
                details.SetPosition(340,100+25*4);
                BombermanManager::Get().GetApp()->Draw(details);
                sprite.SetPosition(340, 130+25*5);
                BombermanManager::Get().GetApp()->Draw(sprite);


                buffer = read_test.GetFile("temp.txt");
                if(buffer != NULL) //S'il existe c'est une map non terminé donc non lancable
                {
                    details.SetText("Les informations de la map sont\nincomplètes.\nCelle-ci ne peut pas être lancée.\nVeuillez contacter le créateur ou \nmodifiez la avec l'éditeur de niveau.");
                    details.SetPosition(340,100+25*17);
                    BombermanManager::Get().GetApp()->Draw(details);
                    MapValide=false;
                }


                if(input.IsKeyDown((Key::Return)))
            {
                if(MapValide)
                {

                    Damier damier(REPERTOIRE_MAP+MAPs[choix]);
                    BombermanManager::Get().RegisterDamier(damier);//On enregistre la map en mémoire et on lance la partie

                    Serveur *serv = new Serveur(1,BombermanManager::Get().GetDamier().getNbJoueurs(), BombermanManager::Get().GetDamier().getNomMap());
                    BombermanManager::Get().RegisterServeur(serv);


                    BombermanManager::Get().GetSound()->chargerSonsJeu();
                    BombermanManager::Get().GetSound()->Stop();

                    BombermanManager::Get().GetSound()->Jouer("Valider");
                    view.SetFromRect(FloatRect(0, 0, BombermanManager::Get().GetDamier().getLongueur()*DEPLACEMENT+GESTION_ITEM_MENU , BombermanManager::Get().GetDamier().getHauteur()*DEPLACEMENT+GESTION_BBM_MENU ));

                    return 7;
                }
            }


            }
            BombermanManager::Get().GetApp()->Display();
        }
    }
    return 0;
}


int listerFichierDAT(string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL)
    {
        if(string(dirp->d_name).find(".") != 0 && string(dirp->d_name).find("dat") != 0)
            files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int partie(sf::View &view, bool server)
{
    /*on charge le chat*/
    Chat *chat = new Chat();
    BombermanManager::Get().RegisterChat(chat);

    /*On créer le poll d'event network*/
    EventNetwork *event = new EventNetwork();
    BombermanManager::Get().RegistrerEvent(event);

    /*LES ADRESSES IP*/
    string iplocale(sf::IPAddress::GetLocalAddress().ToString());
    string ippublique(sf::IPAddress::GetPublicAddress().ToString());


    bool enterText=false;


    /*ATTRIBUTION DE L'IDENTIFICATION*/
    int identifiant=0;
    if(!server)
        identifiant=BombermanManager::Get().getIdentifiant();

    if(identifiant == 1) BombermanManager::Get().setPingeur(true);
    else BombermanManager::Get().setPingeur(false);



    /*ON ATTEND QUE TOUT SOIT EN PLACE (CLIENT ET SERVEUR)*/
    while(BombermanManager::Get().getDebut() == false)
    {
        Event event;

        while (BombermanManager::Get().GetApp()->GetEvent(event))
        {
            if (event.Type == Event::Closed)
            {
                BombermanManager::Get().GetApp()->Close();
                return 5;
            }
            else if (event.Type == Event::Resized)
                view.SetFromRect(FloatRect(0, 0, event.Size.Width, event.Size.Height));
            else if (event.Type == Event::KeyPressed && event.Key.Code == Key::Escape)
                return 1;


            /*pour le chat*/
            if (event.Type == sf::Event::TextEntered && chat->getSizeTextEnCours()<79 && event.Key.Code != Key::Return && (!BombermanManager::Get().GetClient()->getAttenteDeConnexion() || (BombermanManager::Get().IsServer() && BombermanManager::Get().GetServeur()->getNbJoueurs() > 1))) // Saisie de texte
            {
                if (event.Text.Unicode > 30 && event.Text.Unicode < 255) //filtrage des caractères non imprimable
                {
                    chat->updateTextEnCours(event.Text.Unicode);
                    enterText=true;
                }
            }
            if (event.Key.Code == Key::Back && !BombermanManager::Get().GetClient()->getAttenteDeConnexion()) // Si appuie sur la touche retour arriere : supprimer le texte
                chat->retourArriere();

            else if (event.Key.Code == Key::Return && enterText && event.Type == Event::KeyReleased)
            {
                chat->addText(); //ajout du texte dans la fenetre de chat
                chat->eraseTextEnCours(); //on efface la barre d'écriture
                BombermanManager::Get().GetSound()->Jouer("Valider");
                enterText=false;
            }

            /*pour commencer la partie*/
            else if(event.Type == Event::KeyPressed && event.Key.Code == Key::LControl && BombermanManager::Get().IsServer() && (BombermanManager::Get().GetServeur()->getNbJoueurs() == BombermanManager::Get().GetServeur()->getNbMaxJoueurs()) )
            {
                BombermanManager::Get().RegisterDebut(true);
                BombermanManager::Get().GetEvent()->addEvent(DEBUT_PARTIE); //on envoi le paquet de départ
            }

        }

        BombermanManager::Get().GetApp()->Clear();


         //On vérifie si le serveur est mort
        if(BombermanManager::Get().IsGameOver())
        {
            //on affiche Serveur disconnected
            afficher_info("error", "Serveur déconnecté");
            BombermanManager::Get().GetApp()->Display();
            Sleep(2.0f);

            //effacer plein de truc
            BombermanManager::Get().DeleteAll();
            BombermanManager::Get().SetGameOver(false);
            return 0;
        }
        else if(BombermanManager::Get().IsServer())
        {
            string texte;
            std::ostringstream out;
            out << "Attente des clients . . . " << BombermanManager::Get().GetServeur()->getNbJoueurs()<< " / " << BombermanManager::Get().GetServeur()->getNbMaxJoueurs()<<endl;
            out << "ip locale : " << iplocale<<endl;
            out << "ip publique : " << ippublique;

            afficher_info("info", out.str());
        }
        else
        {
            if(BombermanManager::Get().GetClient()->getAttenteDeConnexion())
                afficher_info("info", "En attente de connexion ...");
            else
                afficher_info("info", "Connecté !");
        }

        chat->afficher();

        BombermanManager::Get().GetApp()->Display();

    }







    /*CREATION DES BOMBERMANS*/
    Bomberman *test[10];
    for(int g=0;g<BombermanManager::Get().getNbPseudoJoueurs();g++)
        test[g] = new Bomberman(BombermanManager::Get().GetPseudoJoueurs(g));




    while (BombermanManager::Get().GetApp()->IsOpened())
    {
        Event event;

        while (BombermanManager::Get().GetApp()->GetEvent(event))
        {
            if (event.Type == Event::Closed)
            {

                if(!BombermanManager::Get().IsServer()) BombermanManager::Get().GetEvent()->addEvent(ETAT_JOUEUR, identifiant, MORT); //on envoit l'event de mort au serveur
                else BombermanManager::Get().GetEvent()->addEvent(GAMEOVER); //ou on prévient les clients que le serveurs est dead


                BombermanManager::Get().GetApp()->Close();
                exit(EXIT_SUCCESS);
            }


            if (event.Type == Event::Resized)
                view.SetFromRect(FloatRect(0, 0, event.Size.Width, event.Size.Height));


            if (event.Type == Event::KeyPressed && event.Key.Code == Key::Escape)
            {
                //On affiche le vsh
                Image image;
                Sprite sprite;
                String text[3];
                Font font;

                chargerFontFromMemory(font, text[0], "police/Blox2.ttf", "Revenir à la partie", 25);
                text[0].SetStyle(String::Bold);
                //text[0].SetColor(Color(200,40,40));
                text[0].SetColor(Color(12,46,92));
                text[0].SetPosition(240, 300);
                chargerFontFromMemory(font, text[1], "police/Blox2.ttf", "Revenir au menu principal", 25);
                text[1].SetStyle(String::Bold);
                //text[1].SetColor(Color(200,40,40));
                text[1].SetColor(Color(12,46,92));
                text[1].SetPosition(210, 330);
                chargerFontFromMemory(font, text[2], "police/Blox2.ttf", "Quitter le jeu", 25);
                text[2].SetStyle(String::Bold);
                //text[2].SetColor(Color(200,40,40));
                text[2].SetColor(Color(12,46,92));
                text[2].SetPosition(250, 360);

                bool afficher=true;
                bool menu=true;
                int choix=0;
                Event event_vsh;


                while(BombermanManager::Get().GetApp()->GetEvent(event_vsh) || afficher)
                {
                    const Input& input = BombermanManager::Get().GetApp()->GetInput();

                    if(event_vsh.Type == Event::Closed)
                    {
                        BombermanManager::Get().GetApp()->Close();
                        if(!BombermanManager::Get().IsServer()) BombermanManager::Get().GetEvent()->addEvent(ETAT_JOUEUR, identifiant, MORT); //on envoit l'event de mort au serveur
                        else BombermanManager::Get().GetEvent()->addEvent(GAMEOVER); //ou on prévient les clients que le serveurs est dead

                        return 5;
                    }
                    if(input.IsKeyDown(Key::Up) && choix-1>=0)
                    {
                        //text[choix].SetColor(Color(200,40,40));
                        text[choix].SetColor(Color(12,46,92));
                        choix-=1;
                    }
                    else if(input.IsKeyDown(Key::Down) && choix+1<3)
                    {
                        //text[choix].SetColor(Color(200,40,40));
                        text[choix].SetColor(Color(12,46,92));
                        choix+=1;
                    }

                    if (event_vsh.Type == Event::Resized)
                        view.SetFromRect(FloatRect(0, 0, event_vsh.Size.Width, event_vsh.Size.Height));



                    text[choix].SetColor(Color(255,255,255));
                    BombermanManager::Get().GetApp()->Clear();


                    if(input.IsKeyDown(Key::Return))
                    {
                        BombermanManager::Get().GetSound()->Jouer("Valider");
                        switch(choix)
                        {
                            case 0:
                                afficher=false;
                                menu=false;
                            break;
                            case 1:
                                //on envoit l'event au serveur
                                BombermanManager::Get().GetEvent()->addEvent(ETAT_JOUEUR, identifiant, MORT);

                                //effacer plein de truc
                                BombermanManager::Get().DeleteAll();
                                BombermanManager::Get().GetApp()->SetSize(HAUTEUR_MENU*DEPLACEMENT+GESTION_ITEM_MENU, LARGEUR_MENU*DEPLACEMENT+GESTION_BBM_MENU);
                                return 0;
                            break;
                            case 2:
                                //on envoit l'event au serveur
                                if(!BombermanManager::Get().IsServer()) BombermanManager::Get().GetEvent()->addEvent(ETAT_JOUEUR, identifiant, MORT); //on envoit l'event de mort au serveur
                                else BombermanManager::Get().GetEvent()->addEvent(GAMEOVER); //ou on prévient les clients que le serveurs est dead


                                return 5;
                            break;
                        }
                    }

                    BombermanManager::Get().GetApp()->Draw(text[0]);
                    BombermanManager::Get().GetApp()->Draw(text[1]);
                    BombermanManager::Get().GetApp()->Draw(text[2]);
                    BombermanManager::Get().GetApp()->Display();
                    Sleep(0.07f);
                }
            }
        }

        BombermanManager::Get().GetApp()->Clear();
        BombermanManager::Get().GetDamier().afficher();



        const Input &input = BombermanManager::Get().GetApp()->GetInput();


        //On parcourt la liste des items pour les afficher
        for(int i=0; i<BombermanManager::Get().getNbItem(); i++) BombermanManager::Get().GetItem(i).afficher();

        //On parcourt la liste des bombes pour les afficher
        for(int i=0; i<BombermanManager::Get().getNbBombes(); i++) BombermanManager::Get().GetBombe(i).afficher(i);




        if(BombermanManager::Get().getNbBomberman() > 0)
        {
            if(identifiant < BombermanManager::Get().getNbBomberman()) //Si je ne suis pas mort
            {
                float dep=280*BombermanManager::Get().GetApp()->GetFrameTime()*BombermanManager::Get().GetBomberman(identifiant)->getVitesse();


                if(input.IsKeyDown(Key::Right)) BombermanManager::Get().GetBomberman(identifiant)->deplacer(dep,0);
                else if(input.IsKeyDown(Key::Left)) BombermanManager::Get().GetBomberman(identifiant)->deplacer(-dep,0); //on dit au premier bomberman dans la super class de se déplacer

                if(input.IsKeyDown(Key::Down)) BombermanManager::Get().GetBomberman(identifiant)->deplacer(0,dep);
                else if(input.IsKeyDown(Key::Up)) BombermanManager::Get().GetBomberman(identifiant)->deplacer(0,-dep);

                if(input.IsKeyDown(Key::Space)) BombermanManager::Get().GetBomberman(identifiant)->poserBombe();


                if(!input.IsKeyDown(Key::Down) && !input.IsKeyDown(Key::Up) && !input.IsKeyDown(Key::Right) && !input.IsKeyDown(Key::Left))
                {
                    for(int g=0;g<BombermanManager::Get().getNbBomberman();g++)
                        BombermanManager::Get().GetBomberman(g)->AnimStop();
                }

            }

            /*ON AFFICHE TOUS LES BOMBERMANS*/
            for(int g=0;g<BombermanManager::Get().getNbBomberman();g++)
                BombermanManager::Get().GetBomberman(g)->afficher();
        }

        BombermanManager::Get().GetApp()->Display();



        //On vérifie si le serveur est mort
        if(BombermanManager::Get().IsGameOver())
        {
            //on affiche Serveur disconnected
            BombermanManager::Get().setShowError(true);
            afficher_info("error", "Serveur déconnecté");
            BombermanManager::Get().GetApp()->Display();
            Sleep(2.0f);
            BombermanManager::Get().setShowError(false);


            //effacer plein de truc
            BombermanManager::Get().DeleteAll();
            BombermanManager::Get().GetApp()->SetSize(HAUTEUR_MENU*DEPLACEMENT+GESTION_ITEM_MENU, LARGEUR_MENU*DEPLACEMENT+GESTION_BBM_MENU);

            return 0;
        }


    }

    return 0;
}

