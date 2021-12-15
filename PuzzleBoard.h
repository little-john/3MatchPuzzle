#pragma once
#include "GameObject.h"
class PuzzleBoard :
    public GameObject
{

private:
    /// <summary>
    /// �Ֆʂ�`�悷�鎞�ɁA���E�㉺���Y���margin���邽�߂�
    /// �J�n�ʒu�̃I�t�Z�b�g
    /// </summary>
    Vec2 puzzleBoardStartOffset;

public:
    PuzzleBoard(Vec2 initPos, String goName);
    void Update() override;
    void Draw() override;
    Vec2 GetStartOffset();
};

