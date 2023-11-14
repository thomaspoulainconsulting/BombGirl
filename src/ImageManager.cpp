/*
* Class ImageManager
* Manage sf::Image creation for better use of memory
* @author : Adrian Gaudebert - adrian.gaudebert@gmail.com
*/

#include "ImageManager.hpp"
#include "cDAT.h"
#include "SuperClass.h"

using namespace std;

ImageManager ImageManager::sInstance;

/* -------------------------- */
/* ImageManager::ImageManager */
/* -------------------------- */
ImageManager::ImageManager()
{

}

/* --------------------------- */
/* ImageManager::~ImageManager */
/* --------------------------- */
ImageManager::~ImageManager()
{
    //On détruit toutes les images restantes
    std::map<std::string, sf::Image*>::iterator it;
    for(it = Images.begin(); it != Images.end(); it++)
    {
        delete it->second;
    }
}

/* ------------------------- */
/* ImageManager::SearchImage */
/* ------------------------- */
bool ImageManager::SearchImage(const std::string &ImageLink, sf::Image* &ImgTemp)
{
    std::map<std::string, sf::Image*>::iterator it;

    it = Images.find(ImageLink);

    if(it == Images.end())
    {
        return false;
    }
    else
    {
        ImgTemp = it->second;
    }

    return true;
}

/* ------------------------- */
/* ImageManager::CreateImage */
/* ------------------------- */
sf::Image *ImageManager::CreateImage(const std::string &ImageLink, bool ressources, std::string nomMap)
{
    sf::Image *ImgTemp = new sf::Image();


    cDAT read_test;
    char* buffer;
    if(ressources) read_test.Read("Ressources.dat");
    else if(nomMap != "") read_test.Read(nomMap);
    else read_test.Read(BombermanManager::Get().GetDamier().getNomFichierDat());

    buffer = read_test.GetFile(ImageLink);
    if(buffer == NULL)
    {
        std::cout << "Erreur ouverture image : " << ImageLink<<std::endl;
        std::cout << "Ressouces.dat manquant..." <<std::endl<<"Arret";

        //exit(EXIT_SUCCESS);;
    }

    if(ImgTemp->LoadFromMemory(buffer, read_test.GetFileSize(ImageLink)))
    {
        Images.insert(std::pair<std::string, sf::Image*>(ImageLink, ImgTemp));
        return ImgTemp;
    }
    else
    {
        delete ImgTemp;
        std::cout << "Erreur de chargement de l'image : "<< ImageLink << std::endl;
    }



    return NULL;
}

/* ---------------------- */
/* ImageManager::GetImage */
/* ---------------------- */
sf::Image *ImageManager::GetImage(const std::string &ImageLink, bool ressources, std::string nomMap, bool memeNomMaisImageDifferente)
{
    sf::Image *ImgTemp;

    //Si l'image demandée n'existe pas déjà, on la créée
    if(memeNomMaisImageDifferente || !SearchImage(ImageLink, ImgTemp))
    {
        ImgTemp = CreateImage(ImageLink, ressources, nomMap);
    }

    return ImgTemp;
}

/* ------------------------- */
/* ImageManager::DeleteImage */
/* ------------------------- */
void ImageManager::DeleteImage(sf::Image &Img)
{
    std::map<std::string, sf::Image*>::iterator it;

    //On cherche l'image demandée
    for(it = Images.begin(); it != Images.end(); it++)
    {
        //Si on la trouve, on la détruit et on la retire
        if(it->second == &Img)
        {
            delete &Img;
            Images.erase(it);
            break;
        }
    }
}

/* ------------------------- */
/* ImageManager::DeleteImage */
/* ------------------------- */
void ImageManager::DeleteImage(const std::string& ImageLink)
{
    std::map<std::string, sf::Image*>::iterator it;

    it = Images.find(ImageLink);

    if(it != Images.end())
    {
        if(it->second != NULL)
        {
            delete (it->second);
            Images.erase(it);
        }
    }
}

bool ImageManager::ExistImageOnDat(std::string dat, std::string nomImg)
{
    cDAT read_test;
    char* buffer;
    read_test.Read(dat);

    buffer = read_test.GetFile(nomImg);
    if(buffer == NULL) return false;
    else return true;
}
