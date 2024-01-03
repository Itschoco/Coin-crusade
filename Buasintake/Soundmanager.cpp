#include "SoundManager.h"
#include <iostream>



// Load the sound files for the game
void SoundManager::LoadSounds()
{
	swordSound = LoadSound("sounds/sword.wav"); // Load the soun effect for when the sword swings
	enemySound = LoadSound("sounds/enemy.wav");// Load the sound effect for when a enemy die
	playerSound = LoadSound("sounds/player.wav");// Load the sound effect for when the player die
	collectSound = LoadSound("sounds/collect.wav");//Load the the sound effecr for when the player collects a coin
	backgroundMusic = LoadMusicStream("bgm/background.mp3");// Load the background music
	soundsLoaded = true; // Set the flag indicating that the sounds have been loaded
}

// Play the sound effect for when the swords swings
void SoundManager::PlayHitSound()
{
	PlaySound(swordSound);// Play the sound effect
	SetSoundVolume(swordSound, 0.5f);// Set the volume of the sound effect
}

void SoundManager::PlayEnemyDeadSound()
{
	PlaySound(enemySound);// Play the sound effect
	SetSoundVolume(enemySound, 0.5f);// Set the volume of the sound effect
}

void SoundManager::PlayCollectSound()
{
	PlaySound(collectSound);// Play the sound effect
	SetSoundVolume(collectSound, 0.5f);// Set the volume of the sound effect
}




void SoundManager::PlayPlayerDeadSound()
{
	PlaySound(playerSound);// Play the sound effect
	SetSoundVolume(playerSound, 0.5f);// Set the volume of the sound effect

}


// Play the background music
void SoundManager::PlayBackgroundMusic()
{
	PlayMusicStream(backgroundMusic); // Play the music
	SetMusicVolume(backgroundMusic, 0.1f); // Set the volume of the music
	UpdateMusicStream(backgroundMusic); // Update the music stream
}



// Stop all currently playing sounds
void SoundManager::StopSounds()
{
	StopSound(swordSound); // Stop the sword sound effect
	StopSound(enemySound); // Stop the enemy dead sound effect
	StopSound(playerSound); // Stop the player dead sound effect
	StopSound(collectSound);
}


// Unload the sound files for the game
void SoundManager::UnloadSounds()
{
	if (soundsLoaded)
	{
		UnloadSound(swordSound); // Unload the hit sound effect
		UnloadMusicStream(backgroundMusic); // Unload the background music
		UnloadSound(enemySound); // Unload the score sound effect
		UnloadSound(playerSound); // Unload the hit sound effect
		UnloadSound(collectSound); //Unl
		soundsLoaded = false; // Set the flag indicating that the sounds have been unloaded
	}
}


