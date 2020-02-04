#pragma once
#include"ChilliWin.h"
#include<d3d11.h>

class Graphics {

public:
	Graphics(HWND hWnd);
	~Graphics();
	//delete copy constructor and = operator
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	void EndFrame();
	void ClearBuffer(float red, float green, float blue) noexcept;

private:
	//必要的3个指针
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
	
};