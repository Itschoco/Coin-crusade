#pragma once
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H
#include "raylib.h"

class SoundManager
{
public:
	void LoadSounds();
	void PlayHitSound();
	void PlayEnemyDeadSound();
	void PlayPlayerDeadSound();
	void PlayBackgroundMusic();
	void PlayCollectSound();
	void StopSounds();
	void UnloadSounds();

private:
	Sound swordSound;
	Sound enemySound;
	Sound playerSound;
	Sound collectSound;
	Music backgroundMusic;
	bool soundsLoaded;
};



#endif // SOUNDMANAGER_H
