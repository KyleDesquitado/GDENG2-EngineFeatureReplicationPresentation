#include "AGameObject.h"

AGameObject::AGameObject(string name)
{
	this->name = name;
	this->localPosition = Vector3D(0, 0, 0);
	this->localRotation = Vector3D(0, 0, 0);
	this->localScale = Vector3D(1, 1, 1);
}

AGameObject::~AGameObject()
{

}

void AGameObject::update(float deltaTime)
{
}

void AGameObject::draw(int width, int height)
{
}

void AGameObject::drawGizmo(int width, int height)
{

}

void AGameObject::drawBox(int width, int height)
{
}

void AGameObject::setPosition(float x, float y, float z)
{
	this->localPosition = Vector3D(x, y, z);
}

void AGameObject::setPosition(Vector3D pos)
{
	this->localPosition = pos;
}

Vector3D AGameObject::getLocalPosition()
{
	return this->localPosition;
}

void AGameObject::setScale(float x, float y, float z)
{
	this->localScale = Vector3D(x, y, z);
}

void AGameObject::setScale(Vector3D scale)
{
	this->localScale = scale;
}

Vector3D AGameObject::getLocalScale()
{
	return this->localScale;
}

void AGameObject::setRotation(float x, float y, float z)
{
	this->localRotation = Vector3D(x, y, z);
}

void AGameObject::setRotation(Vector3D rot)
{
	this->localRotation = rot;
}

Vector3D AGameObject::getLocalRotation()
{
	return this->localRotation;
}

VertexBuffer* AGameObject::getVertexBuffer()
{
	return vertexBuffer;
}

VertexBuffer* AGameObject::getGizmoVertexBuffer()
{
	return gizmoVertexBuffer;
}

VertexBuffer* AGameObject::getBoxVertexBuffer()
{
	return boxVertexBuffer;
}

IndexBuffer* AGameObject::getBoxIndexBuffer()
{
	return boxIndexBuffer;
}

IndexBuffer* AGameObject::getIndexBuffer()
{
	return indexBuffer;
}

ConstantBuffer* AGameObject::getConstantBuffer()
{
	return constantBuffer;
}

string AGameObject::getName()
{
	return this->name;
}


VertexShader* AGameObject::getVertexShader()
{
	return vertexShader;
}

PixelShader* AGameObject::getPixelShader()
{
	return pixelShader;
}

void AGameObject::reset()
{
	setRotation(0, 0, 0);
}

