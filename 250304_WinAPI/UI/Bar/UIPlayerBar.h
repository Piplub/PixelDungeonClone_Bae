#pragma once
#include "UIValueBar.h"
#include "../Core/UIContainerBase.h"
#include "../VisualStyle.h"
#include "../Util/UIHelper.h"
#include "../Text/UIText.h"

class UIPlayerBar : public UIContainerBase {
protected:
    ImageStyle bgStyle;
    BarStyle barStyle;
    TextStyle textStyle;
    
    UIValueBar* bar{nullptr};
    UIText* valueText{nullptr};

public:
    void Init(const D2D1_RECT_F& layout = {0,0,0,0},
        const ImageStyle& bs = {nullptr},
        const BarStyle& s = {{nullptr},{nullptr}},
        const TextStyle& ts = {},
        float max = 10.0f);
    void SetStyle(const ImageStyle& bgs, const BarStyle& s, const TextStyle& ts);
    void SetValue(float value);
    void SetMaxValue(float max);

    void UpdateSetTextValue() const;

    void Update(float deltaTime) override;
    void Render(ID2D1HwndRenderTarget* renderTarget) override;
};

inline void UIPlayerBar::Init(const D2D1_RECT_F& layout, const ImageStyle& bgs, const BarStyle& s, const TextStyle& ts, float max) {
    SetRect(layout);
    valueText = UIHelper::ApplyTextStyle(this, GetSizeRect(), textStyle);
    bar = UIHelper::ApplyBarStyle(this, GetSizeRect(), barStyle);
    SetStyle(bgs, s, ts);
    SetMaxValue(max);
    UpdateSetTextValue();
}

inline void UIPlayerBar::SetStyle(const ImageStyle& bgs, const BarStyle& s, const TextStyle& ts)
{
    this->bgStyle = bgs;
    this->barStyle = s;
    this->textStyle = ts;
    bar->SetStyle(s);
    valueText->SetStyle(ts);
}

inline void UIPlayerBar::SetValue(float value) {
    bar->SetValue(value);
    UpdateSetTextValue();
}

inline void UIPlayerBar::SetMaxValue(float max) {
    bar->SetMaxValue(max);
    UpdateSetTextValue();
}

inline void UIPlayerBar::UpdateSetTextValue() const
{
    int curValue = bar->GetValue();
    int maxValue = bar->GetMaxValue();
    wstring text = to_wstring(curValue) + L" / " + to_wstring(maxValue);
    valueText->SetText(text);
}

inline void UIPlayerBar::Update(float deltaTime) {
    if (!isActive || !isVisible) return;
    bar->Update(deltaTime);
    UpdateSetTextValue();
}

inline void UIPlayerBar::Render(ID2D1HwndRenderTarget* rt) {
    if (!isActive || !isVisible) return;
    D2D1_RECT_F rect = GetScaledDrawRect();
    FPOINT ws = GetWorldScale();

    // 🔹 배경
    if (bgStyle.image)
        bgStyle.image->RenderFrameScale(rect.left, rect.top, ws.x, ws.y,
            0, 0, 0.0f, false, false, bgStyle.alpha);

    bar->Render(rt);

    if (valueText)
    {
        valueText->Render(rt);
    }
}
