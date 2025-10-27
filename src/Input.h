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
		glm::vec2 _currMouse;
		glm::vec2 _lastMouse;
		bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];
		bool mouseButtonsReleased[GLFW_MOUSE_BUTTON_LAST];
		bool mouseButtonsJustPressed[GLFW_MOUSE_BUTTON_LAST];

		bool keys[GLFW_KEY_LAST];
		bool keysReleased[GLFW_KEY_LAST];
		bool keysJustPressed[GLFW_KEY_LAST];

		std::vector<input::UnicodeCodePoint> typedCharacters;
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

		void update(GLFWwindow* window);
		
		inline glm::vec2 mouse() const { return _currMouse; }
		inline glm::vec2 lastMouse() const { return _lastMouse; }
		inline glm::vec2 deltaMouse() const { return _currMouse - _lastMouse; }
		inline bool mouseMoved() const { return _currMouse != _lastMouse; }

		inline bool mouseIsPressed(int button) const { return mouseButtons[button]; }
		inline bool mouseReleased(int button) const { return mouseButtonsReleased[button]; }
		inline bool mouseJustPressed(int button) const { return mouseButtonsJustPressed[button]; }

		inline bool keyPressed(int key) const { return keys[key]; }
		inline bool keyJustPressed(int key) const { return keysJustPressed[key]; }
		inline bool keyReleased(int key) const { return keysReleased[key]; }

		inline const std::vector<input::UnicodeCodePoint>& charactersTyped() const { return typedCharacters; }

		
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
		
		friend class Window;
	};

}