#include "GameObject.h"

Vec2 GameObject::GetPos() {
	return pos;
}

String GameObject::GetName()
{
	return name;
}

GameObject::GameObject(Vec2 initPos, String goName)
{
	pos = initPos;
	name = goName;
}