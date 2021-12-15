#pragma once
#include <Siv3D.hpp>

struct OtherPieceInfo;//�O���錾

namespace CalcUtil
{
	/// <summary>
	/// �w��̈ʒu���獶�[�܂ŉ��w��̐F�Ɠ����F���������v�Z
	/// </summary>
	int GetLeftMatchCount(int x, int y, ColorF checkColor);

	/// <summary>
	/// �w��̈ʒu����E�[�܂ŉ��w��̐F�Ɠ����F���������v�Z
	/// </summary>
	int GetRightMatchCount(int x, int y, ColorF checkColor);

	/// <summary>
	/// �w��̈ʒu�����[�܂ŉ��w��̐F�Ɠ����F���������v�Z
	/// </summary>
	int GetUpMatchCount(int x, int y, ColorF checkColor);

	/// <summary>
	/// �w��̈ʒu���牺�[�܂ŉ��w��̐F�Ɠ����F���������v�Z
	/// </summary>
	int GetDownMatchCount(int x, int y, ColorF checkColor);

	/// <summary>
	/// �w��̈ʒu���牺���ɉ��񗎉����邩�v�Z
	/// </summary>
	int GetDropCount(int x, int y, Array<OtherPieceInfo>& matchInfo);

	/// <summary>
	/// �d����s�������߁A�z��Ɋ܂܂�Ă��Ȃ����̂ݒǉ������鏈��
	/// </summary>
	void AddToArrayIfNotExists(OtherPieceInfo info, Array<OtherPieceInfo>& matchInfo);
}