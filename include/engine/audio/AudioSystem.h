#pragma once

#include "engine/audio/AudioBuffer.h"

#include "glm/vec3.hpp"

#include <unordered_map>


class AudioSystem
{
public:

    static void Initialize();
    static void Shutdown();

    static ALuint LoadSound(const std::string &filePath);
    static void UnloadSound(ALuint soundId);
    
    static void PlaySound2D(ALuint soundId, ALfloat volume, ALboolean loop);
    static void PlaySound3D(ALuint soundId, glm::vec3 position, ALfloat volume, ALboolean loop);
    static void SetSoundStartTime(ALuint soundId, ALfloat offsetSec);
    static void ReplaySound(ALuint soundId);
    static void PauseSound(ALuint soundId);
    static void ContinueSound(ALuint soundId);
    static void StopSound(ALuint soundId);

    static void GetListenerPos(glm::vec3 &position);
    static void SetListenerPos(const glm::vec3 &position);

private:
    
    static inline ALCdevice *device;
    static inline ALCcontext *context;
    static inline std::unordered_map<ALuint, std::unique_ptr<AudioBuffer>> audioBuffers;
    static inline std::unordered_map<ALuint, ALuint> soundSources;

    static ALuint GenerateNewSource();
};
