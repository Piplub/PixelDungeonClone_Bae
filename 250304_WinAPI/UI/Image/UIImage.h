﻿#pragma once
#include "UIBaseImage.h"
#include "../../D2DImage.h"
#include "../VisualStyle.h"

class UIImage : public UIBaseImage {
protected:
    ImageStyle style;
public:
    void SetStyle(const ImageStyle& s) {
        style = s;
        SetAlpha(s.alpha);
        SetTint(s.tint);

        D2D1_RECT_F rect = GetLocalRect();
        // 패딩 적용
        rect.left   += style.padding.left;
        rect.top    += style.padding.top;
        rect.right  -= style.padding.right;
        rect.bottom -= style.padding.bottom;

        SetRect(rect);
    }

    void SetImage(D2DImage* image, int frameIdx = 0, int frameIdy = 0) {
        style.image = image;
        style.frameIdx = frameIdx;
        style.frameIdy = frameIdy;
    }

    POINT GetImgSize() {
        if (style.image)
            return { style.image->GetWidth(), style.image->GetHeight() };
        return { 0, 0 };
    }

    void SetAlignCenter()
    {
        if (!parent) return;

        FPOINT parentCenter = { parent->GetWidth() * 0.5f, parent->GetHeight() * 0.5f };
        FPOINT halfSize = { GetWidth() * 0.5f, GetHeight() * 0.5f };
        SetRect({parentCenter.x - halfSize.x, parentCenter.y - halfSize.y, parentCenter.x + halfSize.x, parentCenter.y + halfSize.y});
    }

    void Render(ID2D1HwndRenderTarget* rt) override {
        if (!isVisible || !isActive || !style.image || !rt) return;

        D2D1_RECT_F rect = GetScaledDrawRect();
        FPOINT ws = GetWorldScale();

        style.image->RenderFrameScale(
            rect.left, rect.top, ws.x, ws.y, style.frameIdx, style.frameIdy,
            0.0f, false, false, alpha
        );

        // // 🔸 출력 디버그용 외곽선
        // ID2D1SolidColorBrush* debugBrush = nullptr;
        // rt->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Red), &debugBrush);
        // rt->DrawRectangle(rect, debugBrush, 1.0f);
        // if (debugBrush) debugBrush->Release();
    }
};
