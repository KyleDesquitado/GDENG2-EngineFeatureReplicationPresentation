#include "DeviceContext.h"
#include <iostream>
#include "SwapChain.h"
#include "TVertexBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "GraphicsEngine.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Texture.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context) :m_device_context(device_context)
{
}

void DeviceContext::clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{
	FLOAT clear_color[] = { red,green,blue,alpha };
	m_device_context->ClearRenderTargetView(swap_chain->m_rtv, clear_color);
	m_device_context->ClearDepthStencilView(swap_chain->m_dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	m_device_context->OMSetRenderTargets(1, &swap_chain->m_rtv, swap_chain->m_dsv);
}

void DeviceContext::setVertexBuffer(VertexBuffer* vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::setTVertexBuffer(TVertexBuffer* vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;
	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_layout);
}

void DeviceContext::setIndexBuffer(IndexBuffer* index_buffer)
{
	m_device_context->IASetIndexBuffer(index_buffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedTriangleList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawIndexedLineList(UINT index_count, UINT start_vertex_index, UINT start_index_location)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp = {};
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_device_context->RSSetViewports(1, &vp);
}

void DeviceContext::setVertexShader(VertexShader* vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->vertexShader, nullptr, 0);
}

void DeviceContext::setPixelShader(PixelShader* pixel_shader)
{
	m_device_context->PSSetShader(pixel_shader->pixelShader, nullptr, 0);
}

void DeviceContext::setTexture(VertexShader* vertex_shader, Texture* texture)
{
	if (texture != nullptr)
		m_device_context->VSSetShaderResources(0, 1, &texture->m_shader_res_view);
}

void DeviceContext::setTexture(PixelShader* pixel_shader, Texture* texture)
{
	if (texture != nullptr)
		m_device_context->PSSetShaderResources(0, 1, &texture->m_shader_res_view);
}

void DeviceContext::setConstantBuffer(VertexShader* vertex_shader, ConstantBuffer* buffer)
{
	m_device_context->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setConstantBuffer(PixelShader* pixel_shader, ConstantBuffer* buffer)
{
	m_device_context->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::setDepthStencilState()
{
	m_device_context->OMSetDepthStencilState(GraphicsEngine::get()->getStencilState(), 0xFF);
}


void DeviceContext::setPixelShaderSamplers(UINT startSlot, UINT nSamplers, ID3D11SamplerState*  samplerState)
{
	m_device_context->PSSetSamplers(0, 1, &samplerState);
}

void DeviceContext::setShaderResources(UINT startSlot, UINT numViews, ID3D11ShaderResourceView* texture)
{
	m_device_context->PSSetShaderResources(startSlot, numViews, &texture);
}

void DeviceContext::setWireframeRenderMode()
{
	m_device_context->RSSetState(GraphicsEngine::get()->mWireframeRS);
}

void DeviceContext::setSolidRenderMode()
{
	m_device_context->RSSetState(GraphicsEngine::get()->mSolidRS);
}


ID3D11DeviceContext* DeviceContext::getDeviceContext()
{
	return this->m_device_context;
}

bool DeviceContext::release()
{
	m_device_context->Release();
	delete this;
	return true;
}

DeviceContext::~DeviceContext()
{
}