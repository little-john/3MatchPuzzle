#include "Piece.h"
#include "PieceManager.h"


Piece::Piece(Vec2 initPos, String goName, int x, int y, int pieceSize) : GameObject(initPos, goName){
	gridIndexX = x;
	gridIndexY = y;
	size = pieceSize;

	drawObject = Circle(Vec2(), size);

	//入力が有効な距離
	inputValidDistance = pieceSize;

	//色の抽選
	RandomizeColor();

	//フラグ変数の初期化
	isMoving = false;
	isClicked = false;

	//初期位置の設定
	position = calcDrawPos();

	// 透明度の初期化
	opacity = 1.0;

	// 消える変化の初期化
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
	//駒の色を抽選して持っておく
	//駒の種類の色定義
	ColorF pieceColors[7] = {
		Palette::Red,
		Palette::Darkgreen,
		Palette::Darkblue,
		Palette::Yellow,
		Palette::Purple,
		Palette::Orange,
		Palette::Lightcyan
	};

	//生成時にランダムで色を決める
	int pieceType = Random(6);//入れた数字まで含まれます(0～6)
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
	case Left: // 盤面の一番左にいる場合はそれ以上左へいけません
		cx -= 1;
		if (cx < 0)result = false;
		break;

	case Right: // 盤面の一番右にいる場合はそれ以上右へいけません
		cx += 1;
		if (cx >= 7)result = false;
		break;

	case Up: // 盤面の一番上にいる場合はそれ以上上へいけません
		cy -= 1;
		if (cy < 0)result = false;
		break;

	case Down: // 盤面の一番下にいる場合はそれ以上下へいけません
		cy += 1;
		if (cy >= 7)result = false;
		break;
	}

	return result;
}

// 消える処理の実装テスト
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
	// 透明度の変化をテストする処理
	// TestOpacity(opacity);

	//動いてたら
	if (isMoving)
	{
		Move();

		//移動完了判定(移動開始してからの経過時間が秒経過したら)
		if (moveStartElapsedTime >= 1)
		{
			Move();
			
			UpdateGridIndex(); // 盤面座標の更新

			dir = None;
			moveStartElapsedTime = 0;
			isMoving = false;

			// 駒の移動が完了したら、3Matchが発生したかどうかの判定処理を実行
			
			PieceManager::GetInstance()->Process3MatchLogic(gridIndexX, gridIndexY);
			

			//ErasePiece(); // テストで駒が移動終わったら消してみる
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
	else//入力があったら
	{
		// PieceManagerの入力が無効ならばなにもしない
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

			if (absX > absY) //X軸の入力が大きいので左右の入力とみなしています
			{
				if (absX >= inputValidDistance)
				{
					dir = (mouseDeltaPos.x > 0) ? Right : Left;
				}
			}
			else if (absX < absY) // Y軸の入力が大きいので上下の入力とみなしています
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
	// 盤面上の座標を盤面外の所におく
	gridIndexY = -1;

	//色の抽選
	RandomizeColor();

	//初期位置の設定
	position = calcDrawPos();

	//フラグ変数の初期化
	isMoving = false;
	isClicked = false;

	// 透明度の初期化
	opacity = 1.0;

	// 消える変化の初期化
	eraseTransitionTime = 0.0f;

	isEraseStart = false;
}

void Piece::Draw()
{
	drawObject.setPos(position);

	ColorF drawColor = (isClicked) ? Palette::White : (Color)pieceColor;

	// 駒の色に透明度を反映した最終的に描画する色
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
	moveStartElapsedTime += Scene::DeltaTime() * 3; // 経過時間にdeltaTimeを加算
	
	// 経過時間を1までまるめておく
	moveStartElapsedTime = (moveStartElapsedTime > 1) ? 1 : moveStartElapsedTime;
	
	position = Math::Lerp(fromPos, destPos, moveStartElapsedTime); // 直線上の移動(開始、終了、経過)
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
	//画面のどの位置から始まるか(offset)
	//initPos => posにoffsetを持っておく
	Vec2 drawPos;
	drawPos.x = pos.x;
	drawPos.y = pos.y;

	//画面のXとYのどの位置にあるか
	//XとYの要素にサイズ足す(ずらす)
	drawPos.x += gridIndexX * size * 2 + size;
	drawPos.y += gridIndexY * size * 2 + size;
	return drawPos;
}
