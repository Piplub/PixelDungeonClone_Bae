﻿#include "GameObject.h"
WCHAR GameObject::nowFilePath[MAX_PATH] = L"";

HRESULT GameObject::Init()
{
	return S_OK;
}

void GameObject::Release()
{
}

void GameObject::Update()
{
}

void GameObject::Render(HDC hdc)
{
}

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}
