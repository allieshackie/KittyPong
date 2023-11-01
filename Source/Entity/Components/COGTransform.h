#pragma once
#include <glm/vec2.hpp>

class COGTransform
{
public:
	COGTransform(glm::vec2 position) : mPosition(position)
	{
	}

	const glm::vec2& GetPosition() const { return mPosition; }
	void SetPosition(glm::vec2 newPos) { mPosition = newPos; }

private:
	glm::vec2 mPosition = {0, 0};
};
