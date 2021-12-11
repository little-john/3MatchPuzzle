#pragma once
#include "GameObject.h"
#include "Piece.h"

enum MoveDir;

class PieceManager :
    public GameObject
{
private:
    Array<Piece> pieces;
    bool isInputValid;
    static PieceManager* instance; // �V���O���g�������邽�߂�static�ϐ�
public:
    static PieceManager* GetInstance(); // �V���O���g���̃C���X�^���X�����J
    PieceManager(Vec2 initPos, String goName, int pieceSize);
    void Update() override;
    void Draw() override;
    
    // ���̋�𓮂������߂̏���
    void MoveOtherPiece(int x, int y, MoveDir dir);

    // �`�F�b�N���������W(x,y)�ɂ������擾���鏈��
    Piece* GetPieceAtIndex(int cx, int cy);

    // ���͂��L�����ǂ����̐���p
    void SetInputValid(bool flag);
    bool IsInputValid();

    void Shuffle();//3�}�b�`���������Ȃ��悤�ɐF�̃V���b�t�����s��
    
    void Process3MatchLogic(int cx, int cy);//�����̈ʒu���牡�Əc����3�}�b�`�̔�����s��
};

