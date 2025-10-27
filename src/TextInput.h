#pragma once

#include "TextBox.h"

namespace fc {
	class TextInput : public TextBox {
	private:
		int32_t _cursorPosition = 0;
		int cursorBlinkCounter = 0;
		bool _showCursor = false;
	public:
		TextInput(alignment::ElementAlignment alignment, glm::vec4 backgroundColor, glm::vec4 textColor, float textSize, ShapeRenderer2D& boxRenderer, TextRenderer& textRenderer) :
			TextBox(alignment, backgroundColor, textColor, textSize, boxRenderer, textRenderer) 
		{
			_text.verticallyFlexible = false;
			_text.horisontallyFlexible = false;
			focusable = true;
		}

		virtual void onLetterTyped(Input& input, input::UnicodeCodePoint letter) override {
			std::string str = _text.getText();
			std::string newStr = str.substr(0, _cursorPosition) + static_cast<char>(letter) + str.substr(_cursorPosition);
			setText(newStr);
			_cursorPosition++; 
		}

		virtual void render(const Window& window, time::Duration delta) override {
			TextBox::render(window, delta);
			// Draw cursor
			const int interval = 60; // Blink every 30 frames
			if (cursorBlinkCounter % interval < interval / 2 && _showCursor) {
				const auto pos = cursorPixelPos();
				const float lineHeight = _text._renderer.lineHeight(_text._textSize);
				const float relY = -0.2f * lineHeight;
				_background.m_Renderer.rect(window, glm::vec3(pos, 0) + glm::vec3(0, relY, 0), { 3, lineHeight }, _text._textColor);
			}
			cursorBlinkCounter++;
		}

		virtual void onKeyboardEvent(Input& input, input::KeyboardEvent event) override {
			if (event.action == input::KeyAction::Press || event.action == input::KeyAction::Repeat) {
				switch (event.key) {
					case GLFW_KEY_BACKSPACE:
						if(_cursorPosition == 0) break; 
						if (!_text.getText().empty()) {
							std::string str = _text.getText();
							std::string newStr = str.substr(0, _cursorPosition - 1) + str.substr(_cursorPosition);
							_text.setText(newStr);
							_cursorPosition--;
							clampCursor();
						}
						break;

					case GLFW_KEY_DELETE:
						if (!_text.getText().empty() && _cursorPosition < _text.getText().size()) {
							std::string str = _text.getText();
							std::string newStr = str.substr(0, _cursorPosition) + str.substr(_cursorPosition + 1);
							_text.setText(newStr);
						}
						break;

                    case GLFW_KEY_ENTER: {
                        std::string str = _text.getText();
                        std::string newStr = str.substr(0, _cursorPosition) + '\n' + str.substr(_cursorPosition);
                        setText(newStr);
						_cursorPosition++;
						clampCursor();
                        break;
                    }

					case GLFW_KEY_LEFT:
						_cursorPosition--;
						clampCursor();
						break;

					case GLFW_KEY_RIGHT:
						_cursorPosition++;
						clampCursor();
						break;

					case GLFW_KEY_UP: {
						// Fix: If cursor is at end of text and not after newline, treat as last line
						if (_cursorPosition == _text.getText().size() && !_text.getText().empty() && _text.getText().back() != U'\n') {
							// Move cursor to same column in previous line
							uint32_t line = static_cast<uint32_t>(_text.lines().size() - 1);
							if (line == 0) break;
							uint32_t charIndex = static_cast<uint32_t>(_text.lines().back().first.length());
							uint32_t prevLineLen = static_cast<uint32_t>(_text.lines()[line - 1].first.length());
							_cursorPosition -= charIndex;
							if (prevLineLen >= charIndex) {
								_cursorPosition -= prevLineLen - charIndex;
							} else {
								_cursorPosition -= prevLineLen;
							}
							clampCursor();
							break;
						}

						const uint32_t line = cursorLineNumber();
						if (line == 0) break;
						
						const uint32_t charIndex = cursorPosOnLine();
						_cursorPosition -= charIndex;
						
						const std::string& prevLine = _text.lines()[line - 1].first;
						if(prevLine.length() - 1 /* -1 because of the newline */>= charIndex) {
							_cursorPosition -= prevLine.length() - charIndex;
						} else {
							_cursorPosition--; // For the newline
						}
						clampCursor();
						break;
					}
					
					case GLFW_KEY_DOWN: {
						const uint32_t line = cursorLineNumber();
						if (line >= _text.lines().size() - 1) break;
						
						const uint32_t charIndex = cursorPosOnLine();
						_cursorPosition += _text.lines()[line].first.length() - charIndex;
						
						const std::string& lineUnder = _text.lines()[line + 1].first;
						if(lineUnder.length() - 1 >= charIndex) {
							_cursorPosition += charIndex;
						} else {
							_cursorPosition += lineUnder.size();
						}
						clampCursor();
						break;
					}

					default:
						break;
				}
			}
		}

		virtual void onFocusAquired() override {
			_showCursor = true;
			cursorBlinkCounter = 0;
		}
		virtual void onFocusLost() override {
			_showCursor = false;
		}

		// Get the line the cursor is currently on
		uint32_t cursorLineNumber() const {
			const auto& lines = _text.lines();
			if(lines.size() <= 1) return 0;

			uint32_t line = 0;
			uint32_t characterIndex = 0;
			for (size_t lineNum = 0; lineNum < lines.size(); lineNum++) {
				const std::string& l = lines[lineNum].first;
				characterIndex += l.length();
				if (characterIndex > _cursorPosition) {
					break;
				} else if(characterIndex == _cursorPosition && !l.empty() && l.back() == U'\n' && lineNum != lines.size() -1) {
					line++;
					break;
				}
				line++;
			}
			return line;
		}

		uint32_t cursorPosOnLine() const {
			auto line = cursorLineNumber();
			uint32_t numChars = 0;
			for (uint32_t i = 0; i < line; i++) {
				numChars += _text.lines()[i].first.length();
			}
			return _cursorPosition - numChars;
		}

        // Get the position of the cursor on screen
        glm::vec2 cursorPixelPos() const {
            uint32_t line = cursorLineNumber();
            uint32_t col = cursorPosOnLine();

            const auto& lines = _text.lines();
            std::string lineText;
            if (line < lines.size()) {
                lineText = lines[line].first.substr(0, col);
            }

            // Fix: If cursor is at the very end of the text and not after a newline, keep it on the last line
            if (line >= lines.size() && !_text.getText().empty()) {
                // Only move to imaginary line if the last character is a newline
                if (_text.getText().back() != U'\n') {
                    line = static_cast<uint32_t>(lines.size() - 1);
                    col = static_cast<uint32_t>(lines.back().first.length());
                    lineText = lines.back().first;
                }
            }

            const glm::vec2 pos = getPixelPosition();

            // Calculate X: width of text before cursor on this line
            const float x = pos.x + _text._renderer.width(lineText, _text._textSize);

            // Calculate Y: top + line * lineHeight
            const float lineHeight = _text._renderer.lineHeight(_text._textSize);
            const float y = pos.y + getPixelSize().y - (line + 1) * lineHeight;

            return glm::vec2(x, y);
        }

		void clampCursor() {
			if(_cursorPosition < 0) {
				_cursorPosition = 0;
			} else if(_cursorPosition > _text.getText().length()) {
				_cursorPosition = _text.getText().length();
			}
		}
	};
}