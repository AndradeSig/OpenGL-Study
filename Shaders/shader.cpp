#include <iostream>
#include "../glad/glad.h"
#include <GLFW/glfw3.h>
#include <math.h>

/** PROTOTIPAÇÃO DAS FUNÇÕES: **/
void frameBuffer_size_callback(GLFWwindow* window, int width, int height);
void LoopRender(GLFWwindow* window);
void processInput(GLFWwindow *window);



/** VÉRTICES DO TRIÂNGULO **/
float vertices[] = {
    -0.5f, -0.5f, 0.0f, // ESQUERDA  
     0.5f, -0.5f, 0.0f, // DIREITA 
     0.0f,  0.5f, 0.0f  // CIMA   
}; 

/** VBO E VAO **/
unsigned int VBO, VAO;

/** VERTEX SHADER SOURCE **/
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "out vec4 vertexColor;\n" // Determinar o output da cor para o Fragment Shader
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "	vertexColor = vec4(0.5,0.0,0.0,1.0);\n"
    "}\0";


/** FRAGMENT SHADER SOURCE **/
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n" // Transformo o Fragmento de Cor em uma Vertex Color
    "}\n\0";


/** VERTEX SHADER **/
unsigned int vertexShader;

/** FRAGMENT SHADER **/
unsigned int fragmentShader;

/** SHADER PROGRAM **/
unsigned int shaderProgram;

/** VARIÁVEL DE SUCESSO E A "INFO LOG" **/
int sucess;
char infoLog[512];


/** PROPRIEDADES DOS VALORES DE SHADERS/TRIANGULO **/
float timeValue = glfwGetTime();
float greenValue = (std::sin(timeValue)/2.0f) + 0.5f;
int vertexColorLocation;



/** SHADERS **/
const char *shaders = "#version 303 core\n"
	"in type input1;\n"
	"in type input2;\n"

	"out type output;\n"
	
	"uniform type uniF;\n"

	"void main()\n"
	"{\n"
	"	out = weird_stuff_we_processed;"
	"}\n\0";


const char *arguments = 
	"vec2 vect = vec2(0.5,0.7);\n"
	"vec3 result = vec4(vect,0.0,0.0);\n"
	"vec4 otherResult = vec4(result.xyz,1.0);\n";




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


	/** CRIAR E COMPILAR O FRAGMENT SHADER **/
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);


	/** CRIAR PROGRAMA DE SHADER **/
	shaderProgram = glCreateProgram();

	/** ANEXAR OS SHADERS COMPILADOS AO PROGRAMA DE SHADERS **/
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	// INDIDICAR ERROS NO PROGRAMA DE SHADER **/
	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&sucess);
	if(!sucess){
		glGetProgramInfoLog(shaderProgram,512,NULL,infoLog);
	}

	/** LOCALIZAR O VERTEX COLOR **/
	vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
	
	/** ATIVAR O USO DO PROGRAMA DE SHADERS **/
	glUseProgram(shaderProgram);

	/** DEFINE O VALOR UNIFORME DO VERTEX COLOR **/
	glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);

	/** DELETAR OS OBJETOS DE SHADER **/
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


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
	LoopRender(window);

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


		// TROCAR COR DO SHADER
		glUseProgram(shaderProgram);
		float timeValue = glfwGetTime();
    	float greenValue = sin(timeValue) / 2.0f + 0.5f;
    	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);			


		// DESENHAR O TRIANGULO
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
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