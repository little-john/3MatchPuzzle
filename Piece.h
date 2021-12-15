#pragma once
#include "GameObject.h"

//Pieceは駒を表すクラス
//継承元はGameObjectクラス
class Piece :
    public GameObject
{
private:
    ColorF pieceColor;  //駒の色
    int gridIndexX;     //盤面上のX座標
    int gridIndexY;     //盤面上のY座標

    int size;           //駒のサイズ(半径)
    Vec2 calcDrawPos(); //描画位置の計算処理
    Circle drawObject;  //実際描画する円

    bool isClicked;     //クリック中か
    bool isMoving;      //移動中か
    float inputValidDistance; //入力が有効な距離
    float moveStartElapsedTime; //移動開始から経過時間
    Vec2 destPos;       //目的位置
    Vec2 fromPos;       //移動開始位置
    MoveDir dir;        //移動方向
    Vec2 lastClickPos;  //最後にクリックした座標

    Vec2 position;      //位置
    
    float opacity;      //透明度(　0.0　～　1.0　)
    float eraseTransitionTime;  //駒が消えるための変化
    bool isEraseStart;  //消す処理が始まったか判定

    void Move();        //実際の移動を行う処理
    bool IsValidMoveDir(MoveDir); // 移動方向は有効かどうか判定
    void UpdateGridIndex(); // 移動完了後に盤面上の座標を更新する処理
    void Reset();       //駒が消えて再利用するために状態やデータをリセットする処理

    bool IsInputMove;   //入力により移動された駒かどうか
    MoveDir lastMovedDir;//最後動いた方向

    bool isMatchProcessWait;//3Match処理待ち中か判定用
    OtherPieceInfo otherPiece;//入力して移動してことにより、移動先にいる駒の座標
    int dropCount;      //落下する回数

public:
    Piece(Vec2 initPos, String goName, int x, int y, int pieceSize);//コンストラクタ
    void Update() override; //親クラスのvirtual関数をオーバーライド(約束事を満たすため)
    void Draw() override; //親クラスのvirtual関数をオーバーライド(約束事を満たすため)
    void MoveTo(MoveDir dir); //外から動かすための処理(引数は方向)
    bool IsSameGridIndex(int x, int y);// 盤面上の座標が同じかを判定する

    int GetGridIndexX();// X座標の公開
    int GetGridIndexY();// Y座標の公開
    ColorF GetPieceColor();//色の公開

    void RandomizeColor();// ランダムで色を変更
    void ErasePiece();// 3マッチ発生による消し処理
    void Drop(int time);// 落下処理（渡された回数分下に移動する)
};
