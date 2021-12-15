#include "PieceManager.h"
#include "CalcUtil.h"

PieceManager* PieceManager::instance = NULL; // 初期化

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
					initPos, //初期位置
					U"piece{}_{}"_fmt(x, y),//名前(name => piece0_0 piece1_0)
					x,						//盤面上のX座標
					y,						//盤面上のY座標
					pieceSize				//駒のサイズ(半径)
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

	// Spaceキー押したら全駒の色を降りなおす（デバッグ用)
	if (KeySpace.down()) 
	{
		for (auto it = pieces.begin(); it != pieces.end(); ++it) 
		{
			it->RandomizeColor();
		}

		Shuffle();
	}

	// 落下処理が発生してたら
	if (isDrop) 
	{
		// 他の処理の完了を待ち、実行中の処理がなければ
		if (IsInputValid()) 
		{
			Drop(); // 落下させる
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
	// xとyをdirをみて、探す対象の txとtyに変換する
	// 配列の中から txとtyが一致するpieceを探す
	// 見つかったpieceを dirの逆方向に移動させる

	int cx = x;
	int cy = y;
	MoveDir targetDir = dir;

	switch (dir)
	{
	case Left:
		cx--;	// 自分が左に動くから、左の xの座標にする
		targetDir = Right; // 左の駒を自分が動く方向の逆にする
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
			
			if (cy > 1) // 一番左から２列内の駒の場合、yが1以上だったら(縦方向のみ判定) 
			{
				while (p->GetPieceColor() == GetPieceAtIndex(cx, oneUp)->GetPieceColor() &&
					p->GetPieceColor() == GetPieceAtIndex(cx, twoUp)->GetPieceColor())
				{
					p->RandomizeColor();
				}
			}
			// 無視
			continue;
		}

		if (cy < 2) {

			if (cx > 1) // 一番上から２行内の駒の場合、xが1以上だったら(横方向のみ判定)
			{
				while (p->GetPieceColor() == GetPieceAtIndex(oneLeft, cy)->GetPieceColor() &&
					p->GetPieceColor() == GetPieceAtIndex(twoLeft, cy)->GetPieceColor() )
				{
					p->RandomizeColor();
				}
			}
			// 無視
			continue;
		}

		// 左に２つ、上に２つまでの駒の色を見て、自分と同じ色なら、違う色になるまで抽選し続ける
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
	// 3match処理で見つかった駒達を保持する用
	Array<Piece*> matchPieces;

	Piece* checkPiece = GetPieceAtIndex(cx, cy);
	if (checkPiece == NULL)return false;
	
	// チェックしたい色
	ColorF checkColor = checkPiece->GetPieceColor();
	// 自分の位置から左端まで判定
	int leftCount = CalcUtil::GetLeftMatchCount(cx, cy, checkColor);
	// 自分の位置から右端まで判定
	int rightCount = CalcUtil::GetRightMatchCount(cx, cy, checkColor);
	// 自分の位置から上端まで判定
	int upCount = CalcUtil::GetUpMatchCount(cx, cy, checkColor);
	// 自分の位置から下端まで判定
	int downCount = CalcUtil::GetDownMatchCount(cx, cy, checkColor);

	// 横判定
	int totalCountRow = leftCount + rightCount;
	if (totalCountRow >= 2) 
	{
		for (int i = -leftCount; i <= rightCount; i++)
		{
			matchPieces << GetPieceAtIndex(cx + i, cy);
		}
	}

	// 縦判定
	int totalCountCol = upCount + downCount;
	if (totalCountCol >= 2)
	{
		for (int i = -upCount; i <= downCount; i++)
		{
			matchPieces << GetPieceAtIndex(cx, cy + i);
		}
	}

	// 消す処理
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
