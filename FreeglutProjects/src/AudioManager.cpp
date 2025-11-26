#include "AudioManager.h"

AudioManager::AudioManager() : device(nullptr), context(nullptr) {}

AudioManager::~AudioManager() {
    cleanup();
}

bool AudioManager::initialize() {
    // Open the default device
    device = alcOpenDevice(nullptr);
    if (!device) {
        std::cerr << "Failed to open audio device" << std::endl;
        return false;
    }

    // Create context
    context = alcCreateContext(device, nullptr);
    if (!context) {
        std::cerr << "Failed to create audio context" << std::endl;
        alcCloseDevice(device);
        return false;
    }

    // Make context current
    alcMakeContextCurrent(context);

    // Set listener properties (for 3D audio)
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    ALfloat orientation[] = {0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f};
    alListenerfv(AL_ORIENTATION, orientation);

    return true;
}

void AudioManager::cleanup() {
    stopAllSounds();
    
    // Delete sources
    if (!sources.empty()) {
        alDeleteSources(sources.size(), &sources[0]);
        sources.clear();
    }
    
    // Delete buffers
    if (!buffers.empty()) {
        alDeleteBuffers(buffers.size(), &buffers[0]);
        buffers.clear();
    }
    
    // Cleanup context and device
    if (context) {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
        context = nullptr;
    }
    
    if (device) {
        alcCloseDevice(device);
        device = nullptr;
    }
}

int AudioManager::loadSound(const char* filename) {
    SF_INFO sfInfo;
    SNDFILE* file = sf_open(filename, SFM_READ, &sfInfo);
    
    if (!file) {
        std::cerr << "Failed to open audio file: " << filename << std::endl;
        return -1;
    }

    // Read audio data
    std::vector<short> samples(sfInfo.frames * sfInfo.channels);
    sf_read_short(file, &samples[0], samples.size());
    sf_close(file);

    // Determine format
    ALenum format;
    if (sfInfo.channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (sfInfo.channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        std::cerr << "Unsupported channel count: " << sfInfo.channels << std::endl;
        return -1;
    }

    // Generate buffer
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, &samples[0], samples.size() * sizeof(short), sfInfo.samplerate);

    // Generate source
    ALuint source;
    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    buffers.push_back(buffer);
    sources.push_back(source);

    return sources.size() - 1; // Return index
}

void AudioManager::playSound(int soundIndex, bool loop) {
    if (soundIndex >= 0 && soundIndex < sources.size()) {
        alSourcei(sources[soundIndex], AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
        alSourcePlay(sources[soundIndex]);
    }
}

void AudioManager::stopSound(int soundIndex) {
    if (soundIndex >= 0 && soundIndex < sources.size()) {
        alSourceStop(sources[soundIndex]);
    }
}

void AudioManager::stopAllSounds() {
    for (ALuint source : sources) {
        alSourceStop(source);
    }
}

void AudioManager::setVolume(int soundIndex, float volume) {
    if (soundIndex >= 0 && soundIndex < sources.size()) {
        alSourcef(sources[soundIndex], AL_GAIN, volume);
    }
}