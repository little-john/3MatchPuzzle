#include "CalcUtil.h"
#include "PieceManager.h"

int CalcUtil::GetLeftMatchCount(int x, int y, ColorF checkColor)
{
	int count = 0;

	// �����̈ʒu���獶�[�܂Ŕ���
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

	// �����̈ʒu����E�[�܂Ŕ���
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

	// �����̈ʒu�����[�܂Ŕ���
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

	// �����̈ʒu���牺�[�܂Ŕ���
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
		// ����x���W�Ɉȓ��ꍇ�͗����ΏۊO
		if (it->x != x)continue;
		
		// ��������Ƀ}�b�`���Ă��痎�����Ȃ��̂ŏ��O
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
