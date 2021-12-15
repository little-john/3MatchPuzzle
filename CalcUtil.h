#pragma once
#include <Siv3D.hpp>

struct OtherPieceInfo;//前方宣言

namespace CalcUtil
{
	/// <summary>
	/// 指定の位置から左端まで何個指定の色と同じ色が続くか計算
	/// </summary>
	int GetLeftMatchCount(int x, int y, ColorF checkColor);

	/// <summary>
	/// 指定の位置から右端まで何個指定の色と同じ色が続くか計算
	/// </summary>
	int GetRightMatchCount(int x, int y, ColorF checkColor);

	/// <summary>
	/// 指定の位置から上端まで何個指定の色と同じ色が続くか計算
	/// </summary>
	int GetUpMatchCount(int x, int y, ColorF checkColor);

	/// <summary>
	/// 指定の位置から下端まで何個指定の色と同じ色が続くか計算
	/// </summary>
	int GetDownMatchCount(int x, int y, ColorF checkColor);

	/// <summary>
	/// 指定の位置から下方に何回落下するか計算
	/// </summary>
	int GetDropCount(int x, int y, Array<OtherPieceInfo>& matchInfo);

	/// <summary>
	/// 重複を不正ぐため、配列に含まれていない時のみ追加させる処理
	/// </summary>
	void AddToArrayIfNotExists(OtherPieceInfo info, Array<OtherPieceInfo>& matchInfo);
}