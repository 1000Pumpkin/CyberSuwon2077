#include "pch.h"

Sound::Sound(string name, int szSound)
{
    static int index = 0;
    soundIndex = ++index;
    for (int i = 0; i < szSound; ++i)
    {
        char buffer[255];
        sprintf(buffer, "open %s alias %d_%d", name.c_str(), soundIndex, i);
        mciSendString(buffer, 0, 0, 0);
    }
    this->szSound = szSound;
}

Sound::~Sound()
{
    for (int i = 0; i < szSound; ++i)
    {
        char buffer[255];
        sprintf(buffer, "close %d_%d", soundIndex, i);
        mciSendString(buffer, 0, 0, 0);
    }
}

void Sound::Play(bool loop)
{
    char buffer[255];
    sprintf(buffer, "play %d_%d from 0%s", soundIndex, curSound, loop ? " repeat" : "");
    mciSendString(buffer, 0, 0, 0);
    if (++curSound >= szSound)
        curSound = 0;
}

void Sound::Stop()
{
    for (int i = 0; i < szSound; ++i)
    {
        char buffer[255];
        sprintf(buffer, "stop %d_%d", soundIndex, i);
        mciSendString(buffer, 0, 0, 0);
    }
}

void Sound::SetVolume(int volume)
{
    for (int i = 0; i < szSound; ++i)
    {
        char buffer[255];
        sprintf(buffer, "setaudio %d_%d volume to %d", soundIndex, i, volume);
        mciSendString(buffer, 0, 0, 0);
    }
}
