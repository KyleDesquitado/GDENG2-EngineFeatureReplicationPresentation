#pragma once
#include "AGameObject.h"
class GameObjectManager
{
private:
	GameObjectManager();
	~GameObjectManager();
private:
	int width = 0;
	int height = 0;
public:	
	vector<bool> selectedObjList;
	vector<AGameObject*> objList;
	vector<AGameObject*> mergedObjList;
	bool merge;
	bool instantiate;
public:
	static GameObjectManager* get();
public:
	void updateObjects();
	void initialize();
	void createCube();
	void createPyramid();
	void changePosition(int i, float x, float y, float z);
	void mergeSelected();
	void instantiateMergedActor(int i);
	void enableBox(int index, bool isEnable);
	void changeBoxScale(int i, float x, float y, float z);

	void setSelectedObject(AGameObject* selectedObject);
	AGameObject* getSelectedObject();

private:
	void initalizePlayerStart();
	AGameObject* selectedObject = nullptr;
};

