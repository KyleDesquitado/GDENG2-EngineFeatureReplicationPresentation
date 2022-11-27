#pragma once
#include <string>

typedef std::string String;
class GizmoManager;

class AGizmoScreen
{
protected:
	typedef std::string String;

	AGizmoScreen(String name);
	~AGizmoScreen();

	String getName();
	virtual void drawUI() = 0;

	String name;

	friend class GizmoManager;
};

