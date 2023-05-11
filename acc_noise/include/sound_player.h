#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include <AL/al.h>
#include <AL/alc.h>

#include <queue>


struct Sound{
public:
    ALuint buffer;
    ALfloat *position;
    Sound(ALfloat *data, ALsizei size, ALsizei samplerate, ALfloat *pos);
};


class SoundPlayer {
public:
    ALfloat *listener_pos;
    ALfloat *listener_ort;

    SoundPlayer(ALfloat *lis_pos, ALfloat *lis_ort);
    ~SoundPlayer();
    void play();
    void add_sound(ALfloat *data, ALsizei size, ALsizei samplerate, ALfloat *pos);
    void set_listener(ALfloat *pos, ALfloat *ort);
private:
    ALCdevice *device;
    ALCcontext *context;
    ALuint source;
    std::queue<Sound> sounds;
    std::vector<Sound> removed;
};

#endif