#include "engine/audio/AudioBuffer.h"

#define DR_WAV_IMPLEMENTATION
#include "drLibs/dr_wav.h"
#define DR_FLAC_IMPLEMENTATION
#include "drLibs/dr_flac.h"

#include "cppfs/fs.h"
#include "cppfs/FileHandle.h"
#include "cppfs/FilePath.h"

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
    Unload();

    cppfs::FilePath path(filePath);
    bool isWavExt  = (path.extension() == ".wav");
    bool isFlacExt = (path.extension() == ".flac");

    if (!isWavExt && !isFlacExt)
    {
        std::cerr << "Unsupported audio file format: " << path.fileName() << std::endl;
        return false;
    }

    std::optional<std::pair<AudioInfo, AudioData>> rawData;  
    if (isWavExt)
    {
        rawData = LoadWavFile(filePath);
    }
    else if (isFlacExt)
    {
        rawData = LoadFlacFile(filePath);
    }

    if (!rawData.has_value())
    {
        std::cerr << "Failed to load audio file: " << path.fileName() << std::endl;
        return false;
    }

    //std::clog << "Sound Data size: " << rawData.value().second.size() << std::endl;

    //Copy the audio data
    audioData = std::move(rawData.value().second);

    //Set the audio buffer properties
    format = AL_FORMAT_STEREO16;
    sampleRate = rawData.value().first.sampleRate;
    numSamples = static_cast<ALsizei>(audioData.size()) * sizeof(int16_t);
    numChannels = rawData.value().first.numChannels;

    //Fill audio buffer
    alBufferData(bufferId, format, audioData.data(), numSamples, sampleRate);

    return true;
}

auto AudioBuffer::LoadWavFile(const std::string &filePath) -> std::optional<std::pair<AudioInfo, AudioData>>
{
    drwav wav;
    if (!drwav_init_file(&wav, filePath.c_str(), nullptr))
    {
        std::cerr << "Failed to open WAV file: " << filePath << std::endl;
        drwav_uninit(&wav);
        return std::nullopt;
    }

    AudioInfo audioInfo{};
    audioInfo.sampleRate = wav.sampleRate;
    audioInfo.numChannels = wav.channels;
    audioInfo.bitsPerSample = wav.bitsPerSample;
    audioInfo.dataSize = static_cast<uint32_t>(wav.totalPCMFrameCount * wav.channels * sizeof(int16_t));
    audioInfo.durationInSeconds = static_cast<float>(wav.totalPCMFrameCount) / static_cast<float>(wav.sampleRate);

    //Read and decoding audio data
    AudioData audioData(audioInfo.dataSize / sizeof(int16_t));
    drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, audioData.data());
    drwav_uninit(&wav);

    return std::make_pair(audioInfo, audioData);
}

auto AudioBuffer::LoadFlacFile(const std::string &filePath) -> std::optional<std::pair<AudioInfo, AudioData>>
{
    drflac* pFlac = drflac_open_file(filePath.c_str(), nullptr);
    if (!pFlac)
    {
        std::cerr << "Failed to open FLAC file: " << filePath << std::endl;
        drflac_close(pFlac);
        return std::nullopt;
    }

    if (pFlac->channels == 0 || pFlac->bitsPerSample == 0 || pFlac->sampleRate == 0)
    {
        std::cerr << "Invalid FLAC file format: " << filePath << std::endl;
        drflac_close(pFlac);
        return std::nullopt;
    }

    AudioInfo audioInfo{};
    audioInfo.sampleRate = pFlac->sampleRate;
    audioInfo.numChannels = pFlac->channels;
    audioInfo.bitsPerSample = pFlac->bitsPerSample;
    audioInfo.dataSize = static_cast<uint32_t>(pFlac->totalPCMFrameCount * pFlac->channels * sizeof(int16_t));
    audioInfo.durationInSeconds = static_cast<float>(pFlac->totalPCMFrameCount) / static_cast<float>(pFlac->sampleRate);
    
    AudioData audioData(audioInfo.dataSize / sizeof(int16_t));
    drflac_read_pcm_frames_s16(pFlac, pFlac->totalPCMFrameCount, audioData.data());
    drflac_close(pFlac);

    return std::make_pair(audioInfo, audioData);
}

void AudioBuffer::Unload()
{
	audioData.clear();
}

ALid AudioBuffer::GetBufferId() const
{
	return bufferId;
}

ALuint AudioBuffer::GetFormat() const
{
	return format;
}

ALuint AudioBuffer::GetSampleRate() const
{
	return sampleRate;
}

ALuint AudioBuffer::GetNumSamples() const
{
	return numSamples;
}

ALuint AudioBuffer::GetNumChannels() const
{
	return numChannels;
}