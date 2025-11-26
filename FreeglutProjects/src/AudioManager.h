#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>
#include <iostream>
#include <vector>

class AudioManager {
private:
    ALCdevice* device;
    ALCcontext* context;
    std::vector<ALuint> buffers;
    std::vector<ALuint> sources;

public:
    AudioManager();
    ~AudioManager();
    
    bool initialize();
    void cleanup();
    int loadSound(const char* filename);
    void playSound(int soundIndex, bool loop = false);
    void stopSound(int soundIndex);
    void stopAllSounds();
    void setVolume(int soundIndex, float volume);
};

#endif // AUDIOMANAGER_H