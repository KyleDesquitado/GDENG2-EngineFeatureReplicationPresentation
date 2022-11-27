#pragma once
#include <string>
#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4x4.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include"VertexShader.h"
#include"PixelShader.h"
#include "TVertexBuffer.h"
#include "TexturedVertexBuffer.h"
#include <vector>
#include <array>
using namespace std;

class VertexShader;
class PixelShader;

class AGameObject
{
public:
	AGameObject(string name);
	~AGameObject();

	virtual void update(float deltaTime);
	virtual void draw(int width, int height);
	virtual void drawGizmo(int width, int height);
	virtual void drawBox(int width, int height);

	void setPosition(float x, float y, float z);
	void setPosition(Vector3D pos);
	Vector3D getLocalPosition();

	void setScale(float x, float y, float z);
	void setScale(Vector3D scale);
	Vector3D getLocalScale();

	void setRotation(float x, float y, float z);
	void setRotation(Vector3D rot);
	Vector3D getLocalRotation();

	VertexBuffer* getVertexBuffer();
	VertexBuffer* getGizmoVertexBuffer();
	VertexBuffer* getBoxVertexBuffer();

	IndexBuffer* getBoxIndexBuffer();
	IndexBuffer* getIndexBuffer();
	ConstantBuffer* getConstantBuffer();

	VertexShader* getVertexShader();
	PixelShader* getPixelShader();

	bool enableBoundingBox = false;
	Vector3D boundBoxScale = Vector3D(1, 1, 1);

	void reset();
	bool isSelected = false;
	string getName();

	struct Vertex
	{
		Vector3D position;
		Vector3D color;
		Vector3D color2;
	};

	struct TexturedVertex
	{
		Vector3D position;
		Vector2D texCoord;
	};

	_declspec(align(32))
		struct CBData
	{
		Matrix4x4 worldMatrix;
		Matrix4x4 viewMatrix;
		Matrix4x4 projMatrix;
		float time;
		float alpha;
	};

	Matrix4x4 localMatrix;
protected:
	string name;
	Vector3D localPosition;
	Vector3D localScale;
	Vector3D localRotation;
	vector<AGameObject*> objList;

	VertexBuffer* vertexBuffer;
	VertexBuffer* gizmoVertexBuffer;
	VertexBuffer* boxVertexBuffer;

	IndexBuffer* indexBuffer;
	IndexBuffer* boxIndexBuffer;
	ConstantBuffer* constantBuffer;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;



};

