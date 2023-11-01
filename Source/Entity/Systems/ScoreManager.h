#pragma once
#include <glm/vec2.hpp>

class EngineContext;
class Score;

class ScoreManager
{
public:
	ScoreManager(EntityId ballEntity);

	bool Update(const EngineContext& engine, EntityRegistry& entityRegistry, const EntityId& player1,
	            const EntityId& player2);
	int Menu(const EngineContext& engine, bool choice1, bool choice2, bool enter);

	void SetTimer(int amount);

private:
	void _DrawKitty(const EngineContext& engine);
	void _DrawEyes(const EngineContext& engine);
	void _DrawMouth(const EngineContext& engine);

	int mTimer = 0;
	bool mInput1 = false;
	bool mInput2 = false;

	EntityId mBall;

	glm::vec2 mEyePos1 = {370, 282};
	glm::vec2 mEyePos2 = {428, 282};
};
