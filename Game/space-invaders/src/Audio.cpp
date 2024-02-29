#include "Audio.h"

irrklang::ISoundEngine* Audio::m_SoundEngine = nullptr;

void Audio::Init()
{
    m_SoundEngine = irrklang::createIrrKlangDevice();
}

void Audio::Play2DSound(const std::string& soundFilePath, bool bLoop)
{
    m_SoundEngine->play2D(soundFilePath.c_str(), bLoop);
}
