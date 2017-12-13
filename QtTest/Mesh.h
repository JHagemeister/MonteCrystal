/*
* Mesh.h
*
*
* This class is up to a few changes identical to a class given in an online tutorial by Joey de Vries:
* https://learnopengl.com/#!Model-Loading/Mesh
* https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/mesh.h
*
* License:
* https://creativecommons.org/licenses/by-nc/4.0/
* Joey de Vries on Twitter:
* https://twitter.com/JoeyDeVriez
*
*/


#ifndef MESH_H
#define MESH_H

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GL Includes
#include <glew.h> // Contains all the necessery OpenGL includes

//own
#include "Shader.h"
#include "typedefs.h"

/// Mesh for openGL

class Mesh 
{
public:
	Mesh(void);
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);
	void draw(void);
	void update_buffers(void);
	void update_mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices);

protected:
	std::vector<Vertex> _vertices;
	std::vector<GLuint> _indices;

private:
	GLuint _VAO;
	GLuint _VBO;
	GLuint _EBO;
};

#endif // MESH_H

