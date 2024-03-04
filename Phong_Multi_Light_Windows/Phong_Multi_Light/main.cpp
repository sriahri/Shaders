#include <iostream>
#include <memory>
#include <sstream>
#include <gl/glew.h>
#include <gl/freeglut.h>

#include <MouseCamera.h>
#include <Mesh.h>
#include <Shader.h>

using namespace sgpu;

const char* WINDOW_TITLE = "[CSCI-4800/5800] Shader and GPU Programming";
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 400;
GLint g_glutWindowIdentifier;

const static int LIGHT_COUNT = 4;

std::shared_ptr<MouseCameraf> camera = nullptr;
std::shared_ptr<Mesh> mesh = nullptr;
std::shared_ptr<Mesh> ground_mesh = nullptr;

// Structure for defining the properties of a light. Note that this definition
// should line up with the structure defined in the vertex/fragment shaders.
struct Light {
	Vector3f position;
	Vector3f ambient;
	Vector3f diffuse;
	Vector3f specular;
};

// Array of lights (n=4)
Light lights[LIGHT_COUNT];

// Helper function for loading the meshes for the basic scene setup.
bool LoadMeshes(
	const std::string& plane_name,
	const std::string& object_name,
	const std::string& shader_vert_name,
	const std::string& shader_frag_name) {

	// Display mesh
	if ( !mesh->load(object_name) ) return false;
	if ( !mesh->loadShader(shader_vert_name, shader_frag_name) ) return false;

	// Ground mesh
	if ( !ground_mesh->load(plane_name) ) return false;
	if ( !ground_mesh->loadShader(shader_vert_name, shader_frag_name) ) return false;

	return true;
}

bool SetupLights() {

	lights[0].position = Vector3f(6.0f, 4.0f, 0.0f);
	lights[0].ambient = Vector3f(0.9f, 0.1f, 0.1f);
	lights[0].diffuse = Vector3f(1.0f, 0.2f, 0.2f);
	lights[0].specular = Vector3f(0.6f, 0.0f, 0.0f);

	// Light 1 (Green light on the left)
	lights[1].position = Vector3f(-6.0f, 4.0f, 0.0f);
	lights[1].ambient = Vector3f(0.1f, 0.9f, 0.1f);
	lights[1].diffuse = Vector3f(0.2f, 0.7f, 0.2f);
	lights[1].specular = Vector3f(0.0f, 0.6f, 0.0f);

	// Light 2 (Blue light at the back)
	lights[2].position = Vector3f(0.0f, 5.0f,-10.0f);
	lights[2].ambient = Vector3f(0.1f, 0.1f, 0.9f);
	lights[2].diffuse = Vector3f(0.2f, 0.2f, 0.7f);  
	lights[2].specular = Vector3f(0.0f, 0.0f, 0.6f); 

	// Light 3 (White light in the front)
	lights[3].position = Vector3f(0.0f, 6.0f, 10.0f);
	lights[3].ambient = Vector3f(0.4f, 0.4f, 0.4f);
	lights[3].diffuse = Vector3f(1.0f, 1.0f, 1.0f);  
	lights[3].specular = Vector3f(1.0f, 1.0f, 1.0f);

	return true;
}

void g_init() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glewInit();

	mesh = std::make_shared<Mesh>();
	mesh->setScale(0.5f, 0.5f, 0.5);
	mesh->setPosition(0.0f, 2.0f, 0.0f);

	ground_mesh = std::make_shared<Mesh>();
	ground_mesh->setScale(2.0f, 2.0f, 2.0f);

	bool b = LoadMeshes("models/plane.obj", "models/sphere.obj", "shaders/MultiLight.vert", "shaders/MultiLight.frag");
	if ( b == false ) {
		std::cerr << "[Main] Error loading resource files." << std::endl;
		std::cin.get();
		std::exit(1);
	}

	SetupLights();

    camera = std::make_shared<MouseCameraf>(1.0f);
    camera->setPosition(18.0f, 1.5707f, 1.570f * 0.7f);
}

void g_glutReshapeFunc(int width, int height) {
	glViewport(0, 0, width, height);
    camera->setPerspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
	glutPostRedisplay();
}

void g_glutDisplayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Camera Matrices
	Matrix4f view = camera->getViewMatrix();
	Matrix4f normalMatrix = Matrix4f::Transpose(camera->getViewMatrix().toInverse());
	Matrix4f projectionMatrix = camera->getProjectionMatrix();

	// Object Specific Matrices
    Matrix4f model = mesh->getTransform().toMatrix();
    Matrix4f modelViewMatrix = model * camera->getViewMatrix();
	
	// Render the object
    mesh->beginRender();
    mesh->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
    mesh->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
    mesh->getShader()->uniformMatrix("normalMatrix", normalMatrix);
	// TODO: Uniform light properties...
	// For example, you can uniform to an array variable by its text name in GLSL:
	//
	// mesh->getShader()->uniformVector("lights[0].position", lights[0].position);
	// mesh->getShader()->uniformVector("lights[0].ambient", lights[0].ambient);
	// ...
	//
	// "lights[0].position" -> Can be used to assign to each position in the lights array
	// lights[0].position -> Is the provided vector value for light[0] position
	//
	// This must be done for each light and each property.
	for (int i = 0; i < LIGHT_COUNT; ++i) {
		std::string lightPrefix = "lights[" + std::to_string(i) + "].";
		mesh->getShader()->uniformVector(lightPrefix + "position", lights[i].position);
		mesh->getShader()->uniformVector(lightPrefix + "ambient", lights[i].ambient);
		mesh->getShader()->uniformVector(lightPrefix + "diffuse", lights[i].diffuse);
		mesh->getShader()->uniformVector(lightPrefix + "specular", lights[i].specular);
	}
    mesh->endRender();

	model = ground_mesh->getTransform().toMatrix();
	modelViewMatrix = model * camera->getViewMatrix();

	// Render the ground
	ground_mesh->beginRender();
	ground_mesh->getShader()->uniformMatrix("projectionMatrix", projectionMatrix);
	ground_mesh->getShader()->uniformMatrix("modelViewMatrix", modelViewMatrix);
	ground_mesh->getShader()->uniformMatrix("normalMatrix", normalMatrix);
	// TODO: Uniform light properties...
	for (int i = 0; i < LIGHT_COUNT; ++i) {
		std::string lightPrefix = "lights[" + std::to_string(i) + "].";
		ground_mesh->getShader()->uniformVector(lightPrefix + "position", lights[i].position);
		ground_mesh->getShader()->uniformVector(lightPrefix + "ambient", lights[i].ambient);
		ground_mesh->getShader()->uniformVector(lightPrefix + "diffuse", lights[i].diffuse);
		ground_mesh->getShader()->uniformVector(lightPrefix + "specular", lights[i].specular);
	}
	ground_mesh->endRender();

	glutSwapBuffers();
	glFlush();
}

void g_glutMotionFunc(int x, int y) {
	camera->onMouseMove(x, y);
	glutPostRedisplay();
}

void g_glutMouseFunc(int button, int state, int x, int y) {
	if ( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN ) camera->onMouseButton(LB_DOWN, x, y);
    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN ) camera->onMouseButton(RB_DOWN, x, y);
    if ( button == GLUT_LEFT_BUTTON && state == GLUT_UP ) camera->onMouseButton(LB_UP, x, y);
    if ( button == GLUT_RIGHT_BUTTON && state == GLUT_UP ) camera->onMouseButton(RB_UP, x, y);
}

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	g_glutWindowIdentifier = glutCreateWindow(WINDOW_TITLE);

	glutDisplayFunc(g_glutDisplayFunc);
	glutReshapeFunc(g_glutReshapeFunc);
    glutMotionFunc(g_glutMotionFunc);
    glutMouseFunc(g_glutMouseFunc);

	g_init();

	glutMainLoop();
	return 0;
}

