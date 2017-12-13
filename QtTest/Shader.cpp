/*
* Shader.cpp
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
* Loads and compiles shader programs.
*
* This class is created with help from an online tutorial by Joey de Vries:
* https://learnopengl.com
*
* Joey de Vries on Twitter:
* https://twitter.com/JoeyDeVriez
*
*
*/


#include "Shader.h"


Shader::Shader()
{
}

void Shader::setup_shader(std::string vertexPath, std::string fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);

	std::stringstream vShaderStream, fShaderStream;

	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	
	vShaderFile.close();
	fShaderFile.close();
	
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();

	const GLchar* vShaderCode = vertexCode.c_str();
	const GLchar * fShaderCode = fragmentCode.c_str();

	GLuint vertex;
	GLuint fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	
	this->_program = glCreateProgram();
	glAttachShader(this->_program, vertex);
	glAttachShader(this->_program, fragment);
	glLinkProgram(this->_program);
	
	glDeleteShader(vertex);
	glDeleteShader(fragment);

}

void Shader::use()
{
	glUseProgram(this->_program);
}
