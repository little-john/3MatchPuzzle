#pragma once
#include "GameObject.h"
#include "Piece.h"

enum MoveDir;

class PieceManager :
    public GameObject
{
private:
    Array<Piece> pieces;
    bool isInputValid;
    static PieceManager* instance; // シングルトン化するためのstatic変数
public:
    static PieceManager* GetInstance(); // シングルトンのインスタンスを公開
    PieceManager(Vec2 initPos, String goName, int pieceSize);
    void Update() override;
    void Draw() override;
    
    // 他の駒を動かすための処理
    void MoveOtherPiece(int x, int y, MoveDir dir);

    // チェックしたい座標(x,y)にいる駒を取得する処理
    Piece* GetPieceAtIndex(int cx, int cy);

    // 入力が有効かどうかの制御用
    void SetInputValid(bool flag);
    bool IsInputValid();

    void Shuffle();//3マッチが発生しないように色のシャッフルを行う
    
    void Process3MatchLogic(int cx, int cy);//引数の位置から横と縦一列で3マッチの判定を行る
};

