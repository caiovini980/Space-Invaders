#pragma once

class IScoreHandler
{
public:

    virtual void AddScore(unsigned int total) = 0;
    virtual ~IScoreHandler() = default;

protected:

    IScoreHandler() = default;
};
