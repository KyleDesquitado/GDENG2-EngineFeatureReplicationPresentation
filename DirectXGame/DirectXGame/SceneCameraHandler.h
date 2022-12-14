#pragma once
#include "Camera.h"
#include "Matrix4x4.h"

class SceneCameraHandler
{
public:
	static SceneCameraHandler* getInstance();
	static void initialize();
	static void destroy();

	void update(float deltaTime, int width, int height);

	Matrix4x4 getSceneCameraViewMatrix();
	Camera* getSceneCamera();


private:
	SceneCameraHandler();
	~SceneCameraHandler();
	SceneCameraHandler(SceneCameraHandler const&) {};
	SceneCameraHandler& operator=(SceneCameraHandler const&) {};
	static SceneCameraHandler* sharedInstance;

	Camera* sceneCamera;
	int width = 0;
	int height = 0;
};

