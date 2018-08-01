#define GLEW_STATIC
#include "mesh.h"
#include <GL/glew.h>
//#include "util.h"
#include "debugTimer.h"
#include "curves.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>


Mesh::Mesh(int CylParts,int linkPosition)
{
	auto indexedModel = Surface(CylParts, 8, 8, linkPosition).ToIndexedModel();
	tree.make_tree(indexedModel);
	InitMesh(indexedModel);
}

Mesh::Mesh(const std::string& fileName)
{
	auto indexedModel = OBJModel(fileName).ToIndexedModel();
	tree.make_tree(indexedModel);
    InitMesh(indexedModel);	
}

void Mesh::InitMesh(const IndexedModel& model)
{
	indexedModel = model;
    m_numIndices = model.indices.size();

    GLCall(glGenVertexArrays(1, &m_vertexArrayObject));
	GLCall(glBindVertexArray(m_vertexArrayObject));

	GLCall(glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers));
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[TEXCOORD_VB]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[NORMAL_VB]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(2));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(model.colors[0]) * model.colors.size(), &model.colors[0], GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0));

	if(!model.weights.empty())
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[WEIGHT_VB]));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(model.weights[0])*model.weights.size(), &model.weights[0], GL_STATIC_DRAW));
		GLCall(glEnableVertexAttribArray(4));
		GLCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, 0));
	}

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW));

	GLCall(glBindVertexArray(0));
}

Mesh::Mesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
    IndexedModel model;

	for(unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.texCoords.push_back(*vertices[i].GetTexCoord());
		model.normals.push_back(*vertices[i].GetNormal());
		model.colors.push_back(*vertices[i].GetColor());
	}
	
	for(unsigned int i = 0; i < numIndices; i++)
        model.indices.push_back(indices[i]);
	
	tree.make_tree(model);
    InitMesh(model);
}

void Mesh::delete_mesh() const
{
	GLCall(glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers));
	GLCall(glDeleteVertexArrays(1, &m_vertexArrayObject));
}

Mesh::~Mesh()
{
	delete_mesh();
}

void Mesh::Draw(int mode)
{
	GLCall(glBindVertexArray(m_vertexArrayObject));

	//glDrawElements(GL_LINES, m_numIndices, GL_UNSIGNED_INT, 0);
	
	GLCall(glDrawElementsBaseVertex(mode, m_numIndices, GL_UNSIGNED_INT, 0, 0));
	GLCall(glBindVertexArray(0));
}


