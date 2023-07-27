#pragma once

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include <iostream>
#include <vector>
#include <optional>


using AudioData = std::vector<int16_t>;

struct AudioInfo
{
	int numChannels;
	int sampleRate;
	int bitsPerSample;
	uint32_t dataSize;
	float durationInSeconds;
};


class AudioBuffer
{
public:

	AudioBuffer();
	~AudioBuffer();

	bool LoadFromFile(const std::string &filePath);
	void Unload();

	ALuint GetBufferId() const;
	int GetFormat() const;
	int GetSampleRate() const;
	int GetNumSamples() const;
	int GetNumChannels() const;

private:

	AudioData audioData;
	ALuint bufferId;
	int format;
	int sampleRate;
	int numSamples;
	int numChannels;
	
	static std::optional<std::pair<AudioInfo, AudioData>> LoadWavFile(const std::string &filePath);
};

