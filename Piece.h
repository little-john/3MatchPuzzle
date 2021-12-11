#pragma once
#include "GameObject.h"

//�ړ�����
enum MoveDir
{
    None,
    Left,
    Right,
    Up,
    Down
};

/*
class PieceManager; // PieceManager��Program���̂ǂ����ɂ���N���X�ł���̑O���錾
*/

//Piece�͋��\���N���X
//�p������GameObject�N���X
class Piece :
    public GameObject
{
private:
    
    ColorF pieceColor;  //��̐F
    int gridIndexX;     //�Ֆʏ��X���W
    int gridIndexY;     //�Ֆʏ��Y���W

    int size;           //��̃T�C�Y(���a)
    Vec2 calcDrawPos(); //�`��ʒu�̌v�Z����
    Circle drawObject;  //���ە`�悷��~

    bool isClicked;     //�N���b�N����
    bool isMoving;      //�ړ�����
    float inputValidDistance; //���͂��L���ȋ���
    float moveStartElapsedTime; //�ړ��J�n����o�ߎ���
    Vec2 destPos;       //�ړI�ʒu
    Vec2 fromPos;       //�ړ��J�n�ʒu
    MoveDir dir;        //�ړ�����
    Vec2 lastClickPos;  //�Ō�ɃN���b�N�������W

    Vec2 position;      //�ʒu
    //PieceManager* pieceManager;    //���S�̊Ǘ����Ă��镨�ւ̎Q��

    float opacity;      //�����x(�@0.0�@�`�@1.0�@)
    float eraseTransitionTime;  //������邽�߂̕ω�
    bool isEraseStart;  //�����������n�܂���������

    void Move();        //���ۂ̈ړ����s������
    bool IsValidMoveDir(MoveDir); // �ړ������͗L�����ǂ�������
    void UpdateGridIndex(); // �ړ�������ɔՖʏ�̍��W���X�V���鏈��
    void Reset();       //������čė��p���邽�߂ɏ�Ԃ�f�[�^�����Z�b�g���鏈��
public:
    Piece(Vec2 initPos, String goName, int x, int y, int pieceSize);//�R���X�g���N�^
    void Update() override; //�e�N���X��virtual�֐����I�[�o�[���C�h(�񑩎��𖞂�������)
    void Draw() override; //�e�N���X��virtual�֐����I�[�o�[���C�h(�񑩎��𖞂�������)
    void MoveTo(MoveDir dir); //�O���瓮�������߂̏���(�����͕���)
    //void SetPieceManager(PieceManager* manager);// �}�l�[�W���[�ւ̎Q��
    bool IsSameGridIndex(int x, int y);// �Ֆʏ�̍��W���������𔻒肷��

    int GetGridIndexX();// X���W�̌��J
    int GetGridIndexY();// Y���W�̌��J
    ColorF GetPieceColor();//�F�̌��J

    void RandomizeColor();// �����_���ŐF��ύX
    void ErasePiece();// 3�}�b�`�����ɂ���������
};
