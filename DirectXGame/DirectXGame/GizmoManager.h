#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "AGameObject.h"
#include "AGizmoScreen.h"
#include "AppWindow.h"


class GizmoNames
{
public:
	const String TRANSFORM_GIZMO = "TRANSFORM_GIZMO";

};

class GizmoManager
{
public:
	// aliasing
	typedef std::string String;
	typedef std::vector<AGizmoScreen*> GizmoList;
	typedef std::unordered_map<String, AGizmoScreen*> GizmoTable;

	static GizmoManager* getInstance();
	void initialize();
	static void destroy();

	void drawAllGizmo();
	void enableGizmo(AGameObject* selectedObject);

private:
	GizmoManager();
	~GizmoManager();
	GizmoManager(GizmoManager const&) {};
	GizmoManager& operator=(GizmoManager const&) {};
	static GizmoManager* sharedInstance;


	GizmoList gizmoList;
	GizmoTable gizmoTable;
};

