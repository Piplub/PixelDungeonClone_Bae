#pragma once
#include "config.h"

class Level;
class Entity;

class TurnManager
{
private:
	vector<Entity*> turnQueue;
	int currentActorIndex = 0;
public:
	TurnManager();
	virtual ~TurnManager();

	void Init();

	void AddActor(Entity* actor);
	void ProcessTurns(Level* level);
	void EndTurn();
	Entity* GetCurrentActor();
};

