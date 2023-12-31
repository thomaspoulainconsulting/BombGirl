#ifndef IMGANIM_H
#define IMGANIM_H

#include <SFML/Graphics.hpp>
#include "Anim.h"

/*!
*	\class ImgAnim
*	Comme Anim � la dif�rence pr�s que la classe utilise une image avec tout les sprites
*	au lieu des couches d'une layer.
*/
class ImgAnim :
	public Anim, public sf::Sprite
{
public:
	/*
	*	\param img Image avec les sprites
	*	\param rows Nombre de frame par animation
	*	\param line Nombre de ligne d'animation
	*	\param leReste Voir la documentation de sf::Sprite
	*/
	ImgAnim(const sf::Image &Img, const unsigned int &nbFrame, const unsigned int &line=1,
			const sf::Vector2f &Position=sf::Vector2f(0, 0),
			const sf::Vector2f &Scale=sf::Vector2f(1, 1), float Rotation=0.f,
			const sf::Color &Col=sf::Color(255, 255, 255, 255));

    ImgAnim();


	//!D�finis les dimension d'un frame
	void setFrameDim(const unsigned int &w, const unsigned int &h);

	//!Retourne la dimension d'un frame
	sf::IntRect frameDim() const;

	//!D�finis le d�calage de l'animation sur l'image (permet d'avoir plusieurs anim sur une seule image)
	void setOffset(const unsigned int &x, const unsigned int &y);

	//!Retourne un rectangle avec l'offset
	sf::IntRect offset() const;

	//!D�finis la ligne anim� courante
	void setAnimRow(const unsigned int &row);

	//!Retourne la ligne anim� courante
	int animRow() const;

	//!D�finis le frame courant
	virtual void setFrame(const unsigned int &count);

	//!Met � jour le rectangle d'animation
	void refreshSubRect();

	//!D�finis le nombre de frame dans l'animation
	void setSize(const unsigned int &size);

	//!Retourne le nombre de frame
	virtual unsigned int getSize() const;
protected:
	virtual void Render(sf::RenderTarget& Target) const;

private:
	//!Repr�sente le nombre de frame dans l'animation
	unsigned int m_size;
	//!Ligne d'animation courante
	unsigned int m_animRow;
	//!D�calage � gauche du frameset
	unsigned int m_xOffset;
	//!D�calage en haut du frameset
	unsigned int m_yOffset;
};

#endif
