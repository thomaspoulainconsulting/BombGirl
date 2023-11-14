#ifndef PAUSABLECLOCK_H
#define PAUSABLECLOCK_H

#include <SFML/System.hpp>


class PausableClock : public sf::Clock
{
public:
	//!Initialise le timer et le start
	PausableClock();
	//!Met en pause le timer
	void Pause();
	//!Met en marche le timer
	void Play();
	//!Change l'�tat du timer (pause<->marche)
	bool Toggle();
	//!Arrete le timer et le remet � z�ro
	void Stop();
	//!Remet le timer � z�ro et le met en marche
	void Reset();
	//!Retourne le temps �couler depuis la derni�re fois qu'il � �t� mis en marche (avec Reset ou Play).
	float GetElapsedTime();

private:
	float m_elapsedTime;
	bool m_pause;
};

#endif
