﻿#include "TurnManager.h"
#include "Entity.h"

TurnManager::TurnManager()
{
}

TurnManager::~TurnManager()
{
}

void TurnManager::AddActor(Entity* actor)
{
	// 추가하려는 Entity가 이미 container에 있다면 return
	auto it = find(turnQueue.begin(), turnQueue.end(), actor);
	if (it != turnQueue.end())
		return;

	turnQueue.push_back(actor);
}

void TurnManager::ProcessTurns(Level* level)
{
	if (turnQueue.empty()) return;
	Entity* actor = GetCurrentActor();
	if (!actor || !actor->isActive)
	{
		EndTurn();
		return;
	}
	if (actor->NeedsInput()) {
		actor->Act(level);
	}
	else {
		actor->Act(level);
		if (!actor->IsBusy()) {
			EndTurn();
		}
	}

	//if (!actor->NeedsInput() || !actor->IsBusy())
	//{
	//	actor->Act(game);
	//}
	//else
	//{
	//	actor->Act(game);
	//	EndTurn();
	//}
}

void TurnManager::EndTurn()
{
	if (turnQueue.empty()) return;

	currentActorIndex = (currentActorIndex + 1) % turnQueue.size();
}

Entity* TurnManager::GetCurrentActor()
{
	if (turnQueue.empty()) return nullptr;

	return turnQueue[currentActorIndex];
}
