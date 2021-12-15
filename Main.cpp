# include <Siv3D.hpp>
# include "Common.h"
# include "PuzzleBoard.h"
# include "PieceManager.h"
# include "BattleManager.h"

void Main()
{
	Scene::SetBackground(Palette::Black);

	Window::SetTitle(U"3Match Puzzle");

	Window::Resize(WindowWidth, WindowHeight);

	PuzzleBoard board( Vec2( 0, ViewPortHeight ), U"PuzzleBoard" );

	PieceManager pieceManager(
		board.GetStartOffset() + Vec2(0, ViewPortHeight),
		U"PieceManager",
		GridSize * 0.5);

	BattleManager battleManager(Vec2(), U"BattleManager");

	while (System::Update())
	{
		board.Update();
		pieceManager.Update();
		battleManager.Update();

		board.Draw();
		pieceManager.Draw();
		battleManager.Draw();
	}

}
