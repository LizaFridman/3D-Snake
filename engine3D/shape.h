#pragma once
#include "MovableGLM.h"
#include "mesh.h"
#include "lineMesh.h"
#include "texture.h"
class Shape : public MovableGLM
{
	Mesh *mesh;
	LineMesh *lineMesh;
	Texture *tex;
	bool isCopy;
public:
	enum{triangles,lines};
	Shape(const Shape& shape);
	Shape(const std::string& fileName);
	Shape(const std::string& fileName,const std::string& textureFileName);
	Shape(int CylParts, int linkPosition);
	Shape(int CylParts, int linkPosition,const std::string& textureFileName);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string& textureFileName);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	std::vector<glm::vec3> getVertexPositions();
	void draw(int mode);
	unsigned int getTexture();
	virtual ~Shape(void);
};

