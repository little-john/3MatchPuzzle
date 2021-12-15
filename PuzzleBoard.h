#pragma once
#include "GameObject.h"
class PuzzleBoard :
    public GameObject
{

private:
    /// <summary>
    /// 盤面を描画する時に、左右上下を綺麗にmarginするための
    /// 開始位置のオフセット
    /// </summary>
    Vec2 puzzleBoardStartOffset;

public:
    PuzzleBoard(Vec2 initPos, String goName);
    void Update() override;
    void Draw() override;
    Vec2 GetStartOffset();
};

