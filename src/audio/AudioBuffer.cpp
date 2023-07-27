#include "engine/audio/AudioBuffer.h"
#include <fstream>

AudioBuffer::AudioBuffer() : bufferId(0), format(0), sampleRate(0), numSamples(0), numChannels(0)
{
	alGenBuffers(1, &bufferId);
}

AudioBuffer::~AudioBuffer()
{
	if (bufferId != 0)
	{
		alDeleteBuffers(1, &bufferId);
		bufferId = 0;
	}
}

bool AudioBuffer::LoadFromFile(const std::string &filePath)
{
    //Unload any previously loaded audio data
    Unload();

    //Load WAV file
    auto wavData = LoadWavFile(filePath);
   
    if (!wavData.has_value())
    {
        std::cerr << "Failed to load wav file: " << filePath << std::endl;
        return false;
    }

    std::clog << "Sound Data size: " << wavData.value().second.size() << std::endl;

    //Copy the audio data
    audioData = std::move(wavData.value().second);

    //Set the audio buffer properties
    format = AL_FORMAT_STEREO16;
    sampleRate = wavData.value().first.sampleRate;
    numSamples = static_cast<ALsizei>(audioData.size()) * sizeof(int16_t);
    numChannels = wavData.value().first.numChannels;

    //Fill audio buffer
    alBufferData(bufferId, format, audioData.data(), numSamples, sampleRate);

    return true;
}

std::optional<std::pair<AudioInfo, AudioData>> AudioBuffer::LoadWavFile(const std::string &filePath)
{
    AudioInfo audioInfo{};

    std::ifstream file(filePath, std::ios::binary);
    if (!file) return std::nullopt;
    
    //Read WAV header
    char header[44];
    file.read(header, sizeof(header));

    //Check WAV header
    if (file.gcount() != sizeof(header) ||
        strncmp(header, "RIFF", 4) != 0 ||
        strncmp(header + 8,  "WAVE", 4) != 0 ||
        strncmp(header + 12, "fmt ", 4) != 0 ||
        strncmp(header + 36, "data", 4) != 0)
    {
        //Invalid WAV file
        std::cerr << "\nInvalid wav file\n";
        return std::nullopt;
    }

    //Extract audio format information
    audioInfo.numChannels   = *reinterpret_cast<uint16_t *>(header + 22);
    audioInfo.sampleRate    = *reinterpret_cast<uint32_t *>(header + 24);
    audioInfo.bitsPerSample = *reinterpret_cast<uint16_t *>(header + 34);
    audioInfo.dataSize      = *reinterpret_cast<uint32_t *>(header + 40);

    //Calculate duration in seconds
    audioInfo.durationInSeconds = static_cast<float>(audioInfo.dataSize) / (audioInfo.sampleRate * audioInfo.numChannels * (audioInfo.bitsPerSample / 8));

    //Read audio data
    AudioData audioData(audioInfo.dataSize / sizeof(int16_t));
    file.read(reinterpret_cast<char *>(audioData.data()), audioInfo.dataSize);
 
    return std::make_pair(audioInfo, audioData);
}

void AudioBuffer::Unload()
{
	audioData.clear();
}

ALuint AudioBuffer::GetBufferId() const
{
	return bufferId;
}

int AudioBuffer::GetFormat() const
{
	return format;
}

int AudioBuffer::GetSampleRate() const
{
	return sampleRate;
}

int AudioBuffer::GetNumSamples() const
{
	return numSamples;
}

int AudioBuffer::GetNumChannels() const
{
	return numChannels;
}