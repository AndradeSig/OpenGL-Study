#include <iostream>
#include "../glad/glad.h"
#include <GLFW/glfw3.h>

/** PROTOTIPAÇÃO DAS FUNÇÕES: **/
void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void LoopRender(GLFWwindow* window);
void processInput(GLFWwindow *window);



/** VÉRTICES DO TRIÂNGULO **/
float vertices[] = {
    -0.5f, -0.5f, 0.0f, // Esquerda 
     0.5f, -0.5f, 0.0f, // Direita 
     0.0f,  0.5f, 0.0f  // Cima   
}; 

/** VÉRTICES DO SEGUNDO TRIANGULO **/
float verticesTriangle2[] = {
	-0.5f, -0.5f, 0.0f, // Esquerda 
     0.5f, -0.5f, 0.0f, // Direita 
     0.0f,  0.5f, 0.0f  // Cima  
};

/** VBO E VAO **/
unsigned int VBOs[2], VAOs[2];


/** VERTEX SHADER SOURCE **/
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x-0.5, aPos.y, aPos.z, 1.0);\n"
    "}\0";



/** VERTEX SHADER SOURCE 2 **/
const char *vertexShaderSource2 = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x+0.5, aPos.y, aPos.z, 1.0);\n"
    "}\0";




/** FRAGMENT SHADER SOURCE **/
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";	


/** FRAGMENT SHADER SOURCE 2 **/
const char *fragmentShaderSource2 = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(3.0f, 0.12f, 0.14f, 1.0f);\n"
    "}\n\0";	



/** VERTEX SHADER 01 **/
unsigned int vertexShader;

/** FRAGMENT SHADER 01 **/
unsigned int fragmentShader;

/** SHADER PROGRAM 01 **/
unsigned int shaderProgram;



/** VERTEX SHADER 02 **/
unsigned int vertexShader2;

/** FRAGMENT SHADER 02 **/
unsigned int fragmentShader2;

/** SHADER PROGRAM 02 **/
unsigned int shaderProgram2;


/** VARIÁVEL DE SUCESSO E A "INFO LOG" **/
int sucess;
char infoLog[512];




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


	/** CRIAR E COMPILAR DINAMICAMENTE OS VERTEX SHADERS **/
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
	glCompileShader(vertexShader);


	/** INDICAR MENSAGEM DE ERRO DOS VERTEX SHADERS **/
	glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	/** CRIAR E COMPILAR DINAMICAMENTE OS VERTEX SHADERS 2 **/
	vertexShader2 = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader2,1,&vertexShaderSource2,NULL);
	glCompileShader(vertexShader2);



	/** INDICAR MENSAGEM DE ERRO DOS VERTEX SHADERS 2 **/
	glGetShaderiv(vertexShader2,GL_COMPILE_STATUS,&sucess);
	if(!sucess){
		glGetShaderInfoLog(vertexShader2,512,NULL,infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	/** CRIAR E COMPILAR O FRAGMENT SHADER 1 **/
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);



	/** CRIAR E COMPILAR O FRAGMENT SHADER 2 **/
	fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader2,1,&fragmentShaderSource2,NULL);
	glCompileShader(fragmentShader2);




	/** CRIAR PROGRAMA DE SHADER 01 **/
	shaderProgram = glCreateProgram();

	/** ANEXAR OS SHADERS COMPILADOS AO PROGRAMA DE SHADERS **/
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	// INDIDICAR ERROS NO PROGRAMA DE SHADER 1 **/
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&sucess);
	if(!sucess){
		glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
	}


	/** CRIAR PROGRAMA DE SHADER 02 **/

	/** ANEXAR OS SHADERS COMPILADOS AO PROGRAMA DE SHADERS 2 **/
	shaderProgram2 = glCreateProgram();
	glAttachShader(shaderProgram2, vertexShader2);
	glAttachShader(shaderProgram2, fragmentShader2);
	glLinkProgram(shaderProgram2);


	// INDIDICAR ERROS NO PROGRAMA DE SHADER 2 **/
	glGetProgramiv(shaderProgram2,GL_LINK_STATUS,&sucess);
	if(!sucess){
		glGetProgramInfoLog(shaderProgram2,512,NULL,infoLog);
	}


	/** ATIVAR O USO DO PROGRAMA DE SHADERS 01 e 02 **/
	glUseProgram(shaderProgram);
	glUseProgram(shaderProgram2);

	/** DELETAR OS OBJETOS DE SHADER 01 e 02 **/
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glDeleteShader(vertexShader2);
	glDeleteShader(fragmentShader2);

	// ----------------- SETAR AS PROPRIEDADES DOS TRIANGULOS(COMO OS BUFFERS E AS VERTEX) --------------------- //

	/** BUFFERS DO TRIANGULO 01 **/
	glGenVertexArrays(sizeof(VAOs)/sizeof(int),VAOs);
	glGenBuffers(sizeof(VAOs)/sizeof(int),VBOs);

	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);

	/** FAZER A CÓPIA DOS DADOS DAS VÉRTICES DEFINIDAS DO TRIANGULO 01 **/
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,0,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);




	/** BUFFERS DO TRIANGULO 02 **/
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[1]);

	/** FAZER A CÓPIA DOS DADOS DAS VÉRTICES DEFINIDAS DO TRIANGULO 02 **/
	glBufferData(GL_ARRAY_BUFFER,sizeof(verticesTriangle2),verticesTriangle2,GL_STATIC_DRAW);
	glVertexAttribPointer(0,3,GL_FLOAT,0,3*sizeof(float),(void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);


	/** SETAR O LOOP DE RENDERIZAÇÃO **/
	LoopRender(window);


	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(2, VBOs);
	glDeleteProgram(shaderProgram);
	glDeleteProgram(shaderProgram2);
	glfwTerminate(); // Deletar os recursos alocados
	return 0;
}

void frameBuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}

void LoopRender(GLFWwindow* window){
	while(!glfwWindowShouldClose(window)){

		processInput(window); // Setar o Input
		
		// Renderização
		glClearColor(0.2f,0.3f,0.3f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// DESENHAR O TRIANGULO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES,0,3);

		glUseProgram(shaderProgram2);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES,0,3);


		// verifica e chama eventos e troca os buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void processInput(GLFWwindow *window){
	if(glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(window,1); // Fechar ao clicar no botão "ESCAPE/ESC"
	}
}

