#pragma once
#include "Text.h"
#include "ColoredRect.h"
#include "alignment/MirrorWidth.h"
#include "alignment/MirrorHeight.h"

namespace fc {
	
	class TextBox : public Container {
	public:
		Text& _text;
		ColoredRect& _background;
	public:
		TextBox(alignment::ElementAlignment alignment, glm::vec4 backgroundColor, glm::vec4 textColor, float textSize, ShapeRenderer2D& boxRenderer, TextRenderer& textRenderer) 
			: Container(alignment),
			_background(createChild<ColoredRect>(alignment::ElementAlignment(), backgroundColor, boxRenderer)),
			_text(createChild<Text>(alignment, textColor, textSize, textRenderer))
		{
			this->alignment.setX(alignment.x);
			this->alignment.setY(alignment.y);
			this->alignment.setWidth(alignment::MirrorWidth(_text));
			this->alignment.setHeight(alignment::MirrorHeight(_text));

			_text.alignment.setX(alignment::Pixels(0));
			_text.alignment.setY(alignment::Pixels(0));
			
            _text.wrapTightly = true;
		}

		virtual void render(const Window& window, time::Duration delta) override {
			_background.render(window, delta);
			_text.render(window, delta);
		}

		void setText(const std::string& text) {
			_text.setText(text);
		}

	};

}
