#ifndef SHADER_H
#define SHADER_H

#include "../glad/glad.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{

public: 

	/** O ID DO PROGRAMA **/
	unsigned int ID;

	/** MÉTODO CONSTRUTOR PARA FAZER A LEITURA DO SHADER **/
	Shader(const char* vertexPath, const char* fragmentPath);

	/** MÈTODO PARA USAR/ATIVAR O SHADER **/
	void use();

	/** FUNÇÕES UNIFORMS **/
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;

};


Shader::Shader(const char* vertexPath, const char* fragmentShader){
	
	/** recupere o código-fonte do vértice / fragmento de filePath **/
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	
	// garante que os objetos ifstream podem lançar exceções:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try{

		// Abrir Arquivos
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentShader);
		
		std::stringstream vShaderStream, fShaderStream;

		// lê o conteúdo do buffer do arquivo em streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Fechar arquivos
		vShaderFile.close();
		fShaderFile.close();

		// Converte a stream para string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure e){
		// Alertar erro ao carregar os Arquivos de Leitura do Shader
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	/** COMPILAR SHADERS **/
	unsigned int vertex, fragment;
	int sucess;
	char infoLog[512];

	/** VERTEX SHADER **/
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex,1,&vShaderCode,NULL);
	glCompileShader(vertex);

	// Mostrar erros da compilação das Vértices
	if(!sucess){
		glGetShaderInfoLog(vertex,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	/** FRAGMENT SHADER **/
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment,1,&fShaderCode,NULL);
	glCompileShader(fragment);

	// Mostrar erros da compilação do Fragmento
	if(!sucess){
		glGetShaderInfoLog(fragment,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};


	/** PROGRAMA DE SHADER **/
	ID = glCreateProgram();
	glAttachShader(ID,vertex);
	glAttachShader(ID,fragment);
	glLinkProgram(ID);

	// Mostrar os erros do link
	glGetProgramiv(ID,GL_LINK_STATUS,&sucess);
	if(!sucess){
		glGetProgramInfoLog(ID,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	};


	/** exclua os shaders, pois eles estão ligados ao nosso programa agora e não são mais necessários **/
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}


void Shader::use(){
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const{
	glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}

void Shader::setInt(const std::string &name, int value) const{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}


#endif // SHADER_H