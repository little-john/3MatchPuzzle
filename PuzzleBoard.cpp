#include "PuzzleBoard.h"

PuzzleBoard::PuzzleBoard(Vec2 initPos, String goName) : GameObject(initPos, goName)
{
	// 表示領域のの左右に6px , 上下に1pxをmarginとして設定する
	puzzleBoardStartOffset = Vec2(6, 1);
}

void PuzzleBoard::Update() 
{

}

void PuzzleBoard::Draw() 
{
	// PuzzleBoardの下地の描画(グレー色で描画)
	Rect(pos.x, pos.y, WindowWidth, PuzzleBoardHeight).draw(ColorF(0.2, 0.2, 0.2, 1.0));

	for (int y = 0; y < MaxGridY; y++)
	{
		for (int x = 0; x < MaxGridX; x++)
		{
			int drawPosX = puzzleBoardStartOffset.x;
			drawPosX += GridSize * x;

			int drawPosY = ViewPortHeight + puzzleBoardStartOffset.y;
			drawPosY += GridSize * y;

			// Chess Boardの用に白黒のパターンになるように計算
			ColorF gridColor;
			
			if ((y % 2 == 0))
				gridColor = ((x % 2) == 0) ? Palette::Darkgray : Palette::Gray;
			else
				gridColor = ((x % 2) == 0) ? Palette::Gray : Palette::Darkgray;

			//盤面の角丸四角を描画(3px分角を曲げる)
			RoundRect(drawPosX, drawPosY, GridSize, GridSize, 3).draw(gridColor);
		}
	}
}

Vec2 PuzzleBoard::GetStartOffset()
{
	return puzzleBoardStartOffset;
}

