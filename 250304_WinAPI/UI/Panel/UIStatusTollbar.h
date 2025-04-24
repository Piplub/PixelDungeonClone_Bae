#pragma once
#include "../Core/UIContainer.h"
#include "../Button/UIImageTextButton.h"
#include "../Text/UITextBox.h"
#include "../Util/UIResourceSubManager.h"
#include "../../Observer/IEntityObserver.h"
#include "../Bar/UIPlayerBar.h"
#include "../VisualStyle.h"
#include "Player.h"

class UIStatusToolbar : public UIContainer, public IEntityObserver {
private:
    UIImageTextButton* statusBtn = nullptr;
    UIPlayerBar* hpBar = nullptr;
    UIPlayerBar* expBar = nullptr;
    UITextBox* levelText = nullptr;

    ImageStyle hpBarBgStyle;
    BarStyle hpBarStyle;
    ImageStyle expBarBgStyle;
    BarStyle expBarStyle;
    TextStyle textStyle;
    

public:
    void OnEntityDamageTaken(DamageTakenData data) override
    {
        SetHP(data.entity->GetHP(), data.entity->GetMaxHP());
        SetEXP(data.entity->GetExp(), data.entity->GetMaxExp());
        SetLevel(data.entity->GetLevel());
    }
    
    void Init(const D2D1_RECT_F& rect = { 0.0f, 610.0f, 398.0f, 711.0f }) override
    {
        SetRect(rect);
        UIResourceSubManager::Preload_StatusToolbar();
        SetStyle();
        
        AddHPBar();
        AddEXPBar();
        AddCharacterIcon();
        AddLevelText();
    }

    void UpdateStat(Player* data)
    {
        if (!data) return;
        
        SetHP(data->GetHP(), data->GetMaxHP());
        SetEXP(data->GetExp(), data->GetMaxExp());
        SetLevel(data->GetLevel());
    }

    void SetHP(int current, int max) {
        if (hpBar) {
            hpBar->SetMaxValue((int)max);
            hpBar->SetValue((int)current);
        }
    }

    void SetEXP(int current, int max) {
        if (expBar) {
            expBar->SetMaxValue((int)max);
            expBar->SetValue((int)current);
        }
    }

    void SetLevel(int level) {
        if (levelText) {
            levelText->SetText(L"Lv. " + std::to_wstring(level));
        }
    }

    void SetStatusButtonEvent(const std::function<void()>& onClick)
    {
        if (statusBtn)
        {
            statusBtn->SetOnClick(onClick);
        }
    }

private:
    void AddHPBar() {
        hpBar = new UIPlayerBar();
        hpBar->Init({ 75, 37, 398.5f, 69.0f });
        hpBar->SetStyle(hpBarBgStyle, hpBarStyle, textStyle);
        hpBar->SetMaxValue(20);
        hpBar->SetValue(20);
        AddChild(hpBar);
    }

    void AddEXPBar() {
        expBar = new UIPlayerBar();
        expBar->Init({ 73, 69, 396.5f, 101.13f });
        expBar->SetStyle(expBarBgStyle, expBarStyle, textStyle);
        expBar->SetMaxValue(10);
        expBar->SetValue(10);
        AddChild(expBar);
    }

    void AddCharacterIcon() {
        UIIconStyle charIconStyle =
        {
 {D2DImageManager::GetInstance()->FindImage("status_character_bg"), },
{D2DImageManager::GetInstance()->FindImage("status_character_ico"), { 17.32f, 9.0f, 17.32f, 9.0f }},
        };
            
        statusBtn = UIHelper::ApplyIconStyle(this, { 0, 0, 74.25f, 74.25f }, charIconStyle);
    }

    void AddLevelText() {

        UIButtonStyle style = {
            { D2DImageManager::GetInstance()->FindImage("status_level") },
            { L"pixel", 16.0f, D2D1::ColorF(D2D1::ColorF::White), true,
              DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER }
        };
        levelText = new UITextBox();
        levelText->Init(style, L"Lv. 1", { 0, 74, 74.25f, 98.75f });
        AddChild(levelText);
    }
    
    void SetStyle()
    {
        hpBarBgStyle = ImageStyle
            {
                D2DImageManager::GetInstance()->FindImage("status_hp_bg"), 1.0f
            };
        hpBarStyle = BarStyle
            {
                ImageStyle
                {D2DImageManager::GetInstance()->FindImage("status_hp_bar"), 1.0f},
                ImageStyle
                {nullptr}
            };
        expBarBgStyle = ImageStyle
            {
                D2DImageManager::GetInstance()->FindImage("status_exp_bg"), 1.0f
            };
        expBarStyle = BarStyle
            {
                ImageStyle
                {D2DImageManager::GetInstance()->FindImage("status_exp_bar"), 1.0f},
                ImageStyle
                {nullptr}
            };
        textStyle = TextStyle
            {
                L"pixel", 14.0f, D2D1::ColorF(D2D1::ColorF::White),
                true, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_CENTER,
            };
    }
};
