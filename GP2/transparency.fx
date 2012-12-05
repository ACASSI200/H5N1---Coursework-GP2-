float4x4 matWorld:WORLD<string UIWidget="None";>;
float4x4 matView:VIEW<string UIWidget="None";>;
float4x4 matProjection:PROJECTION<string UIWidget="None";>;

//Ambient
float4 ambientMaterialColour<
	string UIName="Ambient Material";
	string UIWidget="Color";
> =float4(0.5f,0.5f,0.5f,1.0f);

float4 diffuseMaterialColour<
	string UIName="Diffuse Material";
	string UIWidget="Color";
> =float4(0.8f,0.8f,0.8f,1.0f);

float4 specularMaterialColour<
	string UIName="Specular Material";
	string UIWidget="Color";
> =float4(1.0f,1.0f,1.0f,1.0f);

float4 ambientLightColour:AMBIENT;

float4 lightDirection:DIRECTION<
	string Object = "DirectionalLight";
>;

float4 diffuseLightColour:DIFFUSE<
	string Object = "DirectionalLight";
>;

float4 specularLightColour:SPECULAR<
	string Object = "DirectionalLight";
>;

float specularPower<
	string UIName="Specular Power";
	string UIWidget="Slider";
	float UIMin = 0.0;
	float UIMax = 100.0;
	float UIStep = 5.0;
> =25.0f;

float4 cameraPosition:POSITION<
	string Object ="Perspective";
>;

Texture2D diffuseMap<
string UIName="diffuseMap";
>;

bool useDiffuseMap<
string UIName="useDiffuseMap";
>;

SamplerState wrapSampler
{
	filter=MIN_MAG_MIP_LINEAR;
	AddressU=Clamp;
	AddressV=Clamp;
};


struct VS_INPUT
{
	float4 pos:POSITION;
	float3 normal:NORMAL;
	float2 texCoord:TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos:SV_POSITION;
	float3 normal:NORMAL;
	float3 cameraDirection:VIEWDIR;
	float2 texCoord:TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output=(PS_INPUT)0;
	
	float4x4 matViewProjection=mul(matView,matProjection);
	float4x4 matWorldViewProjection=mul(matWorld,matViewProjection);
	
	output.pos=mul(input.pos,matWorldViewProjection);
	output.normal=normalize(mul(input.normal,matWorld));
	
	float4 worldPos=mul(input.pos,matWorld);
	output.cameraDirection=normalize(cameraPosition-worldPos);
	
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(PS_INPUT input):SV_TARGET
{
	float4 diffuseColour = diffuseMaterialColour;
	
	if(useDiffuseMap)
	{
		diffuseColour = diffuseMap.Sample((wrapSampler),input.texCoord);
	};
	float diffuse=saturate(dot(input.normal,lightDirection));
	
	float3 halfVec=normalize(lightDirection+input.cameraDirection);
	float specular=pow(saturate(dot(input.normal,halfVec)),specularPower);
	
	return ((ambientMaterialColour*ambientLightColour)+
	(diffuseColour*diffuseLightColour*diffuse)+
	(specularMaterialColour*specularLightColour*specular));
}

RasterizerState DisableCulling
{
    CullMode = NONE;
};

BlendState AlphaOn
{
	BLENDENABLE[0] = TRUE;
	SRCBLEND = SRC_ALPHA;
	DESTBLEND = INV_SRC_ALPHA;
	BlendOp = ADD;
	SrcBlendAlpha = ZERO;
	DestBlendAlpha = ZERO;
	BlendOpAlpha = ADD;
	RenderTargetWriteMask[0] = 0x0F;
};

technique10 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0,  PS() ) );
		SetRasterizerState(DisableCulling); 
		SetBlendState(AlphaOn, float4(0.0f, 0.0f, 0.0f, 0.0f),0xFFFFFFFF);
	}
}