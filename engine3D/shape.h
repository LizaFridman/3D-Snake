#pragma once
#include "MovableGLM.h"
#include "mesh.h"
#include "lineMesh.h"
#include "texture.h"
#include "kdtree.h"

enum ShapeType { SNAKE_LINK, OBSTICLE_BOX, FOOD, WALL, FLOOR, CAVE, INVISIBLE };

class Shape : public MovableGLM
{
	LineMesh *lineMesh;
	Texture *tex;
	bool isCopy;

public:
	Mesh * mesh;
	Mesh *surrounding_box;
	Mesh *collision_box;
	ShapeType type;

public:
	enum{triangles,lines};
	Shape(const Shape& shape);
	Shape(const std::string& fileName);
	Shape(const std::string& fileName,const std::string& textureFileName);
	Shape(int CylParts, int linkPosition);
	Shape(int CylParts, int linkPosition,const std::string& textureFileName, ShapeType type);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Shape(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);
	Shape(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices, const std::string& textureFileName, ShapeType newType);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices, const std::string& textureFileName);
	void addMesh(Vertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	void addLineMesh(LineVertex* vertices, unsigned int numVertices,unsigned int* indices, unsigned int numIndices);
	std::vector<glm::vec3> getVertexPositions();
	void draw(int mode);
	unsigned int getTexture();
	virtual ~Shape(void);

	bool collides_with(Shape* other);
private:
	bool nodes_collide(Node* a, Node* b, const glm::vec3 &translate_a, const glm::vec3 &translate_b) const;
	static void push_child_nodes(std::pair<Node*, Node*> p, std::vector<std::pair<Node*, Node*>> &to_check);
	void set_collision_box(Node* n);
	static Mesh* create_box(Node* n, glm::vec3 color);
};

