#include "Piece.h"
#include "PieceManager.h"


Piece::Piece(Vec2 initPos, String goName, int x, int y, int pieceSize) : GameObject(initPos, goName){
	gridIndexX = x;
	gridIndexY = y;
	size = pieceSize;

	drawObject = Circle(Vec2(), size);

	//���͂��L���ȋ���
	inputValidDistance = pieceSize;

	//�F�̒��I
	RandomizeColor();

	//�t���O�ϐ��̏�����
	isMoving = false;
	isClicked = false;

	//�����ʒu�̐ݒ�
	position = calcDrawPos();

	// �����x�̏�����
	opacity = 1.0;

	// ������ω��̏�����
	eraseTransitionTime = 0.0f;

	// �e�t���O�̏�����
	isEraseStart = false;
	IsInputMove = false;
	isMatchProcessWait = false;

	lastMovedDir = None;
	dropCount = 0;
}

bool Piece::IsSameGridIndex(int x, int y) 
{
	return (gridIndexX == x) && (gridIndexY == y);
}

int Piece::GetGridIndexX()
{
	return gridIndexX;
}

int Piece::GetGridIndexY()
{
	return gridIndexY;
}

ColorF Piece::GetPieceColor()
{
	return pieceColor;
}

void Piece::RandomizeColor()
{
	//��̐F�𒊑I���Ď����Ă���
	//��̎�ނ̐F��`
	ColorF pieceColors[MaxColor] = {
		Palette::Red,
		Palette::Darkgreen,
		Palette::Darkblue,
		Palette::Yellow,
		Palette::Purple,
		Palette::Orange,
		Palette::Lightcyan
	};

	//�����_���ŐF�����߂�
	//���ꂽ�����܂Ŋ܂܂�܂�
	int pieceType = Random(MaxColor-1);
	pieceColor = pieceColors[pieceType];
}

bool Piece::IsValidMoveDir(MoveDir targetDir)
{
	bool result = true;

	int cx = gridIndexX;
	int cy = gridIndexY;

	switch (targetDir)
	{
	case Left: // �Ֆʂ̈�ԍ��ɂ���ꍇ�͂���ȏ㍶�ւ����܂���
		cx -= 1;
		if (cx < 0)result = false;
		break;

	case Right: // �Ֆʂ̈�ԉE�ɂ���ꍇ�͂���ȏ�E�ւ����܂���
		cx += 1;
		if (cx >= 7)result = false;
		break;

	case Up: // �Ֆʂ̈�ԏ�ɂ���ꍇ�͂���ȏ��ւ����܂���
		cy -= 1;
		if (cy < 0)result = false;
		break;

	case Down: // �Ֆʂ̈�ԉ��ɂ���ꍇ�͂���ȏ㉺�ւ����܂���
		cy += 1;
		if (cy >= 7)result = false;
		break;
	}

	return result;
}

// �����鏈���̎����e�X�g
float fadeTransition = 0.0f;
void TestOpacity(float& opacity) 
{
	fadeTransition += Scene::DeltaTime() * 0.05f;
	fadeTransition = (fadeTransition > 1.0f) ? 1.0f : fadeTransition;
	opacity = Math::Lerp(1.0f, 0.0f, fadeTransition);

	if (KeySpace.down()) 
	{
		fadeTransition = 0.0f;
	}
}

void Piece::Update()
{
	// �����x�̕ω����e�X�g���鏈��
	// TestOpacity(opacity);

	// �ړ�����
	if (isMoving)
	{
		Move();

		//�ړ���������(�ړ��J�n���Ă���̌o�ߎ��Ԃ��b�o�߂�����)
		if (moveStartElapsedTime >= 1)
		{
			Move();
			UpdateGridIndex(); // �Ֆʍ��W�̍X�V

			dir = None;
			moveStartElapsedTime = 0;
			isMoving = false;

			// ���̋�ړ��I����ĂȂ��\��������̂ŁA
			// �����̈ړ����I����Ă������}�b�`���O�������s�킸�ɁA
			// �S�����ړ������I���܂ł܂��Ă���A�}�b�`���O����������悤
			// �t���O�𗧂Ă�
			isMatchProcessWait = true;

			// �����҂��J�E���g�����炷
			PieceManager::GetInstance() ->ProcessLock(false);
		}
	}
	// ��������
	else if (isEraseStart) 
	{
		eraseTransitionTime += Scene::DeltaTime() * PieceEraseSpeed;
		eraseTransitionTime = (eraseTransitionTime > 1.0f) ? 1.0f : eraseTransitionTime;
		opacity = Math::Lerp(1.0f, 0.0f, eraseTransitionTime);
		
		if (eraseTransitionTime >= 1.0f) 
		{
			Reset();
			// �����҂��J�E���g�����炷
			PieceManager::GetInstance()->ProcessLock(false);
		}
	}
	else
	{
		// PieceManager�̏����҂����͂Ȃɂ����Ȃ�
		if (PieceManager::GetInstance()->IsInputValid() == false)
		{
			return;
		}

		if (isMatchProcessWait)
		{
			// �������g�����͂ɂ��ړ�������ꂽ
			// �i�������́A���������j��̏ꍇ�̂�
			// 3�}�b�`�̔�����s���悤�ɂ���
			if (IsInputMove)
			{
				IsInputMove = false;

				// ��̈ړ�������������A3Match�������������ǂ����̔��菈�������s
				bool isMatchHappensSelf = PieceManager::GetInstance()->Process3MatchLogic(gridIndexX, gridIndexY);
				bool isMatchHappensOther = PieceManager::GetInstance()->Process3MatchLogic(otherPiece.x, otherPiece.y);

				// 3�}�b�`���Ȃ�������߂�
				if (isMatchHappensSelf == false && isMatchHappensOther == false)
				{
					PieceManager::GetInstance()->Return(gridIndexX, gridIndexY, lastMovedDir);
				}
			}

			lastMovedDir = None;
			isMatchProcessWait = false;
			return;
		}

		// ���͔��菈��
		if (drawObject.leftClicked())
		{
			isClicked = true;
			lastClickPos = Cursor::Pos();
		}
		else if( MouseL.up() )
		{
			isClicked = false;
			lastClickPos = Vec2();
		}

		if (isClicked)
		{
			Vec2 mouseDeltaPos = Cursor::Pos() - lastClickPos;
			float absX = Math::Abs(mouseDeltaPos.x);
			float absY = Math::Abs(mouseDeltaPos.y);

			if (absX > absY) //X���̓��͂��傫���̂ō��E�̓��͂Ƃ݂Ȃ��Ă��܂�
			{
				if (absX >= inputValidDistance)
				{
					dir = (mouseDeltaPos.x > 0) ? Right : Left;
				}
			}
			else if (absX < absY) // Y���̓��͂��傫���̂ŏ㉺�̓��͂Ƃ݂Ȃ��Ă��܂�
			{
				if (absY >= inputValidDistance)
				{
					dir = (mouseDeltaPos.y < 0) ? Up : Down;
				}
			}
			else
			{
				dir = None;
			}

			if (dir != None)
			{
				if (IsValidMoveDir(dir)) 
				{
					IsInputMove = true;
					lastMovedDir = dir;
					MoveTo(dir);
					otherPiece = PieceManager::GetInstance()->MoveOtherPiece(gridIndexX, gridIndexY, dir);
					
					// �����҂��J�E���g��������
					PieceManager::GetInstance()->ProcessLock(true);
				}
				else
					dir = None;
			}
		}
	}

}

void Piece::UpdateGridIndex()
{
	switch (dir)
	{
	case Left:
		gridIndexX--;
		break;
	case Right:
		gridIndexX++;
		break;
	case Up:
		gridIndexY--;
		break;
	case Down:
		gridIndexY++;
		break;
	
	}
}

void Piece::Reset()
{
	// �Ֆʏ�̍��W��ՖʊO�̏��ɂ���
	PieceManager::GetInstance()->CalcResetYIndex(gridIndexX, gridIndexY);

	// �F�̒��I
	RandomizeColor();

	// �����ʒu�̐ݒ�
	position = calcDrawPos();

	// �t���O�ϐ��̏�����
	isMoving = false;
	isClicked = false;
	isEraseStart = false;
	IsInputMove = false;
	isMatchProcessWait = false;

	// �����x�̏�����
	opacity = 1.0;

	// ������ω��̏�����
	eraseTransitionTime = 0.0f;

	lastMovedDir = None;
}

void Piece::Draw()
{
	drawObject.setPos(position);

	ColorF drawColor = (isClicked) ? Palette::White : (Color)pieceColor;

	// ��̐F�ɓ����x�𔽉f�����ŏI�I�ɕ`�悷��F
	ColorF finalColor(drawColor.r, drawColor.g, drawColor.b, opacity);

	drawObject.draw(finalColor)
		.drawArc(320_deg, 30_deg, 32, 0, ColorF(0, 0, 0, 0.05 * opacity))
		.drawArc(0_deg, 30_deg, 32, 0, ColorF(0, 0, 0, 0.1 * opacity))
		.drawArc(30_deg, 30_deg, 32, 0, ColorF(0, 0, 0, 0.15 * opacity))
		.drawArc(60_deg, 60_deg, 32, 0, ColorF(0, 0, 0, 0.2 * opacity))
		.drawArc(120_deg, 30_deg, 32, 0, ColorF(0, 0, 0, 0.15 * opacity))
		.drawArc(150_deg, 30_deg, 32, 0, ColorF(0, 0, 0, 0.1 * opacity))
		.drawArc(180_deg, 30_deg, 32, 0, ColorF(0, 0, 0, 0.05 * opacity));
		
	//Vec2 debugPos = position + Vec2(-12, 0);
}

void Piece::Move()
{
	// �o�ߎ��Ԃ�deltaTime�����Z
	moveStartElapsedTime += Scene::DeltaTime() * PieceMoveSpeed; 
	
	// �o�ߎ��Ԃ�1�܂ł܂�߂Ă���
	moveStartElapsedTime = (moveStartElapsedTime > 1) ? 1 : moveStartElapsedTime;
	
	// ������̈ړ�(�J�n�A�I���A�o��)
	position = Math::Lerp(fromPos, destPos, moveStartElapsedTime); 
}

void Piece::MoveTo(MoveDir direction)
{
	isClicked = false;
	isMoving = true;

	fromPos = position;
	destPos = fromPos;
	dir = direction;
	moveStartElapsedTime = 0.0f;

	int moveLength = size * 2;
	switch (dir)
	{

	case Left:
		destPos.x -= moveLength;
		break;
	case Right:
		destPos.x += moveLength;
		break;
	case Up:
		destPos.y -= moveLength;
		break;
	case Down:
		destPos.y += moveLength;
		break;
	case MoveDir::Drop: // �����̏ꍇ�͒ʏ�ړ����� * ������
		destPos.y += moveLength * dropCount;
		break;
	default:
		break;
	}
}

Vec2 Piece::calcDrawPos()
{
	//��ʂ̂ǂ̈ʒu����n�܂邩(offset)
	//initPos => pos��offset�������Ă���
	Vec2 drawPos;
	drawPos.x = pos.x;
	drawPos.y = pos.y;

	//��ʂ�X��Y�̂ǂ̈ʒu�ɂ��邩
	//X��Y�̗v�f�ɃT�C�Y����(���炷)
	drawPos.x += gridIndexX * size * 2 + size;
	drawPos.y += gridIndexY * size * 2 + size;
	return drawPos;
}

void Piece::ErasePiece()
{
	// ���ɏ����������������瑦���^�[��
	if (isEraseStart)return;

	isEraseStart = true;
	eraseTransitionTime = 0.0f;

	// �����҂��J�E���g��������
	PieceManager::GetInstance()->ProcessLock(true);
}

void Piece::Drop(int time)
{
	dropCount = time;
	MoveTo(MoveDir::Drop);
	gridIndexY += dropCount;
	dropCount = 0;

	// �����I�����Ƀ}�b�`���O�������s�������̂�
	// �t���O�𗧂Ă�
	IsInputMove = true;

	// �����҂��J�E���g��������
	PieceManager::GetInstance()->ProcessLock(true);
}
