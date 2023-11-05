#pragma once

class EngineContext;

class ScoreManager
{
public:
	bool Update(const EngineContext& engine, EntityRegistry& entityRegistry, const EntityId& player1,
	            const EntityId& player2) const;
};
