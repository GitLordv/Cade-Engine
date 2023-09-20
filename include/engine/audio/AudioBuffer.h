#pragma once

#include "AL/al.h"
#include "AL/alc.h"
#include "AL/alext.h"

#include <string>
#include <vector>
#include <optional>
#include <iostream>

using AudioData = std::vector<int16_t>;
using ALid = ALuint;

struct AudioInfo
{
	uint32_t sampleRate;
	uint16_t numChannels;
	uint16_t bitsPerSample;
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

	ALid GetBufferId() const;
	ALuint GetFormat() const;
	ALuint GetSampleRate() const;
	ALuint GetNumSamples() const;
	ALuint GetNumChannels() const;

private:

	AudioData audioData;
	ALuint bufferId;
	ALuint format;
	ALuint sampleRate;
	ALuint numSamples;
	ALuint numChannels;
	
	static auto LoadWavFile(const std::string &filePath) -> std::optional<std::pair<AudioInfo, AudioData>>;
	static auto LoadFlacFile(const std::string &filePath) -> std::optional<std::pair<AudioInfo, AudioData>>;
};

