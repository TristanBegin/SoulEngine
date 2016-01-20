/******************************************************************************
Author: Caleb Klomparens

Name: KMODEngine.c

Created: 01/11/16

Description: This file implements FMOD with the low-level API. It serves as
the master class for major audio functions.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "fmod.h"
#include "AEEngine.h"

FMOD_SYSTEM *kmod;
FMOD_RESULT result;

void KMOD_Initialize(int numChannels)
{
	// Create the system and check for success.
	result = FMOD_System_Create(&kmod);

	if (result != FMOD_OK)
	{
		// If FMOD failed to start, kill the program!
		MessageBox(NULL, "FMOD failed to create!", "Error:",
			MB_ICONEXCLAMATION | MB_OK);
	}

	// Initialize the system with super duper settings.
	result = FMOD_System_Init(kmod, numChannels, FMOD_INIT_NORMAL, NULL);
	if (result != FMOD_OK)
	{
		// If FMOD failed to initialize, kill the program!
		MessageBox(NULL, "FMOD failed to Initialize!", "Error:",
			MB_ICONEXCLAMATION | MB_OK);
	}
}

void KMOD_Terminate(void)
{
	result = FMOD_System_Release(&kmod);
	if (result != FMOD_OK)
	{
		// If FMOD failed to end, force end it!
		MessageBox(NULL, "FMOD failed to terminate!", "Error:",
			MB_ICONEXCLAMATION | MB_OK);
	}
}

/******************************************************************************
Name: KPlaySound

Description: Plays a sound with FMOD. Use for short uncompressed samples.

Inputs:
filename - The name of the file that contains the sound to play.
vol - The volume at which the sound should play. 0.0f - 1.0f

Returns: The channel of the played sound.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
FMOD_CHANNEL *KMOD_PlaySound(char *filename, float vol)
{
	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel;

	// Create a sound from a file.
	result = FMOD_System_CreateSound(kmod, filename, FMOD_DEFAULT, 0, &sound);
	if (result != FMOD_OK)
	{
		// If FMOD failed to make a sound, kill it!
		MessageBox(NULL, filename, "Couldn't find source file:",
			MB_ICONEXCLAMATION | MB_OK);
		exit(-1);
	}

	// Play the sound!
	FMOD_System_PlaySound(kmod, sound, 0, 0, &channel);
	if (result != FMOD_OK)
	{
		// If FMOD failed to play the sound, kill it!
		MessageBox(NULL, "FMOD failed to play the sound!", filename,
			MB_ICONEXCLAMATION | MB_OK);
		exit(-1);
	}

	FMOD_Channel_SetVolume(channel, vol);
	return channel;
}

/******************************************************************************
Name: KPlayStream

Description: Plays a stream with FMOD. Use for long samples like music tracks.

Inputs:
filename - The name of the file that contains the stream to play.
loop - A boolean to choose whether the track should automatically loop or not.

Returns: The channel of the played stream.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/
FMOD_CHANNEL *KMOD_PlayStream(char *filename, int loopSound)
{
	FMOD_SOUND *sound;
	FMOD_CHANNEL *channel;

	// Create a sound from a file. Choose whether it should loop, or no.
	if (!loopSound)
		result = FMOD_System_CreateStream(kmod, filename, FMOD_DEFAULT, 0, &sound);
	else result = FMOD_System_CreateStream(kmod, filename, FMOD_LOOP_NORMAL, 0, &sound);

	if (result != FMOD_OK)
	{
		// If FMOD failed to make a sound, kill it!
		MessageBox(NULL, filename, "Couldn't find source file:",
			MB_ICONEXCLAMATION | MB_OK);
	}

	// Play the sound!
	FMOD_System_PlaySound(kmod, sound, 0, 0, &channel);
	if (result != FMOD_OK)
	{
		// If FMOD failed to play the sound, kill it!
		MessageBox(NULL, "FMOD failed to play the sound!", filename,
			MB_ICONEXCLAMATION | MB_OK);
	}

	return channel;
}