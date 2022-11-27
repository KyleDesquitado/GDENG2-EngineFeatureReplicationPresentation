#pragma once
#include <vector>
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "InputListener.h"

// buffers
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

// shaders
#include "VertexShader.h"
#include "PixelShader.h"

//math
#include "Matrix4x4.h"

//primitives
#include "Cube.h"
#include "Plane.h"
#include "TexturedQuad.h"



class AppWindow : public Window, public InputListener
{
public:
	AppWindow();

	void update();

	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
	virtual void onFocus() override;
	virtual void onKillFocus() override;

	 //Inherited via InputListener
	/*virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mouse_pos) override;
	virtual void onLeftMouseDown(const Point& mouse_pos) override;
	virtual void onLeftMouseUp(const Point& mouse_pos) override;
	virtual void onRightMouseDown(const Point& mouse_pos) override;
	virtual void onRightMouseUp(const Point& mouse_pos) override;*/


	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	// Inherited via InputListener
	virtual void onMouseMove(const Point deltaPos) override;
	virtual void onLeftMouseDown(const Point deltaPos) override;
	virtual void onLeftMouseUp(const Point deltaPos) override;
	virtual void onRightMouseDown(const Point deltaPos) override;
	virtual void onRightMouseUp(const Point deltaPos) override;

	bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);

private:
	SwapChain* m_swap_chain;
	
private:
	//Instantiated objects
	vector<AGameObject*> objList;
	vector<AGameObject*> outlineList;

	TexturedQuad* quad;

	int width = 0;
	int height = 0;

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;

public:
	static vector<bool> selectedObjList;
};
