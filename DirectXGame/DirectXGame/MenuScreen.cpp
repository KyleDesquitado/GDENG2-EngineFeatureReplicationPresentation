#include "MenuScreen.h"
#include "imgui.h"
#include "UIManager.h"
#include "HierarchyScreen.h"
#include "GameObjectManager.h"

MenuScreen::MenuScreen() : AUIScreen("MenuScreen")
{

}

MenuScreen::~MenuScreen()
{
    AUIScreen::~AUIScreen();
}

void MenuScreen::drawUI()
{
    bool* p_open = new bool;
    *p_open = true;
    ImGui::SetNextWindowSize(ImVec2(UIManager::WINDOW_WIDTH, UIManager::WINDOW_HEIGHT));
	if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Open Editor..")) { HierarchyScreen::isOpen = true; }
            if (ImGui::MenuItem("Create Cube..")) { GameObjectManager::get()->createCube(); }
            if (ImGui::MenuItem("Create Pyramid..")) { GameObjectManager::get()->createPyramid(); }
            if (ImGui::MenuItem("Merge Selected..")) { GameObjectManager::get()->mergeSelected(); }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}
