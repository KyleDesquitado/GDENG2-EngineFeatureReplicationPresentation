#include "TransformGizmoScreen.h"
#include "imgui.h"
#include "Matrix4x4.h"
#include "SceneCameraHandler.h"
#include "ImGuizmo.h"
#include "UIManager.h"
#include "GameObjectManager.h"
#include "Utils.h"
#include <iostream>
TransformGizmoScreen::TransformGizmoScreen(): AGizmoScreen("TransformGizmoScreen")
{
}

TransformGizmoScreen::~TransformGizmoScreen()
{
	AGizmoScreen::~AGizmoScreen();

}

void TransformGizmoScreen::drawUI()
{
	// create gimzo widget
	ImGui::SetNextWindowSize(ImVec2(160, 120));
	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	ImGui::Begin("Transform Gizmos", nullptr, ImGuiWindowFlags_NoCollapse);


	// radio button translation
	if (ImGui::RadioButton("Translate", isTranslating))
	{
		ResetAllFlags();
		this->isTranslating = true;
	}
	//radio button scaling
	if (ImGui::RadioButton("Scale", isScaling))
	{
		ResetAllFlags();
		this->isScaling = true;
	}
	// radio button rotation
	if (ImGui::RadioButton("Rotate", isRotating))
	{
		ResetAllFlags();
		this->isRotating = true;
	}
	
	ImGui::End();
#pragma endregion Gizmo

	ImGuizmo::BeginFrame();
	// get the selected object from the game object manager
	this->selectedObject = GameObjectManager::get()->getSelectedObject();
	if (this->selectedObject != nullptr)
	{
		UpdateTransform();
	}

}

void TransformGizmoScreen::UpdateTransform()
{
	// Set up neede matrices
	Matrix4x4 viewMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	Matrix4x4 projectionMatrix = SceneCameraHandler::getInstance()->getSceneCamera()->getProjectionMatrix();
	Matrix4x4 worldMatrix = selectedObject->localMatrix;

	// get float pointer
	float* viewMatPtr = *(viewMatrix.m_mat);
	float* projMatPtr = *(projectionMatrix.m_mat);
	float* worldMatPtr = *(worldMatrix.m_mat);


	if (this->isTranslating || this->isRotating || this->isScaling)
	{
		if (this->isTranslating)
		{	// display gizmo
			Manipulate(viewMatPtr, projMatPtr, ImGuizmo::TRANSLATE, ImGuizmo::WORLD, worldMatPtr);
			selectedObject->currAction = "translate";
		}
		else if (this->isScaling)
		{	// display gizmo
			Manipulate(viewMatPtr, projMatPtr, ImGuizmo::SCALE, ImGuizmo::WORLD, worldMatPtr);
			selectedObject->currAction = "scale";
		}
		else if (this->isRotating)
		{	// display gizmo
			Manipulate(viewMatPtr, projMatPtr, ImGuizmo::ROTATE, ImGuizmo::WORLD, worldMatPtr);
			selectedObject->currAction = "rotate";
		}
		// variable for decomposition
		float translate[3], rotate[3], scale[3];

		// decompose matrix to float*
		ImGuizmo::DecomposeMatrixToComponents(worldMatPtr, translate, rotate, scale);

		
		if (selectedObject->prevAction == " ") {
			selectedObject->prevAction = selectedObject->currAction;
		}

		if (selectedObject->prevAction != selectedObject->currAction) {
			//Save transform
			if (selectedObject->prevAction == "translate")
			{
				cout << "translate" << endl;
				Matrix4x4 temp;
				temp.setIdentity();
				temp.setTranslation(Vector3D(translate[0], translate[1], translate[2]));
				selectedObject->transforms.push_back(temp);
				selectedObject->prevAction = selectedObject->currAction;
			}
			else if (selectedObject->prevAction == "scale")
			{
				cout << "scale" << endl;
				Matrix4x4 temp;
				temp.setIdentity();
				temp.setScale(Vector3D(scale[0], scale[1], scale[2]));
				selectedObject->transforms.push_back(temp);
				selectedObject->prevAction = selectedObject->currAction;
			}
			else if (selectedObject->prevAction == "rotate")
			{
				cout << "rotate" << endl;
				Matrix4x4 temp; temp.setIdentity();
				Matrix4x4 store; store.setIdentity();

				temp.setIdentity();
				temp.setRotationX(Utils::degToRad(rotate[0]));
				store *= temp;

				temp.setIdentity();
				temp.setRotationY(Utils::degToRad(rotate[1]));
				store *= temp;

				temp.setIdentity();
				temp.setRotationZ(Utils::degToRad(rotate[2]));
				store *= temp;

				selectedObject->transforms.push_back(store);

				selectedObject->prevAction = selectedObject->currAction;
			}
		}

		// update transform
		this->selectedObject->setPosition(Vector3D(translate[0], translate[1], translate[2]));
		this->selectedObject->setRotation(Vector3D(Utils::degToRad(rotate[0]), Utils::degToRad(rotate[1]), Utils::degToRad(rotate[2])));
		this->selectedObject->setScale(Vector3D(scale[0], scale[1], scale[2]));
	}
}

void TransformGizmoScreen::ResetAllFlags()
{
	isScaling = false;
	isTranslating = false;
	isRotating = false;
}

