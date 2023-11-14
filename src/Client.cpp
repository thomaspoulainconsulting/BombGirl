#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include "Client.h"
#include "Partie.h"
#include "SuperClass.h"
#include "fonctions.h"
#include "EventNetwork.h"

const int PORT_TCP = 61585;
enum {HAUT, BAS, GAUCHE, DROITE};

using namespace sf;
using namespace std;




Client::Client(IPAddress serveur) : mAddrServeur(serveur) {}


bool Client::getAttenteDeConnexion() {return mAttenteDeConnexion; }
void Client::setAttenteDeConnexion(bool value) {mAttenteDeConnexion=value;}



void Client_Complet(void* UserData)
{
    UserData=NULL;

    sf::IPAddress ServerAddress;
    ServerAddress=BombermanManager::Get().GetIpServeur();


    sf::SocketTCP Socket;

    BombermanManager::Get().GetClient()->setAttenteDeConnexion(true); //en attente ...

    if (Socket.Connect(PORT_TCP, ServerAddress) != sf::Socket::Done)
        return;

    BombermanManager::Get().GetClient()->setAttenteDeConnexion(false); //connecté !

    bool Connected = true;
    bool ecoute_server_begin=false;

    while (Connected)
    {
        if(ecoute_server_begin)
        {

            //On liste les eventnetwork et on les envoit au serveur
            BombermanManager::Get().MutexLock();
            for(int i=0;i<BombermanManager::Get().GetEvent()->getNbEvent();i++)
            {
                Packet event = BombermanManager::Get().GetEvent()->getEventAt(i);
                Socket.Send(event);
            }
            BombermanManager::Get().GetEvent()->removeAllEvent();
            BombermanManager::Get().MutexUnLock();


            /*ON ECOUTE LES INFOS DU SERVEUR SUR LES EVENEMENTS DES CLIENTS*/
            Packet infos_serveur;
            if(Socket.Receive(infos_serveur) == sf::Socket::Done)
            {
                int typepacket;
                int identifiant;
                int x;
                int y;
                int etat;
                int etat2;
                std::string pseudo, message;


                infos_serveur >> typepacket >> identifiant >> etat >> x >> y >> etat2 >> pseudo >> message;
                if(typepacket != MOUVEMENT)
                cout << typepacket << " " << identifiant << " " << etat << " " << x << " " << y<<etat2 << endl;

                // Si le paquet ne concerne pas l'action qu'on vient de faire, on exécute ce qu'on reçoit
                //if(identifiant != BombermanManager::Get().getIdentifiant())
               //{
                    if(typepacket == MOUVEMENT)
                    {
                        BombermanManager::Get().GetBomberman(identifiant)->setCoord(x, y);
                        BombermanManager::Get().GetBomberman(identifiant)->setFrame(etat, etat2);
                    }
                    else if(typepacket == BOMBE)
                    {
                        if(etat == DEPOSE)
                            BombermanManager::Get().GetBomberman(identifiant)->poserBombe(x, y, identifiant, etat2);
                        else if(etat == DEPLACER)
                        {
                            BombermanManager::Get().GetBombe(identifiant).setX(x);
                            BombermanManager::Get().GetBombe(identifiant).setY(y);
                        }
                    }
                    else if(typepacket == ITEM)
                    {
                        if(etat == DEPOSE) {
                            //cout << "depos item " << " " << identifiant << " " << etat << " " << x << " " << y<<etat2 << endl;
                            BombermanManager::Get().RegisterItem(Item(x, y, identifiant));
                        }
                        else if(etat == PRIS)
                        {
                            //on cherche dans le vector d'item celui qui correspond à celui qu'on reçoit
                            //puis on l'efface du vector
                            int case_x, case_y;

                            /*switch(etat2) //etat2 = direction du perso
                            {
                                case HAUT:
                                    case_x= ceil((float)(x-GESTION_ITEM_MENU) / (float)DEPLACEMENT); //on cast en float pour pouvoir arrondir
                                    case_y = (float)(y-GESTION_BBM_MENU) / (float)DEPLACEMENT; //pas de ceil pour arrondir (colonnes)
                                break;
                                case BAS:
                                    case_x= (float)(x-GESTION_ITEM_MENU) / (float)DEPLACEMENT; //on cast en float pour pouvoir arrondir
                                    case_y = (float)(y-GESTION_BBM_MENU) / (float)DEPLACEMENT; //pas de ceil pour arrondir (colonnes)
                                break;
                                case GAUCHE:
                                break;
                                case DROITE:
                                    case_x= ceil((float)(x-GESTION_ITEM_MENU) / (float)DEPLACEMENT); //on cast en float pour pouvoir arrondir
                                    case_y = (float)(y-GESTION_BBM_MENU) / (float)DEPLACEMENT; //pas de ceil pour arrondir (colonnes)
                                break;
                            }

                            int case_x = ceil((float)(x-GESTION_ITEM_MENU) / (float)DEPLACEMENT); //on cast en float pour pouvoir arrondir
                            int case_y = (float)(y-GESTION_BBM_MENU) / (float)DEPLACEMENT; //pas de ceil pour arrondir (colonnes)

                            cout << case_x << " - " << case_y << endl;*/

                            BombermanManager::Get().EraseItem(etat2);




                            //on liste les items présents
                            /*for(int i=0;i<BombermanManager::Get().getNbItem();i++)
                            {
                                if(BombermanManager::Get().GetItem(i).getX() == case_x && BombermanManager::Get().GetItem(i).getY() == case_y)
                                    BombermanManager::Get().EraseItem(i);
                            }*/
                        }
                    }
                    else if(typepacket == ETAT_JOUEUR)
                    {
                        if(etat == MORT)
                            BombermanManager::Get().GetBomberman(identifiant)->SetMort(true);
                    }
                    else if(typepacket == GAMEOVER)
                    {
                        BombermanManager::Get().SetGameOver(true);
                    }
                    else if(typepacket == CHAT)
                    {
                        BombermanManager::Get().GetChat()->addText(pseudo, message);
                    }
                    else if(typepacket == DEBUT_PARTIE)
                    {
                        BombermanManager::Get().RegisterDebut(true);
                    }

                //}

            }
            else sf::Sleep(0.009f);

        }
        else
        {
            string pseudo=BombermanManager::Get().GetPseudo();

            /*ENVOI DU PSEUDO*/
            sf::Packet envoie_pseudo, packet_infos;
            envoie_pseudo << pseudo;
            Connected = (Socket.Send(envoie_pseudo) == sf::Socket::Done);


            if (Socket.Receive(packet_infos) == sf::Socket::Done)
            {
                /*RECEPTION INFOS PSEUDO ET IDENTIFIANT*/
                std::string pseudo1;
                int identifiant;
                int nbJoueursMax;
                string nomMap;
                packet_infos >> pseudo1 >> identifiant >> nbJoueursMax >> nomMap;

                /*ENREGISTREMENT PSEUDO ET IDENTIFIANT*/
                BombermanManager::Get().RegisterIdentifiant(identifiant);
                BombermanManager::Get().RegisterPseudo(pseudo1);

                /*CHARGEMENT DE LA MAP*/
                if(nomMap != "") {
                    string ext = ".dat";
                    Damier damier(REPERTOIRE_MAP+nomMap+ext);
                    BombermanManager::Get().RegisterDamier(damier);
                    BombermanManager::Get().GetSound()->chargerSonsJeu();

                    /*RECEPTION DE TOUS LES PSEUDOS DES JOUEURS*/
                    Packet pseudo_recu;
                    if (Socket.Receive(pseudo_recu) == sf::Socket::Done)
                    {
                        for(int i=0;i<nbJoueursMax;i++)
                        {
                            string pseudo_joueurs;
                            pseudo_recu>>pseudo_joueurs;
                            BombermanManager::Get().RegisterPseudoJoueurs(pseudo_joueurs);
                        }
                    }

                    /*ON SIGNALE AU JEU LE DEBUT DE LA PARTIE*/
                    //BombermanManager::Get().RegisterDebut(true);
                    ecoute_server_begin=true;

                    /*ON DEFINIE LA SOCKET COMME NON BLOQUANTE POUR POUVOIR ECOUTER ET ENVOYER EN MEME TEMPS*/
                    Socket.SetBlocking(false);

                }
            }

        }

    }
    cout << "Client closed" << endl;
    Socket.Close();
}


