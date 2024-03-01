#pragma once
#include <string>

#include <irrKlang.h>

class Audio
{
public:

    static void Init();
    static void Play2DSound(const std::string& soundFilePath, bool bLoop = false, float volume = 1.f);

private:

    static irrklang::ISoundEngine* m_SoundEngine;
};
