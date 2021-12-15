#pragma once
#include "GameObject.h"
#include "Piece.h"

//enum MoveDir;// 前方宣言

class PieceManager :
    public GameObject
{

private:
    
    /// <summary>
    /// ゲーム中に管理していく駒の配列
    /// </summary>
    Array<Piece> pieces;

    /// <summary>
    /// 3matchが発生した盤面座標配列
    /// </summary>
    Array<OtherPieceInfo> matchHappenIndices;

    /// <summary>
    /// 処理完了待ちを行うためのロックカウント
    /// </summary>
    int lockCount;

    /// <summary>
    /// シングルトン化するためのstatic変数
    /// </summary>
    static PieceManager* instance;

    /// <summary>
    /// 3matchによる落下処理が発生したか判定用
    /// </summary>
    bool isDrop = false;

public:
    
    /// <summary>
    /// シングルトンのインスタンスを公開
    /// </summary>
    /// <returns>static インスタンス</returns>
    static PieceManager* GetInstance();

    /// <summary>
    /// コンストラクター
    /// </summary>
    /// <param name="initPos"></param>
    /// <param name="goName"></param>
    /// <param name="pieceSize">駒のサイズ（半径）</param>
    PieceManager(Vec2 initPos, String goName, int pieceSize);

    void Update() override;
    void Draw() override;
    
    /// <summary>
    /// 他の駒を動かすための処理
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="dir"></param>
    /// <returns></returns>
    OtherPieceInfo MoveOtherPiece(int x, int y, MoveDir dir);

    /// <summary>
    /// チェックしたい座標(x,y)にいる駒を取得する処理
    /// 見つからんかった場合、NULLが変える
    /// </summary>
    /// <param name="cx"></param>
    /// <param name="cy"></param>
    /// <returns></returns>
    Piece* GetPieceAtIndex(int cx, int cy);

    /// <summary>
    /// 処理完了待ち用のカウントを増減する処理
    /// </summary>
    /// <param name="flag">trueの場合、lock++; falseの場合lock--;</param>
    void ProcessLock(bool flag);

    /// <summary>
    /// 処理中でない状態かどうか取得用
    /// </summary>
    /// <returns></returns>
    bool IsInputValid();

    /// <summary>
    /// 3マッチが発生しないように色のシャッフルを行う処理
    /// </summary>
    void Shuffle();
    
    /// <summary>
    /// 指定の位置に３Matchが発生するかどうか判定用
    /// 発生した場合、中で消す処理を呼び出す
    /// </summary>
    /// <param name="cx"></param>
    /// <param name="cy"></param>
    /// <returns>3matchが発生したらtrueが変える</returns>
    bool Process3MatchLogic(int cx, int cy);

    /// <summary>
    /// 3Matchが発生しなかった場合に元の位置に戻すようの処理
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="orginalDir"></param>
    void Return(int x, int y, MoveDir orginalDir);

    /// <summary>
    /// 3Matchにより駒を消したあと、補充して落下させる処理
    /// </summary>
    void Drop();

    /// <summary>
    /// 駒が消えて補充する際、どのY位置にいさせるか計算用
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    void CalcResetYIndex(int x, int& y);
};

