#include "Base.h"
#include "Audio.h"

namespace rainbow
{

Audio* Audio::_audio = nullptr;

Audio* Audio::getAudio()
{
    if (!_audio)
    {
        _audio = new Audio();
    }
    return _audio;
}

void Audio::onInitialize()
{
}

void Audio::onPause()
{
}

void Audio::onResume()
{
}

void Audio::onUpdate(float elapsedTime)
{
}

std::shared_ptr<AudioListener> Audio::getAudioListener() const
{
    return nullptr;
}

}
