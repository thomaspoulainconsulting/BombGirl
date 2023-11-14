#include <SFML/Network.hpp>
#include <iostream>
#include "SuperClass.h"
#include "Reseau.h"
#define PORT 4567

using namespace std;
using namespace sf;


Reseau::Reseau() : AddrServeur("127.0.0.1")
{

}

void Reseau::envoiBroadCast()
{
    SocketUDP broadcast;

    IPAddress Address = IPAddress::GetLocalAddress();

    char Buffer[] = "Bonjour";

    if (broadcast.Send(Buffer, sizeof(Buffer), "255.255.255.255", PORT) != Socket::Done) //Broadcast
        cout << "Erreur de création du serveur"<<endl;

    cout << "Envoit Broadcast" << endl;

}

void Reseau::lancerServeur()
{

}

void Reseau::lancerClient()
{

}

void Reseau::ecouteBroadCast()
{
    //On écoute s'il y a un serveur de créée
    SocketUDP ecoute;
    ecoute.SetBlocking(false);

    if (!ecoute.Bind(PORT))
        cout << "Erreur d'écoute du client";

    char Buffer[128];
    size_t Received;
    IPAddress Sender;
    unsigned short Port;

    sf::Socket::Status Status = ecoute.Receive(Buffer, sizeof(Buffer), Received, Sender, Port);
    if(Status == Socket::Status::Done)
        cout << "Message recu : "<< Buffer << ", sender : " << Sender.ToString() <<endl;

    AddrServeur=Sender.ToString();
    ecoute.Close();
}


