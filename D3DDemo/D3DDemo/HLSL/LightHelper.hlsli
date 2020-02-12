//物体的表面材质
struct Material {
	float4 Ambient;	//环境光
	float4 Diffuse;	//漫反射光
	float4 Specular;	//高光
	float4 Reflect;
};

//方向光
struct DirectionalLight {
	float4 Ambient;	//环境光
	float4 Diffuse;	//漫反射光
	float4 Specular;	//高光
	float3 Direction;	//照射方向
	float Pad;	// 最后用一个浮点数填充使得该结构体大小满足16的倍数，便于我们以后在HLSL设置数组
	
};

//点光
struct PointLight {
	float4 Ambient;	//环境光
	float4 Diffuse;	//漫反射光
	float4 Specular;	//高光
	
	float3 Position;	//位置
	float Range;	//光照范围

	float3 Att;		//衰减的系数a
	float Pad;

};

struct SpotLight {

	float4 Ambient;	//环境光
	float4 Diffuse;	//漫反射光
	float4 Specular;	//高光

	float3 Position;	//位置
	float Range;	//光照范围

	float3 Direction;	//光向量方向
	float Spot;		//光的汇聚程度

	float3 Att;	//衰减系数a
	float Pad;

};

void ComputeDirectionalLight(Material mat, DirectionalLight L,
	float3 normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec) {

	//初始化输出
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//光向量和照射方向相反
	float3 lightVec = -L.Direction;

	//添加环境光
	ambient = mat.Ambient * L.Ambient;	//这里的乘号表示各个分量相乘，最终得到的颜色向量为
										//(la.r * ma.r, la.g * ma.g, la.b * ma.b, la.a * ma.a)

	//添加漫反射光和镜面光
	float diffuseFactor = dot(lightVec, normal);	//反射系数 L dot n

	//展开，避免动态分支
	[flatten]
	if(diffuseFactor > 0.0f){	//有漫反射光和镜面光
		float3 r = reflect(-lightVec, normal);	//计算反射向量
		float specFactor = pow(max(dot(r, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}
}

void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// 初始化输出
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	//从表面到光源的向量
	float3 lightVec = L.Position = pos;

	//表面到光源的距离
	float d = length(lightVec);

	//灯光范围
	if (d > L.Range)
		return;		//减少距离过大的物体的漫射光和镜面光的计算

	//标准化光向量
	lightVec /= d;

	//环境光的计算
	ambient = mat.Ambient * L.Ambient;

	//添加漫反射光和镜面光
	float diffuseFactor = dot(lightVec, normal);	//反射系数 L dot n

	//展开，避免动态分支
	[flatten]
	if (diffuseFactor > 0.0f) {	//有漫反射光和镜面光
		float3 r = reflect(-lightVec, normal);	//计算反射向量
		float specFactor = pow(max(dot(r, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	//光的衰减
	float att = 1.0f / dot(L.Att, float3(1.0f, d, d*d));

	diffuse *= att;
	spec *= att;
}

void ComputeSpotLight(Material mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	// 初始化输出
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// // 从表面到光源的向量
	float3 lightVec = L.Position - pos;

	// 表面到光源的距离
	float d = length(lightVec);

	// 范围测试
	if (d > L.Range)
		return;

	// 标准化光向量
	lightVec /= d;

	// 计算环境光部分
	ambient = mat.Ambient * L.Ambient;


	// 计算漫反射光和镜面反射光部分
	float diffuseFactor = dot(lightVec, normal);

	// 展开以避免动态分支
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	// 计算汇聚因子和衰弱系数
	float spot = pow(max(dot(-lightVec, L.Direction), 0.0f), L.Spot);	//汇聚因子
	float att = spot / dot(L.Att, float3(1.0f, d, d * d));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}

