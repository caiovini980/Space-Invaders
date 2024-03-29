﻿#pragma once
#include "../GameObject.h"

class IProjectileHandler
{
public:
    virtual ~IProjectileHandler() = default;
    virtual void AddPlayerProjectile(GameObject&& projectile) = 0;
    virtual void AddEnemyProjectile(GameObject&& projectile) = 0;
};
