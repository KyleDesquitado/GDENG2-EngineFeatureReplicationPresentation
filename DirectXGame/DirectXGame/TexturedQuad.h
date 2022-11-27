#pragma once
#include "AGameObject.h"
#include <vector>
#include"Texture.h"


class TexturedQuad : public AGameObject
{
public:
	TexturedQuad(string name, Texture* texture);
	~TexturedQuad();

	void update(float deltaTime) override;
	void draw(int width, int height) override;
	void drawGizmo(int width, int height) override;
	void setAnimSpeed(float speed);
	void setLookAt(Vector3D lookAtPos);
	bool LoadTextureFromFile(const char* filename, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	void setTexture(ID3D11ShaderResourceView* tex);
	void setSamplerState(ID3D11SamplerState* sampler);

private:
	float speed = 4.0f;
	float deltaTime = 0;
	Matrix4x4 m_world_matrix;
	TVertexBuffer* tVertexBuffer;
	ID3D11ShaderResourceView* myTexture;
	ID3D11SamplerState* samplerState;


	Texture* myTex;;
};


