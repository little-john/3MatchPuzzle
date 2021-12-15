#include "BattleManager.h"

BattleManager::BattleManager(Vec2 pos, String name):GameObject(pos,name)
{
	//RPG‚Á‚Û‚¢‰æ‘œ‚ðƒ[ƒh‚µ‚Ä‚¨‚­
	background = Texture(U"dungeon.jpg");
	monster = Texture(U"monster.png");
}

void BattleManager::Update()
{

}

void BattleManager::Draw()
{
	//Viewport‚Ì‰º’n‚Ì•`‰æ
	Rect(0, 0, WindowWidth, ViewPortHeight).draw(Palette::Black);

	// ‘•ü‚Æ‚µ‚ÄRPG‚Á‚Û‚¢‰æ–Ê‚Ì•`‰æ
	// TODO:–{—ˆ‚ÍPPGƒp[ƒg‚ðì‚éˆ—‚Ì•û‚ÉŠÇ—‚â•`‰æ‚ð”C‚¹‚é
	background.draw();
	monster.draw(Scene::Center().x - 64, 32);

	// TODO:‘Ì—ÍƒQ[ƒW‚ÌŽÀ‘•
	RoundRect(Scene::Center().x - 64, 160, 128, 16, 5).draw(Palette::Greenyellow);
	RoundRect(Scene::Center().x - 64, 160, 96, 16, 5).draw(Palette::Green);
}
