/*
* Shader.h
*
* Loads and compiles shader programs.
*
* This class is up to a few changes identical to a class given in an online tutorial by Joey de Vries:
* https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
*
* License:
* https://creativecommons.org/licenses/by-nc/4.0/
* Joey de Vries on Twitter:
* https://twitter.com/JoeyDeVriez
*
* 
*/

#ifndef SHADER_H
#define SHADER_H

// standard includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// GL Includes
#include <glew.h>

/// Vertex and fragment shader

class Shader
{
public:
	GLuint _program;

	Shader();
	void setup_shader(std::string vertexPath, std::string fragmentPath);	
	void use();
};

#endif  // SHADER_H