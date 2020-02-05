# Direct 3D学习笔记

### 使用ComPtr管理Direct 3D com object

见：

在使用ComPtr之前，我们在建立Direct 3D的COM对象时需要在使用完成后手动Release

例如：

	D3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;

	// gain access to texture subresource in swap chain (back buffer)
	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
	//如果在这里抛出异常，那么pBackBuffer将不会被Release，则会产生指针悬空的问题
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget));
	pBackBuffer->Release();


那么有没有类似C++原生的智能指针的机制来防止这样的问题？

可以使用Microsoft:: WRL ::ComPtr来达到类似智能指针的效果

demo:

	#include <wrl.h>

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO( pSwap->GetBuffer( 0,__uuidof(ID3D11Resource),&pBackBuffer ) );
	GFX_THROW_INFO( pDevice->CreateRenderTargetView( pBackBuffer.Get(),nullptr,&pTarget ) );

ComPtr是一个指向具体Com指针的指针，当需要具体Com指针的时候则调用ComPtr的Get方法即可获取。

为什么不用unique_ptr?

1.unique_ptr在释放内存的时候需要调用delete关键字，如果重载对应Com obj的delete关键字也可以达到该效果，但ComPtr已经实现该步骤。

2.需要使用com obj的地址（指向指针的指针）,ComPtr重载了&运算符

当ComPtr使用&运算符时会先调用Release(),再取地址。

倘若不想调用Release(),可以调用GetAddressOf().

3.Com obj可以被多个对象引用，而unique_ptr仅可被一个对象引用


