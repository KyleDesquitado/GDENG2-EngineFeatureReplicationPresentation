#include "GizmoManager.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "GraphicsEngine.h"

// screens
#include "TransformGizmoScreen.h"

GizmoManager* GizmoManager::sharedInstance = nullptr;

GizmoManager* GizmoManager::getInstance()
{
	return sharedInstance;
}

void GizmoManager::initialize()
{
	sharedInstance = new GizmoManager();
}

void GizmoManager::destroy()
{
	delete sharedInstance;
}


GizmoManager::GizmoManager()
{

	GizmoNames uiNames;

	TransformGizmoScreen* transformGizmoScreen = new TransformGizmoScreen();
	this->gizmoTable[uiNames.TRANSFORM_GIZMO] = transformGizmoScreen;
	this->gizmoList.push_back(transformGizmoScreen);



}

GizmoManager::~GizmoManager()
{

}

void GizmoManager::drawAllGizmo()
{


	for (int i = 0; i < this->gizmoList.size(); i++)
	{
		this->gizmoList[i]->drawUI();
	}

}
