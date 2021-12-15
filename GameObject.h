#pragma once
#include<Siv3D.hpp>
#include "Common.h"

struct OtherPieceInfo {
	int x;
	int y;
};

/// <summary>
/// 移動方向
/// </summary>
enum MoveDir
{
	None,
	Left,
	Right,
	Up,
	Down,
	Drop
};

class GameObject
{
protected:
	Vec2 pos;		// 座標
	String name;	// 識別するための名前

public:
	Vec2 GetPos();		// 座標を取得する関数
	String GetName();	// 名前を取得する関数

	GameObject(Vec2 initPos,String goName); // コンストラクタ

	// virtualを使う意味に関しては、interfaceを調べてみる
	virtual void Update() = 0;	// 更新処理
	virtual void Draw() = 0;	// 描画処理
};