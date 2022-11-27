#include "HierarchyScreen.h"
#include "imgui.h"
#include "UIManager.h"
#include <iostream>
#include "GameObjectManager.h"
#include "ImGuizmo.h"

bool HierarchyScreen::isOpen = false;

HierarchyScreen::HierarchyScreen() : AUIScreen("HierarchyScreen")
{

}

HierarchyScreen::~HierarchyScreen()
{
    AUIScreen::~AUIScreen();
}

void HierarchyScreen::drawUI()
{
    if (isOpen)
    {
        static float xP, yP, zP;
        static float xS = 1; 
        static float yS = 1;
        static float zS = 1;
        static bool isEnabled = false;
        
        //Outline Selection
        //ImGui::SetNextWindowSize(ImVec2(315, 200));
        ImGui::Begin("Hierarchy", &isOpen);
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();

        string currGO = {};

        for (int i = 0; i < GameObjectManager::get()->objList.size(); i++) {
            string str = "Select " + to_string(i);
            const char* selectLabel = str.data();

            currGO = GameObjectManager::get()->objList[i]->getName();

            if (ImGui::Button(selectLabel)) {
                GameObjectManager::get()->selectedObjList[i] = true;
            }
            ImGui::SameLine();
            str = "Unselect " + to_string(i);
            const char* unselectLabel = str.data();
            if (ImGui::Button(unselectLabel)) {
                GameObjectManager::get()->selectedObjList[i] = false;
            }          
            ImGui::SameLine();
            str = "Object " + to_string(i) + " Properties";
            const char* objLabel = str.data();
            if (ImGui::Button(objLabel)) {
                index = i;

                isEnabled = GameObjectManager::get()->objList[index]->enableBoundingBox;
                xP = GameObjectManager::get()->objList[index]->getLocalPosition().m_x;
                yP = GameObjectManager::get()->objList[index]->getLocalPosition().m_y;
                zP = GameObjectManager::get()->objList[index]->getLocalPosition().m_z;
                xS = GameObjectManager::get()->objList[index]->boundBoxScale.m_x;
                yS = GameObjectManager::get()->objList[index]->boundBoxScale.m_y;
                zS = GameObjectManager::get()->objList[index]->boundBoxScale.m_z;

                //update selected GameObject
                GameObjectManager::get()->setSelectedObject(GameObjectManager::get()->objList[index]);
            }

            //name
            ImGui::SameLine();
            ImGui::Text(currGO.c_str());
        }
        ImGui::End();

        //Inspector
        if (!GameObjectManager::get()->objList.empty()) {
            ImGui::SetNextWindowSize(ImVec2(160, 300));
            ImGui::Begin("Inspector", &isOpen, ImGuiWindowFlags_NoResize);
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            //name

            currGO = GameObjectManager::get()->objList[index]->getName();

            ImGui::Text("Name: ");
            ImGui::SameLine();
            ImGui::Text(currGO.c_str());
            ImGui::Spacing();

            // BOUNDING BOX
            ImGui::Checkbox("Enable Box", &isEnabled);
            GameObjectManager::get()->enableBox(index, isEnabled);

            ImGui::Spacing();
            ImGui::Spacing();

            ImGui::Text("Box Scale: ");
            ImGui::SliderFloat("X", &xS, 1.0f, 10.0f);
            ImGui::SliderFloat("Y", &yS, 1.0f, 10.0f);
            ImGui::SliderFloat("Z", &zS, 1.0f, 10.0f);
            GameObjectManager::get()->changeBoxScale(index, xS, yS, zS);
            ImGui::End();
        }   

        //Saved Merge Actors
        if (!GameObjectManager::get()->mergedObjList.empty()) {
            ImGui::SetNextWindowSize(ImVec2(170, 200));
            ImGui::Begin("Merged Actors", &isOpen, ImGuiWindowFlags_NoResize);
            for (int i = 0; i < GameObjectManager::get()->mergedObjList.size(); i++) {
                String str = "Create Merged Actor " + to_string(i);
                const char* mALabel = str.data();
                if (ImGui::Button(mALabel)) {
                    GameObjectManager::get()->instantiateMergedActor(i);
                }
            }
            ImGui::End();
        }
    }
}
