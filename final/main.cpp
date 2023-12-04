//Curso: Computación gráfica, CCOMP7-1

#ifndef GLAD_GL_IMPLEMENTATION
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#endif

#ifndef GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif

#include <glm/gtc/type_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <cmath>
#include <vector>
#include <thread>
#include <map>
#include <chrono>
#include <string>
#include <filesystem>
#include "mylib/rubik.h"
#include "mylib/shader.h"
#include "mylib/camera.h"
#include <mutex> //for solvers

// solver: https://github.com/jamesmtexas/Rubik
#include "mylib/solver.hpp"
#include "Cube.hpp"
#include "Centers.hpp"
#include "Cross.hpp"
#include "Corners.hpp"
#include "Edges.hpp"
#include "OLL.hpp"
#include "PLL.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::pair;
using std::string;


// Declarar mutex global
std::mutex movementMutex;


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void simulation(const vector<string>& movements);
void simulation2(const vector<string>& movements2);

string path = std::filesystem::absolute(__FILE__).string() + "/../";
string shader_path = path + "mylib/shader_code/";

GLFWwindow* window;
const int SCR_WIDTH = 900;
const int SCR_HEIGHT = 900;
int delay = 1000; // Microseconds
int counter = 0;

// Cube global configuration
void solver_global(const vector<string>& movements, void (*simulationFunction)(const vector<string>&));
bool movsEnabled = true;
const float move_step = 10.0f;
bool expandCubesRequested = false;
bool resetCubesRequested = false;
bool temblorRequested = false;
bool acercarCubos = false;
int tam_explocion = 10;
int acaba_explocion = 0;
bool animar=false;
bool rotar_cubos=false;

// Variables específicas del primer cubo
Rubik* rubik;
bool moved = true;
rubik_side::Side side;
bool counter_clockwise = true;

// Variables específicas del segundo cubo
Rubik* secondRubik;
bool second_moved = true;
rubik_side::Side second_side;
bool second_counter_clockwise = true;



// Map movs variables Cube 1
vector<string> map_movs;
string clockwise_str;


map<string, pair<rubik_side::Side, bool>> rubik_movs{
    {"U'", {rubik_side::U, true}},
    {"U", {rubik_side::U, false}},
    {"D'", {rubik_side::D, true}},
    {"D", {rubik_side::D, false}},
    {"R'", {rubik_side::R, true}},
    {"R", {rubik_side::R, false}},
    {"L'", {rubik_side::L, true}},
    {"L", {rubik_side::L, false}},
    {"F'", {rubik_side::F, true}},
    {"F", {rubik_side::F, false}},
    {"B'", {rubik_side::B, true}},
    {"B", {rubik_side::B, false}},
    {"S'", {rubik_side::M, true}},
    {"S", {rubik_side::M, false}},
    {"E'", {rubik_side::E, true}},
    {"E", {rubik_side::E, false}},
    {"M'", {rubik_side::S, true}},
    {"M", {rubik_side::S, false}}
};


// Map movs variables Cube 2
vector<string> second_map_movs;
string second_clockwise_str;


map<string, pair<rubik_side::Side, bool>> second_rubik_movs{
    {"U'", {rubik_side::U, true}},
    {"U", {rubik_side::U, false}},
    {"D'", {rubik_side::D, true}},
    {"D", {rubik_side::D, false}},
    {"R'", {rubik_side::R, true}},
    {"R", {rubik_side::R, false}},
    {"L'", {rubik_side::L, true}},
    {"L", {rubik_side::L, false}},
    {"F'", {rubik_side::F, true}},
    {"F", {rubik_side::F, false}},
    {"B'", {rubik_side::B, true}},
    {"B", {rubik_side::B, false}},
    {"S'", {rubik_side::M, true}},
    {"S", {rubik_side::M, false}},
    {"E'", {rubik_side::E, true}},
    {"E", {rubik_side::E, false}},
    {"M'", {rubik_side::S, true}},
    {"M", {rubik_side::S, false}}
};




// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// lighting
glm::vec3 lightPos(0.0f, 1.0f, 0.0f);
glm::vec3 lightAmbient(0.5f, 0.5f, 0.5f);
glm::vec3 lightDiffuse(0.5f, 0.5f, 0.5f);
glm::vec3 lightSpecular(1.0f, 1.0f, 1.0f);
float lightShininess = 5.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	srand(time(nullptr));
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    gladLoadGL(glfwGetProcAddress);

    string light_shader = shader_path + "4.2.lighting_maps.vs";
    string light_frag = shader_path + "4.2.lighting_maps.fs";
    Shader lighting_shader(light_shader.c_str(), light_frag.c_str());
    
	////////////////////////////////////////////////////////////////////////////////////////////
    // Create drawables
	rubik = new Rubik(Vec4f(-2.0f, 0.0f, 0.0f, 1.0f), 1.0f);   // Primer cubo, alejado a la izquierda
	secondRubik = new Rubik(Vec4f(2.0f, 0.0f, 0.0f, 1.0f), 1.0f);  // Segundo cubo, alejado a la derecha
	////////////////////////////////////////////////////////////////////////////////////////////
	
	
    glPointSize(5.0f);
    glLineWidth(3.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //GL_FILL:GL_LINE


    GLuint color_location = glGetUniformLocation(lighting_shader.ID, "color");
    lighting_shader.setInt("material.diffuse", 1);
    lighting_shader.setInt("material.specular", 1);

    //Simulation of rubik movements
    //std::thread(simulation, movs).detach();
	int movimiento_acercarse = 0;
	int angulo_de_rotacion=15.0f;
    
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glClearColor(0.6, 0.6, 0.6, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Transforms para el primer cubo
		if (!moved) if (rubik->rotate_side(side, counter_clockwise, move_step)) moved = true;
		if (!second_moved) if (secondRubik->rotate_side(second_side, second_counter_clockwise, move_step)) second_moved = true;


		//ANIMACION TOTAL
		if(animar)
		{
			rotar_cubos=true;
			
			acercarCubos=true;
			if(angulo_de_rotacion==43){
				animar=false;
				expandCubesRequested = false;
				resetCubesRequested = false;
				temblorRequested = false;
				acercarCubos = false;
				rotar_cubos=false;
				
				rubik = new Rubik(Vec4f(-1.5f, 0.0f, 0.0f, 1.0f), 1.0f); 
				secondRubik = new Rubik(Vec4f(1.5f, 0.0f, 0.0f, 1.0f), 1.0f);
				map_movs.clear(); map_movs.clear();
				
				movimiento_acercarse=0;
				angulo_de_rotacion=15.0f;
			}	
			else if(movimiento_acercarse==150) {
				temblorRequested=true;
				resetCubesRequested=true;
				}
			else if(movimiento_acercarse>200 && movimiento_acercarse<=220) {
				angulo_de_rotacion+=1.0f;
				std::cout<<angulo_de_rotacion<<endl;
			}
			else if(movimiento_acercarse==249) {
				acercarCubos=false; 
				resetCubesRequested=false;
				expandCubesRequested=true;
				movimiento_acercarse=0;
			}
			
			if(acaba_explocion==118)
			{
				expandCubesRequested=false;
				rotar_cubos=false;
			}
		}
		
		
		if(rotar_cubos){
			rubik->rotateWholeCubeY(angulo_de_rotacion);
			secondRubik->rotateWholeCubeY(angulo_de_rotacion);
		}

		//acercar cubos
		if(acercarCubos)
		{
			// Llama a la función moveToCoordinate para iniciar la animación de movimiento
			rubik->translation(Vec4f(1.0f, 0.0f, 0.0f, 1.0f), 0.008f);
			secondRubik->translation(Vec4f(-1.0f, 0.0f, 0.0f, 1.0f), 0.008f);
			
			movimiento_acercarse++;
		}

		//temblar
		if (temblorRequested)
        {
			rubik->shake();
			secondRubik->shake(); //segundo cubo
			//rubik->shake2();
			//rubik->temblar();
		}
	
		//Exploción
        if (expandCubesRequested)
        {
			rubik->expandCubes();
			secondRubik->expandCubes(); //segundo cubo
			std::cout<<"acaba explocion: "<<acaba_explocion<<endl;
			acaba_explocion++;
        }
		
		//Regresa explocion
		if (resetCubesRequested)
        {
			rubik->resetCubes();
			secondRubik->resetCubes(); //segundo cubo
			acaba_explocion++;
		}
		
		
		
        //rubik->rotation_centroid(Vec4f(1.0f, 1.0f, 0.0f, 1.0f), 0.5f);

        // be sure to activate shader when setting uniforms/drawing objects
        lighting_shader.use();
        lighting_shader.setVec3("light.position", lightPos);
        lighting_shader.setVec3("viewPos", camera.get_position());
        // light properties
        lighting_shader.setVec3("light.ambient", lightAmbient);
        lighting_shader.setVec3("light.diffuse", lightDiffuse);
        lighting_shader.setVec3("light.specular", lightSpecular);
        // material properties
        lighting_shader.setFloat("material.shininess", lightShininess);
        // View
        glm::mat4 view = camera.getViewMatrix();
        lighting_shader.setMat4("view", view);
        // Projection
        glm::mat4 projection = glm::perspective(glm::radians(camera.get_zoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lighting_shader.setMat4("projection", projection);
        // Draw
        rubik->draw(color_location);
		secondRubik->draw(color_location);

        glfwSwapBuffers(window);
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::microseconds(delay));
    }

    glfwTerminate();
    return 0;
}

// Input

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        camera.processKeyboard(camera_movement::LEFT, deltaTime);
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        camera.processKeyboard(camera_movement::RIGHT, deltaTime);
    }
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        camera.processKeyboard(camera_movement::FORWARD, deltaTime);
    }
    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        camera.processKeyboard(camera_movement::BACKWARD, deltaTime);
    }
    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        camera.processKeyboard(camera_movement::UP, deltaTime);
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        camera.processKeyboard(camera_movement::DOWN, deltaTime);
    }
    
    if (key == GLFW_KEY_C && (action == GLFW_PRESS || action == GLFW_REPEAT)){
        rubik = new Rubik(Vec4f(-1.0f, 0.0f, 0.0f, 1.0f), 1.0f); 
		secondRubik = new Rubik(Vec4f(1.0f, 0.0f, 0.0f, 1.0f), 1.0f);
        map_movs.clear(); map_movs.clear();
    }
    
    if (movsEnabled){ // Disable movement when simulation is running
        // Solver
		if (key == GLFW_KEY_Z && action == GLFW_PRESS){
			solver_global(map_movs, simulation);
        }
		
		// Solver for the second cube
		if (key == GLFW_KEY_X && action == GLFW_PRESS) {
			solver_global(second_map_movs, simulation2);
		}

		
        // Movements by keyboard
		if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS && moved){
            counter_clockwise = true;
            clockwise_str = "'";
        } else if ( moved ) {
            counter_clockwise = false;
            clockwise_str = "";
        }
		
		//First Cube
        if (key == GLFW_KEY_J && action == GLFW_PRESS && moved){
            side = rubik_side::L;
            moved = false;
            map_movs.push_back("L" + clockwise_str);
        }
        if (key == GLFW_KEY_L && action == GLFW_PRESS && moved){
            side = rubik_side::R;
            moved = false;
            map_movs.push_back("R" + clockwise_str);
        }
        if (key == GLFW_KEY_I && action == GLFW_PRESS && moved){
            side = rubik_side::U;
            moved = false;
            map_movs.push_back("U" + clockwise_str);
        }
        if (key == GLFW_KEY_K && action == GLFW_PRESS && moved){
            side = rubik_side::D;
            moved = false;
            map_movs.push_back("D" + clockwise_str);
        }
        if (key == GLFW_KEY_U && action == GLFW_PRESS && moved){
            side = rubik_side::F;
            moved = false;
            map_movs.push_back("F" + clockwise_str);
        }
        if (key == GLFW_KEY_O && action == GLFW_PRESS && moved){
            side = rubik_side::B;
            moved = false;
            map_movs.push_back("B" + clockwise_str);
        }
        if (key == GLFW_KEY_M && action == GLFW_PRESS && moved){
            side = rubik_side::M;
            moved = false;
            map_movs.push_back("S" + clockwise_str);
        }
        if (key == GLFW_KEY_COMMA && action == GLFW_PRESS && moved){
            side = rubik_side::E;
            moved = false;
            map_movs.push_back("E" + clockwise_str);
        }
        if (key == GLFW_KEY_PERIOD && action == GLFW_PRESS && moved){
            side = rubik_side::S;
            moved = false;
            map_movs.push_back("M" + clockwise_str);
        }
		
		//Second Cube
		if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS && second_moved){
            second_counter_clockwise = true;
            second_clockwise_str = "'";
        } else if ( second_moved ) {
            second_counter_clockwise = false;
            second_clockwise_str = "";
        }
		
		
		if (key == GLFW_KEY_F && action == GLFW_PRESS && second_moved)
		{
			second_side = rubik_side::L;
			second_moved = false;
			second_map_movs.push_back("L" + second_clockwise_str);
		}
        if (key == GLFW_KEY_H && action == GLFW_PRESS && second_moved){
            second_side = rubik_side::R;
            second_moved = false;
            second_map_movs.push_back("R" + second_clockwise_str);
        }
        if (key == GLFW_KEY_T && action == GLFW_PRESS && second_moved){
            second_side = rubik_side::U;
            second_moved = false;
            second_map_movs.push_back("U" + second_clockwise_str);
        }
        if (key == GLFW_KEY_G && action == GLFW_PRESS && second_moved){
            second_side = rubik_side::D;
            second_moved = false;
            second_map_movs.push_back("D" + second_clockwise_str);
        }
        if (key == GLFW_KEY_R && action == GLFW_PRESS && second_moved){
            second_side = rubik_side::F;
            second_moved = false;
            second_map_movs.push_back("F" + second_clockwise_str);
        }
        if (key == GLFW_KEY_Y && action == GLFW_PRESS && second_moved){
            second_side = rubik_side::B;
            second_moved = false;
            second_map_movs.push_back("B" + second_clockwise_str);
        }
        if (key == GLFW_KEY_V && action == GLFW_PRESS && second_moved){
            second_side = rubik_side::M;
            second_moved = false;
            second_map_movs.push_back("S" + second_clockwise_str);
        }
        if (key == GLFW_KEY_B && action == GLFW_PRESS && second_moved){
            second_side = rubik_side::E;
            second_moved = false;
            second_map_movs.push_back("E" + second_clockwise_str);
        }
        if (key == GLFW_KEY_N && action == GLFW_PRESS && second_moved){
            second_side = rubik_side::S;
            second_moved = false;
            second_map_movs.push_back("M" + second_clockwise_str);
        }
		
		// Animation cubes
		if (key == GLFW_KEY_1 && action == GLFW_PRESS && !expandCubesRequested)
		{
			expandCubesRequested = true;
		}
		if (key == GLFW_KEY_2 && action == GLFW_PRESS && !expandCubesRequested)
		{
			resetCubesRequested=true;
		}
		if (key == GLFW_KEY_3 && action == GLFW_PRESS && !expandCubesRequested)
		{
			temblorRequested=true;
		}
		if (key == GLFW_KEY_4 && action == GLFW_PRESS && !expandCubesRequested)
		{
			acercarCubos=true;
		}
		if (key == GLFW_KEY_9 && action == GLFW_PRESS && !expandCubesRequested)
		{
			std::cout<<"La animacion comenzo"<<endl;
			animar=true;
		}
		if (key == GLFW_KEY_8 && action == GLFW_PRESS && !expandCubesRequested)
		{
			std::cout<<"Reseteo"<<endl;
			temblorRequested=false;
		}
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.processMouseScroll(float(yoffset));
}

void solver_global(const vector <string>& movements, void (*simulationFunction)(const vector<string>&)) {
    // Crear string de movimientos
    string movementsStr;
    for (const auto& mov : movements) {
        movementsStr += mov + " ";
    }
    std::cout << "Movements: " << movementsStr << "\n";

    // Crear cubo y procesar movimientos
    solver::Cube myCube(false);
    movementsStr = solver::format(movementsStr);
    myCube.moves(movementsStr, false);

    solver::Centers::solveCenters(myCube);
    solver::Cross::solveCross(myCube);
    solver::Corners::solveCorners(myCube);
    solver::Edges::solveEdges(myCube);
    solver::OLL::solveOLL(myCube);
    solver::PLL::solvePLL(myCube);

    std::cout << "Solution: " << myCube.solution << "\n\n";

    // Lanzar hilo de simulación
    std::thread(simulationFunction, solver::split(myCube.solution)).detach();
}


void simulation(const vector<string>& movements){
    movsEnabled = false;
    int i = 0, n_movs = movements.size();
    while (i < n_movs) {
        std::unique_lock<std::mutex> lock(movementMutex);
        if (moved) {
            side = rubik_movs[movements[i]].first;
            counter_clockwise = rubik_movs[movements[i]].second;
            moved = false;
            i++;
        }
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    movsEnabled = true;
}

void simulation2(const vector<string>& movements2){
    movsEnabled = false;
    int i = 0, n_movs = movements2.size();
    while (i < n_movs) {
        std::unique_lock<std::mutex> lock(movementMutex);
        if (second_moved) {
            second_side = second_rubik_movs[movements2[i]].first;
            second_counter_clockwise = second_rubik_movs[movements2[i]].second;
            second_moved = false;
            i++;
        }
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
    movsEnabled = true;
}
