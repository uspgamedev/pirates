#ifndef INPUTMANAGER_H_
#define INPUTMANAGER_H_

#include <cstdlib>
#include "eventhandler.h"

class InputManager {
private:
	InputManager() {
		//window->enable_keyboard;
	}	
	static InputManager* reference_;
	static void ClickDownEvent(const Event* e, void *data){}
	static void ClickUpEvent(const Event* e, void *data){}

public:
	static InputManager* reference(){
		if (reference_ == NULL)
			return (reference_ = new InputManager());
		else
			return reference_;
	}
	~InputManager() {
		reference_ = NULL;
	}
};

#endif