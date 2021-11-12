#pragma once

#include "Core.h"
#include "fmod/fmod.hpp"
#include "unordered_map.h"


/// Audio API Reference
////// bool InitSound();
////// bool LoadSound(const char* path);
////// bool PlaySound(const char* path);
////// bool StopSound(const char* path);

FMOD::System* audio_system;

// these variables are currently not in use...playSound is being passed nullptr instead of audio channels for the time being
FMOD::Channel* audio_channels[4];   // audio system can play up to 4 sounds simultaneously
s32 audio_channels_in_use = 0;

en::unordered_map<std::string, FMOD::Sound*> scene_sounds;

bool InitSound()
{
  if (FMOD::System_Create(&audio_system) != FMOD_OK) return false;
  if (audio_system->init(64, FMOD_INIT_NORMAL, 0)) return false;

  return true;
}

FMOD::Sound* LoadSound(const char* path, bool looping)
{
  FMOD::Sound* sound;
  std::string sound_path = AssetPath("audio/").append(path);
  if (looping)
  {
    if (audio_system->createSound(sound_path.c_str(), FMOD_DEFAULT | FMOD_LOOP_NORMAL, 0, &sound) == FMOD_OK)
    {
      DebugPrintToConsole("Successfully loaded audio asset: ", sound_path);
      return sound;
    }
  }
  else
  {
    if (audio_system->createSound(sound_path.c_str(), FMOD_DEFAULT, 0, &sound) == FMOD_OK)
    {
      DebugPrintToConsole("Successfully loaded audio asset: ", sound_path);
      return sound;
    }
  }

  return nullptr;
}

bool PlaySound(const char* sound)
{
  std::string sound_path = AssetPath("audio/").append(sound);
  if (audio_system->playSound(scene_sounds.At(sound), nullptr, false, nullptr) == FMOD_OK)
  {
    return true;
  }

  DebugPrintToConsole("Failed to play sound!");
  return false;
}

bool StopSound(const char* sound)           // TODO : write this function!
{
  return false;
}
