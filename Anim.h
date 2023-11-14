#ifndef ANIM_H
#define ANIM_H

#include <SFML/System.hpp>
#include "PausableClock.h"

/*!
*	\class Anim
*	Impl�mente la gestion des animation selon le temps.
*/
class Anim
{
public:
	Anim(void);
	virtual ~Anim(void);

	//!Change au prochain frame d'animation
	void nextFrame();

	//!Change au frame d'animation d�fini par 'count'
	virtual void setFrame(const unsigned int &count);

	//!Change au premier frame animation
	void reset();

	//!Retourne le frame d'animation courante
	unsigned int currentFrame() const;


	//!D�finis si l'animation est en boucle (choice=true)
	void loop(const bool &choice);

	//!Retourne si l'animation est jou� en boucle
	bool isLoop() const;


	//!Joue l'animation
	void play();

	//!Arrete l'animation et remet le compteur � z�ro
	void stop();

	//!Met l'animation en pause et laisse le compteur o� il en est.
	void pause();

	//!Retourne true si l'animation joue
	bool isPlaying() const;

	//!D�finis le d�lais en seconde entre chaque frame.
	void setDelay(const float &delay);

	//!Retourne le d�lai entre chaque frame
	float delay() const;

	//!Met � jour l'animation
	virtual void update();

	//!Retourne le nombre de frame de l'animation
	virtual unsigned int getSize() const=0;
protected:
	//!Timer de l'animation
	PausableClock m_time;

private:
	//!Frame courant de l'animation
	unsigned int m_frameCount;
	//!Delai en seconde entre chaque animation
	float m_delay;
	//!Si l'animation est en boucle
	bool m_isLoop;
	//!Si l'animation est en train de jouer
	bool m_play;
};

#endif
