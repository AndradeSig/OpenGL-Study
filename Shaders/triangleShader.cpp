#include <iostream>
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <math.h>

/** PROTOTIPAÇÃO DAS FUNÇÕES: **/
void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void LoopRender(GLFWwindow* window);
void processInput(GLFWwindow *window);



/** VÉRTICES DO TRIÂNGULO **/
float vertices[] = {
	// Posição:
    -0.5f, -0.5f, 0.0f, // ESQUERDA  
     0.5f, -0.5f, 0.0f, // DIREITA 
     0.0f,  0.5f, 0.0f  // CIMA
}; 


/** VBO E VAO **/
unsigned int VBO, VAO;



int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE); // Apenas ao MacOS X

	/** CRIAR JANELA **/
	GLFWwindow* window = glfwCreateWindow(800,600,"Janela OpenGL",NULL,NULL);
	if(window == NULL){
		std::cout << "Erro ao criar janela GLFW" << std::endl;
		glfwTerminate();
		return -1;
	}

	/** CRIAR O CONTEXTO DA JANELA **/
	glfwMakeContextCurrent(window);


	/** SETAR A VIEWPORT DA JANELA **/
	glfwSetFramebufferSizeCallback(window,frameBuffer_size_callback);


	/** SINALIZAR ERRO NO GLAD **/
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
		std::cout << "Falha ao inicializar GLAD" << std::endl;
		return -1;
	}


	/** ARQUIVO DO VERTEX SHADER E DO FRAGMENT SHADER **/
    Shader ourShader("shader.vs","shader.fs");



	// ----------------- SETAR AS PROPRIEDADES DO TRIANGULO(COMO OS BUFFERS E AS VERTEX) --------------------- //

	/** BUFFERS**/
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	/** FAZER A CÓPIA DOS DADOS DAS VÉRTICES DEFINIDAS **/
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);


	/** INTERPRETAR OS DADOS DA VÉRTICE **/
	glVertexAttribPointer(0,3,GL_FLOAT,0,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);


	/** SETAR O LOOP DE RENDERIZAÇÃO **/
	while(!glfwWindowShouldClose(window)){

		processInput(window); // Setar o Input
		
		// Renderização
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// DESENHAR O TRIANGULO
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,3);


		// verifica e chama eventos e troca os buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate(); // Deletar os recursos alocados
	return 0;
}

void frameBuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}


void processInput(GLFWwindow *window){
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window,1); // Fechar ao clicar no botão "ESCAPE/ESC"
	}
}