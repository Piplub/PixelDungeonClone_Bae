﻿#pragma once
#include "Equipment.h"
class Weapon : public Equipment
{
private:


public:
	Weapon(FPOINT position, string name);
	virtual ~Weapon();

	void Render(HDC hdc) override;
};

