#pragma once
#include "GameObject.h"
#include "Piece.h"

//enum MoveDir;// �O���錾

class PieceManager :
    public GameObject
{

private:
    
    /// <summary>
    /// �Q�[�����ɊǗ����Ă�����̔z��
    /// </summary>
    Array<Piece> pieces;

    /// <summary>
    /// 3match�����������Ֆʍ��W�z��
    /// </summary>
    Array<OtherPieceInfo> matchHappenIndices;

    /// <summary>
    /// ���������҂����s�����߂̃��b�N�J�E���g
    /// </summary>
    int lockCount;

    /// <summary>
    /// �V���O���g�������邽�߂�static�ϐ�
    /// </summary>
    static PieceManager* instance;

    /// <summary>
    /// 3match�ɂ�闎����������������������p
    /// </summary>
    bool isDrop = false;

public:
    
    /// <summary>
    /// �V���O���g���̃C���X�^���X�����J
    /// </summary>
    /// <returns>static �C���X�^���X</returns>
    static PieceManager* GetInstance();

    /// <summary>
    /// �R���X�g���N�^�[
    /// </summary>
    /// <param name="initPos"></param>
    /// <param name="goName"></param>
    /// <param name="pieceSize">��̃T�C�Y�i���a�j</param>
    PieceManager(Vec2 initPos, String goName, int pieceSize);

    void Update() override;
    void Draw() override;
    
    /// <summary>
    /// ���̋�𓮂������߂̏���
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="dir"></param>
    /// <returns></returns>
    OtherPieceInfo MoveOtherPiece(int x, int y, MoveDir dir);

    /// <summary>
    /// �`�F�b�N���������W(x,y)�ɂ������擾���鏈��
    /// ������񂩂����ꍇ�ANULL���ς���
    /// </summary>
    /// <param name="cx"></param>
    /// <param name="cy"></param>
    /// <returns></returns>
    Piece* GetPieceAtIndex(int cx, int cy);

    /// <summary>
    /// ���������҂��p�̃J�E���g�𑝌����鏈��
    /// </summary>
    /// <param name="flag">true�̏ꍇ�Alock++; false�̏ꍇlock--;</param>
    void ProcessLock(bool flag);

    /// <summary>
    /// �������łȂ���Ԃ��ǂ����擾�p
    /// </summary>
    /// <returns></returns>
    bool IsInputValid();

    /// <summary>
    /// 3�}�b�`���������Ȃ��悤�ɐF�̃V���b�t�����s������
    /// </summary>
    void Shuffle();
    
    /// <summary>
    /// �w��̈ʒu�ɂRMatch���������邩�ǂ�������p
    /// ���������ꍇ�A���ŏ����������Ăяo��
    /// </summary>
    /// <param name="cx"></param>
    /// <param name="cy"></param>
    /// <returns>3match������������true���ς���</returns>
    bool Process3MatchLogic(int cx, int cy);

    /// <summary>
    /// 3Match���������Ȃ������ꍇ�Ɍ��̈ʒu�ɖ߂��悤�̏���
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="orginalDir"></param>
    void Return(int x, int y, MoveDir orginalDir);

    /// <summary>
    /// 3Match�ɂ�������������ƁA��[���ė��������鏈��
    /// </summary>
    void Drop();

    /// <summary>
    /// ������ĕ�[����ہA�ǂ�Y�ʒu�ɂ������邩�v�Z�p
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    void CalcResetYIndex(int x, int& y);
};

