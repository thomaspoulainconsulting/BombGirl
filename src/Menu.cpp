#include <fstream>
#include "Menu.h"
#include "SuperClass.h"
#include "fonctions.h"

const int NB_MENU=5;


using namespace std;
using namespace sf;

void modifie(int &x, int value)
{
    x=value;
}

void ecriture(string texte, string chemin)
{
    ofstream fichier(chemin, ios::out | ios::trunc);
    if(fichier)
    {
        fichier.clear();
        fichier << texte;
        fichier.close();
    }
}

string lecture(string chemin)
{
    //ifstream fichier(chemin, ios::in);

    fstream fichier;
    fichier.open(chemin);
    if(fichier)
    {
        string contenu;
        getline(fichier, contenu);
        fichier.close();


        return contenu;
    }
    else
        return "Joueur";
}


string changerNom(View &view, string pseudo)
{
    Font police;
    string texte=pseudo;
    String nom, indication, indication2;
    Image img_fond;
    Sprite sprite_fond;
    chargerImageFromMemory(img_fond, sprite_fond, "ressources/fond.jpg", false, false, true);

    chargerFontFromMemory(police, nom, "police/Balkeno.ttf", "", 60);

    chargerFontFromMemory(police, indication, "police/Balkeno.ttf", "Entrez votre nom :", 20);
    chargerFontFromMemory(police, indication2, "police/Balkeno.ttf", "Valider : Entrée\nRetour. : Echap", 20);
    indication.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-indication.GetRect().GetWidth())/2, indication.GetRect().GetHeight()*3);
    indication2.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-indication.GetRect().GetWidth())/2, BombermanManager::Get().GetApp()->GetHeight()-3*indication2.GetRect().GetHeight());

    bool afficher = true, valider = false;

    while(afficher)
    {
        Event event;

        while (BombermanManager::Get().GetApp()->GetEvent(event))
        {
            if (event.Type == sf::Event::TextEntered && texte.size()<15 && event.Key.Code != Key::Return) // Saisie de texte
                if (event.Text.Unicode < 128)
                {
                    texte += static_cast<char>(event.Text.Unicode);
                    valider=true;
                }
            if (event.Key.Code == Key::Back) // Si appuie sur la touche retour arriere : supprimer le texte
                texte=texte.substr(0, texte.size()-1);

            if (event.Key.Code == Key::Escape)
            {
                afficher = false;
            }
            if(event.Type == Event::Closed)
            {
                exit(EXIT_SUCCESS);
            }
            if (event.Type == Event::Resized)
            {
                view.SetFromRect(FloatRect(0, 0, event.Size.Width, event.Size.Height));
                nom.SetPosition((event.Size.Width-nom.GetRect().GetWidth())/2, (event.Size.Height-nom.GetRect().GetHeight())/2); // Repositionne le texte au centre
                indication.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-indication.GetRect().GetWidth())/2, indication.GetRect().GetHeight()*3);
                indication2.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-indication.GetRect().GetWidth())/2, BombermanManager::Get().GetApp()->GetHeight()-3*indication2.GetRect().GetHeight());
            }
            if (event.Key.Code == Key::Return && valider && event.Type == Event::KeyReleased) // Pour que le menu ne soit pas quitté instantanément
            {
                BombermanManager::Get().GetSound()->Jouer("Valider");
                afficher = false;
            }
        }

        BombermanManager::Get().GetApp()->Clear();
        BombermanManager::Get().GetApp()->Draw(sprite_fond);

        nom.SetText(texte);
        nom.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-nom.GetRect().GetWidth())/2, (BombermanManager::Get().GetApp()->GetHeight()-nom.GetRect().GetHeight())/2);

        BombermanManager::Get().GetApp()->Draw(nom);
        BombermanManager::Get().GetApp()->Draw(indication);
        BombermanManager::Get().GetApp()->Draw(indication2);

        BombermanManager::Get().GetApp()->Display();
    }
    if(texte!="")
        ecriture(texte, "profil.txt");
    return texte;
}

int afficherMenu(View &view) // menu > verifie si le jeu a déjà été lancé OU si on a appuyé sur espace pour pas voir nos noms :O !!
{
    /* MENU
       0 : Jouer
       1 : Changer Nom
       2 : Quitter
    */

    bool afficher = true;
    int choix = 0;
    Font credit;
    String text, copyright, textMenu[NB_MENU];
    Image img_fond;
    Sprite sprite_fond;
    chargerImageFromMemory(img_fond, sprite_fond, "ressources/fond.jpg", false, false, true);

    chargerFontFromMemory(credit, textMenu[0], "police/Balkeno.ttf", "Créer partie", 40);
    chargerFontFromMemory(credit, textMenu[1], "police/Balkeno.ttf", "Rejoindre partie", 40);
    chargerFontFromMemory(credit, textMenu[2], "police/Balkeno.ttf", "Profil", 40);
    chargerFontFromMemory(credit, textMenu[3], "police/Balkeno.ttf", "Aide", 40);
    //chargerFontFromMemory(credit, textMenu[4], "police/Balkeno.ttf", "Crédit", 40);
    chargerFontFromMemory(credit, textMenu[4], "police/Balkeno.ttf", "Quitter", 40);

    chargerFontFromMemory(credit, text, "police/Balkeno.ttf", "BombGirl", 60);
    text.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-text.GetRect().GetWidth())/2, BombermanManager::Get().GetApp()->GetHeight()/(2*NB_MENU+1)-text.GetRect().GetHeight()/2);
    text.SetColor(Color(143, 155, 173));

    /*chargerFontFromMemory(credit, copyright, "police/Aria.ttf", "G.POLAERT  &  T.POULAIN", 15);
    copyright.SetPosition(BombermanManager::Get().GetApp()->GetWidth()-copyright.GetRect().GetWidth()-5, BombermanManager::Get().GetApp()->GetHeight()-copyright.GetRect().GetHeight()-10);
    copyright.SetColor(Color(143, 155, 173));*/

    // Placement du menu
    for(int i=0; i<NB_MENU; i++)
    {
        //textMenu[i].SetColor(Color(200,40,40));
        textMenu[i].SetColor(Color(12,46,92));
        textMenu[i].SetPosition((BombermanManager::Get().GetApp()->GetWidth()-textMenu[i].GetRect().GetWidth())/2, text.GetRect().GetHeight()/2+(BombermanManager::Get().GetApp()->GetHeight() - text.GetRect().GetHeight())/(NB_MENU+1)*(i+1));
    }

    while(afficher)
    {
        BombermanManager::Get().GetApp()->Clear();

        BombermanManager::Get().GetApp()->Draw(sprite_fond);

        BombermanManager::Get().GetApp()->Draw(text);
        //BombermanManager::Get().GetApp()->Draw(copyright);

        Event event;
        while(BombermanManager::Get().GetApp()->GetEvent(event))  // EVENEMENT QUITTER OU REDIMENTIONNER
        {
            if (event.Type == Event::Closed)
            {
                BombermanManager::Get().GetApp()->Close();
                exit(EXIT_SUCCESS); // Quitte le programme
            }
            if (event.Type == Event::Resized)
            {
                view.SetFromRect(FloatRect(0, 0, event.Size.Width, event.Size.Height));
                //copyright.SetPosition(BombermanManager::Get().GetApp()->GetWidth()-copyright.GetRect().GetWidth()-5, BombermanManager::Get().GetApp()->GetHeight()-copyright.GetRect().GetHeight()-10);
                text.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-text.GetRect().GetWidth())/2, BombermanManager::Get().GetApp()->GetHeight()/(2*NB_MENU+1)-text.GetRect().GetHeight()/2); // Repositionne le texte au centre
                for(int i=0; i<NB_MENU; i++)
                    textMenu[i].SetPosition((BombermanManager::Get().GetApp()->GetWidth()-textMenu[i].GetRect().GetWidth())/2, text.GetRect().GetHeight()/2+(BombermanManager::Get().GetApp()->GetHeight() - text.GetRect().GetHeight())/(NB_MENU+1)*(i+1));
            }
        }

        const Input& input = BombermanManager::Get().GetApp()->GetInput();

        //textMenu[choix].SetColor(Color(200,40,40)); // On remet sa couleur originale a l'élément du menu
        textMenu[choix].SetColor(Color(12,46,92)); // On remet sa couleur originale a l'élément du menu
        if(input.IsKeyDown(Key::Up))
        {
            if (choix > 0) choix--; // Remonter dans le menu
            else choix=NB_MENU-1;
        }
        if(input.IsKeyDown(Key::Down))
        {
            if(choix < NB_MENU-1) choix++; // Descendre dans le menu
            else choix=0;
        }
        if(input.IsKeyDown(Key::Return))  // Si appuie de la touche Entree
        {
            BombermanManager::Get().GetSound()->Jouer("son/Valider");
            return choix+1;
        }

        textMenu[choix].SetColor(Color(255,255,255)); // On change la couleur de la selection

        for(int i=0; i<NB_MENU; i++)
            BombermanManager::Get().GetApp()->Draw(textMenu[i]); // affichage de tout les elements du menu

        BombermanManager::Get().GetApp()->Display();
        Sleep(0.075f);
    }

    return 6; // Quitter le programme
}

int launcher(View &view)
{
    Font credit;
    String text;
    Clock timer;
    bool afficher = true;

    // Création des textes
    chargerFontFromMemory(credit, text, "police/Balkeno.ttf", "     Développé par\n\n\n Guillaume POLAERT\n\n   Thomas POULAIN\n\n\n         2010-2011", 30);
    text.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-text.GetRect().GetWidth())/2, (BombermanManager::Get().GetApp()->GetHeight()-text.GetRect().GetHeight())/2); // Largeur texte : text.GetRect().GetWidth();
    //text.SetColor(Color(143, 155, 173));
    text.SetColor(Color(255, 255, 255));

    //chargement du fond
    Image img_fond;
    Sprite sprite_fond;
    chargerImageFromMemory(img_fond, sprite_fond, "ressources/fond.jpg", false, false, true);

    timer.Reset();

    while(afficher && timer.GetElapsedTime()<6)
    {
        BombermanManager::Get().GetApp()->Clear();
        BombermanManager::Get().GetApp()->Draw(sprite_fond);


        if(timer.GetElapsedTime()>4) // Au bout de 4 secondes, changer affichage
        {
            text.SetText("BombGirl");
            text.SetColor(Color(143, 155, 173));
            text.SetSize(60);
            text.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-text.GetRect().GetWidth())/2, BombermanManager::Get().GetApp()->GetHeight()/(2*NB_MENU+1)-text.GetRect().GetHeight()/2);
        }

        Event event;
        while(BombermanManager::Get().GetApp()->GetEvent(event))  // EVENEMENT QUITTER OU REDIMENTIONNER
        {
            if (event.Type == Event::Closed)
            {
                BombermanManager::Get().GetApp()->Close();
                return 5; // Quitte le programme
            }
            if (event.Type == Event::Resized)
            {
                view.SetFromRect(FloatRect(0, 0, event.Size.Width, event.Size.Height));
                text.SetPosition((event.Size.Width-text.GetRect().GetWidth())/2, 10+(event.Size.Height-text.GetRect().GetHeight())/2); // Repositionne le texte au centre
            }
            if (event.Key.Code == Key::Space || event.Key.Code == Key::Return || event.Key.Code == Key::Escape) // Si appuie sur espace > passer a l'écran suivant
                afficher=false;
        }

        BombermanManager::Get().GetApp()->Draw(text);
        BombermanManager::Get().GetApp()->Display();
    }
    return 0;
}


string entrerIPServeur(View &view, string ipserveur)
{
    Font police;
    string texte=ipserveur;
    String nom, indication, indication2;
    Image img_fond;
    Sprite sprite_fond;
    chargerImageFromMemory(img_fond, sprite_fond, "ressources/fond.jpg", false, false, true);

    chargerFontFromMemory(police, nom, "police/Balkeno.ttf", "", 60);

    chargerFontFromMemory(police, indication, "police/Balkeno.ttf", "Entrez l'IP du serveur :", 20);
    chargerFontFromMemory(police, indication2, "police/Balkeno.ttf", "Valider : Entrée\nRetour. : Echap", 20);
    indication.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-indication.GetRect().GetWidth())/2, indication.GetRect().GetHeight()*3);
    indication2.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-indication.GetRect().GetWidth())/2, BombermanManager::Get().GetApp()->GetHeight()-3*indication2.GetRect().GetHeight());

    bool afficher = true, valider = false;

    while(afficher)
    {
        Event event;

        while (BombermanManager::Get().GetApp()->GetEvent(event))
        {
            if (event.Type == sf::Event::TextEntered && texte.size()<15 && event.Key.Code != Key::Return) // Saisie de texte
                if (event.Text.Unicode > 30 && event.Text.Unicode < 128) //filtrage des caractères non imprimable
                {
                    texte += static_cast<char>(event.Text.Unicode);
                    valider=true;
                }
            if (event.Key.Code == Key::Back) // Si appuie sur la touche retour arriere : supprimer le texte
                texte=texte.substr(0, texte.size()-1);

            if (event.Key.Code == Key::Escape)
            {
                afficher = false;
                BombermanManager::Get().setShowError(false);
                return "";
            }
            if(event.Type == Event::Closed)
            {
                afficher=false;
                exit(EXIT_SUCCESS);
            }
            if (event.Type == Event::Resized)
            {
                view.SetFromRect(FloatRect(0, 0, event.Size.Width, event.Size.Height));
                nom.SetPosition((event.Size.Width-nom.GetRect().GetWidth())/2, (event.Size.Height-nom.GetRect().GetHeight())/2); // Repositionne le texte au centre
                indication.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-indication.GetRect().GetWidth())/2, indication.GetRect().GetHeight()*3);
                indication2.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-indication.GetRect().GetWidth())/2, BombermanManager::Get().GetApp()->GetHeight()-3*indication2.GetRect().GetHeight());
            }
            if (event.Key.Code == Key::Return && valider && event.Type == Event::KeyReleased) // Pour que le menu ne soit pas quitté instantanément
            {
                BombermanManager::Get().GetSound()->Jouer("Valider");
                afficher = false;
            }
        }

        BombermanManager::Get().GetApp()->Clear();
        BombermanManager::Get().GetApp()->Draw(sprite_fond);

        if(BombermanManager::Get().getShowError()) afficher_info("error", "Adresse ip invalide");

        nom.SetText(texte);
        nom.SetPosition((BombermanManager::Get().GetApp()->GetWidth()-nom.GetRect().GetWidth())/2, (BombermanManager::Get().GetApp()->GetHeight()-nom.GetRect().GetHeight())/2);

        BombermanManager::Get().GetApp()->Draw(nom);
        BombermanManager::Get().GetApp()->Draw(indication);
        BombermanManager::Get().GetApp()->Draw(indication2);

        BombermanManager::Get().GetApp()->Display();
    }

    return texte;
}


void afficher_info(string type, string textToPrint)
{
    Image img_info_icone;
    Sprite sprite_info_icone;
    Shape point;
    Font police;
    String texte;
    int r=0, g=0, b=0, r_c=0, g_c=0, b_c=0;

    chargerFontFromMemory(police, texte, "police/Opificio.ttf", textToPrint);

    if(type == "info")
    {
        chargerImageFromMemory(img_info_icone, sprite_info_icone, "ressources/info.png", true, true, true);
        r=108; g=178; b=230; r_c=48; g_c=160; b_c=224;
        texte.SetPosition(250, 16);
    }
    else if(type == "error")
    {
        chargerImageFromMemory(img_info_icone, sprite_info_icone, "ressources/error.png", true, true, true);
        r=242; g=94; b=94; r_c=240; g_c=48; b_c=48;
        texte.SetPosition(250, 27);
    }


    sprite_info_icone.SetPosition(200, 20);


    point.SetOutlineWidth(2);
    point.EnableFill(false);
    point.AddPoint(Vector2f(200, 15), Color(r, g, b), Color(r_c, g_c, b_c));
    point.AddPoint(Vector2f(440, 15), Color(r, g, b), Color(r_c, g_c, b_c));
    point.AddPoint(Vector2f(440, 60), Color(r, g, b), Color(r_c, g_c, b_c));
    point.AddPoint(Vector2f(200, 60), Color(r, g, b), Color(r_c, g_c, b_c));



    BombermanManager::Get().GetApp()->Draw(point);
    BombermanManager::Get().GetApp()->Draw(sprite_info_icone);
    BombermanManager::Get().GetApp()->Draw(texte);
}



