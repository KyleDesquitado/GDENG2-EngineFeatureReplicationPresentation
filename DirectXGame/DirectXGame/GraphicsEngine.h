#pragma once
#include <d3d11.h>

#include "AUIScreen.h"

#include"Prerequisites.h"
#include "TextureManager.h"



class GraphicsEngine
{
public:
	GraphicsEngine();
	//Initialize the GraphicsEngine and DirectX 11 Device
	bool init();
	//Release all the resources loaded
	bool release();
	~GraphicsEngine();

public:
	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();
	TVertexBuffer* createTVertexBuffer();
	//TexturedVertexBuffer* createTexturedVertexBuffer();
	IndexBuffer* createIndexBuffer();
	ConstantBuffer* createConstantBuffer();
	VertexShader* createVertexShader(const void* shader_byte_code, size_t byte_code_size);
	PixelShader* createPixelShader(const void* shader_byte_code, size_t byte_code_size);

	ID3D11ShaderResourceView* getTexture();
	ID3D11SamplerState* getSamplerState();

	TextureManager* getTextureManager();



public:
	bool compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	bool compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size);
	void releaseCompiledShader();
	void createStencilState(String mode);
	void createBlendState(String mode);
	void initializeSamplers();

public:
	static GraphicsEngine* get();
	ID3D11Device* getDevice();
	ID3D11DepthStencilState* getStencilState();
	ID3D11BlendState* getBlendState();
	ID3D11RasterizerState* mWireframeRS;
	ID3D11RasterizerState* mSolidRS;
private:
	DeviceContext* m_imm_device_context;
private:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_feature_level;
	ID3D11DepthStencilState* m_stencil_state;
private:
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;
	ID3D11DeviceContext* m_imm_context;
private:
	ID3DBlob* m_blob = nullptr;
	ID3DBlob* m_vsblob = nullptr;
	ID3DBlob* m_psblob = nullptr;
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	void InitRenderStates();

	ID3D11SamplerState* samplerState;
	TextureManager* textureManager = nullptr;

private:
	ID3D11BlendState* blendState;

private:
	friend class SwapChain;
	friend class VertexBuffer;
	friend class TVertexBuffer;
	friend class TexturedVertexBuffer;
	friend class IndexBuffer;
	friend class ConstantBuffer;
	friend class VertexShader;
	friend class PixelShader;

};
