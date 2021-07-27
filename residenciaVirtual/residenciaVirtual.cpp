/*---------------------------------------------------------*/
/*-------------- Proyecto: Desierto mexicano --------------*/
/*----------------------   2021-1   -----------------------*/
/*------- Alumno: Elizarraras Herrera Carlos Amador -------*/
/*---------------------------------------------------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>						//main
#include <stdlib.h>		
#include <glm/glm.hpp>					//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>					//Textura

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.5f, -0.2f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
bool
	motorAuto = false,
	avanzaAuto = true,
	recorridoAuto = true,
	motorMoto = false,
	recorridoMoto = true;
float
	ruedaLlantaAuto = 0.0f,
	autoX = 0.0f,
	autoY = 0.0f,
	autoZ = 0.0f,
	giroLlantaAuto = 0.0f,
	giroAuto = 0.0f,
	ruedaLlantaMoto = 0.0f,
	motoX = 0.0f,
	motoY = 0.0f,
	motoZ = 0.0f,
	giroManubrioMoto = 0.0f,
	giroMoto = 0.0f,
	subeMoto = 0.0f;

//Keyframes (Manipulación y dibujo)
float
	piernaCaballoDD = 0.0f,
	piernaCaballoDI = 0.0f,
	piernaCaballoTD = 0.0f,
	piernaCaballoTI = 0.0f,
	pataCaballoDD = 0.0f,
	pataCaballoDI = 0.0f,
	pataCaballoTD = 0.0f,
	pataCaballoTI = 0.0f,
	pataCaballoDDX = 0.0f,
	pataCaballoDIX = 0.0f,
	pataCaballoTDX = 0.0f,
	pataCaballoTIX = 0.0f,
	caballoX = 0.0f,
	caballoY = 0.0f,
	caballoZ = 0.0f,
	giroCaballo = 0.0f,
	subeCaballo = 0.0f;
float
	piernaCaballoDDInc = 0.0f,
	piernaCaballoDIInc = 0.0f,
	piernaCaballoTDInc = 0.0f,
	piernaCaballoTIInc = 0.0f,
	pataCaballoDDInc = 0.0f,
	pataCaballoDIInc = 0.0f,
	pataCaballoTDInc = 0.0f,
	pataCaballoTIInc = 0.0f,
	pataCaballoDDXInc = 0.0f,
	pataCaballoDIXInc = 0.0f,
	pataCaballoTDXInc = 0.0f,
	pataCaballoTIXInc = 0.0f,
	caballoXInc = 0.0f,
	caballoYInc = 0.0f,
	caballoZInc = 0.0f,
	giroCaballoInc = 0.0f,
	subeCaballoInc = 0.0f;

#define MAX_FRAMES 10
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para key frames
	float piernaCaballoDD;
	float piernaCaballoDI;
	float piernaCaballoTD;
	float piernaCaballoTI;
	float pataCaballoDD;
	float pataCaballoDI;
	float pataCaballoTD;
	float pataCaballoTI;
	float pataCaballoDDX;
	float pataCaballoDIX;
	float pataCaballoTDX;
	float pataCaballoTIX;
	float caballoX;
	float caballoY;
	float caballoZ;
	float giroCaballo;
	float subeCaballo;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 10;			//introduce datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].piernaCaballoDD = piernaCaballoDD;
	KeyFrame[FrameIndex].piernaCaballoDI = piernaCaballoDI;
	KeyFrame[FrameIndex].piernaCaballoTD = piernaCaballoTD;
	KeyFrame[FrameIndex].piernaCaballoTI = piernaCaballoTI;
	KeyFrame[FrameIndex].pataCaballoDD = pataCaballoDD;
	KeyFrame[FrameIndex].pataCaballoDI = pataCaballoDI;
	KeyFrame[FrameIndex].pataCaballoTD = pataCaballoTD;
	KeyFrame[FrameIndex].pataCaballoTI = pataCaballoTI;
	KeyFrame[FrameIndex].pataCaballoDDX = pataCaballoDDX;
	KeyFrame[FrameIndex].pataCaballoDIX = pataCaballoDIX;
	KeyFrame[FrameIndex].pataCaballoTDX = pataCaballoTDX;
	KeyFrame[FrameIndex].pataCaballoTIX = pataCaballoTIX;
	KeyFrame[FrameIndex].caballoX = caballoX;
	KeyFrame[FrameIndex].caballoY = caballoY;
	KeyFrame[FrameIndex].caballoZ = caballoZ;
	KeyFrame[FrameIndex].giroCaballo = giroCaballo;
	KeyFrame[FrameIndex].subeCaballo = subeCaballo;

	FrameIndex++;
}

void resetElements(void)
{
	piernaCaballoDD = KeyFrame[0].piernaCaballoDD;
	piernaCaballoDI = KeyFrame[0].piernaCaballoDI;
	piernaCaballoTD = KeyFrame[0].piernaCaballoTD;
	piernaCaballoTI = KeyFrame[0].piernaCaballoTI;
	pataCaballoDD = KeyFrame[0].pataCaballoDD;
	pataCaballoDI = KeyFrame[0].pataCaballoDI;
	pataCaballoTD = KeyFrame[0].pataCaballoTD;
	pataCaballoTI = KeyFrame[0].pataCaballoTI;
	pataCaballoDDX = KeyFrame[0].pataCaballoDDX;
	pataCaballoDIX = KeyFrame[0].pataCaballoDIX;
	pataCaballoTDX = KeyFrame[0].pataCaballoTDX;
	pataCaballoTIX = KeyFrame[0].pataCaballoTIX;
	caballoX = KeyFrame[0].caballoX;
	caballoY = KeyFrame[0].caballoY;
	caballoZ = KeyFrame[0].caballoZ;
	giroCaballo = KeyFrame[0].giroCaballo;
	subeCaballo = KeyFrame[0].subeCaballo;
}

void interpolation(void)
{
	piernaCaballoDDInc = (KeyFrame[playIndex + 1].piernaCaballoDD - KeyFrame[playIndex].piernaCaballoDD) / i_max_steps;
	piernaCaballoDIInc = (KeyFrame[playIndex + 1].piernaCaballoDI - KeyFrame[playIndex].piernaCaballoDI) / i_max_steps;
	piernaCaballoTDInc = (KeyFrame[playIndex + 1].piernaCaballoTD - KeyFrame[playIndex].piernaCaballoTD) / i_max_steps;
	piernaCaballoTIInc = (KeyFrame[playIndex + 1].piernaCaballoTI - KeyFrame[playIndex].piernaCaballoTI) / i_max_steps;
	pataCaballoDDInc = (KeyFrame[playIndex + 1].pataCaballoDD - KeyFrame[playIndex].pataCaballoDD) / i_max_steps;
	pataCaballoDIInc = (KeyFrame[playIndex + 1].pataCaballoDI - KeyFrame[playIndex].pataCaballoDI) / i_max_steps;
	pataCaballoTDInc = (KeyFrame[playIndex + 1].pataCaballoTD - KeyFrame[playIndex].pataCaballoTD) / i_max_steps;
	pataCaballoTIInc = (KeyFrame[playIndex + 1].pataCaballoTI - KeyFrame[playIndex].pataCaballoTI) / i_max_steps;
	pataCaballoDDXInc = (KeyFrame[playIndex + 1].pataCaballoDDX - KeyFrame[playIndex].pataCaballoDDX) / i_max_steps;
	pataCaballoDIXInc = (KeyFrame[playIndex + 1].pataCaballoDIX - KeyFrame[playIndex].pataCaballoDIX) / i_max_steps;
	pataCaballoTDXInc = (KeyFrame[playIndex + 1].pataCaballoTDX - KeyFrame[playIndex].pataCaballoTDX) / i_max_steps;
	pataCaballoTIXInc = (KeyFrame[playIndex + 1].pataCaballoTIX - KeyFrame[playIndex].pataCaballoTIX) / i_max_steps;
	caballoXInc = (KeyFrame[playIndex + 1].caballoX - KeyFrame[playIndex].caballoX) / i_max_steps;
	caballoYInc = (KeyFrame[playIndex + 1].caballoY - KeyFrame[playIndex].caballoY) / i_max_steps;
	caballoZInc = (KeyFrame[playIndex + 1].caballoZ - KeyFrame[playIndex].caballoZ) / i_max_steps;
	giroCaballoInc = (KeyFrame[playIndex + 1].giroCaballo - KeyFrame[playIndex].giroCaballo) / i_max_steps;
	subeCaballoInc = (KeyFrame[playIndex + 1].subeCaballo - KeyFrame[playIndex].subeCaballo) / i_max_steps;
}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			piernaCaballoDD += piernaCaballoDDInc;
			piernaCaballoDI += piernaCaballoDIInc;
			piernaCaballoTD += piernaCaballoTDInc;
			piernaCaballoTI += piernaCaballoTIInc;
			pataCaballoDD += pataCaballoDDInc;
			pataCaballoDI += pataCaballoDIInc;
			pataCaballoTD += pataCaballoTDInc;
			pataCaballoTI += pataCaballoTIInc;
			pataCaballoDDX += pataCaballoDDXInc;
			pataCaballoDIX += pataCaballoDIXInc;
			pataCaballoTDX += pataCaballoTDXInc;
			pataCaballoTIX += pataCaballoTIXInc;
			caballoX += caballoXInc;
			caballoY += caballoYInc;
			caballoZ += caballoZInc;
			giroCaballo += giroCaballoInc;
			subeCaballo += subeCaballoInc;

			i_curr_steps++;
		}
	}
	//Automovil
	if (motorAuto)
	{
		if (avanzaAuto)
		{
			if (recorridoAuto)
			{
				if (((autoZ <= 10.0f) and (autoZ > 0.0f)) and (autoX == 0.0f)) autoZ -= 1.f;
				if ((autoZ == 0.0f) and (autoX == 0.0f)) avanzaAuto = false;
				if (((autoZ >= -10.0f) and (autoZ < 220.0f)) and ((autoX >= 0.0f) and (autoX < 20.0f)) and (giroAuto < 45.0f) and (giroLlantaAuto < 45.0f))
				{
					giroLlantaAuto += 1.0f;
					giroAuto += 1.0f;
					autoZ += 1.0f;
					autoX += 1.0f;
				}
				if (((autoZ >= -10.0f) and (autoZ < 220.0f)) and ((autoX >= 20.0f) and (autoX < 40.0f)) and (giroAuto > 0.0f) and (giroLlantaAuto < 45.0f))
				{
					giroLlantaAuto -= 1.0f;
					giroAuto -= 1.0f;
					autoZ += 1.0f;
					autoX += 1.0f;
				}
				if (((autoZ >= -10.0f) and (autoZ < 220.0f)) and (autoX == 40.0f)) autoZ += 2.0f;
				if (((autoZ >= 210.0f) and (autoZ < 256.0f)) and ((autoX <= 40.0f) and (autoX > -250.0f)) and (giroAuto > -90.0f))
				{
					giroAuto -= 2.0f;
					autoZ += 1.0f;
					autoX -= 1.0f;
				}
				if ((autoZ == 256.0f) and ((autoX <= 40.0f) and (autoX > -250.0f))) autoX -= 2.0f;
				if (((autoZ >= 256.0f) and (autoZ < 600.0f)) and ((autoX <= -250.0f) and (autoX > -296.0f)) and (giroAuto < 0.0f))
				{
					giroAuto += 2.0f;
					autoZ += 1.0f;
					autoX -= 1.0f;
				}
				if (((autoZ >= 256.0f) and (autoZ < 600.0f)) and (autoX == -296.0f)) autoZ += 2.0f;
				if (((autoZ >= 600.0f) and (autoZ < 646.0f)) and ((autoX >= -296.0f) and (autoX < 40.0f)) and (giroAuto < 90.0f))
				{
					giroAuto += 2.0f;
					autoZ += 1.0f;
					autoX += 1.0f;
				}
				if ((autoZ == 646.0f) and ((autoX >= -296.0f) and (autoX < 40.0f))) autoX += 2.0f;
				if (((autoZ <= 646.0f) and (autoZ > 310.0f)) and ((autoX >= 40.0f) and (autoX < 86.0f)) and (giroAuto < 180.0f))
				{
					giroAuto += 2.0f;
					autoZ -= 1.0f;
					autoX += 1.0f;
				}
				if (((autoZ <= 646.0f) and (autoZ > 320.0f)) and (autoX == 86.0f)) autoZ -= 2.0f;
				if (((autoZ <= 320.0f) and (autoZ > 274.0f)) and ((autoX <= 86.0f) and (autoX > -180.0f)) and (giroAuto < 270.0f))
				{
					giroAuto += 2.0f;
					autoZ -= 1.0f;
					autoX -= 1.0f;
				}
				if ((autoZ == 274.0f) and ((autoX <= 86.0f) and (autoX > -180.0f)))
				{
					autoX -= 2.0f;
					if (autoX <= -180.0f) recorridoAuto = false;
				}
			}
			else
			{
				if (((autoZ <= 274.0f) and (autoZ > -130.0f)) and ((autoX <= -180.0f) and (autoX > -226.0f)) and (giroAuto > 180.0f))
				{
					giroAuto -= 2.0f;
					autoZ -= 1.0f;
					autoX -= 1.0f;
				}
				if (((autoZ <= 274.0f) and (autoZ > -150.0f)) and (autoX == -226.0f)) autoZ -= 2.0f;
				if (((autoZ <= -150.0f) and (autoZ > -194.0f)) and ((autoX >= -226.0f) and (autoX < -4.0f)) and (giroAuto > 90.0f))
				{
					giroAuto -= 2.0f;
					autoZ -= 1.0f;
					autoX += 1.0f;
				}
				if ((autoZ == -194.0f) and ((autoX >= -226.0f) and (autoX < -6.0f))) autoX += 2.0f;
				if (((autoZ >= -194.0f) and (autoZ < -50.0f)) and ((autoX >= -7.0f) and (autoX < 40.0f)) and (giroAuto > 0.0f))
				{
					giroAuto -= 2.0f;
					autoZ += 1.0f;
					autoX += 1.0f;
				}
				if (((autoZ >= -194.0f) and (autoZ < -40.0f)) and (autoX == 40.0f)) autoZ += 2.0f;
				if (((autoZ >= -40.0f) and (autoZ < 10.0f)) and ((autoX <= 40.0f) and (autoX > 20.0f)) and (giroAuto > -45.0f) and (giroLlantaAuto > -45.0f))
				{
					giroLlantaAuto -= 1.0f;
					giroAuto -= 1.0f;
					autoZ += 1.0f;
					autoX -= 1.0f;
				}
				if (((autoZ >= -40.0f) and (autoZ < 10.0f)) and ((autoX <= 20.0f) and (autoX > 0.0f)) and (giroAuto < 0.0f) and (giroLlantaAuto < 0.0f))
				{
					giroLlantaAuto += 1.0f;
					giroAuto += 1.0f;
					autoZ += 1.0f;
					autoX -= 1.0f;
				}
				if (((autoZ >= -40.0f) and (autoZ < 10.0f)) and (autoX == 0.0f))
				{
					autoZ += 1.0f;
					if (autoZ == 10.0f) recorridoAuto = true;
				}
			}
			ruedaLlantaAuto += 1.0f;
		}	
		else
		{
			if (autoZ >= -10.0f)
			{
				autoZ -= 1.0f;
				ruedaLlantaAuto -= 1.0f;
				if (autoZ <= -10.0f) avanzaAuto = true;
			}
		}
	}
	//Motocicleta
	if (motorMoto)
	{
		if ((motoX == 0.0f) and ((motoZ <= 0.0f) and (motoZ > -100.0f))) motoZ -= 2.0f;
		if (((motoX <= 0.0f) and (motoX > -400.0f)) and ((motoZ <= -100.0f) and (motoZ > -150.0f)))
		{
			if (giroMoto < 45.0f) giroMoto += 1.0f;
			motoZ -= 1.0f;
			motoX -= 1.0f;
		}
		if (((motoX <= 0.0f) and (motoX > -400.0f)) and ((motoZ <= -150.0f) and (motoZ > -350.0f)))
		{
			if (subeMoto < 45.0f) subeMoto += 1.0f;
			motoZ -= 3.0f;
			motoY += 2.0f;
			motoX -= 3.0f;
		}
		if (((motoX <= 0.0f) and (motoX > -400.0f)) and ((motoZ <= -350.0f) and (motoZ > -400.0f)))
		{
			if (giroMoto < 90.0f) giroMoto += 1.0f;
			motoZ += 1.0f;
			motoX -= 1.0f;
		}
		if ((motoX == -400.0f) and ((motoZ >= -400.0f) and (motoZ < -300.0f)))
		{
			if (giroMoto < 180.0f) giroMoto += 3.0f;
			if (subeMoto > 0.0f) subeMoto -= 2.0f;
			motoZ += 1.0f;
		}
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);
	//--------------------------------------------------------------------------------------------------------------------------
	// Shader configuration
	//--------------------------------------------------------------------------------------------------------------------------

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	//--------------------------------------------------------------------------------------------------------------------------
	//Carga de modelos
	//--------------------------------------------------------------------------------------------------------------------------

	Model cubo("resources/objects/cubo/cube02.obj");
	Model piso("resources/objects/piso/piso.obj");
	Model edificio("resources/objects/Edificio/Edificio.obj");
	Model tienda("resources/objects/Tienda/Tienda.obj");
	Model comedor("resources/objects/Comedor/Comedor.obj");
	Model glorieta("resources/objects/Glorieta/Glorieta.obj");
	Model corredor("resources/objects/Corredor/Corredor.obj");
	Model columpios("resources/objects/Columpios/Estructura.obj");
	Model columpios1("resources/objects/Columpios/Columpio1.obj");
	Model columpios2("resources/objects/Columpios/Columpio2.obj");
	Model resbaladilla("resources/objects/Resbaladilla/Resbaladilla.obj");
	Model pelota("resources/objects/Pelota/Pelota.obj");
	Model banco("resources/objects/Banco/Banco.obj");
	Model cactus("resources/objects/Cactus/Cactus.obj");
	Model montana("resources/objects/Montana/Montana.obj");
	Model aguila("resources/objects/Aguila/Cuerpo.obj");
	Model aguilaDerecha("resources/objects/Aguila/AlaDerecha.obj");
	Model aguilaIzquierda("resources/objects/Aguila/AlaIzquierda.obj");
	Model caballo("resources/objects/Caballo/SinPatas.obj");
	Model caballoCabello("resources/objects/Caballo/Cabello.obj");
	Model caballoCola("resources/objects/Caballo/Cola.obj");
	Model caballoPiernaDD("resources/objects/Caballo/PiernaDD.obj");
	Model caballoPiernaDI("resources/objects/Caballo/PiernaDI.obj");
	Model caballoPiernaTD("resources/objects/Caballo/PiernaTD.obj");
	Model caballoPiernaTI("resources/objects/Caballo/PiernaTI.obj");
	Model caballoPataDD("resources/objects/Caballo/PataDD.obj");
	Model caballoPataDI("resources/objects/Caballo/PataDI.obj");
	Model caballoPataTD("resources/objects/Caballo/PataTD.obj");
	Model caballoPataTI("resources/objects/Caballo/PataTI.obj");
	Model roca("resources/objects/Roca/Roca.obj");
	Model serpiente("resources/objects/Serpiente/Serpiente.obj");
	Model conejo("resources/objects/Conejo/Conejo.obj");
	Model gallina("resources/objects/Gallina/Cuerpo.obj");
	Model gallinaCabeza("resources/objects/Gallina/Cabeza.obj");
	Model gallinaPataD("resources/objects/Gallina/PataDerecha.obj");
	Model gallinaPataI("resources/objects/Gallina/PataIzquierda.obj");
	Model vaca("resources/objects/Vaca/Cuerpo.obj");
	Model arbusto("resources/objects/Arbusto/Arbusto.obj");
	Model vacaCabeza("resources/objects/Vaca/Cabeza.obj");
	Model motocicleta("resources/objects/Motocicleta/Moto.obj");
	Model motocicletaManubrio("resources/objects/Motocicleta/Manubrio.obj");
	Model motocicletaLlanta("resources/objects/Motocicleta/Llanta.obj");
	Model circuito("resources/objects/Circuito/Circuito.obj");
	Model sedan("resources/objects/Sedan/Sedan.obj");
	Model sedanModerno("resources/objects/SedanModerno/Auto.obj");
	Model modernoLlantas("resources/objects/SedanModerno/LLantasTraseras.obj");
	Model modernoLlanta("resources/objects/SedanModerno/LlantaDelantera.obj");
	Model camioneta("resources/objects/Camioneta/Camioneta.obj");
	Model buggi("resources/objects/Buggi/Auto.obj");
	Model buggiLlantasT("resources/objects/Buggi/LlantasTraseras.obj");
	Model buggiLlantaDD("resources/objects/Buggi/LlantaDelanteraD.obj");
	Model buggiLlantaDI("resources/objects/Buggi/LlantaDelanteraI.obj");
	Model motoneta("resources/objects/Motoneta/Motoneta.obj");
	Model bicicleta("resources/objects/Bicicleta/Bicicleta.obj");
	Model leonardo("resources/objects/Leonardo/Cabeza.obj");
	Model leonardoTorso("resources/objects/Leonardo/Torso.obj");
	Model leonardoIngle("resources/objects/Leonardo/Ingle.obj");
	Model leonardoBrazoD("resources/objects/Leonardo/BrazoDerecho.obj");
	Model leonardoManoD("resources/objects/Leonardo/ManoDerecha.obj");
	Model leonardoBrazoI("resources/objects/Leonardo/BrazoIzquierdo.obj");
	Model leonardoManoI("resources/objects/Leonardo/ManoIzquierda.obj");
	Model leonardoPiernaD("resources/objects/Leonardo/PiernaDerecha.obj");
	Model leonardoPieD("resources/objects/Leonardo/PieDerecho.obj");
	Model leonardoPiernaI("resources/objects/Leonardo/PiernaIzquierda.obj");
	Model leonardoPieI("resources/objects/Leonardo/PieIzquierdo.obj");

	//--------------------------------------------------------------------------------------------------------------------------
	//Inicializacion de KeyFrames
	//--------------------------------------------------------------------------------------------------------------------------
	 
	/*for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
	}*/

	KeyFrame[0].piernaCaballoDD = 0.0f;
	KeyFrame[0].piernaCaballoDI = 0.0f;
	KeyFrame[0].piernaCaballoTD = 0.0f;
	KeyFrame[0].piernaCaballoTI = 0.0f;
	KeyFrame[0].pataCaballoDD = 0.0f;
	KeyFrame[0].pataCaballoDI = 0.0f;
	KeyFrame[0].pataCaballoTD = 0.0f;
	KeyFrame[0].pataCaballoTI = 0.0f;
	KeyFrame[0].pataCaballoDDX = 0.0f;
	KeyFrame[0].pataCaballoDIX = 0.0f;
	KeyFrame[0].pataCaballoTDX = 0.0f;
	KeyFrame[0].pataCaballoTIX = 0.0f;
	KeyFrame[0].caballoX = 0.0f;
	KeyFrame[0].caballoY = 0.0f;
	KeyFrame[0].caballoZ = 0.0f;
	KeyFrame[0].giroCaballo = 0.0f;
	KeyFrame[0].subeCaballo = 0.0f;

	KeyFrame[1].piernaCaballoDD = 45.0f;
	KeyFrame[1].piernaCaballoDI = -45.0f;
	KeyFrame[1].piernaCaballoTD = -30.0f;
	KeyFrame[1].piernaCaballoTI = 30.0f;
	KeyFrame[1].pataCaballoDD = 40.0f;
	KeyFrame[1].pataCaballoDI = -40.0f;
	KeyFrame[1].pataCaballoTD = -20.0f;
	KeyFrame[1].pataCaballoTI = 20.0f;
	KeyFrame[1].pataCaballoDDX = 5.0f;
	KeyFrame[1].pataCaballoDIX = -5.0f;
	KeyFrame[1].pataCaballoTDX = -2.0f;
	KeyFrame[1].pataCaballoTIX = 2.0f;
	KeyFrame[1].caballoX = -4.0f;
	KeyFrame[1].caballoY = 0.0f;
	KeyFrame[1].caballoZ = 0.0f;
	KeyFrame[1].giroCaballo = 0.0f;
	KeyFrame[1].subeCaballo = 0.0f;

	KeyFrame[2].piernaCaballoDD = -45.0f;
	KeyFrame[2].piernaCaballoDI = 45.0f;
	KeyFrame[2].piernaCaballoTD = 30.0f;
	KeyFrame[2].piernaCaballoTI = -30.0f;
	KeyFrame[2].pataCaballoDD = -40.0f;
	KeyFrame[2].pataCaballoDI = 40.0f;
	KeyFrame[2].pataCaballoTD = 20.0f;
	KeyFrame[2].pataCaballoTI = -20.0f;
	KeyFrame[2].pataCaballoDDX = -5.0f;
	KeyFrame[2].pataCaballoDIX = 5.0f;
	KeyFrame[2].pataCaballoTDX = 2.0f;
	KeyFrame[2].pataCaballoTIX = -2.0f;
	KeyFrame[2].caballoX = -8.0f;
	KeyFrame[2].caballoY = 0.0f;
	KeyFrame[2].caballoZ = 0.0f;
	KeyFrame[2].giroCaballo = 0.0f;
	KeyFrame[2].subeCaballo = 0.0f;

	KeyFrame[3].piernaCaballoDD = 45.0f;
	KeyFrame[3].piernaCaballoDI = -45.0f;
	KeyFrame[3].piernaCaballoTD = -30.0f;
	KeyFrame[3].piernaCaballoTI = 30.0f;
	KeyFrame[3].pataCaballoDD = 40.0f;
	KeyFrame[3].pataCaballoDI = -40.0f;
	KeyFrame[3].pataCaballoTD = -20.0f;
	KeyFrame[3].pataCaballoTI = 20.0f;
	KeyFrame[3].pataCaballoDDX = 5.0f;
	KeyFrame[3].pataCaballoDIX = -5.0f;
	KeyFrame[3].pataCaballoTDX = -2.0f;
	KeyFrame[3].pataCaballoTIX = 2.0f;
	KeyFrame[3].caballoX = -12.0f;
	KeyFrame[3].caballoY = 0.0f;
	KeyFrame[3].caballoZ = 0.0f;
	KeyFrame[3].giroCaballo = 0.0f;
	KeyFrame[3].subeCaballo = 0.0f;

	KeyFrame[4].piernaCaballoDD = 0.0f;
	KeyFrame[4].piernaCaballoDI = 0.0f;
	KeyFrame[4].piernaCaballoTD = 45.0f;
	KeyFrame[4].piernaCaballoTI = 45.0f;
	KeyFrame[4].pataCaballoDD = 45.0f;
	KeyFrame[4].pataCaballoDI = 45.0f;
	KeyFrame[4].pataCaballoTD = 65.0f;
	KeyFrame[4].pataCaballoTI = 65.0f;
	KeyFrame[4].pataCaballoDDX = 0.0f;
	KeyFrame[4].pataCaballoDIX = 0.0f;
	KeyFrame[4].pataCaballoTDX = 5.0f;
	KeyFrame[4].pataCaballoTIX = 5.0f;
	KeyFrame[4].caballoX = -12.0f;
	KeyFrame[4].caballoY = 0.0f;
	KeyFrame[4].caballoZ = 0.0f;
	KeyFrame[4].giroCaballo = 0.0f;
	KeyFrame[4].subeCaballo = -65.0f;

	KeyFrame[5].piernaCaballoDD = -45.0f;
	KeyFrame[5].piernaCaballoDI = 0.0f;
	KeyFrame[5].piernaCaballoTD = 30.0f;
	KeyFrame[5].piernaCaballoTI = 30.0f;
	KeyFrame[5].pataCaballoDD = -40.0f;
	KeyFrame[5].pataCaballoDI = 0.0f;
	KeyFrame[5].pataCaballoTD = 20.0f;
	KeyFrame[5].pataCaballoTI = 20.0f;
	KeyFrame[5].pataCaballoDDX = -5.0f;
	KeyFrame[5].pataCaballoDIX = 0.0f;
	KeyFrame[5].pataCaballoTDX = 2.0f;
	KeyFrame[5].pataCaballoTIX = 2.0f;
	KeyFrame[5].caballoX = -14.0f;
	KeyFrame[5].caballoY = 0.0f;
	KeyFrame[5].caballoZ = 0.0f;
	KeyFrame[5].giroCaballo = 0.0f;
	KeyFrame[5].subeCaballo = 0.0f;

	KeyFrame[6].piernaCaballoDD = 45.0f;
	KeyFrame[6].piernaCaballoDI = -45.0f;
	KeyFrame[6].piernaCaballoTD = -30.0f;
	KeyFrame[6].piernaCaballoTI = 30.0f;
	KeyFrame[6].pataCaballoDD = 40.0f;
	KeyFrame[6].pataCaballoDI = -40.0f;
	KeyFrame[6].pataCaballoTD = -20.0f;
	KeyFrame[6].pataCaballoTI = 20.0f;
	KeyFrame[6].pataCaballoDDX = 5.0f;
	KeyFrame[6].pataCaballoDIX = -5.0f;
	KeyFrame[6].pataCaballoTDX = -2.0f;
	KeyFrame[6].pataCaballoTIX = 2.0f;
	KeyFrame[6].caballoX = -16.0f;
	KeyFrame[6].caballoY = 0.0f;
	KeyFrame[6].caballoZ = 0.0f;
	KeyFrame[6].giroCaballo = 0.0f;
	KeyFrame[6].subeCaballo = 0.0f;

	KeyFrame[7].piernaCaballoDD = -45.0f;
	KeyFrame[7].piernaCaballoDI = 45.0f;
	KeyFrame[7].piernaCaballoTD = 30.0f;
	KeyFrame[7].piernaCaballoTI = -30.0f;
	KeyFrame[7].pataCaballoDD = -40.0f;
	KeyFrame[7].pataCaballoDI = 40.0f;
	KeyFrame[7].pataCaballoTD = 20.0f;
	KeyFrame[7].pataCaballoTI = -20.0f;
	KeyFrame[7].pataCaballoDDX = -5.0f;
	KeyFrame[7].pataCaballoDIX = 5.0f;
	KeyFrame[7].pataCaballoTDX = 2.0f;
	KeyFrame[7].pataCaballoTIX = -2.0f;
	KeyFrame[7].caballoX = -20.0f;
	KeyFrame[7].caballoY = 0.0f;
	KeyFrame[7].caballoZ = 0.0f;
	KeyFrame[7].giroCaballo = 0.0f;
	KeyFrame[7].subeCaballo = 0.0f;

	KeyFrame[8].piernaCaballoDD = -45.0f;
	KeyFrame[8].piernaCaballoDI = -45.0f;
	KeyFrame[8].piernaCaballoTD = 45.0f;
	KeyFrame[8].piernaCaballoTI = 45.0f;
	KeyFrame[8].pataCaballoDD = 45.0f;
	KeyFrame[8].pataCaballoDI = 45.0f;
	KeyFrame[8].pataCaballoTD = 90.0f;
	KeyFrame[8].pataCaballoTI = 90.0f;
	KeyFrame[8].pataCaballoDDX = -5.0f;
	KeyFrame[8].pataCaballoDIX = -5.0f;
	KeyFrame[8].pataCaballoTDX = 5.0f;
	KeyFrame[8].pataCaballoTIX = 5.0f;
	KeyFrame[8].caballoX = -22.0f;
	KeyFrame[8].caballoY = 0.0f;
	KeyFrame[8].caballoZ = 0.0f;
	KeyFrame[8].giroCaballo = 0.0f;
	KeyFrame[8].subeCaballo = 0.0f;

	KeyFrame[9].piernaCaballoDD = 0.0f;
	KeyFrame[9].piernaCaballoDI = 0.0f;
	KeyFrame[9].piernaCaballoTD = 0.0f;
	KeyFrame[9].piernaCaballoTI = 0.0f;
	KeyFrame[9].pataCaballoDD = 0.0f;
	KeyFrame[9].pataCaballoDI = 0.0f;
	KeyFrame[9].pataCaballoTD = 0.0f;
	KeyFrame[9].pataCaballoTI = 0.0f;
	KeyFrame[9].pataCaballoDDX = 0.0f;
	KeyFrame[9].pataCaballoDIX = 0.0f;
	KeyFrame[9].pataCaballoTDX = 0.0f;
	KeyFrame[9].pataCaballoTIX = 0.0f;
	KeyFrame[9].caballoX = -22.0f;
	KeyFrame[9].caballoY = 0.0f;
	KeyFrame[9].caballoZ = 0.0f;
	KeyFrame[9].giroCaballo = 0.0f;
	KeyFrame[9].subeCaballo = 0.0f;

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//--------------------------------------------------------------------------------------------------------------------------
	//Bucle de renderizado
	//--------------------------------------------------------------------------------------------------------------------------

	while (!glfwWindowShouldClose(window))
	{
		//--------------------------------------------------------------------------------------------------------------------------
		//Configuracion

		skyboxShader.setInt("skybox", 0);
		
		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//--------------------------------------------------------------------------------------------------------------------------
		//Luces

		// don't forget to enable shader before setting uniforms
		staticShader.use();

		//Fuente de luz direccional (Sol)
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.4f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 0.8f, 0.8f));
		staticShader.setVec3("dirLight.specular", glm::vec3(1.0f, 0.0f, 0.0f));

		//Fuente de luz posicional
		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);
		
		//--------------------------------------------------------------------------------------------------------------------------
		//Modelos

		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);
	
		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(262.5f, 0.0f, -164.5f));
		staticShader.setMat4("model", model);
		edificio.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(304.5f, 0.0f, 260.0f));
		staticShader.setMat4("model", model);
		tienda.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(227.0f, 0.0f, 210.0f));
		staticShader.setMat4("model", model);
		comedor.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(227.0f, 0.0f, 311.5f));
		staticShader.setMat4("model", model);
		comedor.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-300.0f, 0.0f, 300.0f));
		staticShader.setMat4("model", model);
		glorieta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-158.0f, 0.0f, 300.0f));
		staticShader.setMat4("model", model);
		corredor.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 400.0f));
		staticShader.setMat4("model", model);
		columpios.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 400.0f));
		staticShader.setMat4("model", model);
		columpios1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 400.0f));
		staticShader.setMat4("model", model);
		columpios2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, 425.0f));
		model = glm::rotate(model, glm::radians(-135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		resbaladilla.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-300.0f, 0.0f, 400.0f));
		staticShader.setMat4("model", model);
		pelota.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-27.0f, 0.0f, 230.0f));
		staticShader.setMat4("model", model);
		banco.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-128.0f, 0.0f, 230.0f));
		staticShader.setMat4("model", model);
		banco.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-228.0f, 0.0f, 230.0f));
		staticShader.setMat4("model", model);
		banco.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-400.0f, 0.0f, 300.0f));
		staticShader.setMat4("model", model);
		cactus.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-700.0f, 0.0f, -700.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", model);
		montana.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1000.0f, 0.0f));
		staticShader.setMat4("model", model);
		aguila.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1000.0f, 0.0f));
		staticShader.setMat4("model", model);
		aguilaDerecha.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1000.0f, 0.0f));
		staticShader.setMat4("model", model);
		aguilaIzquierda.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f+caballoX, 17.0f+caballoY, 0.0f+caballoZ));
		tmp = model = glm::rotate(model, glm::radians(giroCaballo), glm::vec3(0.0f, 1.0f, 0.0f));	//Dependencia tmp
		tmp = model = glm::rotate(model, glm::radians(subeCaballo), glm::vec3(0.0f, 0.0f, 1.0f));	//Dependencia tmp
		staticShader.setMat4("model", model);
		caballo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(-23.0f, 11.0f, 5.0f));
		staticShader.setMat4("model", model);
		caballoCabello.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(4.0f, 5.5f, 0.0f));
		staticShader.setMat4("model", model);
		caballoCola.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(-21.2f, -4.8f, -1.5f));
		model = glm::rotate(model, glm::radians(piernaCaballoDD), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		caballoPiernaDD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(-19.5f, -5.3f, 4.0f));
		model = glm::rotate(model, glm::radians(piernaCaballoDI), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		caballoPiernaDI.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(1.8f, -4.3f, -3.0f));
		model = glm::rotate(model, glm::radians(piernaCaballoTD), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		caballoPiernaTD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(2.0f, -4.5f, 2.0f));
		model = glm::rotate(model, glm::radians(piernaCaballoTI), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		caballoPiernaTI.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(-21.2f+pataCaballoDDX, -11.3f, -1.5f));
		model = glm::rotate(model, glm::radians(pataCaballoDD), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		caballoPataDD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(-18.6f+pataCaballoDIX, -11.3f, 3.5f));
		model = glm::rotate(model, glm::radians(pataCaballoDI), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		caballoPataDI.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(2.3f+pataCaballoTDX, -9.3f, -3.0f));
		model = glm::rotate(model, glm::radians(pataCaballoTD), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		caballoPataTD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(2.5f+pataCaballoTIX, -9.3f, 2.0f));
		model = glm::rotate(model, glm::radians(pataCaballoTI), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		caballoPataTI.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-400.0f, 0.0f, 100.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cactus.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-300.0f, 0.0f, 200.0f));
		model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cactus.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-350.0f, 0.0f, 150.0f));
		model = glm::rotate(model, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cactus.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-300.0f, 0.0f, -50.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		roca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-125.0f, 0.0f, -450.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		roca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-330.0f, 0.0f, -35.0f));
		staticShader.setMat4("model", model);
		serpiente.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 50.0f));
		staticShader.setMat4("model", model);
		conejo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-150.0f, 0.0f, 75.0f));
		staticShader.setMat4("model", model);
		gallina.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-150.0f, 0.0f, 75.0f));
		staticShader.setMat4("model", model);
		gallinaCabeza.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-150.0f, 0.0f, 75.0f));
		staticShader.setMat4("model", model);
		gallinaPataD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-150.0f, 0.0f, 75.0f));
		staticShader.setMat4("model", model);
		gallinaPataI.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, 150.0f));
		staticShader.setMat4("model", model);
		vaca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-200.0f, 0.0f, 150.0f));
		staticShader.setMat4("model", model);
		vacaCabeza.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-330.0f, 0.0f, -40.0f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-370.0f, 0.0f, 100.0f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-250.0f, 0.0f, 60.0f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-400.0f, 0.0f, 200.0f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-450.0f, 0.0f, 150.0f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-450.0f, 0.0f, 300.0f));
		staticShader.setMat4("model", model);
		arbusto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-50.0f+motoX, 5.0f+motoY, -150.0f+motoZ));
		tmp = model = glm::rotate(model, glm::radians(giroMoto), glm::vec3(0.0f, 1.0f, 0.0f));	//Dependencia tmp
		tmp = model = glm::rotate(model, glm::radians(subeMoto), glm::vec3(1.0f, 0.0f, 1.0f));	//Dependencia tmp
		staticShader.setMat4("model", model);
		motocicleta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 11.0f, -26.0f));
		staticShader.setMat4("model", model);
		motocicletaManubrio.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, -32.0f));
		model = glm::rotate(model, glm::radians(ruedaLlantaMoto), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		motocicletaLlanta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(ruedaLlantaMoto), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		motocicletaLlanta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(262.5f, 0.0f, -3.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		circuito.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(375.0f+autoX, 0.0f + autoY, -200.0f + autoZ));
		tmp = model = glm::rotate(model, glm::radians(giroAuto), glm::vec3(0.0f, 1.0f, 0.0f));	//Dependencia tmp
		staticShader.setMat4("model", model);
		sedanModerno.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(0.0f, 6.0f, -21.7f));
		model = glm::rotate(model, glm::radians(ruedaLlantaAuto), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		modernoLlantas.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(-12.7f, 6.0f, 26.2f));
		model = glm::rotate(model, glm::radians(ruedaLlantaAuto), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(giroLlantaAuto), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		modernoLlanta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(tmp, glm::vec3(12.7f, 6.0f, 26.2f));
		model = glm::rotate(model, glm::radians(ruedaLlantaAuto), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f+giroLlantaAuto), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		modernoLlanta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(377.5f, 0.0f, -100.0f));
		staticShader.setMat4("model", model);
		sedan.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 150.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		camioneta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(375.0f, 0.0f, -300.0f));
		staticShader.setMat4("model", model);
		camioneta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -400.0f));
		staticShader.setMat4("model", model);
		buggi.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -400.0f));
		staticShader.setMat4("model", model);
		buggiLlantasT.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -400.0f));
		staticShader.setMat4("model", model);
		buggiLlantaDD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -400.0f));
		staticShader.setMat4("model", model);
		buggiLlantaDI.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(260.0f, 0.0f, 315.5f));
		staticShader.setMat4("model", model);
		motoneta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 220.0f));
		staticShader.setMat4("model", model);
		bicicleta.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardo.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoTorso.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoIngle.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoBrazoD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoBrazoI.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoManoD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoManoI.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoPiernaD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoPiernaI.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoPieD.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(150.0f, 0.0f, -150.0f));
		staticShader.setMat4("model", model);
		leonardoPieI.Draw(staticShader);

		//Caja Transparente
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/

		//--------------------------------------------------------------------------------------------------------------------------
		//Fin de SkyBox

		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		//--------------------------------------------------------------------------------------------------------------------------
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//----------------------------------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);

	//To Configure Model
	/*if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;*/
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//Animacion de auto
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		motorAuto ^= true;
		ruedaLlantaAuto = 0.0f;
		autoX = 0.0f;	//Reseteo
		autoY = 0.0f;	//Reseteo
		autoZ = 0.0f;	//Reseteo
		giroAuto = 0.0f;
		motorMoto ^= true;
		ruedaLlantaMoto = 0.0f;
		motoX = 0.0f;	//Reseteo
		motoY = 0.0f;	//Reseteo
		motoZ = 0.0f;	//Reseteo
		giroMoto = 0.0f;
		subeMoto = 0.0f;
	}

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

//glfw: whenever the window size changed (by OS or user resize) this callback function executes
//----------------------------------------------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

//glfw: whenever the mouse moves, this callback is called
//----------------------------------------------------------------------------------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

//glfw: whenever the mouse scroll wheel scrolls, this callback is called
//----------------------------------------------------------------------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}