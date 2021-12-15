#pragma once
#include "GameObject.h"
class BattleManager :
    public GameObject
{
private:
    Texture background;
    Texture monster;
public:
    BattleManager(Vec2 pos, String name);
    void Update() override;
    void Draw() override;
};

