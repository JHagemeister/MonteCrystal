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
#include <vector>

//own
#include "typedefs.h"

#include <QOpenGLFunctions_3_3_Core>

/// Mesh for openGL

class Mesh
{
public:
	Mesh(QOpenGLFunctions_3_3_Core *glf);
	Mesh(QOpenGLFunctions_3_3_Core *glf, std::vector<Vertex> vertices, std::vector<GLuint> indices);
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
	QOpenGLFunctions_3_3_Core *_glf;
};

#endif // MESH_H

