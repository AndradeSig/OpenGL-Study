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
	
	// POSIÇÕES:
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	// TEXTURA:
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	
}; 


/** POSIÇÕES DOS CUBOS **/
glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f), 
    glm::vec3( 2.0f,  5.0f, -15.0f), 
    glm::vec3(-1.5f, -2.2f, -2.5f),  
    glm::vec3(-3.8f, -2.0f, -12.3f),  
    glm::vec3( 2.4f, -0.4f, -3.5f),  
    glm::vec3(-1.7f,  3.0f, -7.5f),  
    glm::vec3( 1.3f, -2.0f, -2.5f),  
    glm::vec3( 1.5f,  2.0f, -2.5f), 
    glm::vec3( 1.5f,  0.2f, -1.5f), 
    glm::vec3(-1.3f,  1.0f, -1.5f)  
};


/** VBO, VAO E EBO **/
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
	glVertexAttribPointer(0,3,GL_FLOAT,0,5*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);

	/** INTERPRETAR TEXTURA **/
	glVertexAttribPointer(2,2,GL_FLOAT,0,5*sizeof(float),(void*)(3*sizeof(float)));
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
	unsigned char *data = stbi_load("Images/minecraft.jpg",&width,&height,&nrChannels,0);
	
	// Gerar textura com o Mipmap
	if(data){
   		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
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
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// DESENHAR O TRIANGULO COM TEXTURA
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,texture2);

		ourShader.use();



		/** SETAR A PROJEÇÃO ORTOGRÁFICA **/
		// Primeiro e Segundo Parâmetro: Coordenadas da Esquerda e Direira do Tronco
		// Terceiro e Quarto Parâmetro.: Coordenadas das partes Inferior e Superior do Tronco
		glm::ortho(0.0f,800.0f,0.0f,600.0f,0.1f,100.0f);


		/** MATRIZ DE MODELO **/
		// A Matriz de Modelo consiste em translações, escalas e/ou rotações
		// Serve para transformar todos os vértices ao espaço mundial global
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model,glm::radians(-55.0f),glm::vec3(1.0f,0.0f,0.0f));
		model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));



		/** MATRIZ DE VISUALIZAÇÃO **/
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));



		/** MATRIZ DE PROJEÇÃO DE PERSPECTIVA **/
		// Primeiro Parâmetro: FOV(Campo de visão)
		// Segundo Parâmetro: Proporção da Imagem dividindo a largura da janela pela altura da janela
		// Terceiro e Quarto Parâmetro: Definem os planos próximos e distantes do Tronco
		glm::mat4 projection = glm::perspective(glm::radians(45.0f),(float)800/(float)600,0.1f,100.0f);



		unsigned int modelLoc = glGetUniformLocation(ourShader.ID,"model");
		unsigned int viewLoc = glGetUniformLocation(ourShader.ID,"view");
		glUniformMatrix4fv(modelLoc, 1, 0, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, 0, &view[0][0]);

		ourShader.setMat4("projection",projection);


		glBindVertexArray(VAO);

		/** RENDERIZAR MAIS CUBOS NO CENÁRIO **/
		for(unsigned int i = 0; i < 10; i++){
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 10.0f * i;
			model = glm::rotate(model,(float)glfwGetTime() * glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			ourShader.setMat4("model",model);

			glDrawArrays(GL_TRIANGLES,0,36);
		}

		

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