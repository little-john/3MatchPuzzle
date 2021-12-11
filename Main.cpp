//#define DEBUG_CODE
# include <Siv3D.hpp>
# include "PieceManager.h"


const int WindowWidth = 460;
const int WindowHeight = 640;

const int ViewPortHeight = 190;
const int PuzzleBoardHeight = 450;

const int MaxGridX = 7;
const int MaxGridY = 7;

const int GridSize = 64;

void Main()
{
	Scene::SetBackground(Palette::Black);

	Window::SetTitle(U"Puzzle");

	Window::Resize(WindowWidth, WindowHeight);

	//盤面を描画する時に、左右上下を綺麗にmarginするための
	//開始位置のオフセット
	Vec2 puzzleBoardStartOffset(6, 1);

	Circle debugCircle(Scene::Center().x, 100, 25);
	bool isCircleClicked = false;
	bool isCircleMoving = false;
	float movingElapsedTime = 0;
	Vec2 moveStartPos(0, 0);
	Vec2 moveToPos(Scene::Center().x, 100);

	Vec2 lastMouseClickPos(0, 0); //最後にクリックした時のマウスの座標
	Vec2 deltaPos(0, 0);
	String inputDir = U"-"; //入力があった方向

	PieceManager pieceManager(
		puzzleBoardStartOffset + Vec2(0, ViewPortHeight),
		U"PieceManager",
		GridSize / 2);

	//RPGっぽい画像をロードしておく
	Texture dungeon(U"dungeon.jpg");
	Texture dragon(U"dragon.png");

	while (System::Update())
	{
		//Viewportの下地の描画
		Rect(0, 0, WindowWidth, ViewPortHeight).draw(Palette::Black);

		// 装飾としてRPGっぽい画面の描画
		// TODO:本来はPPGパートを作る処理の方に管理や描画を任せる
		dungeon.draw();
		dragon.draw(Scene::Center().x - 64, 32);
		RoundRect(Scene::Center().x - 64, 160, 128, 16, 5).draw(Palette::Greenyellow);
		RoundRect(Scene::Center().x - 64, 160, 96, 16, 5).draw(Palette::Green);

		//PuzzleBoardの下地の描画
		Rect(0, ViewPortHeight, WindowWidth, PuzzleBoardHeight).draw(ColorF(0.2, 0.2, 0.2, 1.0));

		for (int y = 0; y < MaxGridY; y++)
		{
			for (int x = 0; x < MaxGridX; x++)
			{
				int drawPosX = puzzleBoardStartOffset.x;
				drawPosX += GridSize * x;

				int drawPosY = ViewPortHeight + puzzleBoardStartOffset.y;
				drawPosY += GridSize * y;

				ColorF gridColor;

				if ((y % 2 == 0))
					gridColor = ((x % 2) == 0) ? Palette::Darkgray : Palette::Gray;
				else
					gridColor = ((x % 2) == 0) ? Palette::Gray : Palette::Darkgray;

				//盤面の角丸四角を描画
				RoundRect(drawPosX, drawPosY, GridSize, GridSize, 3).draw(gridColor);
			}
		}

		pieceManager.Update();
		pieceManager.Draw();

		if (debugCircle.leftClicked())
		{
			isCircleClicked = true;
			lastMouseClickPos = Cursor::Pos();
		}
		else if (MouseL.up())
		{
			isCircleClicked = false;
			lastMouseClickPos = Vec2(0, 0);
		}

#ifdef DEBUG_CODE
		debugCircle.draw((isCircleClicked) ? Palette::Red : Palette::Blue);

		if (isCircleClicked)
		{
			deltaPos = Cursor::Pos() - lastMouseClickPos;
		}

		float absX = Math::Abs(deltaPos.x);
		float absY = Math::Abs(deltaPos.y);


		if (absX > absY) //X軸の入力が大きいので左右の入力とみなしています
		{
			if (absX >= GridSize)
			{
				inputDir = (deltaPos.x < 0) ? U"←" : U"→";
				deltaPos = Vec2();
			}
		}
		else if (absX < absY) // Y軸の入力が大きいので上下の入力とみなしています
		{
			if (absY <= GridSize / 2)
			{
				inputDir = (deltaPos.y < 0) ? U"↑" : U"↓";
				deltaPos = Vec2();
			}
		}
		else
		{
			inputDir = U"-";
		}

		if (isCircleMoving)
		{
			Vec2 newPos = Math::Lerp(moveStartPos, moveToPos, movingElapsedTime);
			debugCircle.setPos(newPos);
			movingElapsedTime += Scene::DeltaTime();

			if (movingElapsedTime >= 1)
			{
				isCircleMoving = false;
			}
		}
		else	//円が動いていない場合入力があったら
		{
			if (inputDir == U"←")
			{
				movingElapsedTime = 0;
				moveStartPos = Vec2(debugCircle.x, debugCircle.y);
				moveToPos = moveStartPos;
				moveToPos.x -= GridSize;
				isCircleMoving = true;
				isCircleClicked = false;
				inputDir = U"-";
			}
			else if (inputDir == U"→")
			{
				movingElapsedTime = 0;
				moveStartPos = Vec2(debugCircle.x, debugCircle.y);
				moveToPos = moveStartPos;
				moveToPos.x += GridSize;
				isCircleMoving = true;
				isCircleClicked = false;
				inputDir = U"-";
			}
			else if (inputDir == U"↑")
			{
				movingElapsedTime = 0;
				moveStartPos = Vec2(debugCircle.x, debugCircle.y);
				moveToPos = moveStartPos;
				moveToPos.y -= GridSize;
				isCircleMoving = true;
				isCircleClicked = false;
				inputDir = U"-";
			}
			else if (inputDir == U"↓")
			{
				movingElapsedTime = 0;
				moveStartPos = Vec2(debugCircle.x, debugCircle.y);
				moveToPos = moveStartPos;
				moveToPos.y += GridSize;
				isCircleMoving = true;
				isCircleClicked = false;
				inputDir = U"-";
			}
		}

		//入力値のデバッグ
		ClearPrint();
		s3d::Print << U"dx:{} dy:{} dir:{}"_fmt(deltaPos.x, deltaPos.y, inputDir);

#endif // DEBUG
	}
}
