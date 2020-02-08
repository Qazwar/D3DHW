#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;

const D3D11_INPUT_ELEMENT_DESC GameApp::VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{

}

void GameApp::DrawScene() {
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);

	static float blue[4] = { 0.0f, 0.0f, 0.f, 1.f };// RGBA = (0,0,255,255)

	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blue);
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// 绘制三角形
	m_pd3dImmediateContext->Draw(3, 0);
	HR(m_pSwapChain->Present(0, 0));
}

bool GameApp::InitEffect()
{
	ComPtr<ID3D10Blob> blob;

	//创建顶点着色器
	HR(CreateShaderFromFile(
		L"HLSL\\Triangle_VS.cso",
		L"HLSL\\Triangle_VS.hlsl",
		"VS",
		"vs_5_0",
		blob.ReleaseAndGetAddressOf())
	);
	HR(m_pd3dDevice->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		m_pVertexShader.GetAddressOf()
	));

	// 创建并绑定顶点布局
	HR(m_pd3dDevice->CreateInputLayout(
		VertexPosColor::inputLayout,
		ARRAYSIZE(VertexPosColor::inputLayout),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		m_pVertexLayout.GetAddressOf()
	));

	// 创建像素着色器
	HR(CreateShaderFromFile(
		L"HLSL\\Triangle_PS.cso",
		L"HLSL\\Triangle_PS.hlsl",
		"PS",
		"ps_5_0",
		blob.ReleaseAndGetAddressOf()
	));
	HR(m_pd3dDevice->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		nullptr,
		m_pPixelShader.GetAddressOf()
	));

	return true;
}

bool GameApp::InitResource()
{
	// 设置三角形顶点
	// 注意三个顶点的给出顺序应当按顺时针排布
	VertexPosColor vertices[] =
	{
		
		{ XMFLOAT3(0.0f, 0.5f, 0.5f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.5f, -0.5f, 0.5f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, 0.5f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) }
	};

	// 设置顶点缓冲区描述
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;				//cpu gpu读写权限相关
	vbd.ByteWidth = sizeof vertices;				//数据字节数
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//缓冲区类型的标志
	vbd.CPUAccessFlags = 0;							// CPU读写权限的指定

	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertices;					// 用于初始化的数据
	HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));


	// ******************
	// 给渲染管线各个阶段绑定好所需资源
	//

	// 输入装配阶段的顶点缓冲区设置
	UINT stride = sizeof(VertexPosColor);	// 跨越字节数
	UINT offset = 0;						// 起始偏移量

	m_pd3dImmediateContext->IASetVertexBuffers(
			0,								// [In]输入槽索引
			1,								// [In]缓冲区数目
			m_pVertexBuffer.GetAddressOf(), // [In]指向缓冲区数组的指针
			&stride,						// [In]一个数组，规定了对所有缓冲区每次读取的字节数分别是多少
			&offset);						// [In]一个数组，规定了对所有缓冲区的初始字节偏移量

	// 设置图元类型，设定输入布局
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
	// 将着色器绑定到渲染管线
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);

	// ******************
	// 设置调试对象名
	//
	D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosColorLayout");
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
	D3D11SetDebugObjectName(m_pVertexShader.Get(), "Trangle_VS");
	D3D11SetDebugObjectName(m_pPixelShader.Get(), "Trangle_PS");

	return true;
}
