#include <iostream>
#include <fstream>
#include <cassert>
#include "SuperClass.h"
#include "Serveur.h"
#include "fonctions.h"
#include "EventNetwork.h"

const int PORT_TCP = 61585;

using namespace sf;
using namespace std;

Serveur::Serveur(int nbjoueur, int nbmaxjoueur, string nommap) : mNbJoueurs(nbjoueur), mMaxNbJoueurs(nbmaxjoueur), mNomMap(nommap)
{}

int Serveur::getNbJoueurs() {return mNbJoueurs;}
string Serveur::getNomMap() {return mNomMap;}
int Serveur::getNbMaxJoueurs() {return mMaxNbJoueurs;}
void Serveur::incrementeNbJoueurs() { mNbJoueurs+=1;}
void Serveur::decrementeNbJoueurs() { mNbJoueurs-=1;}
void Serveur::addClient(sf::SocketTCP addr){ mClients.push_back(addr);}
sf::SocketTCP Serveur::getClient(int indice){return mClients.at(indice);}
void Serveur::eraseClient(int indice) { assert(indice < (int)this->mClients.size() && "Erreur d'index"); mClients.erase(mClients.begin()+indice); }


void Serveur_Complet(void* UserData)
{
//On ajoute le pseudo du serveur dans le vecteur qui contient tous les pseudos des gens qui jouent
BombermanManager::Get().RegisterPseudoJoueurs(BombermanManager::Get().GetPseudo());


UserData=NULL;


sf::SocketTCP Listener;

if (!Listener.Listen(PORT_TCP))
    return;

cout << "Lancement du serveur sur le port "<< PORT_TCP << " - en attente de connections..."<<endl;

sf::SelectorTCP Selector;
Selector.Add(Listener);

bool ecoute_server_begin=false;

while (true)
{
    // Get the sockets ready for reading
    unsigned int NbSockets = Selector.Wait(0.01f);

    /*ENVOI DES EVENTS RESEAU*/
    if(ecoute_server_begin)
    {
        //On liste les eventnetwork et on les envoit au client
        BombermanManager::Get().MutexLock();
        for(int i=0;i<BombermanManager::Get().GetEvent()->getNbEvent();i++)
        {
            Packet event = BombermanManager::Get().GetEvent()->getEventAt(i);

            for(int f=0;f<BombermanManager::Get().GetServeur()->getNbJoueurs()-1;f++)
            {
                SocketTCP Client = BombermanManager::Get().GetServeur()->getClient(f);
                Client.Send(event);
            }
        }
        BombermanManager::Get().GetEvent()->removeAllEvent();
        BombermanManager::Get().MutexUnLock();
    }


    // We can read from each returned socket
    for (unsigned int i = 0; i < NbSockets; ++i)
    {
        // Get the current socket
        sf::SocketTCP Socket = Selector.GetSocketReady(i);
        if (Socket == Listener)
        {
            // If the listening socket is ready, it means that we can accept a new connection
            sf::IPAddress Address;
            sf::SocketTCP Client;
            Listener.Accept(Client, &Address);
            std::cout << "Client connecte !  - ip (" << Address << ")" << std::endl;

            // Add it to the selector
            Selector.Add(Client);
            BombermanManager::Get().GetServeur()->addClient(Client); //On ajoute la socket du client dans un vector (utile pour redistribuer les infos des clients)
        }
        else
        {
            if(ecoute_server_begin)
            {

                /*ON SE MET EN ECOUTE DES CLIENTS*/
                sf::Packet infos_recu;
                if (Socket.Receive(infos_recu) == sf::Socket::Done)
                {
                    int typepacket;
                    int identifiant;
                    int x;
                    int y;
                    int etat;
                    int etat2;
                    std::string pseudo, message;

                    infos_recu >> typepacket >> identifiant >> etat >> x >> y >> etat2 >> pseudo >> message;

                    if(typepacket != MOUVEMENT)
                        cout << typepacket << " " << identifiant << " " << etat << " " << x << " " << y<< " " << etat2 << pseudo << message << endl;

                    // Si le paquet ne concerne pas l'action qu'on vient de faire, on éxécute ce qu'on reçoit
                    if(identifiant != BombermanManager::Get().getIdentifiant())
                    {
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
                            if(etat == PRIS)
                            {
                                //on cherche dans le vector d'item celui qui correspond à celui qu'on reçoit
                                //puis on l'efface du vector
                                int case_x = ceil((float)(x-GESTION_ITEM_MENU) / (float)DEPLACEMENT); //on cast en float pour pouvoir arrondir
                                int case_y = (y-GESTION_BBM_MENU) / DEPLACEMENT; //pas de ceil pour arrondir (colonnes)


                                //on liste les items présents
                                for(int i=0;i<BombermanManager::Get().getNbItem();i++)
                                {
                                    if(BombermanManager::Get().GetItem(i).getX() == case_x && BombermanManager::Get().GetItem(i).getY() == case_y)
                                        BombermanManager::Get().EraseItem(i);
                                }

                            }
                        }
                        else if(typepacket == ETAT_JOUEUR)
                        {
                            if(etat == MORT)
                                BombermanManager::Get().GetBomberman(identifiant)->SetMort(true);
                        }
                        else if(typepacket == CHAT)
                        {
                            BombermanManager::Get().GetChat()->addText(pseudo, message);
                        }



                        //on répercute les infos reçu vers tous les autres clients
                        if(BombermanManager::Get().GetServeur()->getNbJoueurs() > 1)
                        {
                            for(int i=0;i<BombermanManager::Get().GetServeur()->getNbJoueurs()-1;i++)
                            {
                                if(i+1 != identifiant)
                                {
                                    SocketTCP Client = BombermanManager::Get().GetServeur()->getClient(i);
                                    if(Client.Send(infos_recu) != sf::Socket::Done)
                                        cout << "erreur envoit serveur"<<endl;
                                }
                            }
                        }

                    }

                }

            }
            else
            {

                sf::Packet Packet_pseudo;

                /*RECEPTION PSEUDO*/
                if (Socket.Receive(Packet_pseudo) == sf::Socket::Done)
                {
                    std::string pseudo;
                    Packet_pseudo >> pseudo;

                    /*if(chaine_complexe == "69aec83e7c576a185d8dd35d3f22cb2a7927dd39")
                    {
                        cout << "chaine complexe"<<endl;
                        //on retire le pseudo de la liste
                        for(int i=0; i<BombermanManager::Get().getNbPseudoJoueurs(); i++)
                        {
                            if(BombermanManager::Get().GetPseudoJoueurs(i) == pseudo)
                            {
                                BombermanManager::Get().ErasePseudoJoueurs(i);
                                cout << "pseudo"<<endl;
                                break;
                            }
                        }
                        BombermanManager::Get().GetServeur()->eraseClient(identifiant);
                        BombermanManager::Get().GetServeur()->decrementeNbJoueurs();

                        break;
                    }*/


                    /*GESTION PSEUDO UNIQUE*/
                    for(int i=0; i<BombermanManager::Get().getNbPseudoJoueurs(); i++)
                    {
                        if(BombermanManager::Get().GetPseudoJoueurs(i) == pseudo)
                        {
                            pseudo=pseudo+"1";
                        }
                    }

                    /*GESTION IDENTIFIANT*/
                    int identifiant=BombermanManager::Get().getNbPseudoJoueurs();

                    /*ENREGISTREMENT DU PSEUDO*/
                    BombermanManager::Get().RegisterPseudoJoueurs(pseudo);


                    sf::Packet packet_infos;
                    packet_infos << pseudo << identifiant << BombermanManager::Get().GetServeur()->getNbMaxJoueurs() << BombermanManager::Get().GetServeur()->getNomMap();



                    /*ON INCREMENTE LE NOMBRE DE JOUEURS*/
                    BombermanManager::Get().GetServeur()->incrementeNbJoueurs();

                    //On envoit le paquet d'infos
                    Socket.Send(packet_infos);

                    /*SI TOUTES LES PLACES SONT PRISES*/
                    if(BombermanManager::Get().GetServeur()->getNbJoueurs() == BombermanManager::Get().GetServeur()->getNbMaxJoueurs())
                    {
                        /*ON ENVOIT LE SIGNAL DE DEPART*/
                        Packet depart;

                        for(int i=0;i< BombermanManager::Get().getNbPseudoJoueurs();i++)
                        {
                            depart << BombermanManager::Get().GetPseudoJoueurs(i);
                        }

                        for(int i=0;i<BombermanManager::Get().getNbPseudoJoueurs()-1;i++)
                        {
                            sf::SocketTCP Client = BombermanManager::Get().GetServeur()->getClient(i);
                            Client.Send(depart);
                        }

                        /*ON SE MET EN ECOUTE DES CLIENTS*/
                        ecoute_server_begin=true;

                        /*ON SIGNALE AU JEU LE DEBUT DE LA PARTIE*/
                        //BombermanManager::Get().RegisterDebut(true);

                        /*ON DEBLOQUE LA SOCKET POUR POUVOIR ECOUTER ET ENVOYER DES INFOS EN MEME TEMPS*/
                        Socket.SetBlocking(false);

                    }
                }
                else
                {
                    // Error : we'd better remove the socket from the selector
                    Selector.Remove(Socket);
                }

            }
        }
    }


}
}

void Serveur::deleteClient()
{
    mClients.clear();
}
