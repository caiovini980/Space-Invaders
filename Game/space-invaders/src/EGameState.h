#pragma once
#include <cstdint>

enum class EGameState : uint8_t
{
    MainMenu,
    Playing,
    GameOver,
    GameWin
};
