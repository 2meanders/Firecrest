#pragma once

#include "Text.h"
#include "Container.h"
#include "VerticalCenterer.h"
#include "HorisontalCenterer.h"
#include "RoundedColoredRect.h"

namespace fc {
    class Button : public Container {
    public:
        using Callback = std::function<void()>;
    public:
        glm::vec4 color;
        glm::vec4 hoverColor;
        glm::vec4 clickColor;
        Callback onClickCallback;
        
        RoundedColoredRect& background;
        Text& text;
    public:
        Button(alignment::ElementAlignment alignment, glm::vec4 color, glm::vec4 hoverColor, glm::vec4 clickColor, glm::vec4 textColor, float textSize, Callback onClickCallback, ShapeRenderer2D& shapeRenderer, TextRenderer& textRenderer)
            : Container(alignment)
            , color(color)
            , hoverColor(hoverColor)
            , clickColor(clickColor)
            , onClickCallback(onClickCallback)
            , background(createChild<RoundedColoredRect>(alignment::ElementAlignment(), color, 15.0f, shapeRenderer))
            , text(createChild<VerticalCenterer>().createChild<HorisontalCenterer>().createChild<Text>(alignment::ElementAlignment(), textColor, textSize, textRenderer))
        {
            focusable = true;
            text.wrapTightly = true;
        }

        virtual void onMouseMotionEvent(Input& input, input::MouseMotionEvent event) override {
            switch(event.action) {
            case input::MouseMotionAction::Enter:
                background.m_Color = hoverColor;
                break;
            case input::MouseMotionAction::Exit:
                background.m_Color = color;
                break;
            };
        }

        virtual void onMouseButtonEvent(Input& input, input::MouseButtonEvent event) override {
            if(event.button == input::MouseButton::Left) {
                switch(event.action) {
                case input::MouseButtonAction::Press:
                    background.m_Color = clickColor;
                    onClickCallback();
                    break;
                case input::MouseButtonAction::Up:
                    background.m_Color = hoverColor;
                    break;
                }
            }
        }

    private:
        static constexpr glm::vec4 calculateTextColor(glm::vec4 backgroundColor) {
            const float luminance = 0.299f * backgroundColor.r + 0.587f * backgroundColor.g + 0.114f * backgroundColor.b;
            if(luminance < 0.726) {
                return { 1.0f, 1.0f, 1.0f, 1.0f };
            } else {
                return { 0.0f, 0.0f, 0.0f, 1.0f };
            }
        }
    };
}