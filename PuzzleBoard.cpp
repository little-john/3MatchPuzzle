#include "PuzzleBoard.h"

PuzzleBoard::PuzzleBoard(Vec2 initPos, String goName) : GameObject(initPos, goName)
{
	// �\���̈�̂̍��E��6px , �㉺��1px��margin�Ƃ��Đݒ肷��
	puzzleBoardStartOffset = Vec2(6, 1);
}

void PuzzleBoard::Update() 
{

}

void PuzzleBoard::Draw() 
{
	// PuzzleBoard�̉��n�̕`��(�O���[�F�ŕ`��)
	Rect(pos.x, pos.y, WindowWidth, PuzzleBoardHeight).draw(ColorF(0.2, 0.2, 0.2, 1.0));

	for (int y = 0; y < MaxGridY; y++)
	{
		for (int x = 0; x < MaxGridX; x++)
		{
			int drawPosX = puzzleBoardStartOffset.x;
			drawPosX += GridSize * x;

			int drawPosY = ViewPortHeight + puzzleBoardStartOffset.y;
			drawPosY += GridSize * y;

			// Chess Board�̗p�ɔ����̃p�^�[���ɂȂ�悤�Ɍv�Z
			ColorF gridColor;
			
			if ((y % 2 == 0))
				gridColor = ((x % 2) == 0) ? Palette::Darkgray : Palette::Gray;
			else
				gridColor = ((x % 2) == 0) ? Palette::Gray : Palette::Darkgray;

			//�Ֆʂ̊p�ێl�p��`��(3px���p���Ȃ���)
			RoundRect(drawPosX, drawPosY, GridSize, GridSize, 3).draw(gridColor);
		}
	}
}

Vec2 PuzzleBoard::GetStartOffset()
{
	return puzzleBoardStartOffset;
}

