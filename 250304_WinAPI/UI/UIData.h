#pragma once

class D2DImage;

struct UIInventorySlotData
{
    //int itemId = 0;
    string name = "";
    D2DImage* icon = nullptr;
    int quantity = 0;
    int enhancement = 0;
};

struct PlayerStatData {
    std::wstring title{L"스탯창"};
    int attack = 0;
    int hp = 0;
    int maxHp = 0;
    int exp = 0;
    int maxExp = 0;
    int def = 0;
    int level = 0;
};
