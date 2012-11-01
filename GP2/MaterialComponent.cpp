#include "MaterialComponent.h"
#include "DefaultEffect.h"
#include "Vertex.h"

CMaterialComponent::CMaterialComponent()
{
	//set to null
	m_pEffect=NULL;
	m_pVertexLayout=NULL;
	m_pDiffuseTexture=NULL;
	m_EffectName="";
	m_TechniqueName="Render";
	ZeroMemory(&m_TechniqueDesc,sizeof(D3D10_TECHNIQUE_DESC));
	m_strName="MaterialComponent";

	m_DiffuseMaterial=D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
	m_AmbientMaterial=D3DXCOLOR(0.5f,0.5f,0.5f,1.0f);
	m_SpecularMaterial=D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	m_SpecularPower=25.0f;
}

CMaterialComponent::~CMaterialComponent()
{
	//
	if (m_pDiffuseTexture)
	{
		m_pDiffuseTexture->Release();
		m_pDiffuseTexture=NULL;
	}
	//vertex layout
	if (m_pVertexLayout)
	{
		m_pVertexLayout->Release();
		m_pVertexLayout=NULL;
	}
	//effect
	if (m_pEffect)
	{
		m_pEffect->Release();
		m_pEffect=NULL;
	}
}

//load effect from memory
void CMaterialComponent::loadEffectFromMemory(const char * pData)
{
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program. - BMD
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob * pErrorBuffer=NULL;
	if (FAILED(D3DX10CreateEffectFromMemory(pData,
		strlen(pData),
		"DefaultEffect.fx",
		NULL,
		NULL,
		"fx_4_0", //A string which specfies the effect profile to use, in this case fx_4_0(Shader model 4) - BMD
		dwShaderFlags, //Shader flags, this can be used to add extra debug information to the shader - BMD
		0,//Fx flags, effect compile flags set to zero - BMD
        m_pD3D10Device, //ID3D10Device*, the direct3D rendering device - BMD
		NULL, //ID3D10EffectPool*, a pointer to an effect pool allows sharing of variables between effects - BMD
		NULL, //ID3DX10ThreadPump*, a pointer to a thread pump this allows multithread access to shader resource - BMD
		&m_pEffect, //ID3D10Effect**, a pointer to a memory address of the effect object. This will be initialised after this - BMD
		&pErrorBuffer, //ID3D10Blob**, a pointer to a memory address of a blob object, this can be used to hold errors from the compilation - BMD
		NULL )))//HRESULT*, a pointer to a the result of the compilation, this will be NULL - BMD
	{
		//If the creation of the Effect fails then a message box will be shown
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
	}
}

//load effect from file
void CMaterialComponent::loadEffectFromFile(const string &name)
{
	setEffectFilename(name);
	DWORD dwShaderFlags = D3D10_SHADER_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	// Set the D3D10_SHADER_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program. - BMD
	dwShaderFlags |= D3D10_SHADER_DEBUG;
#endif

	ID3D10Blob * pErrorBuffer=NULL;
	//Create the effect - BMD
	//http://msdn.microsoft.com/en-us/library/bb172658%28v=vs.85%29.aspx -BMD
	if( FAILED(D3DX10CreateEffectFromFileA( name.c_str(), //The filename of the effect - BMD
		NULL, //An array of shader macros we leave this NULL - BMD
		NULL, //ID3D10Include*, this allows to include other files when we are compiling the effect - BMD
		"fx_4_0", //A string which specfies the effect profile to use, in this case fx_4_0(Shader model 4) - BMD
		dwShaderFlags, //Shader flags, this can be used to add extra debug information to the shader - BMD
		0,//Fx flags, effect compile flags set to zero - BMD
        m_pD3D10Device, //ID3D10Device*, the direct3D rendering device - BMD
		NULL, //ID3D10EffectPool*, a pointer to an effect pool allows sharing of variables between effects - BMD
		NULL, //ID3DX10ThreadPump*, a pointer to a thread pump this allows multithread access to shader resource - BMD
		&m_pEffect, //ID3D10Effect**, a pointer to a memory address of the effect object. This will be initialised after this - BMD
		&pErrorBuffer, //ID3D10Blob**, a pointer to a memory address of a blob object, this can be used to hold errors from the compilation - BMD
		NULL )))//HRESULT*, a pointer to a the result of the compilation, this will be NULL - BMD
	{
		//If the creation of the Effect fails then a message box will be shown
		OutputDebugStringA((char*)pErrorBuffer->GetBufferPointer());
    }
}

void CMaterialComponent::loadDiffuseTexture(const string& filename)
{
	if (FAILED(D3DX10CreateShaderResourceViewFromFileA(m_pD3D10Device,
		filename.c_str(), NULL, NULL,&m_pDiffuseTexture , NULL)))
	{
		OutputDebugStringA("Can't load Texture");
	}
}

//init
void CMaterialComponent::init()
{
	//load effect
	if (m_EffectName.empty())
	{
		loadEffectFromMemory(pDefaultEffect);
	}
	else
	{
		loadEffectFromFile(m_EffectName);
	}

	//Grab the technique
	m_pTechnique=m_pEffect->GetTechniqueByName(m_TechniqueName.c_str());
	m_pTechnique->GetDesc(&m_TechniqueDesc);
	//create vertex layout
	createVertexLayout();

	//Retrieve all variables using semantic
	m_pWorldMatrixVariable=m_pEffect->GetVariableBySemantic("WORLD")->AsMatrix();
	m_pViewMatrixVariable=m_pEffect->GetVariableBySemantic("VIEW")->AsMatrix();
	m_pProjectionMatrixVariable=m_pEffect->GetVariableBySemantic("PROJECTION")->AsMatrix();
	m_pDiffuseTextureVariable=m_pEffect->GetVariableByName("diffuseMap")->AsShaderResource();

	//lights
	m_pAmbientLightColourVariable=m_pEffect->GetVariableByName("ambientLightColour")->AsVector();
	m_pDiffuseLightColourVariable=m_pEffect->GetVariableByName("diffuseLightColour")->AsVector();
	m_pSpecularLightColourVariable=m_pEffect->GetVariableByName("specularLightColour")->AsVector();
	m_pLightDirectionVariable=m_pEffect->GetVariableByName("lightDirection")->AsVector();

	//Material
	m_pAmbientMaterialVariable=m_pEffect->GetVariableByName("ambientMaterialColour")->AsVector();
	m_pDiffuseMaterialVariable=m_pEffect->GetVariableByName("diffuseMaterialColour")->AsVector();
	m_pSpecularMaterialVariable=m_pEffect->GetVariableByName("specularMaterialColour")->AsVector();
	m_pSpecularPowerVariable=m_pEffect->GetVariableByName("specularPower")->AsScalar();

	//Camera
	m_pCameraPositionVariable=m_pEffect->GetVariableByName("cameraPosition")->AsVector();
}

//create vertex layout
void CMaterialComponent::createVertexLayout()
{
	//Number of elements in the layout - BMD
    UINT numElements = sizeof( layout ) / sizeof(D3D10_INPUT_ELEMENT_DESC);
	//Get the Pass description, we need this to bind the vertex to the pipeline - BMD
    D3D10_PASS_DESC PassDesc;
    m_pTechnique->GetPassByIndex( 0 )->GetDesc( &PassDesc );
	//Create Input layout to describe the incoming buffer to the input assembler - BMD
    if (FAILED(m_pD3D10Device->CreateInputLayout( layout, //The layout describing our vertices - BMD
		numElements, //The number of elements in the layout
		PassDesc.pIAInputSignature,//Input signature of the description of the pass - BMD
        PassDesc.IAInputSignatureSize, //The size of this Signature size of the pass - BMD
		&m_pVertexLayout ))) //The pointer to an address of Vertex Layout - BMD
	{
		OutputDebugStringA("Can't create layout");
	}
}

void CMaterialComponent::setAmbientLightColour(D3DXCOLOR& colour)
{
	m_pAmbientLightColourVariable->SetFloatVector((float*)colour);
}