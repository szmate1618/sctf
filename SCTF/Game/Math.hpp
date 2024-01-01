#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>


float GetLength(const sf::Vector2f& v)
{
	return std::sqrt(v.x * v.x + v.y * v.y);
}

static float precalculated_sqrts[1024];

class Initializer
{
public:
	Initializer()
	{
		for (size_t i = 0; i < 1024; i++)
		{
			precalculated_sqrts[i] = std::sqrtf(i / 1024.0f);
		}
	}
};

static const Initializer initializer;

float GetLengthForUnitOrShorter(const sf::Vector2f& v)
{
	float length_squared = v.x * v.x + v.y * v.y;
	return precalculated_sqrts[static_cast<size_t>(length_squared * 1024)];
}

float GetLengthSquared(const sf::Vector2f& v)
{
	return v.x * v.x + v.y * v.y;
}

sf::Vector2f GetNormalized(const sf::Vector2f& v)
{
	float l = GetLength(v);
	return { v.x / l, v.y / l };
}

sf::Vector2f GetNormalizedForUnitOrShorter(const sf::Vector2f& v)
{
	float l = GetLengthForUnitOrShorter(v);
	return { v.x / l, v.y / l };
}

/*sf::Vector2f operator*(const sf::Vector2f& v, const float f)
{
	return { v.x * f, v.y * f };
}

sf::Vector2f operator/(const sf::Vector2f& v, const float f)
{
	return { v.x / f, v.y / f };
}*/
