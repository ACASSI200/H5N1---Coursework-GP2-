#pragma once

#include "IComponent.h"

#include <D3D10.h>
#include <D3DX10.h>

#include <string>

using namespace std;

//Material component, represent an effect and associated 
//information to render an object
class CMaterialComponent:public CBaseComponent
{
public: 
	CMaterialComponent();

	virtual ~CMaterialComponent();

	//set effect filename
	void setEffectFilename(const string& name)
	{
		m_EffectName=name;
	};

	//set technique name we are going to grab
	void setTechniqueName(const string& name)
	{
		m_TechniqueName=name;
	};

	//init, virtual so we can override later
	virtual void init();

	//load effect from file
	void loadEffectFromFile(const string &name);
	//load effect from memory
	void loadEffectFromMemory(const char * pData);
	//create vertex layout
	void createVertexLayout();

	//get number of passes
	UINT getNumberOfPasses()
	{
		return m_TechniqueDesc.Passes;
	};

	//Apply the current pass, if we have a technique
	void applyPass(UINT currentIndex)
	{
		m_pCurrentPass=m_pTechnique->GetPassByIndex(currentIndex);
		m_pCurrentPass->Apply(0);
	};

	void loadDiffuseTexture(const string& filename);

	void setTextures()
	{
		m_pDiffuseTextureVariable->SetResource(m_pDiffuseTexture);
	};
	//set the world matrix
	void setWorldMatrix(float * pMatrix)
	{
		m_pWorldMatrixVariable->SetMatrix(pMatrix);
	};

	//set projection matrix
	void setProjectionMatrix(float * pMatrix)
	{
		m_pProjectionMatrixVariable->SetMatrix(pMatrix);
	};

	//set view matrix
	void setViewMatrix(float * pMatrix)
	{
		m_pViewMatrixVariable->SetMatrix(pMatrix);
	};

	void setAmbientLightColour(D3DXCOLOR& colour);

	//bind the vertex layout
	void bindVertexLayout()
	{
		m_pD3D10Device->IASetInputLayout(m_pVertexLayout);
	};


protected:
	//effect variables
	ID3D10Effect*           m_pEffect;
	ID3D10EffectTechnique*  m_pTechnique;
	ID3D10EffectPass *m_pCurrentPass;
	//input layout
	ID3D10InputLayout*      m_pVertexLayout;

	//effect name
	string m_EffectName;
	//technique name
	string m_TechniqueName;

	//technique desc
	D3D10_TECHNIQUE_DESC m_TechniqueDesc;

	//effect variables(constants)
	ID3D10EffectMatrixVariable * m_pViewMatrixVariable;
	ID3D10EffectMatrixVariable * m_pProjectionMatrixVariable;
	ID3D10EffectMatrixVariable * m_pWorldMatrixVariable;
	ID3D10EffectShaderResourceVariable *m_pDiffuseTextureVariable;

	//Colour variables
	ID3D10EffectVectorVariable *m_pAmbientColourVariable;

	//Color
	D3DXCOLOR m_AmbientColour;

	//Textures
	ID3D10ShaderResourceView *m_pDiffuseTexture;
};