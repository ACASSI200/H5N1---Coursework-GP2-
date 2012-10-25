#include "ModelLoader.h"
#include <fbxsdk.h>

#include "GeometryComponent.h"

//might need this for new versions of the SDK
//#include <fbxsdk/utils/>

CModelLoader::CModelLoader()
{
}

CModelLoader::~CModelLoader()
{
}

CGeometryComponent * CModelLoader::createCube(ID3D10Device *pDevice,float width, float height, float length)
{
	CGeometryComponent * pRenderable=new CGeometryComponent();

    Vertex vertices[] =
    {
		//front
		{D3DXVECTOR3( -width/2.0f, height/2.0f, length/2.0f ),D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,0.0f)}, //0 top left 
        {D3DXVECTOR3( width/2.0f, -height/2.0f, length/2.0f ),D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),D3DXVECTOR2(1.0f,1.0f)},//1 bottom right 
        {D3DXVECTOR3( -width/2.0f, -height/2.0f, length/2.0f ),D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,1.0f)}, //2 bottom left
		{D3DXVECTOR3( width/2.0f, height/2.0f, length/2.0f ), D3DXCOLOR(0.0f,1.0f,0.0f,1.0f), D3DXVECTOR2(1.0f,0.0f)}, //3 top right

		//back
        {D3DXVECTOR3(-width/2.0f, height/2.0f, -length/2.0f ),D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),D3DXVECTOR2(0.0f,0.0f)}, //4 top left
        {D3DXVECTOR3( width/2.0f, -height/2.0f, -length/2.0f ),D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),D3DXVECTOR2(1.0f,1.0f)},//5 bottom right
        {D3DXVECTOR3(-width/2.0f, -height/2.0f, -length/2.0f), D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),D3DXVECTOR2(0.0f,1.0f)}, //6 bottom left
		{D3DXVECTOR3( width/2.0f, height/2.0f, -length/2.0f),D3DXCOLOR(1.0f,1.0f,0.0f,1.0f),D3DXVECTOR2(1.0f,0.0f)} //7 top right
    };

	int indices[]={0,1,2,0,3,1,//front
					4,5,6,4,7,5, //back
					0,6,4,0,2,6, //left
					3,5,7,3,1,5, //right
					0,4,3,0,3,7, //top
					2,6,1,6,5,1	 //bottom
					};

	for (int i=0;i<8;i++)
	{
		pRenderable->addVertex(vertices[i]);
	}

	for (int i=0;i<36;i++)
	{
		pRenderable->addIndex(indices[i]);
	}


	return pRenderable;
}

CGeometryComponent *CModelLoader::loadModelFromFile(ID3D10Device *pDevice,const string& filename)
{
	CGeometryComponent * pRenderable=NULL;
	string extension=filename.substr(filename.find('.')+1);

	if (extension.compare("fbx")==0)
		pRenderable=loadFbxModelFromFile(pDevice,filename);

	return pRenderable;
}

CGeometryComponent * CModelLoader::loadFbxModelFromFile(ID3D10Device *pDevice,const string& filename)
{
	CGeometryComponent * pRenderable=NULL;

	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using our sdk manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
	//Sean: uncomment back to this when you compile, I am using the latest version of fbx SDK
	//KFbxGeometryConverter converter( lSdkManager);
	//Sean: has comment out the line below
	FbxGeometryConverter converter( lSdkManager);

	 
    // Use the first argument as the filename for the importer.
	if(!lImporter->Initialize(filename.c_str(), -1, lSdkManager->GetIOSettings())) {
		return NULL;
    }

	// Create a new scene so it can be populated by the imported file.
    FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");
	FbxAxisSystem SceneAxisSystem = lScene->GetGlobalSettings().GetAxisSystem();
    //FbxAxisSystem::DirectX.ConvertScene( lScene );

    INT iUpAxisSign;
    //Sean: Uncomment this below
	//KFbxAxisSystem::eUpVector UpVector = SceneAxisSystem.GetUpVector( iUpAxisSign );
	//Sean: and comment this out
	FbxAxisSystem::EUpVector UpVector = SceneAxisSystem.GetUpVector( iUpAxisSign );

    // Import the contents of the file into the scene.
    lImporter->Import(lScene);

    // The file has been imported; we can get rid of the importer.
    lImporter->Destroy();

	FbxNode* lRootNode = lScene->GetRootNode();
	FbxMesh * pMesh=NULL;
	if(lRootNode) {
		for (int i=0;i<lRootNode->GetChildCount();i++){
			FbxNode * modelNode=lRootNode->GetChild(i);
			for(int i=0;i<modelNode->GetNodeAttributeCount();i++)
			{
				FbxNodeAttribute *pAttributeNode=modelNode->GetNodeAttributeByIndex(i);
				//Sean: Uncomment this
				//if (pAttributeNode->GetAttributeType()==KFbxNodeAttribute::eMESH)
				//Sean Comment this out
				if (pAttributeNode->GetAttributeType()==FbxNodeAttribute::eMesh)
				{
					//found mesh
					pMesh=(FbxMesh*)pAttributeNode;
					break;
				}
			}
		}
		if (pMesh)
		{
			pMesh=converter.TriangulateMesh(pMesh);
			FbxVector4 * verts=pMesh->GetControlPoints();
			int noVerts=pMesh->GetControlPointsCount();

			int noIndices=pMesh->GetPolygonVertexCount();
			int *pIndices=pMesh->GetPolygonVertices();

			Vertex * pVerts=new Vertex[noVerts];
			for(int i=0;i<noVerts;i++)
			{

					pVerts[i].Pos.x=verts[i][0];
					pVerts[i].Pos.y=verts[i][1];
					pVerts[i].Pos.z=verts[i][2];
			}

			for (int iPolygon = 0; iPolygon < pMesh->GetPolygonCount(); iPolygon++) { 
				for (unsigned iPolygonVertex = 0; iPolygonVertex < 3; iPolygonVertex++) {	
					int fbxCornerIndex = pMesh->GetPolygonVertex(iPolygon, iPolygonVertex);
					FbxVector4 fbxVertex = verts[fbxCornerIndex];

					FbxVector4 fbxNormal;	
					pMesh->GetPolygonVertexNormal(iPolygon, iPolygonVertex, fbxNormal);	
					fbxNormal.Normalize();	
					//pVerts[fbxCornerIndex].Normal=D3DXVECTOR3(fbxNormal[0],fbxNormal[1],fbxNormal[2]);
					FbxVector2 fbxUV = FbxVector2(0.0, 0.0);	
					FbxLayerElementUV* fbxLayerUV = pMesh->GetLayer(0)->GetUVs();
					// Get texture coordinate	
					if (fbxLayerUV) {		
						int iUVIndex = 0;		
						switch (fbxLayerUV->GetMappingMode()) {	
							//Sean Uncomment this
							//case KFbxLayerElement::eBY_CONTROL_POINT:
							//Sean comment the line below out
							case FbxLayerElement::eByControlPoint:
								iUVIndex = fbxCornerIndex;				
							break;	
							//Sean Uncomment this
							//case KFbxLayerElement::eBY_POLYGON_VERTEX:
							//Sean comment the line below out
							case FbxLayerElement::eByPolygonVertex:
								//Sean Uncomment this
								//iUVIndex = pMesh->GetTextureUVIndex(iPolygon, iPolygonVertex, KFbxLayerElement::eDIFFUSE_TEXTURES);	
								//Sean comment this out
								iUVIndex = pMesh->GetTextureUVIndex(iPolygon, iPolygonVertex, FbxLayerElement::eTextureDiffuse);	
							break;		
						}		
						fbxUV = fbxLayerUV->GetDirectArray().GetAt(iUVIndex);	
						//pVerts[fbxCornerIndex].TextureCoords.x=fbxUV[0];
						//pVerts[fbxCornerIndex].TextureCoords.y= 1.0f-fbxUV[1];
					}
				}
			}

			pRenderable=new CGeometryComponent();
			for (int i=0;i<noVerts;i++)
			{
				pRenderable->addVertex(pVerts[i]);
			}
			for (int i=0;i<noIndices;i++)
			{
				pRenderable->addIndex(pIndices[i]);
			}
			//pRenderable->create<TexturedLitVertex>(pDevice,noVerts,noIndices,pVerts,(UINT*)pIndices);
			if (pVerts)
			{
				delete [] pVerts;
				pVerts=NULL;
			}
			//}

		}
    }

	lSdkManager->Destroy();

	

	return pRenderable;
}