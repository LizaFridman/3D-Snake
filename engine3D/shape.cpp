#include "shape.h"

Shape::Shape(const Shape& shape)
{
	if(shape.mesh)
		mesh = shape.mesh;
	if(shape.lineMesh)
		lineMesh = shape.lineMesh;
	tex = shape.tex;
	isCopy = true;
}
Shape::Shape(const std::string& fileName){
	mesh = new Mesh(fileName);
	tex = 0;
	isCopy = false;
}

Shape::Shape(const std::string& fileName,const std::string& textureFileName){
	mesh = new Mesh(fileName); 
	tex = new Texture(textureFileName);
	isCopy = false;
}

Shape::Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices);
	tex = 0; 
	isCopy = false;
}

Shape::Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{

	lineMesh = new LineMesh(vertices,numVertices,indices,numIndices);
	tex = 0; 
	isCopy = false;
}


Shape::Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices,const std::string& textureFileName)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices);
	tex = new Texture(textureFileName);
	isCopy = false;
}

	Shape::Shape(int CylParts,int linkPosition)
	{
		mesh = new Mesh(CylParts,linkPosition);
		tex = 0; 
		isCopy = false;
	}
	Shape::Shape(int CylParts,int linkPosition,const std::string& textureFileName)
	{
		mesh = new Mesh(CylParts,linkPosition);
		tex = new Texture(textureFileName); 
		isCopy = false;
	}

void Shape::addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices);
	tex = new Texture(textureFileName);
}

void Shape::addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	mesh = new Mesh(vertices,numVertices,indices,numIndices);
}

void Shape::addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices)
{
	lineMesh = new LineMesh(vertices,numVertices,indices,numIndices);
}

std::vector<glm::vec3> Shape::getVertexPositions()
{
	if (mesh)
		return mesh->getIndexedModel().positions;

	return IndexedModel().positions;
}

void Shape::draw(int mode)
{
	//transformations
	if(tex)
		tex->Bind();
	if(mesh)
		mesh->Draw(mode);
	if(lineMesh)
		lineMesh->Draw();
}

unsigned int Shape::getTexture()
{
	return tex->getTexture();
}

Shape::~Shape(void)
{
	if(!isCopy)
	{
		if(mesh)
 			delete mesh;
		if(lineMesh)
			delete lineMesh;
		if(tex)
			delete tex;
	}
}
