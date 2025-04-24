﻿#include "Entity.h"
#include "Level.h"
#include "D2DImage.h"
#include "D2DImageManager.h"
#include "TimerManager.h"
#include "UIManager.h"
#include "Animator.h"

Entity::Entity()
{
	image = D2DImageManager::GetInstance()->AddImage("warrior", L"Image/warrior.png", 21, 7);
	//image = D2DImageManager::GetInstance()->AddImage("entityTest", L"Image/rat.png", 16, 3);
	isActive = true;
	animator = new Animator;
}

Entity::~Entity()
{
}

void Entity::Update()
{
	//curTime += TimerManager::GetInstance()->GetDeltaTime();
	//if (curTime > maxAnimTime)
	//{
	//	curAnimFrame++;
	//	curTime = 0;
	//	if (!stayEndFrame)
	//	{
	//		if (curAnimFrame > endFrame)
	//		{
	//			curAnimFrame = startFrame;
	//		}
	//	}
	//	else if (curAnimFrame > endFrame)
	//	{
	//		curAnimFrame = endFrame;
	//		return;
	//	}
	//	
	//}
	float delta = TimerManager::GetInstance()->GetDeltaTime();
	animator->Update(delta);
	curAnimFrame = animator->GetCurFrame();
}

void Entity::Render(HDC hdc)
{
	// Rectangle(hdc, position.x - TILE_SIZE / 2, position.y - TILE_SIZE / 2,
	// 	position.x + TILE_SIZE / 2, position.y + TILE_SIZE / 2);
	image->Middle_RenderFrameScale(position.x, position.y, 2, 2, curAnimFrame, 1 );
	//image->Middle_RenderFrame(position.x, position.y, 0, 0);
	//wsprintf(szText, TEXT("hp:%d, att:%d, df:%d"), hp, attackDmg, defense);
	//TextOut(hdc, position.x - TILE_SIZE, position.y, szText, wcslen(szText));
  
	//if (image)
  //      image->Middle_RenderFrameScale(position.x, position.y, 2.f, 2.f, 0, 0);
}

void Entity::Act(Level* level)
{
}

void Entity::SetAimData(int start, int end, float maxTime)
{
	startFrame = start;
	endFrame = end;
	maxAnimTime = maxTime;
	curAnimFrame = startFrame;
	stayEndFrame = false;
	curTime = 0;
}

void Entity::TakeDamage(int dmg)
{
	hp -= dmg;
	Stop();
	if (type == EntityType::PLAYER)
	{
		entityObserver.NotifyDamageTaken(this, dmg, D2D1::ColorF(D2D1::ColorF::Red));
	}
	else
	{
		entityObserver.NotifyDamageTaken(this, dmg, D2D1::ColorF(D2D1::ColorF::White));
	}
	if (hp <= 0)
	{
		hp = 0;
		//isActive = false;
		curState = EntityState::DEAD; // 몬스터 죽은거 확인용
		SetState(EntityState::DEAD);
		entityObserver.NotifyDeath(this);
	}
}

void Entity::TakeExp(int exp)
{
	this->exp += exp;
	if (this->exp / level == 20)
	{
		this->exp = this->exp % level;
		LevelUp();
	}
}

void Entity::LevelUp()
{
	level++;
	attackDmg *= 1.5;
	maxHp += 10;
	hp += 10;
}
