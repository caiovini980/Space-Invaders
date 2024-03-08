#pragma once
#include <map>
#include <memory>

#include "EnemyDefinition.h"

class EnemyDatabase
{
public:

    std::shared_ptr<EnemyDefinition> GetDefinition(unsigned int id);

    bool IsValidId(unsigned int id) const;

private:

    std::map<unsigned int, std::shared_ptr<EnemyDefinition>> m_Definitions;

    void LoadFromFile(unsigned int id);
};
