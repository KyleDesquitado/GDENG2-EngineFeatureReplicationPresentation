#pragma once
#include "AGameObject.h"

#include <vector>

class TexturedCube : public AGameObject
{
public:
	TexturedCube(string name);
	//Cube(vector<Vertex> v1, vector<Vertex> v2, string name, void* shaderByteCode, size_t sizeShader);
	~TexturedCube();

	void update(float deltaTime) override;
	void draw(int width, int height) override;
	void drawGizmo(int width, int height) override;
	void setAnimSpeed(float speed);

	ID3D11ShaderResourceView* myTexture;
	ID3D11SamplerState* samplerState;

	//vector<Vertex> getVertexList();
private:
	float speed = 4.0f;
	float deltaTime = 0;
	Matrix4x4 m_world_matrix;
	TVertexBuffer* tVertexBuffer;
	//vector<Vertex> vertex_list_copy;
};

