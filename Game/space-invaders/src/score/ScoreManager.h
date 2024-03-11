#pragma once
#include "IScoreHandler.h"

class ScoreManager : public IScoreHandler
{
public:

    void AddScore(unsigned int total) override;
    void Restart();

    inline unsigned int GetTotalScore() const { return m_Score; }

private:

    unsigned int m_Score{0};
};
