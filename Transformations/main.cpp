#include <iostream>
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include "Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../std_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

/** PROTOTIPAÇÃO DAS FUNÇÕES: **/
void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void LoopRender(GLFWwindow* window);
void processInput(GLFWwindow *window);



/** VÉRTICES DO TRIÂNGULO **/
float vertices[] = {
	// Posição:         	// CORES: 				// TEXTURA:
     0.5f,  0.5f, 0.0f, 	1.0f, 0.0f, 0.0f,		1.0f, 1.0f, // CANTO SUPERIOR DIREITO
     0.5f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f,		1.0f, 0.0f, // CANTO INFERIOR DIREITO
    -0.5f, -0.5f, 0.0f, 	0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // CANTO INFERIOR ESQUERDO
    -0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,       0.0f, 1.0f  // CANTO SUPERIOR ESQUERDO
}; 


/** INDÍCES DO OBETO **/
unsigned int indices[] = {
	0, 1, 3,
	1, 2, 3
};




/** VBO, VAO E EBO **/
unsigned int VBO, VAO, EBO;



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
	glGenBuffers(1,&EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);

	/** FAZER A CÓPIA DOS DADOS DAS VÉRTICES DEFINIDAS **/
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);


	/** FAZER A CÓPIA DOS INDICES DEFINIDOS(EBO) **/
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


	/** INTERPRETAR OS DADOS DA VÉRTICE **/
	glVertexAttribPointer(0,3,GL_FLOAT,0,8*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	/** INTERPRETAR COR **/
	glVertexAttribPointer(1,3,GL_FLOAT,0,8*sizeof(float),(void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	/** INTERPRETAR TEXTURA **/
	glVertexAttribPointer(2,2,GL_FLOAT,0,8*sizeof(float),(void*)(6*sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);




	/** ID DA TEXTURA **/
	unsigned int texture1, texture2;

   	/** INFORMAR O ID DA TEXTURA **/
   	glGenTextures(1,&texture1);

   	/** VINCULAR TEXTURA **/
   	glBindTexture(GL_TEXTURE_2D,texture1);

    // define as opções de envolvimento / filtragem da textura (no objeto de textura atualmente vinculado)
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	/** DEFINIÇÕES DA TEXTURA **/
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // Inverter a imagem no Eixo Y  
	unsigned char *data = stbi_load("Images/minecraft.png",&width,&height,&nrChannels,0);
	
	// Gerar textura com o Mipmap
	if(data){
   		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
   		glGenerateMipmap(GL_TEXTURE_2D);
   	}else{
   		std::cout << "Falha ao carregar Textura" << std::endl;
   	}

   	/** LIBERAR A MEMÓRIA DA IMAGEM 1 **/
   	stbi_image_free(data);



	/** INFORMAR O ID DA TEXTURA 2 **/
   	glGenTextures(1,&texture2);

   	/** VINCULAR TEXTURA 2 **/
   	glBindTexture(GL_TEXTURE_2D,texture2);

    // define as opções de envolvimento / filtragem da textura (no objeto de textura atualmente vinculado)
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	/** DEFINIÇÕES DA TEXTURA 2 **/
	data = stbi_load("Images/trollFace.png",&width,&height,&nrChannels,0);
	
	// Gerar textura com o Mipmap
	if(data){
   		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
   		glGenerateMipmap(GL_TEXTURE_2D);
   	}else{
   		std::cout << "Falha ao carregar Textura" << std::endl;
   	}

   	/** LIBERAR A MEMÓRIA DA IMAGEM 2 **/
   	stbi_image_free(data);
	

   	ourShader.use();
   	glUniform1i(glGetUniformLocation(ourShader.ID,"texture1"),0);
   	ourShader.setInt("texture2",1);


	/** SETAR O LOOP DE RENDERIZAÇÃO **/
	while(!glfwWindowShouldClose(window)){

		processInput(window); // Setar o Input
		
		// Renderização
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// DESENHAR O TRIANGULO COM TEXTURA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,texture2);

		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);



		/** ROTAÇÃO E TRANSLAÇÃO **/
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans,glm::vec3(0.0f,0.0f,0.0f)); /** TRANSLAÇÃO E POSIÇÕES **/
		trans = glm::rotate(trans,(float)glfwGetTime(),glm::vec3(0.0f,0.0f,1.0f)); /** ROTAÇÃO **/

   		unsigned int transformLoc = glGetUniformLocation(ourShader.ID,"transform");
  		glUniformMatrix4fv(transformLoc,1,0,glm::value_ptr(trans));



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