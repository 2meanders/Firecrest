#pragma once

#include "VerticalPositioning.h"
#include "ShapeRenderer2D.h"
#include "core/Maths.h"
#include <algorithm>
#include "gl/RenderRegion.h"

namespace fc {

	class Scrollable : public VerticalPositioning {
	private:
		float _verticalScrollOffset = 0.0f;
		Element& _flexElement;
		ShapeRenderer2D& _renderer;
		bool stickingToBottom = false; // Whether we are currently sticking to the bottom
	public:
		float scrollSpeed = 40.0f;
		bool clampScroll = true; // If true, the scroll will be clamped to the content height
	public:
		Scrollable(alignment::ElementAlignment alignment, ShapeRenderer2D& renderer)
			: VerticalPositioning(alignment, VerticalPositioning::Direction::TopToBottom),
			_flexElement(createChild<Element>(alignment::ElementAlignment().setHeight(alignment::Pixels(0.0f)))),
			_renderer(renderer) {}
	
		virtual void render(const Window& window, time::Duration delta) override {
			if(stickingToBottom) {
				goToBottom();
			}

			gl::RenderRegion::push(getPixelRectangle(), gl::RenderRegion::Mode::Scissor);

			const auto elSize = getPixelSize();
			const auto elPos = getPixelPosition();

			Container::render(window, delta);

			// Draw the scrollbar
			if (clampScroll) {
				const float width = 6.0f;
				const float padding = 2.0f;
				const float totalHeight = contentHeight();
				const float visualHeight = elSize.y - padding / 2;

				const float height = std::clamp(visualHeight * (visualHeight / totalHeight), 30.0f, visualHeight);
				const float minOffset = std::min(0.0f, elSize.y - totalHeight);
				const float y = maths::map(_verticalScrollOffset, minOffset, 0.0f, 0.0f, visualHeight - height);
				_renderer.roundedRect(window, elPos + glm::vec2(elSize.x - width - padding, y), { width, height }, { 0.5, 0.5, 0.5, 0.75 }, width * 0.5f, 12);
			}
			
			gl::RenderRegion::pop();
		}

		// Positive brings the content upwards, negative brings it downwards
		void scroll(float pixels) {
			_verticalScrollOffset += pixels * scrollSpeed;
			
			if (stickingToBottom && pixels > 0) {
				stickingToBottom = false;
			}
			else if (!stickingToBottom && getPixelSize().y - contentHeight() >= _verticalScrollOffset) {
				stickingToBottom = true;
			}

			if (clampScroll) {
				float minOffset = std::min(0.0f, getPixelSize().y - contentHeight());
				_verticalScrollOffset = std::clamp(_verticalScrollOffset, minOffset, 0.0f);
			}

			_flexElement.alignment.setHeight(alignment::Pixels(_verticalScrollOffset));
		}

		// Scrolls to the top
		void goToTop() {
			_verticalScrollOffset = 0.0f;
		}

		// Scrolls to the bottom
		void goToBottom() {
			goToTop();
			scroll(getPixelSize().y - contentHeight());
		}

		virtual void onScroll(Input& input, input::ScrollEvent event) override {
			scroll(event.offset.y);
		} 

		float contentHeight() const {
			float totalHeight = 0;
			for (const auto& child : children()) {
				totalHeight += child->getPixelSize().y;
			}
			return totalHeight - _flexElement.getPixelSize().y;
		}

		float getScrollOffset() const {
			return _verticalScrollOffset;
		}
	};
}