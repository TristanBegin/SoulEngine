/******************************************************************************
Author: Caleb Klomparens

Name: KKSOUND.c

Created: 01/21/16

Description: This file wraps KMOD (< FMOD) into a usable component in the
SoulEngine file. It serves as the master class for major functions
that modify the sounds at runtime.

All content © 2016 DigiPen (USA) Corporation, all rights reserved.
******************************************************************************/

#include "fmod.h"
#include "SoulEngine.h"

void KSOUND_Init(KSOUND **s);

void KSOUND_Play(KSOUND *s);

void KSOUND_SetPan(KSOUND *s, float pan);

void KSOUND_SetVolume(KSOUND *s, float vol);

void KSOUND_SetLooping(KSOUND *s, BOOL loop);

void KSOUND_Free(KSOUND *s);