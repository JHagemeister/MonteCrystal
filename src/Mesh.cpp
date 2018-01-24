/*
* Mesh.cpp
*
* Copyright 2017 Julian Hagemeister
*
* This file is part of MonteCrystal.
*
* MonteCrystal is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* MonteCrystal is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with MonteCrystal.  If not, see <http://www.gnu.org/licenses/>.
*
*/

/*
*
* This class was created by help of an online tutorial by Joey de Vries:
* https://learnopengl.com
*
* Joey de Vries on Twitter:
* https://twitter.com/JoeyDeVriez
*
*/

#include "Mesh.h"

Mesh::Mesh(QOpenGLFunctions_3_3_Core *glf)
{
	_glf = glf;
	_glf->glGenVertexArrays(1, &this->_VAO);
	_glf->glGenBuffers(1, &this->_VBO);
	_glf->glGenBuffers(1, &this->_EBO);
}

Mesh::Mesh(QOpenGLFunctions_3_3_Core *glf, std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	_glf = glf;
	_glf->glGenVertexArrays(1, &this->_VAO);
	_glf->glGenBuffers(1, &this->_VBO);
	_glf->glGenBuffers(1, &this->_EBO);

	_vertices = vertices;
	_indices = indices;
	update_buffers();
}

void Mesh::draw(void)
{
	_glf->glBindVertexArray(this->_VAO);
	_glf->glDrawElements(GL_TRIANGLES, (GLsizei) this->_indices.size(), GL_UNSIGNED_INT, 0);
	_glf->glBindVertexArray(0);
}

void Mesh::update_buffers(void)
{
	_glf->glBindVertexArray(this->_VAO);
	_glf->glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	
	_glf->glBufferData(GL_ARRAY_BUFFER, this->_vertices.size() * sizeof(Vertex), &this->_vertices[0], GL_DYNAMIC_DRAW);

	_glf->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
	_glf->glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indices.size() * sizeof(GLuint), &this->_indices[0], GL_DYNAMIC_DRAW);

	_glf->glEnableVertexAttribArray(0);
	_glf->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	
	_glf->glEnableVertexAttribArray(1);
	_glf->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	_glf->glBindBuffer(GL_ARRAY_BUFFER, 0);
	_glf->glBindVertexArray(0);
}

void Mesh::update_mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	_vertices = vertices;
	_indices = indices;
	update_buffers();
}