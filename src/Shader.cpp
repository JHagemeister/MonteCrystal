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

#include <iostream>
#include <sstream>
#include <QFile>


Shader::Shader()
{
}

void Shader::setup_shader(const QString &vertexPath, const QString &fragmentPath, QOpenGLFunctions_3_3_Core *glf)
{	
	QFile vShaderFile{vertexPath};
	QFile fShaderFile{fragmentPath};

	vShaderFile.open(QIODevice::ReadOnly);
	fShaderFile.open(QIODevice::ReadOnly);
	
	std::string vertexCode = QString{vShaderFile.readAll()}.toStdString();
	std::string fragmentCode = QString{fShaderFile.readAll()}.toStdString();

	vShaderFile.close();
	fShaderFile.close();

	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();

	GLuint vertex;
	GLuint fragment;

	vertex = glf->glCreateShader(GL_VERTEX_SHADER);
	glf->glShaderSource(vertex, 1, &vShaderCode, NULL);
	glf->glCompileShader(vertex);
	GLint isCompiled = 0;
	glf->glGetShaderiv(vertex, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		exit(0);
	}
	
	fragment = glf->glCreateShader(GL_FRAGMENT_SHADER);
	glf->glShaderSource(fragment, 1, &fShaderCode, NULL);
	glf->glCompileShader(fragment);
	
	this->_program = glf->glCreateProgram();
	glf->glAttachShader(this->_program, vertex);
	glf->glAttachShader(this->_program, fragment);
	glf->glLinkProgram(this->_program);
	
	glf->glDeleteShader(vertex);
	glf->glDeleteShader(fragment);
}

void Shader::use(QOpenGLFunctions_3_3_Core *glf)
{
	glf->glUseProgram(this->_program);
}
