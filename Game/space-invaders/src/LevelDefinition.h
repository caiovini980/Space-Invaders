#pragma once
#include <vector>

struct LevelDefinition
{
    std::vector<std::vector<unsigned int>> EnemyIds{};
    unsigned int TotalEnemyRows;
    unsigned int TotalEnemyColumns;
    float Padding;
    float HorizontalMargin;
    float TopMargin;
    float MinBottomMargin;
    float GameOverBottomThreshold;
    unsigned int TotalBarriers;
    float BarrierPadding;
    float BarrierHorizontalMargin;
    float BarrierBottomMargin;
    float MaxBarrierWidth;
};
