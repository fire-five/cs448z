#include "sound_player.h"

#include <sndfile.h>
#include <cstring>


Sound::Sound(const char *filename, ALfloat *p, ALfloat g) {
    position = p;
    gain = g;
    SF_INFO file_info;
    SNDFILE *file = sf_open(filename, SFM_READ, &file_info);
    std::vector<ALshort> data(file_info.frames * file_info.channels); 
    sf_read_short(file, data.data(), data.size());
    sf_close(file);
    alGenBuffers(1, &buffer);
    alBufferData(buffer, file_info.channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16,
                 data.data(), data.size() * sizeof(ALshort), file_info.samplerate);
}


SoundPlayer::SoundPlayer(ALfloat *lis_pos, ALfloat *lis_ort) {
    // initialize OpenAL
    device = alcOpenDevice(nullptr);
    context = alcCreateContext(device, nullptr);
    alcMakeContextCurrent(context);
    alGenSources(1, &source);
    // set default positions and orientation
    alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    set_listener(lis_pos, lis_ort);
}


SoundPlayer::~SoundPlayer() {
    // stop all playing sounds before deleting SoundPlayer
    alSourceStop(source);
    while (!sounds.empty()) {
        Sound sound = sounds.front();
        alSourcei(source, AL_BUFFER, 0);
        alDeleteBuffers(1, &sound.buffer);
        sounds.pop();
    }
    for (auto& sound : removed) {
        alSourcei(source, AL_BUFFER, 0);
        alDeleteBuffers(1, &sound.buffer);
    }
    // clean up OpenAL resources
    alDeleteSources(1, &source);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context);
    alcCloseDevice(device); 
}


void SoundPlayer::add_sound(const char *filename, ALfloat *pos, ALfloat gain) {
    Sound sound(filename, pos, 0.1 * gain);
    sounds.push(sound);
}


void SoundPlayer::play() {
    while (!sounds.empty()) {
        Sound sound = sounds.front();
        alSourcefv(source, AL_POSITION, sound.position);
        alSourceQueueBuffers(source, 1, &sound.buffer);
        alSourcef(source, AL_GAIN, sound.gain);
        alSourcePlay(source);
        sounds.pop();
        removed.push_back(sound);
    }
}


void SoundPlayer::set_listener(ALfloat *pos, ALfloat *ort) {
    listener_pos = pos;
    listener_ort = ort;
    alListenerfv(AL_POSITION, listener_pos);
    alListenerfv(AL_ORIENTATION, listener_ort);
}

