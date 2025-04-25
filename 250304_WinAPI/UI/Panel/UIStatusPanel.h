#pragma once
#include "../Text/UIText.h"
#include "../Button/UIImageTextButton.h"
#include "../Util/UIHelper.h"
#include "../UIData.h"
#include "../VisualStyle.h"
#include "D2DImageManager.h"
#include "../../Observer/IEntityObserver.h"

class UIStatusPanel : public UIContainerBase, public IEntityObserver {
private:
    UIText* titleText = nullptr;
    UIImageTextButton* characterIcon = nullptr;
    UIText* lavelTexts[6] = {};
    UIText* infoTexts[6] = {};
    UIImageTextButton* tabButtons[3] = {};
    UIImageTextButton* infoButton = nullptr;

public:
    void Init(const D2D1_RECT_F& rect);
    void SetStatData(const PlayerStatData& data);

    void UpdatePlayerStat(Entity* entity)
    {
        SetStatData({
            L"스탯창",
            entity->GetAttDmg().max,
            entity->GetHP(),
            entity->GetMaxHP(),
            entity->GetExp(),
            entity->GetMaxExp(),
            entity->GetDefense(),
        });
    }
};




inline void UIStatusPanel::Init(const D2D1_RECT_F& rect = { 365, 87, 365 + 378, 87 +469 }) {
    SetRect(rect);


    NinePatchStyle bgNineStyle;
    bgNineStyle = UIHelper::CreateNinePatchFromSheet("chrome", "chrome", D2D1_SIZE_F {6.0f, 6.0f});
    auto bg = UIHelper::ApplyNinePathStyle(this, GetSizeRect(), bgNineStyle);

    // 아이콘
    characterIcon = UIHelper::ApplyIconStyle(this, {22, 22, 66, 66},
        UIIconStyle { {D2DImageManager::GetInstance()->FindImage("status_character_ico")}}
        );
    characterIcon->SetScale({1.3f, 1.3f});

    // 제목
    titleText = UIHelper::ApplyTextStyle(this, {120, 22, GetWidth() - 20, 100},
        {L"Algerian", 24.0f, D2D1::ColorF(D2D1::ColorF::Gold), true, DWRITE_TEXT_ALIGNMENT_LEADING, DWRITE_PARAGRAPH_ALIGNMENT_CENTER});
    titleText->SetText(L"스탯창");

    // info 버튼
    infoButton = UIHelper::ApplyIconStyle(this, {this->GetWidth() - 24, 4, this->GetWidth() - 4, 24},
        {{D2DImageManager::GetInstance()->FindImage("info_icon")}});

    // 스탯 라벨 텍스트
    auto verticlaContainer = new UIContainer();
    verticlaContainer->Init({22, 100, 155, GetHeight()});
    AddChild(verticlaContainer);
    verticlaContainer->SetLayout(new UIVerticalLayout({22, 0, false}));
    for (int i = 0; i < 6; ++i) {
        lavelTexts[i] = UIHelper::ApplyTextStyle(verticlaContainer, {0,  0, 133, 30},
            TextStyle{L"궁서체", 24.0f, D2D1::ColorF(D2D1::ColorF::White)});
    }
    lavelTexts[0]->SetText(L"공격력 ");
    lavelTexts[1]->SetText(L"체력");
    lavelTexts[2]->SetText(L"경험치");
    lavelTexts[3]->SetText(L"방어력");
    lavelTexts[4]->SetText(L"");
    lavelTexts[5]->SetText(L"");
    verticlaContainer->UpdateLayout();

    auto verticlaContainer2 = new UIContainer();
    verticlaContainer2->Init({155, 100, GetWidth(), GetHeight()});
    AddChild(verticlaContainer2);
    verticlaContainer2->SetLayout(new UIVerticalLayout({22, 0, false}));
    for (int i = 0; i < 6; ++i) {
        infoTexts[i] = UIHelper::ApplyTextStyle(verticlaContainer2, {0,  0, 133, 30},
            TextStyle{L"Algerian", 24.0f, D2D1::ColorF(D2D1::ColorF::White)});
    }
    infoTexts[0]->SetText(L"111 ");
    infoTexts[1]->SetText(L"111");
    infoTexts[2]->SetText(L"111");
    infoTexts[3]->SetText(L"111");
    infoTexts[4]->SetText(L"111");
    infoTexts[5]->SetText(L"111");
    verticlaContainer2->UpdateLayout();

    // // 탭 버튼
    // UIButtonStyle buttonStyle =
    //     {
    //         ImageStyle {D2DImageManager::GetInstance()->FindImage("inventory_bg")},
    //     TextStyle {L"pixel", 13.0f, D2D1::ColorF(D2D1::ColorF::White)},
    //     };
    // NinePatchStyle btnNinePatchStyle = UIHelper::CreateNinePatchFromSheet("chrome", "chrome", D2D1_SIZE_F {6.0f, 6.0f});
    //
    // for (int i = 0; i < 3; ++i) {
    //     D2D1_RECT_F rect = {12 + i * 72.0f, 130, 12 + i * 72.0f + 60, 150};
    //     tabButtons[i] = UIHelper::ApplyNinePatchButtonStyle(this, rect,
    //         buttonStyle, btnNinePatchStyle);
    //     UIHelper::SetButtonText(*tabButtons[i], L"탭", 0);
    //     tabButtons[i]->SetOnClick([]()
    //     {
    //         
    //     });
    // }
}

inline void UIStatusPanel::SetStatData(const PlayerStatData& data) {
    if (titleText) titleText->SetText(data.title);

    if (infoTexts[0]) infoTexts[0]->SetText(std::to_wstring(data.attack));
    if (infoTexts[1]) infoTexts[1]->SetText(std::to_wstring(data.hp) + L" / " + std::to_wstring(data.maxHp));
    if (infoTexts[2]) infoTexts[2]->SetText(std::to_wstring(data.exp) + L" / " + std::to_wstring(data.maxExp));
    if (infoTexts[3]) infoTexts[3]->SetText(std::to_wstring(data.def));
    if (infoTexts[4]) infoTexts[4]->SetText(L"");
    if (infoTexts[5]) infoTexts[5]->SetText(L"");
}
