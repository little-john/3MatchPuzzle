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

	
	isEraseStart = false;
}

//void Piece::SetPieceManager(PieceManager* manager)
//{
//	pieceManager = manager;
//}

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
	ColorF pieceColors[7] = {
		Palette::Red,
		Palette::Darkgreen,
		Palette::Darkblue,
		Palette::Yellow,
		Palette::Purple,
		Palette::Orange,
		Palette::Lightcyan
	};

	//�������Ƀ����_���ŐF�����߂�
	int pieceType = Random(6);//���ꂽ�����܂Ŋ܂܂�܂�(0�`6)
	pieceColor = pieceColors[pieceType];
}

void Piece::ErasePiece()
{
	isEraseStart = true;
	eraseTransitionTime = 0.0f;
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

	//�����Ă���
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

			// ��̈ړ�������������A3Match�������������ǂ����̔��菈�������s
			
			PieceManager::GetInstance()->Process3MatchLogic(gridIndexX, gridIndexY);
			

			//ErasePiece(); // �e�X�g�ŋ�ړ��I�����������Ă݂�
			PieceManager::GetInstance() -> SetInputValid(true);
		}
	}
	else if (isEraseStart) 
	{
		eraseTransitionTime += Scene::DeltaTime() * 2;
		eraseTransitionTime = (eraseTransitionTime > 1.0f) ? 1.0f : eraseTransitionTime;
		opacity = Math::Lerp(1.0f, 0.0f, eraseTransitionTime);
		
		if (eraseTransitionTime >= 1.0f) {
			Reset();
		}
	}
	else//���͂���������
	{
		// PieceManager�̓��͂������Ȃ�΂Ȃɂ����Ȃ�
		if (PieceManager::GetInstance()->IsInputValid() == false)
		{
			return;
		}

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
					MoveTo(dir);
					PieceManager::GetInstance()->MoveOtherPiece(gridIndexX, gridIndexY, dir);
					PieceManager::GetInstance()->SetInputValid(false);
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
	gridIndexY = -1;

	//�F�̒��I
	RandomizeColor();

	//�����ʒu�̐ݒ�
	position = calcDrawPos();

	//�t���O�ϐ��̏�����
	isMoving = false;
	isClicked = false;

	// �����x�̏�����
	opacity = 1.0;

	// ������ω��̏�����
	eraseTransitionTime = 0.0f;

	isEraseStart = false;
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
		.drawArc(180_deg, 30_deg, 32, 0, ColorF(0, 0, 0, 0.05 * opacity))
		.drawFrame(2, ColorF(0, 0, 0, opacity));
}

void Piece::Move()
{
	moveStartElapsedTime += Scene::DeltaTime() * 3; // �o�ߎ��Ԃ�deltaTime�����Z
	
	// �o�ߎ��Ԃ�1�܂ł܂�߂Ă���
	moveStartElapsedTime = (moveStartElapsedTime > 1) ? 1 : moveStartElapsedTime;
	
	position = Math::Lerp(fromPos, destPos, moveStartElapsedTime); // ������̈ړ�(�J�n�A�I���A�o��)
}

void Piece::MoveTo(MoveDir direction)
{
	isClicked = false;
	isMoving = true;

	fromPos = position;
	destPos = fromPos;
	dir = direction;
	moveStartElapsedTime = 0.0f;

	switch (dir)
	{

	case Left:
		destPos.x -= size * 2;
		break;
	case Right:
		destPos.x += size * 2;
		break;
	case Up:
		destPos.y -= size * 2;
		break;
	case Down:
		destPos.y += size * 2;
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
