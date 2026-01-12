#pragma once
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <vector>
#include <functional>
#include "fiv.hpp"
#include "input/RawEvents.h"

namespace fc {
	class Window;

	class Input {
	public:
		using MouseMotionEventCallback = std::function<void(input::RawMouseMotionEvent)>;
		using MouseButtonEventCallback = std::function<void(input::RawMouseButtonEvent)>;
		using ScrollCallback = std::function<void(input::RawScrollEvent)>;
		using KeyEventCallback = std::function<void(input::RawKeyboardEvent)>;
		using CharEventCallback = std::function<void(input::UnicodeCodePoint)>;
	private:
		GLFWwindow* _window = nullptr;
		void setWindow(GLFWwindow* window) {
			this->_window = window;
		}

		glm::vec2 _currMouse;
		glm::vec2 _lastMouse;
		bool _mouseButtons[GLFW_MOUSE_BUTTON_LAST];
		bool _mouseButtonsReleased[GLFW_MOUSE_BUTTON_LAST];
		bool _mouseButtonsJustPressed[GLFW_MOUSE_BUTTON_LAST];

		bool _keys[GLFW_KEY_LAST];
		bool _keysReleased[GLFW_KEY_LAST];
		bool _keysJustPressed[GLFW_KEY_LAST];

		std::vector<input::UnicodeCodePoint> _typedCharacters;
	private:
		std::vector<input::UnicodeCodePoint> _recentCharacterEvents;
		std::vector<input::RawKeyboardEvent> _recentKeyEvents;
		std::vector<input::RawMouseButtonEvent> _recentMouseButtonEvents;
		void characterCallback(GLFWwindow* window, unsigned int codepoint);
		void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		void mousePosCallback(GLFWwindow* window, double xPos, double yPos);
		void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		fiv::Vector<KeyEventCallback> _keyEventCallbacks;
		fiv::Vector<MouseMotionEventCallback> _mouseMotionEventCallbacks;
		fiv::Vector<MouseButtonEventCallback> _mouseButtonEventCallbacks;
		fiv::Vector<CharEventCallback> _charEventCallbacks;
		fiv::Vector<ScrollCallback> _scrollEventCallbacks;
	public:
		Input();

		void update();
		
		inline glm::vec2 mouse() const { return _currMouse; }
		inline glm::vec2 lastMouse() const { return _lastMouse; }
		inline glm::vec2 deltaMouse() const { return _currMouse - _lastMouse; }
		inline bool mouseMoved() const { return _currMouse != _lastMouse; }

		inline bool mouseIsPressed(int button) const { return _mouseButtons[button]; }
		inline bool mouseReleased(int button) const { return _mouseButtonsReleased[button]; }
		inline bool mouseJustPressed(int button) const { return _mouseButtonsJustPressed[button]; }

		inline bool keyPressed(int key) const { return _keys[key]; }
		inline bool keyJustPressed(int key) const { return _keysJustPressed[key]; }
		inline bool keyReleased(int key) const { return _keysReleased[key]; }

		inline const std::vector<input::UnicodeCodePoint>& charactersTyped() const { return _typedCharacters; }

		
		[[nodiscard]]
		fiv::ID subscripeKeyEvent(KeyEventCallback callback) { return _keyEventCallbacks.push(callback); }
		void unsubscribeKeyEvent(fiv::ID id) { _keyEventCallbacks.remove(id); }

		[[nodiscard]]
		fiv::ID subscribeMouseMotionEvent(MouseMotionEventCallback callback) { return _mouseMotionEventCallbacks.push(callback); }
		void unsubscribeMouseMotionEvent(fiv::ID id) { _mouseMotionEventCallbacks.remove(id); }

		[[nodiscard]]
		fiv::ID subscribeMouseButtonEvent(MouseButtonEventCallback callback) { return _mouseButtonEventCallbacks.push(callback); }
		void unsubscribeMouseButtonEvent(fiv::ID id) { _mouseButtonEventCallbacks.remove(id); }

		[[nodiscard]]
		fiv::ID subscribeCharEvent(CharEventCallback callback) { return _charEventCallbacks.push(callback); }
		void unsubscribeCharEvent(fiv::ID id) { _charEventCallbacks.remove(id); }
		
		[[nodiscard]]
		fiv::ID subscribeScrollEvent(ScrollCallback callback) { return _scrollEventCallbacks.push(callback); }
		void unsubscribeScrollEvent(fiv::ID id) { _scrollEventCallbacks.remove(id); }
	
		const char* clipboard() const;
		void setClipboard(const std::string& str);

		friend class Window;
	};

}