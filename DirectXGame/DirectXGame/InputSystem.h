//#pragma once
//#include "InputListener.h"
//#include <unordered_set>
//#include "Point.h"
//
//class InputSystem
//{
//public:
//	InputSystem();
//	~InputSystem();
//
//	void update();
//	void addListener(InputListener* listener);
//	void removeListener(InputListener* listener);
//
//	void setCursorPosition(const Point& pos);
//	void showCursor(bool show);
//
//	bool isKeyDown(int key);
//	bool isKeyUp(int key);
//
//public:
//	static InputSystem* get();
//private:
//	std::unordered_set<InputListener*> m_set_listeners;
//	unsigned char m_keys_state[256] = {};
//	unsigned char m_old_keys_state[256] = {};
//	Point m_old_mouse_pos;
//	bool m_first_time = true;
//};

// Set's Version
#pragma once
#include <iostream>
#include "InputListener.h"
#include <Windows.h>
#include <vector>
#include "Point.h"

typedef std::vector<InputListener*> List;

class InputSystem
{
public:
	static InputSystem* getInstance();
	static void initialize();
	static void destroy();

	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point& pos);
	void showCursor(bool show);

	void update();

	bool isKeyDown(int key);
	bool isKeyUp(int key);

private:
	InputSystem();
	~InputSystem();
	InputSystem(InputSystem const&) {};             // copy constructor is private
	InputSystem& operator=(InputSystem const&) {};  // assignment operator is private*/

	void callOnKeyDown(int key);
	void callOnKeyUp(int key);

	void onMouseMove(Point deltaPt);
	void onLeftMouseDown(Point deltaPt);
	void onLeftMouseUp(Point deltaPt);
	void onRightMouseDown(Point deltaPt);
	void onRightMouseUp(Point deltaPt);

	static InputSystem* sharedInstance;
	List inputListenerList;
	unsigned char keyStates[256] = {};
	unsigned char oldKeyStates[256] = {};

	Point oldMousePos;
	bool firstTimeCall = true;

};