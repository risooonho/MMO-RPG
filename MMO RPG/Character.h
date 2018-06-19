#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include <map>

#include "Animation.h"
#include "Graphics.h"

class Character
{
protected:
	enum class Direction
	{
		Up,
		Down,
		Left,
		Right,
		Still
	};

public:
	void Update();
	void Draw(const Graphics& gfx) const;

protected:
	explicit Character(const sf::Sprite& sprite);
	virtual ~Character() = default;

protected:
	virtual sf::Vector2f PickMovement() = 0;

protected:
	sf::Sprite sprite;

	std::map<Direction, Animation> animations;

	Direction movementDirection;
};