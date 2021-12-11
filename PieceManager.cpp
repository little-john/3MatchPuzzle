#include "PieceManager.h"

PieceManager* PieceManager::GetInstance()
{
	return instance;
}

PieceManager* PieceManager::instance = nullptr; // ������

PieceManager::PieceManager(Vec2 initPos, String goName, int pieceSize)
	:GameObject(initPos,goName)
{
	for (int y = 0; y < 7; y++)
	{
		for (int x = 0; x < 7; x++)
		{
			Piece piece(
					initPos, //�����ʒu
					U"piece{}_{}"_fmt(x, y),//���O(name => piece0_0 piece1_0)
					x,						//�Ֆʏ��X���W
					y,						//�Ֆʏ��Y���W
					pieceSize				//��̃T�C�Y(���a)
				);

			//piece.SetPieceManager(this);

			pieces << piece;
		}
	}

	Shuffle();

	isInputValid = true;

	instance = this;
}

void PieceManager::Update()
{
	for (auto it = pieces.begin(); it != pieces.end(); ++it)
	{
		it->Update();
	}
}

void PieceManager::Draw()
{
	for (auto it = pieces.begin(); it != pieces.end(); ++it)
	{
		it->Draw();
	}
}

void PieceManager::MoveOtherPiece(int x, int y, MoveDir dir)
{
	// TODO: 
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

	Piece* otherPiece = GetPieceAtIndex(cx, cy);
	otherPiece->MoveTo(targetDir);
}

Piece* PieceManager::GetPieceAtIndex(int cx, int cy)
{
	for (int i=0, max = pieces.size(); i < max; i++)
	{
		if ( pieces[i].IsSameGridIndex(cx,cy))
		{
			return &pieces[i];
		}
	}
	
	assert(cx >= 0 && cy >= 0);

	return nullptr;
}

void PieceManager::SetInputValid(bool flag)
{
	isInputValid = flag;
}

bool PieceManager::IsInputValid()
{
	return isInputValid;
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
				while (p->GetPieceColor() == GetPieceAtIndex(cx, oneUp)->GetPieceColor() ||
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
				while (p->GetPieceColor() == GetPieceAtIndex(oneLeft, cy)->GetPieceColor() ||
					p->GetPieceColor() == GetPieceAtIndex(twoLeft, cy)->GetPieceColor() )
				{
					p->RandomizeColor();
				}
			}
			// ����
			continue;
		}

		// ���ɂQ�A��ɂQ�܂ł̋�̐F�����āA�����Ɠ����F�Ȃ�A�Ⴄ�F�ɂȂ�܂Œ��I��������
		while( p->GetPieceColor() == GetPieceAtIndex(oneLeft,cy)->GetPieceColor() ||
			p->GetPieceColor() == GetPieceAtIndex(twoLeft, cy)->GetPieceColor() ||
			p->GetPieceColor() == GetPieceAtIndex(cx, oneUp)->GetPieceColor() ||
			p->GetPieceColor() == GetPieceAtIndex(cx, twoUp)->GetPieceColor())
		{ 
			p->RandomizeColor();
		}

	}
}

void PieceManager::Process3MatchLogic(int cx, int cy)
{
	// 3match�����Ō���������B��ێ�����p
	Array<Piece> matchPieces;

	// �`�F�b�N�������F
	ColorF checkColor = GetPieceAtIndex(cx, cy)->GetPieceColor();

	// ����񔻒�
	// (��ԍ���������A��ԉE����Q�R�}�ȓ��Ȃ�3�}�b�`���������Ȃ��̂�
	// ���̏ꍇ�͔��肵�Ȃ��i���̂��߁A���肷��͈͂� ����E����Q�R�}�ڂ���ɂ���)
	if (cx >= 2 && cx < 5) 
	{
		int count = 0;
		
		// �����̈ʒu���獶�[�܂Ŕ���
		int x = cx -1 ;
		
		while (x >= 0)
		{
			Piece* p = GetPieceAtIndex(x, cy);

			if (p == NULL) {
				Print << U"{} {}"_fmt(x, cy);
				return;
			}

			if ((*p).GetPieceColor() == checkColor)
			{
				count++;
			}
			else 
			{
				break;
			}
			x--;
		}

		// count���Q�ȏ゠������i�������ɑ΂��ĂR�}�b�`���������Ă���)
		// �����ׂ����z��ɂ���Ă���
		if (count >= 2)
		{
			for (int i=1; i <= count; i++)
			{
				matchPieces << *GetPieceAtIndex(cx - i, cy);
			}
		}

		count = 0; // ����������قȂ�̂Ń��Z�b�g����

		// �����̈ʒu����E�[�܂Ŕ���
		x = cx + 1;

		while (x < 5)
		{
			Piece* p = GetPieceAtIndex(x, cy);

			if ((*p).GetPieceColor() == checkColor)
			{
				count++;
			}
			else
			{
				break;
			}
			x++;
		}

		//Print << U"Right:{}"_fmt(count);

		// count���Q�ȏ゠������i�E�����ɑ΂��ĂR�}�b�`���������Ă���)
		// �����ׂ����z��ɂ���Ă���
		if (count >= 2)
		{
			for (int i = 1; i <= count; i++)
			{
				matchPieces << *GetPieceAtIndex(cx + i, cy);
			}
		}
	}

	for (auto it = matchPieces.begin(); it != matchPieces.end(); ++it)
	{
		it->ErasePiece();
	}


	// �c��񔻒�
	// (��ԏゾ������A��ԉ�����Q�R�}�ȓ��Ȃ�3�}�b�`���������Ȃ��̂�
	// ���̏ꍇ�͔��肵�Ȃ��i���̂��߁A���肷��͈͂� ��≺����Q�R�}�ڂ���ɂ���)
	if (cy >= 2 && cy < 5)
	{
	
	}
}
