#include "TexturedQuad.h"
#include <iostream>
#include "DeviceContext.h"
#include "EngineTime.h"
#include "GraphicsEngine.h"
#include "SceneCameraHandler.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TexturedQuad::TexturedQuad(string name, Texture* texture) :AGameObject(name)
{
	Vector3D position_list[] =
	{
		{ Vector3D(-0.5f,-0.5f,-0.5f)},
		{ Vector3D(-0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,0.5f,-0.5f) },
		{ Vector3D(0.5f,-0.5f,-0.5f)},
	};

	Vector2D texcoord_list[] =
	{
		{ Vector2D(0.0f,0.0f) },
		{ Vector2D(0.0f,1.0f) },
		{ Vector2D(1.0f,0.0f) },
		{ Vector2D(1.0f,1.0f) }
	};

	TexturedVertex vertex_list[] =
	{
		//X - Y - Z
		//FRONT FACE
		{ position_list[0],texcoord_list[1] },
		{ position_list[1],texcoord_list[0] },
		{ position_list[2],texcoord_list[2] },
		{ position_list[3],texcoord_list[3] },
	};


	unsigned int index_list[] =
	{
		//FRONT SIDE
		0,1,2,  //FIRST TRIANGLE
		2,3,0,  //SECOND TRIANGLE
	};

	myTex = texture;

	this->indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	this->indexBuffer->load(index_list, ARRAYSIZE(index_list));

	//Vertex Shader
	GraphicsEngine::get()->compileVertexShader(L"TVertexShader.hlsl", "tvsmain", &shaderByteCode, &sizeShader);
	vertexShader = GraphicsEngine::get()->createVertexShader(shaderByteCode, sizeShader);

	//Vertex Buffer
	this->tVertexBuffer = GraphicsEngine::get()->createTVertexBuffer();
	this->tVertexBuffer->load(vertex_list, sizeof(TexturedVertex), ARRAYSIZE(vertex_list), shaderByteCode, sizeShader);

	//this->gizmoVertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	//this->gizmoVertexBuffer->load(vertex_list_gizmo, sizeof(Vertex), ARRAYSIZE(vertex_list_gizmo), shaderByteCode, sizeShader);

	GraphicsEngine::get()->releaseCompiledShader();

	//Pixel Shader
	GraphicsEngine::get()->compilePixelShader(L"TPixelShader.hlsl", "tpsmain", &shaderByteCode, &sizeShader);
	pixelShader = GraphicsEngine::get()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->releaseCompiledShader();

	//Create constant buffer
	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));


	//// loading texture from file
	//GraphicsEngine::get()->initializeSamplers();

	//int width = 0;
	//int height = 0;
	//bool ret = LoadTextureFromFile(texture.c_str(), &myTexture, &width, &height);
	//IM_ASSERT(ret);

	////get sampler state in graphics engine
	//this->samplerState = GraphicsEngine::get()->getSamplerState();

	setAnimSpeed(4);
}

TexturedQuad::~TexturedQuad()
{
	this->indexBuffer->release();
	this->vertexShader->release();
	this->pixelShader->release();
	this->constantBuffer->release();
}

void TexturedQuad::update(float delta_time)
{
	
}

void TexturedQuad::draw(int width, int height)
{
	this->isSelected = false;
	
	setLookAt(SceneCameraHandler::getInstance()->getSceneCamera()->getLocalPosition());

	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphEngine->getImmediateDeviceContext();

	CBData cbData = {};

	cbData.time = deltaTime;

	//Add object transformation
	Matrix4x4 temp;

	cbData.worldMatrix.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	/*temp.setIdentity();
	temp.setScale(getLocalScale());
	cbData.worldMatrix *= temp;*/

	temp.setIdentity();
	temp.setRotationX((getLocalRotation().m_x));
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(getLocalRotation().m_y);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(getLocalRotation().m_z);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setTranslation(getLocalPosition());
	cbData.worldMatrix *= temp;

	this->localMatrix = cbData.worldMatrix;

	//Add camera transformation
	Matrix4x4 cameraMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cbData.viewMatrix = cameraMatrix;

	Camera* cam = SceneCameraHandler::getInstance()->getSceneCamera();
	//Perspective View
	cbData.projMatrix.setPerspectiveFovLH(cam->getFOVinRad(), float(width) / (float)height, cam->getzNear(), cam->getzFar());
	this->constantBuffer->update(deviceContext, &cbData);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(pixelShader);

	//SET TEXTURE
	GraphicsEngine::get()->getImmediateDeviceContext()->setTexture(pixelShader, this->myTex);



	deviceContext->setSolidRenderMode();

	//SET DEFAULT BUFFER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	deviceContext->setConstantBuffer(vertexShader, this->constantBuffer);
	deviceContext->setConstantBuffer(pixelShader, this->constantBuffer);

	deviceContext->setIndexBuffer(this->indexBuffer);
	deviceContext->setTVertexBuffer(tVertexBuffer);

	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void TexturedQuad::drawGizmo(int width, int height)
{
	GraphicsEngine* graphEngine = GraphicsEngine::get();
	DeviceContext* deviceContext = graphEngine->getImmediateDeviceContext();

	CBData cbData = {};

	cbData.time = deltaTime;

	//Add object transformation
	Matrix4x4 temp;

	cbData.worldMatrix.setIdentity();

	Matrix4x4 world_cam;
	world_cam.setIdentity();

	temp.setIdentity();
	temp.setScale(getLocalScale() * 1.05f);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationX(getLocalRotation().m_x);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationY(getLocalRotation().m_y);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setRotationZ(getLocalRotation().m_z);
	cbData.worldMatrix *= temp;

	temp.setIdentity();
	temp.setTranslation(getLocalPosition());
	cbData.worldMatrix *= temp;

	//Add camera transformation
	Matrix4x4 cameraMatrix = SceneCameraHandler::getInstance()->getSceneCameraViewMatrix();
	cbData.viewMatrix = cameraMatrix;

	Camera* cam = SceneCameraHandler::getInstance()->getSceneCamera();

	//Perspective View
	cbData.projMatrix.setPerspectiveFovLH(cam->getFOVinRad(), float(width) / (float)height, cam->getzNear(), cam->getzFar());

	this->constantBuffer->update(deviceContext, &cbData);

	//SET DEFAULT SHADER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(vertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(pixelShader);

	//SET DEFAULT BUFFER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	deviceContext->setConstantBuffer(vertexShader, this->constantBuffer);
	deviceContext->setConstantBuffer(pixelShader, this->constantBuffer);

	deviceContext->setIndexBuffer(this->indexBuffer);

	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void TexturedQuad::setAnimSpeed(float speed)
{
	this->speed = speed;
}

void TexturedQuad::setLookAt(Vector3D lookAtPos)
{
	Vector3D pos = this->getLocalPosition();
	//Verify that look at pos is not the same as cam pos. They cannot be the same as that wouldn't make sense and would result in undefined behavior.
	if (lookAtPos.m_x == pos.m_x && lookAtPos.m_y == pos.m_y && lookAtPos.m_z == pos.m_z)
		return;

	// current pos - camera pos
	lookAtPos.m_x = -pos.m_x + lookAtPos.m_x;
	lookAtPos.m_y = -pos.m_y + lookAtPos.m_y;
	lookAtPos.m_z = -pos.m_z + lookAtPos.m_z;

	float pitch = 0.0f; // x rotation
	if (lookAtPos.m_y != 0.0f)
	{
		const float distance = sqrt((lookAtPos.m_x * lookAtPos.m_x) + (lookAtPos.m_z * lookAtPos.m_z));
		pitch = atan(lookAtPos.m_y / distance);
	//	cout << "pitch: " << pitch<<"\n";

	}

	float yaw = 0.0f; // y rotation

	if (lookAtPos.m_x != 0.0f)
	{
		yaw = atan(lookAtPos.m_x / lookAtPos.m_z);
	//	cout << "yaw: " << yaw << "\n";
	}

	if (lookAtPos.m_z > 0)
		yaw += 3.14;

	this->setRotation(pitch, yaw, 0.0f);

}

// Simple helper function to load an image into a DX11 texture with common settings
bool TexturedQuad:: LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height)
{
	ID3D11Device* g_pd3dDevice = GraphicsEngine::get()->getDevice();
	// Load from disk into a raw RGBA buffer
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = image_width;
	desc.Height = image_height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;

	ID3D11Texture2D* pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = image_data;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

	// Create texture view
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;
	g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
	pTexture->Release();

	*out_width = image_width;
	*out_height = image_height;
	stbi_image_free(image_data);

	return true;
}

void TexturedQuad::setTexture(ID3D11ShaderResourceView* tex)
{
	this->myTexture = tex;
}

void TexturedQuad::setSamplerState(ID3D11SamplerState* sampler)
{
	this->samplerState = sampler;

}


