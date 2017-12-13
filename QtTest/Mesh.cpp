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

Mesh::Mesh(void)
{
	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);
	glGenBuffers(1, &this->_EBO);
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	glGenVertexArrays(1, &this->_VAO);
	glGenBuffers(1, &this->_VBO);
	glGenBuffers(1, &this->_EBO);

	_vertices = vertices;
	_indices = indices;
	update_buffers();
}

void Mesh::draw(void)
{
	glBindVertexArray(this->_VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei) this->_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::update_buffers(void)
{
	glBindVertexArray(this->_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->_VBO);
	
	glBufferData(GL_ARRAY_BUFFER, this->_vertices.size() * sizeof(Vertex), &this->_vertices[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->_indices.size() * sizeof(GLuint), &this->_indices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::update_mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices)
{
	_vertices = vertices;
	_indices = indices;
	update_buffers();
}