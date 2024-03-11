#include "ScoreManager.h"

void ScoreManager::AddScore(unsigned int total)
{
    m_Score += total;
}

void ScoreManager::Restart()
{
    m_Score = 0;
}
