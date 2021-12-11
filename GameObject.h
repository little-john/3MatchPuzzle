#pragma once
#include<Siv3D.hpp>

class GameObject
{
protected:
	Vec2 pos;		// ���W
	String name;	// ���ʂ��邽�߂̖��O

public:
	Vec2 GetPos();		// ���W���擾����֐�
	String GetName();	// ���O���擾����֐�

	GameObject(Vec2 initPos,String goName); // �R���X�g���N�^

	// virtual���g���Ӗ��Ɋւ��ẮAinterface�𒲂ׂĂ݂�
	virtual void Update() = 0;	// �X�V����
	virtual void Draw() = 0;	// �`�揈��
};