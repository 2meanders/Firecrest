#include "Input.h"

namespace fc {

	void Input::characterCallback(GLFWwindow* window, unsigned int codepoint) {
		for(CharEventCallback& callback : _charEventCallbacks) {
			callback(codepoint);
		}
		_recentCharacterEvents.push_back(static_cast<input::UnicodeCodePoint>(codepoint));
	}

	void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		input::RawKeyAction keyAction = input::RawKeyAction::Up;
		switch (action) {
		case GLFW_PRESS:
			keyAction = input::RawKeyAction::Press;
			break;
		case GLFW_REPEAT:
			keyAction = input::RawKeyAction::Repeat;
			break;
		case GLFW_RELEASE:
			keyAction = input::RawKeyAction::Up;
			break;
		}
		input::RawKeyboardEvent event {
			key,
			keyAction,
			scancode,
			mods,
		};
		for(KeyEventCallback& callback : _keyEventCallbacks) {
			callback(event);
		}
		_recentKeyEvents.push_back(event);
	}

	void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
		input::RawMouseButtonEvent event;
		event.button = static_cast<input::RawMouseButton>(button);
		event.action = action == GLFW_PRESS ? input::RawMouseButtonAction::Press : input::RawMouseButtonAction::Up;
		
		for(MouseButtonEventCallback& callback : _mouseButtonEventCallbacks) {
			callback(event);
		}

		_recentMouseButtonEvents.push_back(event);
	}

    void Input::mousePosCallback(GLFWwindow *window, double xPos, double yPos) {
		int height;
		glfwGetWindowSize(window, NULL, &height);

		_lastMouse = _currMouse;
		_currMouse = { static_cast<float>(xPos), height - static_cast<float>(yPos) };
		
		input::RawMouseMotionEvent event;
		event.position = _currMouse;
		event.lastPosition = _lastMouse;
		
		for(MouseMotionEventCallback& callback : _mouseMotionEventCallbacks) {
			callback(event);
		}

    }

    void Input::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
		float x = static_cast<float>(xoffset);
		float y = static_cast<float>(yoffset);
		input::RawScrollEvent event;
		event.offset = {x, y};
		for(ScrollCallback& callback : _scrollEventCallbacks) {
			callback(event);
		}
    }

    Input::Input() : mouseButtons(), mouseButtonsReleased(), mouseButtonsJustPressed(), _currMouse({-1, -1}), _lastMouse({-1, -1}), keys(), keysReleased(), keysJustPressed() {
		for (bool& button : mouseButtons) button = false;
		for (bool& button : mouseButtonsReleased) button = false;
		for (bool& button : mouseButtonsJustPressed) button = false;
		for (bool& key : keys) key = false;
		for (bool& key : keysReleased) key = false;
	}

	void Input::update(GLFWwindow* window) {
		_lastMouse = _currMouse;

		for(bool& button : mouseButtonsJustPressed)
			button = false;
		for(bool& button : mouseButtonsReleased)
			button = false;
		for(bool& key : keysJustPressed)
			key = false;
		for(bool& key : keysReleased)
			key = false;

		typedCharacters = _recentCharacterEvents;
		_recentCharacterEvents.clear();

		for(auto& e : _recentKeyEvents) {
			switch(e.action) {
			case input::RawKeyAction::Press:
				keys[e.key] = true;
				keysJustPressed[e.key] = true;
				break;
			case input::RawKeyAction::Repeat:
				keys[e.key] = true;
				break;
			case input::RawKeyAction::Up:
				keys[e.key] = false;
				keysReleased[e.key] = true;
			}
		}
		_recentKeyEvents.clear();

		for(auto& e : _recentMouseButtonEvents) {
			switch(e.action) {
			case input::RawMouseButtonAction::Press:
				mouseButtons[static_cast<int>(e.button)] = true;
				mouseButtonsJustPressed[static_cast<int>(e.button)] = true;
			case input::RawMouseButtonAction::Up:
				mouseButtons[static_cast<int>(e.button)] = false;
				mouseButtonsReleased[static_cast<int>(e.button)] = true;
			}
		}	
		_recentMouseButtonEvents.clear();
	}
}
