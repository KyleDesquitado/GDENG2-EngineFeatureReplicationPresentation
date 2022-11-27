#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "TVertexBuffer.h"

#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "TexturedVertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"

#include <d3dcompiler.h>
#include <iostream>
#include <ostream>

#include "UIManager.h"

GraphicsEngine::GraphicsEngine()
{
	try
	{
		textureManager = new TextureManager();
	}
	catch (...) { throw std::exception("TextureManager not created successfully"); }
}

bool GraphicsEngine::init()
{
	D3D_DRIVER_TYPE driver_types[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;

	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
			num_feature_levels, D3D11_SDK_VERSION, &m_d3d_device, &m_feature_level, &m_imm_context);
		if (SUCCEEDED(res))
			break;
		++driver_type_index;
	}
	if (FAILED(res))
	{
		return false;
	}

	m_imm_device_context = new DeviceContext(m_imm_context);

	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);

	//For render mode
	InitRenderStates();

	return true;
}


bool GraphicsEngine::release()
{
	if (vertexShader)vertexShader->Release();
	if (pixelShader)pixelShader->Release();

	if (m_vsblob)m_vsblob->Release();
	if (m_psblob)m_psblob->Release();

	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	m_imm_device_context->release();


	m_d3d_device->Release();
	return true;
}

GraphicsEngine::~GraphicsEngine()
{
}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain();
}


DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return this->m_imm_device_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

TVertexBuffer* GraphicsEngine::createTVertexBuffer()
{
	return new TVertexBuffer();
}

IndexBuffer* GraphicsEngine::createIndexBuffer()
{
	return new IndexBuffer();
}

ConstantBuffer* GraphicsEngine::createConstantBuffer()
{
	return new ConstantBuffer();
}

VertexShader* GraphicsEngine::createVertexShader(const void* shader_byte_code, size_t byte_code_size)
{
	VertexShader* vs = new VertexShader();

	if (!vs->init(shader_byte_code, byte_code_size))
	{
		vs->release();
		return nullptr;
	}

	return vs;
}

PixelShader* GraphicsEngine::createPixelShader(const void* shader_byte_code, size_t byte_code_size)
{
	PixelShader* ps = new PixelShader();

	if (!ps->init(shader_byte_code, byte_code_size))
	{
		ps->release();
		return nullptr;
	}

	return ps;
}

ID3D11SamplerState* GraphicsEngine::getSamplerState()
{
	return samplerState;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return this->textureManager;
}

bool GraphicsEngine::compileVertexShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "vs_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

bool GraphicsEngine::compilePixelShader(const wchar_t* file_name, const char* entry_point_name, void** shader_byte_code, size_t* byte_code_size)
{
	ID3DBlob* error_blob = nullptr;
	if (!SUCCEEDED(D3DCompileFromFile(file_name, nullptr, nullptr, entry_point_name, "ps_5_0", 0, 0, &m_blob, &error_blob)))
	{
		if (error_blob) error_blob->Release();
		return false;
	}

	*shader_byte_code = m_blob->GetBufferPointer();
	*byte_code_size = m_blob->GetBufferSize();

	return true;
}

void GraphicsEngine::releaseCompiledShader()
{
	if (m_blob)m_blob->Release();
}

void GraphicsEngine::createStencilState(String mode)
{
	D3D11_DEPTH_STENCIL_DESC desc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	if(mode == "Write")
	{
		desc.StencilEnable = TRUE;
		desc.DepthEnable = TRUE;
		desc.StencilWriteMask = 0xFF;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	}
	else if(mode == "Mask")
	{
		desc.DepthEnable = FALSE;
		desc.StencilEnable = TRUE;
		desc.StencilReadMask = 0xFF;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	}

	m_d3d_device->CreateDepthStencilState(&desc, &m_stencil_state);
}

void GraphicsEngine::createBlendState(String mode)
{
	//Create Blend State
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	D3D11_RENDER_TARGET_BLEND_DESC rtbd;
	ZeroMemory(&rtbd, sizeof(rtbd));

	rtbd.BlendEnable = true; // enable or disable blending

	// PIXEL COLOR
	rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;     //specifies the blend factor for the source pixel           
	rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;          //specifies the blend factor for the destination pixel
	rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;      //specifies the RGB blending operation  

	// ALPHA
	rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;     //specifies blend factor for source pixel's alpha component  
	rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;     //specifies the blend factor destination pixel's alpha component
	rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD; //the blending operation for the Alpha component            

	//// PIXEL COLOR
	//rtbd.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;     //specifies the blend factor for the source pixel           
	//rtbd.DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;          //specifies the blend factor for the destination pixel
	//rtbd.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;      //specifies the RGB blending operation  

	//// ALPHA
	//rtbd.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;     //specifies blend factor for source pixel's alpha component  
	//rtbd.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;     //specifies the blend factor destination pixel's alpha component
	//rtbd.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD; //the blending operation for the Alpha component   


	if (mode == "ALL")
	{
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL; // enable all color
	}
	else if (mode == "RED")
	{
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_RED; // enable all color

	}

	blendDesc.RenderTarget[0] = rtbd;

	HRESULT hr = this->m_d3d_device->CreateBlendState(&blendDesc, &blendState);
	if (FAILED(hr))
	{
		cout << " failed to create blend state";
	}
}

void GraphicsEngine::initializeSamplers()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = 0;
	desc.MaxLOD = D3D11_FLOAT32_MAX;
	this->getDevice()->CreateSamplerState(&desc, &samplerState);
}



ID3D11Device* GraphicsEngine::getDevice()
{
	return this->m_d3d_device;
}

ID3D11DepthStencilState* GraphicsEngine::getStencilState()
{
	return this->m_stencil_state;
}

ID3D11BlendState* GraphicsEngine::getBlendState()
{
	return this->blendState;
}


GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}

void GraphicsEngine::InitRenderStates()
{
	D3D11_RASTERIZER_DESC wfd;
	ZeroMemory(&wfd, sizeof(D3D11_RASTERIZER_DESC));
	wfd.FillMode = D3D11_FILL_WIREFRAME;
	wfd.CullMode = D3D11_CULL_NONE;
	wfd.DepthClipEnable = true;

	m_d3d_device->CreateRasterizerState(&wfd, &mWireframeRS);

	D3D11_RASTERIZER_DESC sd = CD3D11_RASTERIZER_DESC{ CD3D11_DEFAULT{} };

	m_d3d_device->CreateRasterizerState(&sd, &mSolidRS);
}
