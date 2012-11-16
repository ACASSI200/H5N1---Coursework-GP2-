#include "ModelLoader.h"
#include <fbxsdk.h>

#include "MeshComponent.h"

//might need this for new versions of the SDK
//#include <fbxsdk/utils/>

CModelLoader::CModelLoader()
{
}

CModelLoader::~CModelLoader()
{
}

CMeshComponent * CModelLoader::createCube(ID3D10Device *pDevice,float width, float height, float length)
{
	CMeshComponent * pMesh=new CMeshComponent();

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

	CGeometry * pGeom=new CGeometry(pDevice);
	for (int i=0;i<8;i++)
	{
		pGeom->addVertex(vertices[i]);
	}

	for (int i=0;i<36;i++)
	{
		pGeom->addIndex(indices[i]);
	}
	pMesh->addSubset(pGeom);

	return pMesh;
}

CMeshComponent *CModelLoader::loadModelFromFile(ID3D10Device *pDevice,const string& filename)
{
	CMeshComponent * pRenderable=NULL;
	string extension=filename.substr(filename.find('.')+1);

	if (extension.compare("fbx")==0)
		pRenderable=loadFbxModelFromFile(pDevice,filename);

	return pRenderable;
}


CMeshComponent * CModelLoader::loadFbxModelFromFile(ID3D10Device *pDevice,const string& filename)
{
	CMeshComponent * pMeshComponent=new CMeshComponent();
	int noVerts=0;
	int noIndices=0;
	int *pIndices=NULL;
	Vertex * pVerts=NULL;

	FbxManager* lSdkManager = FbxManager::Create();
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
    lSdkManager->SetIOSettings(ios);

    // Create an importer using our sdk manager.
    FbxImporter* lImporter = FbxImporter::Create(lSdkManager,"");
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
				if (pAttributeNode->GetAttributeType()==FbxNodeAttribute::eMesh)
				{
					//found mesh
					pMesh=(FbxMesh*)pAttributeNode;
					if (pMesh)
					{
						CGeometry *pGeom=new CGeometry(pDevice);
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
								pVerts[fbxCornerIndex].Normal=D3DXVECTOR3(fbxNormal[0],fbxNormal[1],fbxNormal[2]);

								FbxVector2 fbxUV = FbxVector2(0.0, 0.0);	
								FbxLayerElementUV* fbxLayerUV = pMesh->GetLayer(0)->GetUVs();
								// Get texture coordinate	
								if (fbxLayerUV) {		
									int iUVIndex = 0;		
									switch (fbxLayerUV->GetMappingMode()) {	
										case FbxLayerElement::eByControlPoint:
											iUVIndex = fbxCornerIndex;				
										break;	
										case FbxLayerElement::eByPolygonVertex:
											iUVIndex = pMesh->GetTextureUVIndex(iPolygon, iPolygonVertex, FbxLayerElement::eTextureDiffuse);	
										break;		
									}		
									fbxUV = fbxLayerUV->GetDirectArray().GetAt(iUVIndex);	
									pVerts[fbxCornerIndex].TexCoords.x=fbxUV[0];
									pVerts[fbxCornerIndex].TexCoords.y= 1.0f-fbxUV[1];
								}
							}
						}

						computeTangents(pVerts,noVerts);
			
						for (int i=0;i<noVerts;i++)
						{
							pGeom->addVertex(pVerts[i]);
						}
						for (int i=0;i<noIndices;i++)
						{
							pGeom->addIndex(pIndices[i]);
						}
			
						if (pVerts)
						{
							delete [] pVerts;
							pVerts=NULL;
						}

						pMeshComponent->addSubset(pGeom);
					}
				}
			}
		}
    }

	lSdkManager->Destroy();


	

	return pMeshComponent;
}


void CModelLoader::computeTangents(Vertex *pVerts,int vertexCount)
{
	int triCount=vertexCount/3;
	D3DXVECTOR3 * tan1=new D3DXVECTOR3[vertexCount];
	D3DXVECTOR3 * tan2=new D3DXVECTOR3[vertexCount];

	for (int i=0;i<triCount;i+=3)
	{
		D3DXVECTOR3 v1=pVerts[i].Pos;
		D3DXVECTOR3 v2=pVerts[i+1].Pos;
		D3DXVECTOR3 v3=pVerts[i+2].Pos;

		D3DXVECTOR2 uv1=pVerts[i].TexCoords;
		D3DXVECTOR2 uv2=pVerts[i+1].TexCoords;
		D3DXVECTOR2 uv3=pVerts[i+2].TexCoords;
			
		float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;

        float s1 = uv2.x - uv1.x;
        float s2 = uv3.x - uv1.x;
        float t1 = uv2.y - uv1.y;
        float t2 = uv3.y - uv1.y;

		float r=1.0f/(s1*t2-s2*t1);
		D3DXVECTOR3 sdir = D3DXVECTOR3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
        D3DXVECTOR3 tdir = D3DXVECTOR3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

        tan1[i] += sdir;
        tan1[i+1] += sdir;
        tan1[i+2] += sdir;

        tan2[i] += tdir;
        tan2[i+1] += tdir;
        tan2[i+2] += tdir;
	}
	for (int i=0;i<vertexCount;i++)
	{
		D3DXVECTOR3 n=pVerts[i].Normal;
		D3DXVECTOR3 t=tan1[i];
		D3DXVECTOR3 tmp = (t - n * D3DXVec3Dot(&n, &t));
		D3DXVec3Normalize(&tmp,&tmp);
		pVerts[i].Tangent = D3DXVECTOR3(tmp.x, tmp.y, tmp.z);
        //tangents[a].w = (Vector3.Dot(Vector3.Cross(n, t), tan2[a]) < 0.0f) ? -1.0f : 1.0f;

	}
	if (tan1)
	{
		delete [] tan1;
		tan1=NULL;
	}
	if (tan2)
	{
		delete [] tan2;
		tan2=NULL;
	}
}