#include "Audio.h"

irrklang::ISoundEngine* Audio::m_SoundEngine = nullptr;

void Audio::Init()
{
    m_SoundEngine = irrklang::createIrrKlangDevice();
}

void Audio::Play2DSound(const std::string& soundFilePath, bool bLoop, float volume)
{
    if(volume < 1.f)
    {
        irrklang::ISound* sound = m_SoundEngine->play2D(soundFilePath.c_str(), bLoop, true);
        sound->setVolume(volume);
        sound->setIsPaused(false);
        sound->drop();
    }
    else
    {
        m_SoundEngine->play2D(soundFilePath.c_str(), bLoop);
    }
}
