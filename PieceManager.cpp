#include "PieceManager.h"
#include "CalcUtil.h"

PieceManager* PieceManager::instance = NULL; // ������

PieceManager* PieceManager::GetInstance()
{
	return instance;
}

PieceManager::PieceManager(Vec2 initPos, String goName, int pieceSize)
	:GameObject(initPos,goName)
{
	for (int y = 0; y < MaxGridY; y++)
	{
		for (int x = 0; x < MaxGridX; x++)
		{
			Piece piece(
					initPos, //�����ʒu
					U"piece{}_{}"_fmt(x, y),//���O(name => piece0_0 piece1_0)
					x,						//�Ֆʏ��X���W
					y,						//�Ֆʏ��Y���W
					pieceSize				//��̃T�C�Y(���a)
				);
			pieces << piece;
		}
	}

	matchHappenIndices.clear();

	Shuffle();

	lockCount = 0;

	instance = this;
}

void PieceManager::Update()
{
	for (auto it = pieces.begin(); it != pieces.end(); ++it)
	{
		it->Update();
	}

	// Space�L�[��������S��̐F���~��Ȃ����i�f�o�b�O�p)
	if (KeySpace.down()) 
	{
		for (auto it = pieces.begin(); it != pieces.end(); ++it) 
		{
			it->RandomizeColor();
		}

		Shuffle();
	}

	// �����������������Ă���
	if (isDrop) 
	{
		// ���̏����̊�����҂��A���s���̏������Ȃ����
		if (IsInputValid()) 
		{
			Drop(); // ����������
			isDrop = false;
		}
	}
}

void PieceManager::Draw()
{
	for (auto it = pieces.begin(); it != pieces.end(); ++it)
	{
		it->Draw();
	}
}

OtherPieceInfo PieceManager::MoveOtherPiece(int x, int y, MoveDir dir)
{
	// x��y��dir���݂āA�T���Ώۂ� tx��ty�ɕϊ�����
	// �z��̒����� tx��ty����v����piece��T��
	// ��������piece�� dir�̋t�����Ɉړ�������

	int cx = x;
	int cy = y;
	MoveDir targetDir = dir;

	switch (dir)
	{
	case Left:
		cx--;	// ���������ɓ�������A���� x�̍��W�ɂ���
		targetDir = Right; // ���̋�����������������̋t�ɂ���
		break;
	case Right:
		cx++;
		targetDir = Left;
		break;
	case Up:
		cy--;
		targetDir = Down;
		break;
	case Down:
		cy++;
		targetDir = Up;
		break;
	
	}

	OtherPieceInfo info = {};
	Piece* otherPiece = GetPieceAtIndex(cx, cy);
	if (otherPiece == NULL)return info;

	otherPiece->MoveTo(targetDir);
	info.x = x;
	info.y = y;
	return info;
}

Piece* PieceManager::GetPieceAtIndex(int cx, int cy)
{
	for (auto it = pieces.begin(); it != pieces.end(); ++it)
	{
		if (it->IsSameGridIndex(cx, cy))return &(*it);
	}

	return NULL;
}

void PieceManager::ProcessLock(bool flag)
{
	if (flag)lockCount++;
	else lockCount--;
	
	lockCount = (lockCount < 0) ? 0 : lockCount;
}

bool PieceManager::IsInputValid()
{
	return lockCount == 0;
}

void PieceManager::Shuffle()
{
	for (int i=0,max=pieces.size(); i<max; i++)
	{
		Piece* p = &pieces[i];
		int cx = p->GetGridIndexX();
		int cy = p->GetGridIndexY();

		int oneLeft = cx - 1;
		int twoLeft = cx - 2;

		int oneUp = cy - 1;
		int twoUp = cy - 2;

		if (cx < 2) {
			
			if (cy > 1) // ��ԍ�����Q����̋�̏ꍇ�Ay��1�ȏゾ������(�c�����̂ݔ���) 
			{
				while (p->GetPieceColor() == GetPieceAtIndex(cx, oneUp)->GetPieceColor() &&
					p->GetPieceColor() == GetPieceAtIndex(cx, twoUp)->GetPieceColor())
				{
					p->RandomizeColor();
				}
			}
			// ����
			continue;
		}

		if (cy < 2) {

			if (cx > 1) // ��ԏォ��Q�s���̋�̏ꍇ�Ax��1�ȏゾ������(�������̂ݔ���)
			{
				while (p->GetPieceColor() == GetPieceAtIndex(oneLeft, cy)->GetPieceColor() &&
					p->GetPieceColor() == GetPieceAtIndex(twoLeft, cy)->GetPieceColor() )
				{
					p->RandomizeColor();
				}
			}
			// ����
			continue;
		}

		// ���ɂQ�A��ɂQ�܂ł̋�̐F�����āA�����Ɠ����F�Ȃ�A�Ⴄ�F�ɂȂ�܂Œ��I��������
		while( p->GetPieceColor() == GetPieceAtIndex(oneLeft,cy)->GetPieceColor() &&
			p->GetPieceColor() == GetPieceAtIndex(twoLeft, cy)->GetPieceColor() ||
			p->GetPieceColor() == GetPieceAtIndex(cx, oneUp)->GetPieceColor() &&
			p->GetPieceColor() == GetPieceAtIndex(cx, twoUp)->GetPieceColor())
		{ 
			p->RandomizeColor();
		}

	}
}

bool PieceManager::Process3MatchLogic(int cx, int cy)
{
	// 3match�����Ō���������B��ێ�����p
	Array<Piece*> matchPieces;

	Piece* checkPiece = GetPieceAtIndex(cx, cy);
	if (checkPiece == NULL)return false;
	
	// �`�F�b�N�������F
	ColorF checkColor = checkPiece->GetPieceColor();
	// �����̈ʒu���獶�[�܂Ŕ���
	int leftCount = CalcUtil::GetLeftMatchCount(cx, cy, checkColor);
	// �����̈ʒu����E�[�܂Ŕ���
	int rightCount = CalcUtil::GetRightMatchCount(cx, cy, checkColor);
	// �����̈ʒu�����[�܂Ŕ���
	int upCount = CalcUtil::GetUpMatchCount(cx, cy, checkColor);
	// �����̈ʒu���牺�[�܂Ŕ���
	int downCount = CalcUtil::GetDownMatchCount(cx, cy, checkColor);

	// ������
	int totalCountRow = leftCount + rightCount;
	if (totalCountRow >= 2) 
	{
		for (int i = -leftCount; i <= rightCount; i++)
		{
			matchPieces << GetPieceAtIndex(cx + i, cy);
		}
	}

	// �c����
	int totalCountCol = upCount + downCount;
	if (totalCountCol >= 2)
	{
		for (int i = -upCount; i <= downCount; i++)
		{
			matchPieces << GetPieceAtIndex(cx, cy + i);
		}
	}

	// ��������
	for (auto it = matchPieces.begin(); it != matchPieces.end(); ++it)
	{
		(*it)->ErasePiece();
		OtherPieceInfo info;
		info.x = (*it)->GetGridIndexX();
		info.y = (*it)->GetGridIndexY();
		CalcUtil::AddToArrayIfNotExists(info, matchHappenIndices);
	}

	if (!isDrop)isDrop = matchPieces.size() >= 3;

	return matchPieces.size() >= 3;
}

void PieceManager::Return(int x, int y, MoveDir orginalDir)
{
	Piece* self = GetPieceAtIndex(x, y);
	MoveDir selfMoveDir = orginalDir;

	switch (selfMoveDir)
	{
	case Left:
		selfMoveDir = Right;
		break;
	case Right:
		selfMoveDir = Left;
		break;
	case Up:
		selfMoveDir = Down;
		break;
	case Down:
		selfMoveDir = Up;
		break;
	}

	if (self == NULL)return;
	if (selfMoveDir == None)return;

	ProcessLock(true);

	self->MoveTo(selfMoveDir);

	MoveOtherPiece(x, y, selfMoveDir);
}

void PieceManager::Drop() 
{
	for (auto it = pieces.begin(); it != pieces.end(); ++it)
	{
		int px = it->GetGridIndexX();
		int py = it->GetGridIndexY();
		int dropCount = CalcUtil::GetDropCount(px, py, matchHappenIndices);
		if (dropCount == 0)continue;
		it->Drop(dropCount);
	}

	matchHappenIndices.clear();
}

void PieceManager::CalcResetYIndex(int x, int& y)
{
	int checkY = -1;
	while (true)
	{
		Piece* p = GetPieceAtIndex(x, checkY);
		if (p == NULL)break;

		checkY--;
	}

	y = checkY;
}
