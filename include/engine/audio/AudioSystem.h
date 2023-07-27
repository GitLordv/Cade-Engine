#pragma once

#include "engine/audio/AudioBuffer.h"
#include <unordered_map>

class AudioSystem
{
public:

    static void Initialize();
    static void Shutdown();

    static ALuint LoadSound(const std::string &filePath);
    static void UnloadSound(ALuint soundId);

    static void PlaySound(ALuint soundId, float volume, bool loop);
    static void PauseSound(ALuint soundId);
    static void UnPauseSound(ALuint soundId);
    static void StopSound(ALuint soundId);

private:
    
    static inline ALCdevice *device;
    static inline ALCcontext *context;
    static inline std::unordered_map<ALuint, std::unique_ptr<AudioBuffer>> audioBuffers;
    static inline std::unordered_map<ALuint, ALuint> soundSources;

    static ALuint GenerateNewSource();
};
