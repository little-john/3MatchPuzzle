#include "PieceManager.h"

PieceManager* PieceManager::GetInstance()
{
	return instance;
}

PieceManager* PieceManager::instance = nullptr; // 初期化

PieceManager::PieceManager(Vec2 initPos, String goName, int pieceSize)
	:GameObject(initPos,goName)
{
	for (int y = 0; y < 7; y++)
	{
		for (int x = 0; x < 7; x++)
		{
			Piece piece(
					initPos, //初期位置
					U"piece{}_{}"_fmt(x, y),//名前(name => piece0_0 piece1_0)
					x,						//盤面上のX座標
					y,						//盤面上のY座標
					pieceSize				//駒のサイズ(半径)
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
			
			if (cy > 1) // 一番左から２列内の駒の場合、yが1以上だったら(縦方向のみ判定) 
			{
				while (p->GetPieceColor() == GetPieceAtIndex(cx, oneUp)->GetPieceColor() ||
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
				while (p->GetPieceColor() == GetPieceAtIndex(oneLeft, cy)->GetPieceColor() ||
					p->GetPieceColor() == GetPieceAtIndex(twoLeft, cy)->GetPieceColor() )
				{
					p->RandomizeColor();
				}
			}
			// 無視
			continue;
		}

		// 左に２つ、上に２つまでの駒の色を見て、自分と同じ色なら、違う色になるまで抽選し続ける
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
	// 3match処理で見つかった駒達を保持する用
	Array<Piece> matchPieces;

	// チェックしたい色
	ColorF checkColor = GetPieceAtIndex(cx, cy)->GetPieceColor();

	// 横一列判定
	// (一番左だったり、一番右から２コマ以内なら3マッチが発生しないので
	// その場合は判定しない（そのため、判定する範囲は 左や右から２コマ目からにする)
	if (cx >= 2 && cx < 5) 
	{
		int count = 0;
		
		// 自分の位置から左端まで判定
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

		// countが２つ以上あったら（左方向に対して３マッチが発生している)
		// 消すべき駒を配列にいれておく
		if (count >= 2)
		{
			for (int i=1; i <= count; i++)
			{
				matchPieces << *GetPieceAtIndex(cx - i, cy);
			}
		}

		count = 0; // 見る方向が異なるのでリセットする

		// 自分の位置から右端まで判定
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

		// countが２つ以上あったら（右方向に対して３マッチが発生している)
		// 消すべき駒を配列にいれておく
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


	// 縦一列判定
	// (一番上だったり、一番下から２コマ以内なら3マッチが発生しないので
	// その場合は判定しない（そのため、判定する範囲は 上や下から２コマ目からにする)
	if (cy >= 2 && cy < 5)
	{
	
	}
}
