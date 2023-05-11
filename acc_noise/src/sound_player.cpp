#include "sound_player.h"

#include <sndfile.h>
#include <cstring>


Sound::Sound(ALfloat *data, ALsizei size, ALsizei samplerate, ALfloat *pos) {
    alGenBuffers(1, &buffer);
    alBufferData(buffer, AL_FORMAT_MONO16,
                 data, size*sizeof(ALfloat), samplerate);
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


void SoundPlayer::add_sound(ALfloat *data, ALsizei size, ALsizei samplerate, ALfloat *pos) {
    Sound sound(data, size, samplerate, pos);
    sounds.push(sound);
}


void SoundPlayer::play() {
    while (!sounds.empty()) {
        Sound sound = sounds.front();
        alSourcefv(source, AL_POSITION, sound.position);
        alSourceQueueBuffers(source, 1, &sound.buffer);
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

