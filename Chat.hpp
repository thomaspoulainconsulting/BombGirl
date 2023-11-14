#ifndef DEF_CHAT
#define DEF_CHAT
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <string>


class Chat
{
    private:
        std::vector<std::string> mText;
        std::vector<std::string> mPseudos;
        std::vector<sf::SocketTCP> mClients;
        std::string mTextEnCours;
        sf::String mStringEnCours;

    public:
        Chat();
        void afficher();
        void addText();
        void addText(std::string pseudo, std::string texte);
        void updateTextEnCours(char text);
        void eraseTextEnCours();
        void retourArriere();
        int getSizeTextEnCours();
        sf::Color getColorFromPseudo(std::string pseudo);
        /*int getNbClients();
        void addClient(sf::SocketTCP addr);
        sf::SocketTCP getClient(int indice);*/

};



#endif
