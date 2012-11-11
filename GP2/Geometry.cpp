#include "Geometry.h"

//COnstructor
CGeometry::CGeometry(ID3D10Device *pDevice)
{
	//everythign set to null
	m_pD3D10Device=pDevice;
	m_pVertexBuffer=NULL;
	m_pIndexBuffer=NULL;
}

//Deconstructor
CGeometry::~CGeometry()
{
	//Release the vertex buffer
	if (m_pVertexBuffer)
	{
		m_pVertexBuffer->Release();
		m_pVertexBuffer=NULL;
	}
	//Release the index buffer
	if (m_pIndexBuffer)
	{
		m_pIndexBuffer->Release();
		m_pIndexBuffer=NULL;
	}
}

//Init function, used to initialise the component
void CGeometry::createBuffers()
{
	//Call create vertex buffer
	createVertexBuffer();
	//Call create index buffer
	createIndexBuffer();
}

//Add vertex
void CGeometry::addVertex(Vertex vert)
{
	//Add vertex to the vector
	m_Vertices.push_back(vert);
}

//Add index 
void CGeometry::addIndex(int index)
{
	//Add index to the index buffer
	m_Indices.push_back(index);
}

//Create vertex buffer
void CGeometry::createVertexBuffer()
{
	//if we have no vertices don't create the buffer
	if (m_Vertices.size()>0)
	{
		//Buffer desc
		D3D10_BUFFER_DESC bd;
		bd.Usage = D3D10_USAGE_DEFAULT;//Usuage flag,this describes how the buffer is read/written to. Default is the most common - BMD
		bd.ByteWidth = sizeof( Vertex ) * m_Vertices.size();//The size of the buffer, this is the size of one vertex * by the num of vertices -BMD
		bd.BindFlags = D3D10_BIND_VERTEX_BUFFER;//BindFlags, says how the buffer is going to be used. In this case as a Vertex Buffer - BMD
		bd.CPUAccessFlags = 0;//CPUAccessFlag, sepcfies if the CPU can access the resource. 0 means no CPU access - BMD
		bd.MiscFlags = 0;//MiscCreation flags, this will be zero most of the time - BMD

		//This is used to supply the initial data for the buffer - BMD
		//http://msdn.microsoft.com/en-us/library/bb172456%28VS.85%29.aspx - BMD
		D3D10_SUBRESOURCE_DATA InitData;
		//A pointer to the initial data
		InitData.pSysMem = &m_Vertices.at(0);
    
		//Create the Buffer using the buffer description and initial data - BMD
		//http://msdn.microsoft.com/en-us/library/bb173544%28v=VS.85%29.aspx - BMD
		if (FAILED(m_pD3D10Device->CreateBuffer( 
			&bd, //Memory address of a buffer description - BMD
			&InitData, //Memory address of the initial data - BMD
			&m_pVertexBuffer )))//A pointer to a memory address of a buffer, this will be initialise after - BMD
		{
			OutputDebugStringA("Can't create vertex buffer");
		}
	}
}

//Create index buffer
void CGeometry::createIndexBuffer()
{
	//if we have no indices don't create
	if (m_Indices.size()>0)
	{
		//Index buffer desc
		D3D10_BUFFER_DESC ibBd;
		ibBd.Usage=D3D10_USAGE_DEFAULT;
		ibBd.ByteWidth=sizeof(int)*m_Indices.size();
		ibBd.BindFlags=D3D10_BIND_INDEX_BUFFER;
		ibBd.CPUAccessFlags=0;
		ibBd.MiscFlags=0;
		D3D10_SUBRESOURCE_DATA IndexBufferInitData;
		//A pointer to the initial data
		IndexBufferInitData.pSysMem = &m_Indices.at(0);

		//create the index buffer
		if (FAILED(m_pD3D10Device->CreateBuffer(&ibBd,&IndexBufferInitData,&m_pIndexBuffer)))
		{
			OutputDebugStringA("Can't create index buffer");
		}
	}
}

//Bind the vertex buffer and index buffer
void CGeometry::bindBuffers()
{
    //Get the stride(size) of the a vertex, we need this to tell the pipeline the size of one vertex - BMD
    UINT stride = sizeof( Vertex );
    //The offset from start of the buffer to where our vertices are located - BMD
    UINT offset = 0;
    //Bind the vertex buffer to input assembler stage - BMD
    //http://msdn.microsoft.com/en-us/library/bb173591%28v=VS.85%29.aspx - BMD
    m_pD3D10Device->IASetVertexBuffers( 
		0, //The input slot to bind, zero indicates the first slot - BMD
		1, //The number of buffers - BMD
		&m_pVertexBuffer, //A pointer to an array of vertex buffers - BMD
		&stride, //Pointer to an array of strides of vertices in the buffer - BMD
		&offset );//Pointer to an array of offsets to the vertices in the vertex buffers - BMD

	//set index buffer
	m_pD3D10Device->IASetIndexBuffer(m_pIndexBuffer,DXGI_FORMAT_R32_UINT,0);
}