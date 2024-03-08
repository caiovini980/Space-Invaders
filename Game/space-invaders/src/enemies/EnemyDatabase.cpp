#include "EnemyDatabase.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include "utils/Color.h"

std::shared_ptr<EnemyDefinition> EnemyDatabase::GetDefinition(unsigned int id)
{
    if(!IsValidId(id))
    {
        return nullptr;
    }
    
    if(m_Definitions.find(id) == m_Definitions.end())
    {
        LoadFromFile(id);
    }

    return m_Definitions[id];
}

bool EnemyDatabase::IsValidId(unsigned int id) const
{
    return id > 0;
}

void EnemyDatabase::LoadFromFile(unsigned int id)
{
    std::shared_ptr<EnemyDefinition> definition = nullptr;
    
    try
    {
        std::ostringstream path;
        path << "res/data/enemies/" << id << ".enemy";
        
        std::ifstream file(path.str());
        std::string line;
        std::vector<std::string> rawProperties{};
        
        while (std::getline(file, line))
        {
            size_t colonPosition = line.find(':');
            std::string rawProperty = line.substr(colonPosition + 1);
            rawProperty.erase(0, rawProperty.find_first_not_of(' '));

            rawProperties.emplace_back(std::move(rawProperty));
        }

        definition = std::make_shared<EnemyDefinition>();
        definition->Name = rawProperties[0];
        definition->SpritePath = rawProperties[1];
        definition->Color = Color::HexToRGB(rawProperties[2]);
        definition->Points = stoul(rawProperties[3]);
    }
    catch (const std::exception& e)
    {
        std::cout << "EnemyDatabase: could not load enemy with id " << id;
    }

    m_Definitions[id] = definition;
}
