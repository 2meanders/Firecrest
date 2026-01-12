#pragma once
#include "TextRenderer.h"
#include <cctype>
#include "core/StringUtils.h"
#include "Element.h"
#include "gl/RenderRegion.h"
#include "Container.h"


namespace fc {

    class Text : public Element {
    public:
        enum class WrapMode {
            NoWrap,     // Text will not wrap, it will be cut off if too long
            Wrap,       // Text will wrap to the next line if too long
            // WordWrap    // Text will wrap at word boundaries if too long
        };
    public:
        glm::vec4 _textColor;
        float _textSize;
        WrapMode _wrapMode;
        
        alignment::AlignmentFunction defaultWidth;
        alignment::AlignmentFunction defaultHeight;
    private:
        std::string _text;
        // vector<pair<lineText, position relative to self>>
        std::vector<std::pair<std::string, glm::vec2>> _linesCache;

        glm::vec2 _lastParentSize = { -1, -1 };
    public:
        TextRenderer& _renderer;

        bool wrapTightly = false;
    public:
        Text(alignment::ElementAlignment alignment, glm::vec4 textColor, float textSize, TextRenderer& textRenderer) : Element(alignment), _textSize(textSize), _renderer(textRenderer), _wrapMode(WrapMode::Wrap), _textColor(textColor), defaultWidth(alignment.width), defaultHeight(alignment.height) {}

        virtual void render(const Window& window, time::Duration delta) override {
            const glm::vec2 parentSize = parent().getPixelSize();
            if (parentSize != _lastParentSize) {
                buildLinesCache();
                _lastParentSize = parentSize;
            } 
            if (_text.empty()) return;

            gl::RenderRegion::push(getPixelRectangle(), gl::RenderRegion::Mode::Scissor);
            const glm::vec2 pos = getPixelPosition();
            const glm::vec2 size = getPixelSize();
            const float top = pos.y + size.y;
            const float bottom = pos.y;

            for (auto& line : _linesCache) {
                glm::vec2 linePos = line.second + pos;
                float lineY = linePos.y;

                // Only render if line is within vertical bounds
                if (lineY <= top && lineY >= bottom) {
                    _renderer.renderText(window, line.first, glm::vec3(linePos, 0), _textSize, _textColor);
                }
            }

            gl::RenderRegion::pop();

        }

        void buildLinesCache() {
            _linesCache.clear();
            const std::string string = _text;
            const float maxLineWidth = this->defaultWidth(parent().getPixelSize().x, parent().getPixelSize().y);
			const float lineHeight = _renderer.lineHeight(_textSize);
            
            bool isWrapped = false;

            if (_wrapMode == WrapMode::NoWrap) {
                _linesCache.push_back({ string, { 0, -lineHeight } });
            } else if (_wrapMode == WrapMode::Wrap) {
                float yPos = 0;

				auto segments = utils::strsplit(string, "\n", true);
                for (auto& str : segments) {
                    if(str.empty()) {
                        yPos -= lineHeight;
                        _linesCache.push_back({str, {0, yPos}});
                        continue;
                    }

                    size_t charIndex = 0;
                    while (!str.empty()) {
                        const float lineWidth = _renderer.width(str.substr(0, charIndex), _textSize);
						const bool atEnd = charIndex > str.length();
						const bool isTooWide = lineWidth > maxLineWidth;
                        if (atEnd || isTooWide) {
                            const size_t count = charIndex <= 1 ? charIndex : charIndex - 1;
                            const std::string line = str.substr(0, count);

                            str = str.substr(count);
                        
                            yPos -= lineHeight;
                            _linesCache.push_back({ line, {0, yPos} });

                            charIndex = 0;
                        }
                        if (isTooWide) {
                            isWrapped = true;
                        }
                        charIndex++;
                    }
                }
            }
            
            if(!isWrapped && wrapTightly) {
                // Find widest line width
                float widestLineWidth = _renderer.width(std::max_element(_linesCache.begin(), _linesCache.end(),
                    [&](const std::pair<std::string, glm::vec2>& a, const std::pair<std::string, glm::vec2>& b) {
                        return _renderer.width(a.first, _textSize) < _renderer.width(b.first, _textSize);
                    })->first, _textSize);

                
                if(widestLineWidth <= maxLineWidth) {
                    alignment.setWidth(alignment::Pixels(widestLineWidth));
                } else {
                    alignment.setWidth(defaultWidth); 
                }
            } else {
                alignment.setWidth(defaultWidth);
            }
            
            if (wrapTightly) {
                const float height = lineHeight * _linesCache.size() - _renderer.descenderHeight(_textSize);
                alignment.setHeight(alignment::Pixels(height));
            } else {
                alignment.setHeight(defaultHeight);
            }


            // Offset the positions
            const glm::vec2 basePos = glm::vec2(0, getPixelSize().y);
            for (auto& line : _linesCache) {
                line.second += basePos;
            }
        }

        // vector<pair<lineText, position>>
        std::vector<std::pair<std::string, glm::vec2>> lines() const {
            std::vector<std::pair<std::string, glm::vec2>> l = _linesCache;
            const glm::vec2 pos = getPixelPosition();
            for(auto& line : l) {
                line.second += pos;
            }
            return l;
        }

        const std::string& getText() const {
			return _text;
        }

        void setText(const std::string& text) {
            _text = text;
            buildLinesCache();
        }

    };

}
