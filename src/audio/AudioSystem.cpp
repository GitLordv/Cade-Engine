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

	ALfloat listenerPos[] = {0.0,0.0,0.0};
	ALfloat listenerVel[] = {0.0,0.0,0.0};
	ALfloat listenerOri[] = {0.0,0.0,-1.0,0.0,1.0,0.0};

	ALuint error = 0;

	alListenerfv(AL_POSITION, listenerPos);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "ALerror: Listener position: " << error << std::endl;
		return;
	}

	alListenerfv(AL_VELOCITY, listenerVel);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "ALerror: Listener velocity: " << error << std::endl;
		return;
	}

	alListenerfv(AL_ORIENTATION, listenerOri);
	if ((error = alGetError()) != AL_NO_ERROR)
	{
		std::cout << "ALerror: Listener orientation: " << error << std::endl;
		return;
	}

	alGetError();
}

void AudioSystem::GetListenerPos(glm::vec3 &position)
{
	ALfloat posX, posY, posZ;
	alGetListener3f(AL_POSITION, &posX, &posY, &posZ);
	position = glm::vec3(posX, posY, posZ);
}

void AudioSystem::SetListenerPos(const glm::vec3 &position)
{
	alListener3f(AL_POSITION, position.x, position.y, position.z);
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

	if (audioBuffer->LoadFromFile(filePath))
	{
		ALuint sourceId = GenerateNewSource();
		alSourcei(sourceId, AL_BUFFER, audioBuffer->GetBufferId());
		soundSources[audioBuffer->GetBufferId()] = sourceId;
		audioBuffers[audioBuffer->GetBufferId()] = std::move(audioBuffer);
		return sourceId;
	}

	return 0;
}
void AudioSystem::UnloadSound(ALuint soundId)
{
	auto it = audioBuffers.find(soundId);
	if (it != audioBuffers.end())
	{
		it->second->Unload();
		alDeleteSources(1, &soundId);
		soundSources.erase(soundId);
		audioBuffers.erase(it);
	}
}

void AudioSystem::PlaySound2D(ALuint soundId, ALfloat volume, ALboolean loop)
{
	auto it = soundSources.find(soundId);
	if (it != soundSources.end())
	{
		ALuint sourceId = it->second;

		alSourcef(sourceId, AL_GAIN, volume);
		alSourcei(sourceId, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
		alSourcePlay(sourceId);
	}
}

void AudioSystem::PlaySound3D(ALuint soundId, glm::vec3 position, ALfloat volume, ALboolean loop)
{
	auto it = soundSources.find(soundId);
	if (it != soundSources.end())
	{
		ALuint sourceId = it->second;

		float referenceDistance = 2.0F;
		float rolloffFactor = 1.0F;
		float maxDistance = 10.0F;

		alSourcef(sourceId, AL_GAIN, volume);
		alSourcei(sourceId, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
		alSourcef(sourceId, AL_REFERENCE_DISTANCE, referenceDistance);
		alSourcef(sourceId, AL_ROLLOFF_FACTOR, rolloffFactor);
		alSourcef(sourceId, AL_MAX_DISTANCE, maxDistance);
		alSourcei(sourceId, AL_SOURCE_RELATIVE, AL_FALSE);
		alSource3f(sourceId, AL_POSITION, position.x, position.y, position.z);

		alSourcePlay(sourceId);
	}
}

void AudioSystem::SetSoundStartTime(ALuint soundId, ALfloat offsetSec)
{
	auto it = soundSources.find(soundId);
	if (it != soundSources.end())
	{
		ALuint sourceId = it->second;

		alSourcef(sourceId, AL_SEC_OFFSET, offsetSec);
	}
}


void AudioSystem::ReplaySound(ALuint soundId)
{
	auto it = soundSources.find(soundId);
	if (it != soundSources.end())
	{
		ALuint sourceId = it->second;

		alSourceRewind(sourceId);
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

void AudioSystem::ContinueSound(ALuint soundId)
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
