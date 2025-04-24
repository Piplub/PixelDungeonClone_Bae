﻿#pragma once
#include "UIValueBar.h"
#include "../Core/UIContainerBase.h"
#include "../VisualStyle.h"
#include "../Util/UIHelper.h"
#include "../Text/UIText.h"

class UIMopBar : public UIContainerBase, public IEntityObserver {
protected:
    ImageStyle bgStyle;
    BarStyle barStyle;
    
    UIValueBar* bar{nullptr};

    Camera* camera{nullptr};
    Entity* target{nullptr};

    D2D1_SIZE_F offset = {0.f, -30.f};

public:
    void OnEntityDamageTaken(DamageTakenData data) override;
    
    void Init(const D2D1_RECT_F& layout = {0,0,49,8},
        const ImageStyle& bs = {nullptr},
        const BarStyle& s = {{nullptr},{nullptr}},
        float max = 10.0f);
    void SetStyle(const ImageStyle& bgs, const BarStyle& s);
    void SetValue(float value);
    void SetMaxValue(float max);
    void SetTarget(Entity* target, Camera* cam);

    void UpdateSetTextValue() const;

    void Update(float deltaTime) override;
    void Render(ID2D1HwndRenderTarget* renderTarget) override;

protected:
    void ChaseTargetPos();
};

void UIMopBar::OnEntityDamageTaken(DamageTakenData data)
{
    if (!data.entity) return;

    target = data.entity;

    auto maxHP = target->GetMaxHP();
    auto curHP = target->GetHP();

    if (maxHP != target->GetMaxHP())
    {
        SetMaxValue(static_cast<float>(maxHP));
    }
    SetValue(static_cast<float>(curHP));
    if (curHP != maxHP)
    {
        SetActive(true);
        UIManager::GetInstance()->SendLog(L"Attach Target : " + to_wstring(bar->GetFillPercent()), D2D1::ColorF(D2D1::ColorF::ColorF::Green));
    }
}

void UIMopBar::Init(const D2D1_RECT_F& layout, const ImageStyle& bgs,
                const BarStyle& s, float max)
{
    SetRect(layout);
    bar = UIHelper::ApplyBarStyle(this, GetSizeRect(), barStyle);
    SetStyle(bgs, s);
    SetMaxValue(max);
    SetActive(false);
}

void UIMopBar::SetStyle(const ImageStyle& bgs, const BarStyle& s)
{
    this->bgStyle = bgs;
    this->barStyle = s;
    bar->SetStyle(s);
}

void UIMopBar::SetValue(float value)
{
    bar->SetValue(value);
}

void UIMopBar::SetMaxValue(float max) {
    bar->SetMaxValue(max);
}

inline void UIMopBar::SetTarget(Entity* target, Camera* cam)
{
    if (!target || !cam) return;
    this->target = target;
    this->camera = cam;
}

void UIMopBar::Update(float deltaTime)
{
    if (!isActive) return;
    
    bar->Update(deltaTime);
    ChaseTargetPos();
}

void UIMopBar::Render(ID2D1HwndRenderTarget* rt)
{
    if (!isActive || !isVisible) return;
    
    D2D1_RECT_F rect = GetScaledDrawRect();
    FPOINT ws = GetWorldScale();
    
    // 🔹 배경
    if (bgStyle.image)
        bgStyle.image->RenderFrameScale(rect.left, rect.top, ws.x, ws.y,
            0, 0, 0.0f, false, false, bgStyle.alpha);

    bar->Render(rt);

    // 🔸 출력 디버그용 외곽선
    ID2D1SolidColorBrush* debugBrush = nullptr;
    rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue), &debugBrush);
    rt->DrawRectangle(rect, debugBrush, 1.0f);
    if (debugBrush) debugBrush->Release();
}

void UIMopBar::ChaseTargetPos()
{
    if (!target || !camera) return;

    auto curPos = target->GetPosition();
    FPOINT scrPos = {camera->ConvertToRendererX(curPos.x), camera->ConvertToRendererY(curPos.y)};
    auto ws = GetWorldScale();
    
    float halfWidth = GetWidth() * 0.5f;
    float halfHeight = GetHeight() * 0.5f;
    FPOINT scaleOffset = {offset.width * ws.x, offset.height * ws.y};
    FPOINT startPos = {scrPos.x - halfWidth * ws.x + scaleOffset.x, scrPos.y - halfHeight * ws.y + scaleOffset.y};

    D2D1_RECT_F setRect = {startPos.x, startPos.y,
    startPos.x + GetWidth(), startPos.y  + GetHeight()};

    // 배치 연산 줄이기
    // // 위치 변동 비교
    auto curHPRect = GetWorldRect();
    if (setRect.left == curHPRect.left && setRect.top == curHPRect.top
        && setRect.right == curHPRect.right && setRect.bottom == curHPRect.bottom)
    {
        return;
    }
    SetWorldRect(setRect);

}
