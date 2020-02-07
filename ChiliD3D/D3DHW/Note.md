# Direct 3D学习笔记

### D3D简述

Direct3D 是一种底层绘图 API，位于应用程序和绘图硬件之间，这样我们就不必担心 3D 硬件的 实现细节，只要设备支持 Direct3D11，我们就可以通过 Direct3D11API 来控制 3D 硬件了。 

### COM

组件对象模型（COM）技术使 DirectX 独立于任何编程语言，并具有版本向后兼容的特 性。我们经常把 COM 对象称为接口，并把它当成一个普通的 C++类来使用。

须通过特定的函数或其他的COM接口方法来获取指向COM接口的指针，而不能用C++的 new 关键字来创建 COM 接口.

当我们不再使用某个接口时，必须调用它的 Release 方法来释放它（所有的 COM 接口都继承于 IUnknown 接口，而 Release 方法是 IUnknown 接口的成员），而不能用 delete 语句——COM 对象在其自身内部实现所有的内存管理工作。 


### 纹理和数据资源格式（Texture）

2D 纹理（texture）是一种数据元素矩阵。2D 纹理的用途之一是存储 2D 图像数据，在 纹理的每个元素中存储一个像素颜色。

纹理只支持特定格式的数据存储，这些格式由 DXGI_FORMAT 枚举类型描述。一些常用的格式如下：  
	
	DXGI_FORMAT_R32G32B32_FLOAT：每个元素包含 3 个 32 位浮点分量。  
	DXGI_FORMAT_R16G16B16A16_UNORM：每个元素包含 4 个 16 位分量，分量 的取值范围在[0,1]区间内。 
	DXGI_FORMAT_R32G32_UINT：每个元素包含两个 32 位无符号整数分量。  
	DXGI_FORMAT_R8G8B8A8_UNORM：每个元素包含 4 个 8 位无符号分量，分 量的取值范围在[0,1]区间内。 
	DXGI_FORMAT_R8G8B8A8_SNORM：每个元素包含 4 个 8 位有符号分量，分 量的取值范围在[−1,1] 区间内。 
	DXGI_FORMAT_R8G8B8A8_SINT：每个元素包含 4 个 8 位有符号整数分量，分 量的取值范围在[−128,127] 区间内。 
	DXGI_FORMAT_R8G8B8A8_UINT：每个元素包含 4 个 8 位无符号整数分量，分 量的取值范围在[0,255]区间内。

 字母 R、G、B、A 分别表示 red（红）、green（绿）、blue（蓝）和 alpha（透明度）

### 交换链和页面翻转(Swap chain and Page flipping)

为了避免在动画中出现闪烁，最好的做法是在一个离屏（off-screen）纹理中执行所有的 动画帧绘制工作，这个离屏纹理称为后台缓冲区（back buffer）。当我们在后台缓冲区中完 成给定帧的绘制工作后，便可以将后台缓冲区作为一个完整的帧显示在屏幕上；使用这种方 法，用户不会察觉到帧的绘制过程，只会看到完整的帧。

硬件会自动维护两个内置的纹理缓冲区来实现这一 功能，这两个缓冲区分别称为前台缓冲区（frontbuffer）和后台缓冲区。前台缓冲区存储 了当前显示在屏幕上的图像数据，而动画的下一帧会在后台缓冲区中执行绘制。当后台缓冲 区的绘图工作完成之后，前后两个缓冲区的作用会发生翻转：后台缓冲区会变为前台缓冲区， 而前台缓冲区会变为后台缓冲区，为下一帧的绘制工作提前做准备。我们将前后缓冲区功能 互换的行为称做呈现（presenting）

前后缓冲区形成了一个交换链（swap chain）。在 Direct3D 中 ， 交换链由IDXGISwapChain 接口表示。该接口保存了前后缓冲区纹理，并提供了用于调整缓冲区尺寸的方法（IDXGISwapChain ::ResizeBuffers）和呈现方法（IDXGISwapChain ::Present）。 

### 深度缓冲区

深度缓冲区（depth buffer）是一个不包含图像数据的纹理对象。在一定程度上，深度 信息可以被认为是一种特殊的像素。常见的深度值范围在 0.0 到 1.0 之间，其中 0.0 表示离 观察者最近的物体，1.0 表示离观察者最远的物体。

深度缓冲区用于为每个像素计算深度值和实现深度测试。深度测试通过比较 像素深度来决定是否将该像素写入后台缓冲区的特定像素位置。只有离观察者最近的像素才 会胜出，成为写入后台缓冲区的最终像素。这很容易理解，因为离观察者最近的像素会遮挡 它后面的其他像素。

深度缓冲区是一个纹理，所以在创建它时必须指定一种数据格式。用于深度缓存的格式 如下：  

	DXGI_FORMAT_D32_FLOAT_S8X24_UINT：32 位浮点深度缓冲区。为模板缓冲区预留8位（无符号整数），每个模板值的取值范围为[0,255]。其余 24 位闲置。
	DXGI_FORMAT_D32_FLOAT：32 位浮点深度缓冲区。 
	DXGI_FORMAT_D24_UNORM_S8_UINT：无符号 24 位深度缓冲区，每个深度值的 取值范围为[0,1]。为模板缓冲区预留 8 位（无符号整数），每个模板值的取值范围为 [0,255]。 
	DXGI_FORMAT_D16_UNORM：无符号 16 位深度缓冲区，每个深度值的取值范围为 [0,1]。 

### 纹理资源视图

纹理可以被绑定到渲染管线（renderingpipeline）的不同阶段（stage）； 

常见的情况是将纹理作为渲染目标（即，Direct3D 渲染到纹理）或着色器资源（即，在着色器 中对纹理进行采样）。

当创建用于这两种目的的纹理资源时，应使用绑定标志值： D3D11_BIND_RENDER_TARGET|D3D10_BIND_SHADER_RESOURCE 

资源不能被直接绑定到一个管线阶段；我们 只能把与资源关联的资源视图绑定到不同的管线阶段。无论以哪种方式使用纹理，Direct3D 始终要求我们在初始化时为纹理创建相关的资源视图（resourceview）。

当把纹理作为一个渲染目标和着色器资源时， 我们要为它创建两种视图：渲染目标视图（ID3D11RenderTargetView）和着色器资源视图 （ID3D11ShaderResourceView）。

资源视图主要有两个功能：

（1）告诉 Direct3D 如何使用资源（即，指定资源所要绑定的管线阶段）； 

（2）如果在创建资源时指定的是弱类型（typeless） 格式，那么在为它创建资源视图时就必须指定明确的资源类型。对于弱类型格式，纹理元素 可能会在一个管线阶段中视为浮点数，而在另一个管线阶段中视为整数。 


### 多重采样（multi-sampling）---解决像素锯齿

当无法提高显示器分辨率或分辨率不够高时，我们可以使用抗锯齿（antialiasing）技术。 

一种技术叫做超级采样（supersampling），它把后台缓冲和深度缓冲的大小提高到屏 幕分辨率的 4 倍。3D 场景会以这个更大的分辨率渲染到后台缓存中，当在屏幕上呈现后台 缓冲时，后台缓冲会将 4 个像素的颜色取平均值后得到一个像素的最终颜色。从效果上来说， 超级采样的工作原理就是以软件的方式提升分辨率。

另一种称为多重采样（multisampling）的抗锯齿技术，它通过对一个像素的 子像素进行采样计算出该像素的最终颜色，比超级采样节省资源。假如我们使用的是 4X 多 重采样（每个像素采样 4 个邻接像素），多重采样仍然会使用屏幕分辨率 4 倍大小的后台缓 冲和深度缓冲，但是，不像超级采样那样计算每个子像素的颜色，而是只计算像素中心颜色 一次，然后基于子像素的可见性（基于子像素的深度/模板测试）和范围（子像素中心在多 边形之外还是之内）共享颜色信息。

supersampling 与 multisampling 的关键区别在于：使用 supersampling 时，图像的 颜色需要通过每个子像素的颜色计算得来，而每个子像素颜色可能不同；使用 multisampling （图 4.5）时，每个像素的颜色只计算一次，这个颜色会填充到所有可见的、被多边形覆盖 的子像素中，即这个颜色是共享的。

### Direct3D 中的多重采样

填充一个 DXGI_SAMPLE_DESC 结构体。

	typedef struct DXGI_SAMPLE_DESC { 
		UINT Count; UINT Quality; 
	} DXGI_SAMPLE_DESC, *LPDXGI_SAMPLE_DESC; 

Count 成员用于指定每个像素的采样数量，Quality 成员用于指定希望得到的质量级别 （不同硬件的质量级别表示的含义不一定相同）。质量级别越高，占用的系统资源就越多

可以使用如下方法，通过指定纹理格式和采样数量来查询相应的质量级别： 

	HRESULT ID3D11Device::CheckMultisampleQualityLevels( DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels); 

如果纹理格式和采样数量的组合不被设备支持，则该方法返回 0。反之，通过
pNumQualityLevels 参数返回符合给定的质量等级数值。有效的质量级别范围为 0 到 pNumQualityLevels−1。 


采样数量通常使用 4 或 8，可以兼顾性能和内存消耗。

需要为交换链缓冲区和深度缓冲区各填充一个 DXGI_SAMPLE_DESC 结构 体。当创建后台缓冲区和深度缓冲区时，必须使用相同的多重采样设置；

需要为交换链缓冲区和深度缓冲区各填充一个 DXGI_SAMPLE_DESC 结构 体。当创建后台缓冲区和深度缓冲区时，必须使用相同的多重采样设置；

### 特征级别--支持的D3D特征等级

 特 征 等 级 （ feature levels ， 在 代 码 中 由 枚 举 类 型 D3D_FEATURE_LEVEL 表示）

特征等级定义了一系列支持不同 d3d 功能的相应的等级（每个特征等级支持的功能可参 见 SDK 文档），用意即如果一个用户的硬件不支持某一特征等级，程序可以选择较低的等级。 

程序会 从最新的硬件一直检查到最旧的

	D3D_FEATURE_LEVEL featureLevels [4] = { 
		D3D_FEATURE_LEVEL_11_0, // First check D3D 11 support 
		D3D_FEATURE_LEVEL_10_1, // Second check D3D 10.1 support 
		D3D_FEATURE_LEVEL_10_0, // Next,check D3D 10 support 
		D3D_FEATURE_LEVEL_9_3 // Finally,check D3D 9.3 support 
	} ; 

### [使用ComPtr管理Direct 3D com object](https://github.com/traviszeng/D3DHW/commit/0af998e64bc2c8884bc942c2c00a11274ded5fac)


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


