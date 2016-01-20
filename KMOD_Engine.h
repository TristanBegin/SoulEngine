/******************************************************************************
Author: Caleb Klomparens

Name: KMODEngine.h

Created: 01/11/16

Description: Header file for KMODEngine.c

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#pragma once
#include "fmod.h"

void KMOD_Initialize(int numChannels);

void KMOD_Terminate(void);

FMOD_CHANNEL *KMOD_PlaySound(char *filename, float vol);

FMOD_CHANNEL *KMOD_PlayStream(char *filename, int loop);