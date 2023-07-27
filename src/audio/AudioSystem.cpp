#include "engine/audio/AudioSystem.h"
#include <algorithm>

void AudioSystem::Initialize()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        std::cerr << "Failed to open audio device" << std::endl;
        return;
    }

    context = alcCreateContext(device, nullptr);
    if (!context)
    {
        std::cerr << "Failed to create audio context" << std::endl;
        alcCloseDevice(device);
        return;
    }

    if (!alcMakeContextCurrent(context))
    {
        std::cerr << "Failed to make audio context current" << std::endl;
        alcDestroyContext(context);
        alcCloseDevice(device);
        return;
    }

    // Setup listener properties
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);

    ALfloat listenerOrientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
    alListenerfv(AL_ORIENTATION, listenerOrientation);

    // Clear any error that occurred during setup
    alGetError();
}

void AudioSystem::Shutdown()
{
    audioBuffers.clear();

    for (auto &sourcePair : soundSources)
    {
        alDeleteSources(1, &sourcePair.second);
    }
    soundSources.clear();

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

ALuint AudioSystem::LoadSound(const std::string &filePath)
{
    std::unique_ptr<AudioBuffer> audioBuffer = std::make_unique<AudioBuffer>();
    std::clog << "\nAudio buffer id: " << audioBuffer->GetBufferId() << std::endl;

    if (audioBuffer->LoadFromFile(filePath))
    {
        ALuint sourceId = GenerateNewSource();
        alSourcei(sourceId, AL_BUFFER, audioBuffer->GetBufferId());
        soundSources[audioBuffer->GetBufferId()] = sourceId;
        audioBuffers[audioBuffer->GetBufferId()] = std::move(audioBuffer);
        return sourceId;
    }

    // Если загрузка не удалась, вернем 0, умный указатель автоматически удалит объект AudioBuffer
    return 0;
}
void AudioSystem::UnloadSound(ALuint soundId)
{
    auto it = audioBuffers.find(soundId);
    if (it != audioBuffers.end())
    {
        it->second->Unload();
        alDeleteSources(1, &soundId);
        soundSources.erase(soundId); // Удаляем источник из soundSources
        audioBuffers.erase(it);
    }
}

void AudioSystem::PlaySound(ALuint soundId, float volume, bool loop)
{
    auto it = soundSources.find(soundId);
    std::cout << "Iter: " << it->first << " " << it->second;
    if (it != soundSources.end())
    {
        ALuint sourceId = it->second;

        alSourcef(sourceId, AL_GAIN, volume);
        alSourcei(sourceId, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcePlay(sourceId);
    }
}

void AudioSystem::PauseSound(ALuint soundId)
{
    auto it = soundSources.find(soundId);
    if (it != soundSources.end())
    {
        ALuint sourceId = it->second;
        if (alIsSource(sourceId) == AL_TRUE)
        {
            alSourcePause(sourceId);
        }
    }
}

void AudioSystem::UnPauseSound(ALuint soundId)
{
    auto it = soundSources.find(soundId);
    if (it != soundSources.end())
    {
        ALuint sourceId = it->second;
        if (alIsSource(sourceId) == AL_TRUE)
        {
            alSourcePlay(sourceId);
        }
    }
}

void AudioSystem::StopSound(ALuint soundId)
{
    auto it = soundSources.find(soundId);
    if (it != soundSources.end())
    {
        ALuint sourceId = it->second;
        if (alIsSource(sourceId) == AL_TRUE)
        {
            alSourceStop(sourceId);
        }
    }
}

ALuint AudioSystem::GenerateNewSource()
{
    ALuint sourceId;
    alGenSources(1, &sourceId);
    return sourceId;
}

//void AudioSystem::Initialize()
//{
//    device = alcOpenDevice(nullptr);
//    if (!device)
//    {
//        std::cerr << "Failed to open audio device" << std::endl;
//        return;
//    }
//
//    context = alcCreateContext(device, nullptr);
//    if (!context)
//    {
//        std::cerr << "Failed to create audio context" << std::endl;
//        alcCloseDevice(device);
//        return;
//    }
//
//    if (!alcMakeContextCurrent(context))
//    {
//        std::cerr << "Failed to make audio context current" << std::endl;
//        alcDestroyContext(context);
//        alcCloseDevice(device);
//        return;
//    }
//
//    // Setup listener properties
//    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
//    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
//
//    ALfloat listenerOrientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
//    alListenerfv(AL_ORIENTATION, listenerOrientation);
//
//    // Clear any error that occurred during setup
//    alGetError();
//}
//
//void AudioSystem::Shutdown()
//{
//    alcMakeContextCurrent(nullptr);
//    alcDestroyContext(context);
//    alcCloseDevice(device);
//}
//
//ALuint AudioSystem::LoadSound(const std::string &filePath)
//{
//    AudioBuffer audioBuffer;
//    std::clog << "\n\nAudio buffer id: " << audioBuffer.GetBufferId() << std::endl;
//
//    if (audioBuffer.LoadFromFile(filePath))
//    {
//        if (id == 0)
//        {
//            alGenSources(1, &id);
//        }
//
//        alSourcei(id, AL_BUFFER, audioBuffer.GetBufferId());
//
//        return id;
//    }
//    return 0;
//}
//
//void AudioSystem::UnloadSound(ALuint soundId)
//{
//    for (auto it = audioBuffers.begin(); it != audioBuffers.end(); ++it)
//    {
//        if (it->GetBufferId() == soundId)
//        {
//            it->Unload();
//            audioBuffers.erase(it);
//            break;
//        }
//    }
//}
//
//void AudioSystem::PlaySound(ALuint soundId, float volume, bool loop)
//{
//    if (soundId == 0)
//    {
//        std::cerr << "Failed to play sound. No source available." << std::endl;
//        return;
//    }
//
//    alSourcef(soundId, AL_GAIN, volume);
//    alSourcei(soundId, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
//
//    alSourcePlay(soundId);
//}
//
//void AudioSystem::PauseSound(ALuint soundId)
//{
//    if (alIsSource(soundId) == AL_FALSE)
//    {
//        std::cerr << "Failed to pause sound. Invalid source ID." << std::endl;
//        return;
//    }
//
//    alSourcePause(soundId);
//}
//
//void AudioSystem::UnPauseSound(ALuint soundId)
//{
//    if (alIsSource(soundId) == AL_FALSE)
//    {
//        std::cerr << "Failed to unpause sound. Invalid source ID." << std::endl;
//        return;
//    }
//
//    alSourcePlay(soundId);
//}
//
//void AudioSystem::StopSound(ALuint soundId)
//{
//    auto it = soundSources.find(soundId);
//    if (it == soundSources.end())
//    {
//        std::cerr << "Failed to stop sound. Sound with ID " << soundId << " not found." << std::endl;
//        return;
//    }
//
//    ALuint sourceId = it->second;
//    alSourceStop(sourceId);
//}
