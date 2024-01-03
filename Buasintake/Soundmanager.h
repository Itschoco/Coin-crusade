#pragma once
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "raylib.h"

class SoundManager
{
public:
	// Function declarations
	void LoadSounds();
	void PlayHitSound();
	void PlayEnemyDeadSound();
	void PlayPlayerDeadSound();
	void PlayBackgroundMusic();
	void PlayCollectSound();
	void StopSounds();
	void UnloadSounds();

private:
	Sound swordSound;       // Sword sound
	Sound enemySound;     // Enemy dead sound
	Sound playerSound;    // Player dead sound
	Sound collectSound;
	Music backgroundMusic;
	bool soundsLoaded;
};



#endif // SOUNDMANAGER_H
