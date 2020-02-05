# Direct 3Dѧϰ�ʼ�

### ʹ��ComPtr����Direct 3D com object

����

��ʹ��ComPtr֮ǰ�������ڽ���Direct 3D��COM����ʱ��Ҫ��ʹ����ɺ��ֶ�Release

���磺

	D3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;

	// gain access to texture subresource in swap chain (back buffer)
	ID3D11Resource* pBackBuffer = nullptr;
	GFX_THROW_INFO(pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer)));
	//����������׳��쳣����ôpBackBuffer�����ᱻRelease��������ָ�����յ�����
	GFX_THROW_INFO(pDevice->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget));
	pBackBuffer->Release();


��ô��û������C++ԭ��������ָ��Ļ�������ֹ���������⣿

����ʹ��Microsoft:: WRL ::ComPtr���ﵽ��������ָ���Ч��

demo:

	#include <wrl.h>

	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;

	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO( pSwap->GetBuffer( 0,__uuidof(ID3D11Resource),&pBackBuffer ) );
	GFX_THROW_INFO( pDevice->CreateRenderTargetView( pBackBuffer.Get(),nullptr,&pTarget ) );

ComPtr��һ��ָ�����Comָ���ָ�룬����Ҫ����Comָ���ʱ�������ComPtr��Get�������ɻ�ȡ��

Ϊʲô����unique_ptr?

1.unique_ptr���ͷ��ڴ��ʱ����Ҫ����delete�ؼ��֣�������ض�ӦCom obj��delete�ؼ���Ҳ���Դﵽ��Ч������ComPtr�Ѿ�ʵ�ָò��衣

2.��Ҫʹ��com obj�ĵ�ַ��ָ��ָ���ָ�룩,ComPtr������&�����

��ComPtrʹ��&�����ʱ���ȵ���Release(),��ȡ��ַ��

�����������Release(),���Ե���GetAddressOf().

3.Com obj���Ա�����������ã���unique_ptr���ɱ�һ����������


