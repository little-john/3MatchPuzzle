#include "CalcUtil.h"
#include "PieceManager.h"

int CalcUtil::GetLeftMatchCount(int x, int y, ColorF checkColor)
{
	int count = 0;

	// 自分の位置から左端まで判定
	int cx = x - 1;

	while (cx >= 0)
	{
		Piece* p = PieceManager::GetInstance()->GetPieceAtIndex(cx, y);

		if (p == NULL)
		{
			return 0;
		}

		if ((*p).GetPieceColor() == checkColor)
		{
			count++;
		}
		else
		{
			break;
		}
		cx--;
	}

	return count;
}

int CalcUtil::GetRightMatchCount(int x,int y, ColorF checkColor)
{
	int count = 0;

	// 自分の位置から右端まで判定
	int cx = x + 1;

	while (cx < MaxGridX)
	{
		Piece* p = PieceManager::GetInstance()->GetPieceAtIndex(cx, y);

		if (p == NULL) 
		{
			return 0;
		}

		if ((*p).GetPieceColor() == checkColor)
		{
			count++;
		}
		else
		{
			break;
		}
		cx++;
	}

	return count;
}

int CalcUtil::GetUpMatchCount(int x, int y, ColorF checkColor) 
{
	int count = 0;

	// 自分の位置から上端まで判定
	int cy = y - 1;

	while (cy >= 0)
	{
		Piece* p = PieceManager::GetInstance()->GetPieceAtIndex(x, cy);

		if (p == NULL)
		{
			return 0;
		}

		if ((*p).GetPieceColor() == checkColor)
		{
			count++;
		}
		else
		{
			break;
		}
		cy--;
	}

	return count;
}

int CalcUtil::GetDownMatchCount(int x, int y, ColorF checkColor) 
{
	int count = 0;

	// 自分の位置から下端まで判定
	int cy = y + 1;

	while (cy < MaxGridY)
	{
		Piece* p = PieceManager::GetInstance()->GetPieceAtIndex(x, cy);

		if (p == NULL)
		{
			return 0;
		}

		if ((*p).GetPieceColor() == checkColor)
		{
			count++;
		}
		else
		{
			break;
		}
		cy++;
	}

	return count;
}

int CalcUtil::GetDropCount(int x, int y, Array<OtherPieceInfo>& matchInfo)
{
	int dropCount = 0;

	for (auto it = matchInfo.begin(); it != matchInfo.end(); ++it)
	{
		// 同じx座標に以内場合は落下対象外
		if (it->x != x)continue;
		
		// 自分より上にマッチしてたら落下しないので除外
		if (y >= it->y)continue; 
		
		dropCount++;
	}

	return dropCount;
}

void CalcUtil::AddToArrayIfNotExists(OtherPieceInfo info, Array<OtherPieceInfo>& matchInfo)
{
	for (auto it = matchInfo.begin(); it != matchInfo.end(); ++it)
	{
		if (info.x == it->x && info.y == it->y) 
		{
			return;
		}
	}

	matchInfo << info;
}
