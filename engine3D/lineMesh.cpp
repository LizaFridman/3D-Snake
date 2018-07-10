#define GLEW_STATIC
#include "lineMesh.h"
#include <GL/glew.h>
//#include "util.h"
#include "debugTimer.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>

void LineMesh::InitLineMesh(const IndexedModel& model)
{
    m_numIndices = model.indices.size();

    GLCall(glGenVertexArrays(1, &m_vertexArrayObject));
	GLCall(glBindVertexArray(m_vertexArrayObject));

	GLCall(glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers));
	
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[POSITION_VB]));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));

	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_vertexArrayBuffers[COLOR_VB]));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(model.colors[0]) * model.colors.size(), &model.colors[0], GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(3));
    GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexArrayBuffers[INDEX_VB]));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW));

	GLCall(glBindVertexArray(0));
}

LineMesh::LineMesh(LineVertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
    IndexedModel model;

	for(unsigned int i = 0; i < numVertices; i++)
	{
		model.positions.push_back(*vertices[i].GetPos());
		model.colors.push_back(*vertices[i].GetColor());
	}
	
	for(unsigned int i = 0; i < numIndices; i++)
        model.indices.push_back(indices[i]);

    InitLineMesh(model);
}

LineMesh::~LineMesh()
{
	GLCall(glDeleteBuffers(NUM_BUFFERS, m_vertexArrayBuffers));
	GLCall(glDeleteVertexArrays(1, &m_vertexArrayObject));
}

void LineMesh::Draw()
{
	GLCall(glBindVertexArray(m_vertexArrayObject));
	
	GLCall(glDrawElementsBaseVertex(GL_LINES, m_numIndices, GL_UNSIGNED_INT, 0, 0));

	GLCall(glBindVertexArray(0));
}
