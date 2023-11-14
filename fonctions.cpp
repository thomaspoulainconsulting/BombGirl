#include <iostream>
#include "fonctions.h"
#include "cDAT.h"
#include "ImageManager.hpp"
#include <cstdio>

using namespace std;
using namespace sf;

void chargerImageFromMemory(sf::Image &image, sf::Sprite &sprite, string nomImage, bool transparence, bool setsmooth, bool ressources, string nomMap, bool alpha, bool memeNomMaisImageDifferente)
{
    image = (*ImageManager::Get().GetImage(nomImage, ressources, nomMap, memeNomMaisImageDifferente)); //On déréférence le pointeur avec *

    if(transparence) image.CreateMaskFromColor(Color(255,255,255));
    if(setsmooth) image.SetSmooth(false);
    if(alpha) sprite.SetColor(Color(255,255,255,192));
    sprite.SetImage(image);
}

void chargerFontFromMemory(sf::Font &font, sf::String &mNom, std::string nomFont, std::string nomTexte, int taille)
{
    cDAT read_test;
    char* buffer;
    read_test.Read("Ressources.dat");

    buffer = read_test.GetFile(nomFont);
    if(buffer == NULL) { cout << "Erreur ouverture " << nomFont<<endl; return; }

    font.LoadFromMemory(buffer, read_test.GetFileSize(nomFont));
    mNom.SetText(nomTexte); // Texte
    mNom.SetFont(font); // Fonte
    mNom.SetSize(taille); // Taille de police
    mNom.SetStyle(String::Regular); // Mode par défaut
}

bool is_valid_ip(const char *ip_str)
{
	unsigned int n1,n2,n3,n4;

	if(sscanf(ip_str,"%u.%u.%u.%u", &n1, &n2, &n3, &n4) != 4) return false;

	if((n1 != 0) && (n1 <= 255) && (n2 <= 255) && (n3 <= 255) && (n4 <= 255))
		return true;

	return false;
}

