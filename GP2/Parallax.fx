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

Texture2D diffuseMap;
bool useDiffuseTexture
<
string UIName="Use Diffuse Texture";
> = false;

Texture2D specularMap;
bool useSpecularTexture
<
string UIName="Use Specular Texture";
> = false;

Texture2D bumpMap;
bool useBumpTexture
<
string UIName="Use Bumpmapping";
> = false;

Texture2D heightMap;
bool useHeightTexture
<
string UIName="Use Parrallax";
> = false;

float scale=0.05;
float bias=0.01;

SamplerState wrapSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_INPUT
{
	float4 pos:POSITION;
	float3 normal:NORMAL;
	float3 tangent:TANGENT;
	float2 texCoord:TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos:SV_POSITION;
	float3 normal:NORMAL;
	float3 cameraDirection:VIEWDIR;
	float3 lightDir:LIGHTDIR;
	float2 texCoord:TEXCOORD0;
};

PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output=(PS_INPUT)0;
	
	float4x4 matViewProjection=mul(matView,matProjection);
	float4x4 matWorldViewProjection=mul(matWorld,matViewProjection);
	float4 worldPos=mul(input.pos,matWorld);
	if (useBumpTexture){
		float3x3 worldToTangent;
		worldToTangent[0]=mul(input.tangent,matWorld);
		worldToTangent[1]=mul(cross(input.tangent,input.normal),matWorld);
		worldToTangent[2]=mul(input.normal,matWorld);
		output.normal=normalize(mul(input.normal,worldToTangent));
		output.cameraDirection=mul(normalize(cameraPosition-worldPos),worldToTangent);
		output.lightDir=mul(lightDirection,worldToTangent);		
	}
	else
	{
		output.normal=normalize(mul(input.normal,matWorld));
		output.cameraDirection=mul(normalize(cameraPosition-worldPos),matWorld);
		output.lightDir=lightDirection;			
	}
	
	output.pos=mul(input.pos,matWorldViewProjection);

	output.texCoord=input.texCoord;
	return output;
}



float4 PS(PS_INPUT input):SV_TARGET
{
	float3 normal=input.normal;
	float2 texCoord=input.texCoord;
	
	if (useHeightTexture){
		float heightVal=heightMap.Sample(wrapSampler,input.texCoord).x;
		float height=scale*heightVal-bias;
		texCoord=height*input.cameraDirection.xy+input.texCoord;
	}
	
	
	
	if (useBumpTexture){
		normal=normalize((2*(bumpMap.Sample(wrapSampler,texCoord)))-1.0);
	}
	
	float3 lightDir=normalize(input.lightDir);
	
	float4 diffuseColour=diffuseMaterialColour;
	float4 specularColour=specularMaterialColour;
	if (useDiffuseTexture)
		diffuseColour=diffuseMap.Sample(wrapSampler,texCoord);
	if (useSpecularTexture)
		specularColour=specularMap.Sample(wrapSampler,texCoord);
		
	float diffuse=saturate(dot(normal,lightDir));
	
	float3 halfVec=normalize(lightDir+input.cameraDirection);
	float specular=pow(saturate(dot(normal,halfVec)),specularPower);
	
	return ((ambientMaterialColour*ambientLightColour)+
	(diffuseColour*diffuseLightColour*diffuse)+
	(specularColour*specularLightColour*specular));
}

RasterizerState DisableCulling
{
    CullMode = NONE;
};

technique10 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0,  PS() ) );
		SetRasterizerState(DisableCulling); 
	}
}