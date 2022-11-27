#pragma once
#include "AGizmoScreen.h"
#include "AGameObject.h"

class GizmoManager;
class TransformGizmoScreen: public AGizmoScreen
{
private:
	TransformGizmoScreen();
	~TransformGizmoScreen();

	virtual void drawUI() override;
	void UpdateTransform();
	void ResetAllFlags();
	friend class GizmoManager;

private:
	AGameObject* selectedObject = nullptr;

	bool isTranslating = true;
	bool isRotating = false;
	bool isScaling = false;

public:
	void setTranslationActive(bool flag);
	void setScalingActive(bool flag);
	void setRotationActive(bool flag);

};

