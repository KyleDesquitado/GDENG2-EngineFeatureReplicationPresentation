#include "Pyramid.h"
#include <iostream>
#include "DeviceContext.h"
#include "EngineTime.h"
#include "GraphicsEngine.h"
#include "SceneCameraHandler.h"

//Utils* utils = new Utils();
Pyramid::Pyramid(string name) :AGameObject(name)
{
	//Create buffers for drawing. Vertex data that needs to be drawn are temporarily placed here.
	Vertex vertex_list[] =
	{
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0,1,0) },
		{Vector3D(0.0f,0.5f,0.0f),    Vector3D(0,1,0), Vector3D(0,1,1) },
		{ Vector3D(0.5f,-0.5f,-0.5f),   Vector3D(0,0,1),  Vector3D(1,0,0) },
		{ Vector3D(0.5f,-0.5f,0.5f),     Vector3D(1,1,0), Vector3D(0,0,1) },
		{ Vector3D(-0.5f,-0.5f,0.5f),    Vector3D(1,0,0), Vector3D(0,1,0) },
	};

	Vertex vertex_list_gizmo[] =
	{
		{Vector3D(-0.5f,-0.5f,-0.5f),	Vector3D(1,0,0), Vector3D(0.2f,0,0) },
		{Vector3D(0.0f,0.5f,0.0f),    Vector3D(1,0,0), Vector3D(0.2f,0.2f,0) },
		{Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0),  Vector3D(0.2f,0.2f,0) },
		{Vector3D(0.5f,-0.5f,0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,-0.5f,0.5f),	Vector3D(1,0,0), Vector3D(0,0.2f,0) }
	};

	unsigned int index_list[] =
	{
		//FRONT SIDE
		//0, 1, 2,
		//2, 4, 0
		//0, 1, 3,
		//3, 0, 4,
		//4, 2, 1,
		//1, 3, 4

		0,1,2,
		2,1,3,
		3,1,4,
		4,1,0,
		0,2,3,
		3,4,0
	};

	Vertex vertex_list_box[] =
	{
		{Vector3D(-0.5f,-0.5f,-0.5f),	Vector3D(1,0,0), Vector3D(0.2f,0,0) },
		{Vector3D(0.0f,0.5f,0.0f),    Vector3D(1,0,0), Vector3D(0.2f,0.2f,0) },
		{Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0),  Vector3D(0.2f,0.2f,0) },
		{Vector3D(0.5f,-0.5f,0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,-0.5f,0.5f),	Vector3D(1,0,0), Vector3D(0,0.2f,0) }
	};

	unsigned int index_line_list[] =
	{
		1, 2,
		1, 0,
		0, 2,

		2, 3,
		1, 3,

		3, 4,
		1, 4,

		4, 0,

		0, 3
	};

	//Index Buffer
	this->indexBuffer = GraphicsEngine::get()->createIndexBuffer();
	this->indexBuffer->load(index_list, ARRAYSIZE(index_list));

	//Index Buffer
	this->boxIndexBuffer = GraphicsEngine::get()->createIndexBuffer();
	this->boxIndexBuffer->load(index_line_list, ARRAYSIZE(index_line_list));

	//Vertex Shader
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &shaderByteCode, &sizeShader);
	vertexShader = GraphicsEngine::get()->createVertexShader(shaderByteCode, sizeShader);

	//Vertex Buffer
	this->vertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	this->vertexBuffer->load(vertex_list, sizeof(Vertex), ARRAYSIZE(vertex_list), shaderByteCode, sizeShader);

	this->gizmoVertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	this->gizmoVertexBuffer->load(vertex_list_gizmo, sizeof(Vertex), ARRAYSIZE(vertex_list_gizmo), shaderByteCode, sizeShader);

	this->boxVertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	this->boxVertexBuffer->load(vertex_list_box, sizeof(Vertex), ARRAYSIZE(vertex_list_box), shaderByteCode, sizeShader);

	GraphicsEngine::get()->releaseCompiledShader();

	//Pixel Shader
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	pixelShader = GraphicsEngine::get()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->releaseCompiledShader();

	//Create constant buffer
	CBData cbData = {};
	cbData.time = 0;
	this->constantBuffer = GraphicsEngine::get()->createConstantBuffer();
	this->constantBuffer->load(&cbData, sizeof(CBData));

	setAnimSpeed(4);

	boundBoxScale = Vector3D(1.1, 1.1, 1.1);
}

Pyramid::~Pyramid()
{
	this->vertexBuffer->release();
	this->indexBuffer->release();
	this->vertexShader->release();
	this->pixelShader->release();
	this->constantBuffer->release();
}

void Pyramid::update(float delta_time)
{

}

void Pyramid::draw(int width, int height)
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
	temp.setScale(getLocalScale());
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

	//SET DEFAULT BUFFER IN THE GRAPHICS PIPELINE TO BE ABLE TO DRAW
	deviceContext->setConstantBuffer(vertexShader, this->constantBuffer);
	deviceContext->setConstantBuffer(pixelShader, this->constantBuffer);

	deviceContext->setIndexBuffer(this->indexBuffer);
	deviceContext->setVertexBuffer(this->vertexBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setSolidRenderMode();
	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void Pyramid::drawGizmo(int width, int height)
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
	deviceContext->setVertexBuffer(this->gizmoVertexBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setSolidRenderMode();
	deviceContext->drawIndexedTriangleList(this->indexBuffer->getSizeIndexList(), 0, 0);
}

void Pyramid::drawBox(int width, int height)
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
	temp.setScale(boundBoxScale);
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

	deviceContext->setIndexBuffer(this->boxIndexBuffer);
	deviceContext->setVertexBuffer(this->boxVertexBuffer);

	GraphicsEngine::get()->getImmediateDeviceContext()->setWireframeRenderMode();

	deviceContext->drawIndexedLineList(this->boxIndexBuffer->getSizeIndexList(), 0, 0);
}

void Pyramid::setAnimSpeed(float speed)
{
	this->speed = speed;
}


