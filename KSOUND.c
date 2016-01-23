/******************************************************************************
Author: Caleb Klomparens

Name: KSOUND.c

Created: 01/21/16

Description: This file wraps KMOD (< FMOD) into a usable component in the
SoulEngine file. It serves as the master class for major functions
that modify the sounds at runtime.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "KSOUND.h"

void KSOUND_Init(KSOUND **Init)
{
	*Init = malloc(sizeof(KSOUND));

	(*Init)->Volume = 1; // Acceptable inputs: 0 - 1.0f
	(*Init)->Positional = TRUE; // TODO : add UpdateAttenuation() function!
	(*Init)->MaxReach = 50; // Radius (pixels) of farthest distance at which sound is hearable.
	(*Init)->SoundFile = ""; // Name of sound file (in case we need to reallocate it.)
	(*Init)->PlayOnStart = FALSE;
	(*Init)->Loop = FALSE;
	(*Init)->StreamIt = FALSE;
}

void KSOUND_Play(KSOUND * s)
{

}

void KSOUND_SetPan(KSOUND * s, float pan)
{
}

void KSOUND_SetVolume(KSOUND * s, float vol)
{
}

void KSOUND_SetLooping(KSOUND * s, BOOL loop)
{
}
