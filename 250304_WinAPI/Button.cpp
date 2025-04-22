﻿#include "Button.h"

#include "D2DImage.h"
#include "D2DImageManager.h"
// #include "Image.h"

HRESULT Button::Init(int posX, int posY)
{
	state = ButtonState::None;
	image = D2DImageManager::GetInstance()->
		AddImage("버튼", L"Image/button.bmp", 1, 2);
	if (image == nullptr)
	{
		return E_FAIL;
	}

	pos.x = posX;
	pos.y = posY;

	rc.left = pos.x - image->GetWidth() / 2;
	rc.top = pos.y - image->GetHeight() / 2;
	rc.right = rc.left + image->GetWidth();
	rc.bottom = rc.top + image->GetHeight();

	buttonFunc = nullptr;

    return S_OK;
}

void Button::Release()
{
}

void Button::Update()
{
	if (PtInRect(&rc, g_ptMouse))
	{
		if (MouseManager::GetInstance()->GetIsDragging(MOUSE_LEFT) == true)
		{
			state = ButtonState::Down;
		}
		else if (state == ButtonState::Down &&
			MouseManager::GetInstance()->GetClickValueUsed()==false)
		{
			state = ButtonState::Up;
			// 기능 수행
			// TilemapTool::Save()
			//if (buttonFunc && obj)	obj->buttonFunc();
			if (buttonFunc) buttonFunc();
		}
	}
	else
	{
		state = ButtonState::None;
	}
}

void Button::Render(HDC hdc)
{
	switch (state)
	{
	case ButtonState::None:
	case ButtonState::Up:
		image->RenderFrame( pos.x, pos.y, 0, 0);
		break;
	case ButtonState::Down:
		image->RenderFrame(pos.x, pos.y, 0, 1);
		break;
	}
}
