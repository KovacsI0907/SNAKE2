#include <shaderprogram.h>
#include <shadersources.h>
#include <scene.h>
#include <camera.h>
#include <observer.h>

class Engine {
protected:
	static unsigned int width, height;
	static ShaderProgram mainShader, depthShader;

	static unsigned int depthMapResolution;
	static unsigned int depthMapFBO;
	static unsigned int depthMapTexture;
	
	static void onDisplay();
	static void onIdle();
	static OrthographicCamera getDepthCam(float shadowRadius, vec4 lightPos);
	static void loadUniforms(Object* object, Camera* camera, ShaderProgram* shaderProgram);

	static long time;

public:
	static Scene* activeScene;

	Engine() = delete;
	Engine(Engine& other) = delete;
	void operator=(const Engine&) = delete;

	static void initialize(int* argc, char* argv[]);
	static void start();
	static void render(Scene* scene);

	static Event<void> initPreGL;
	static Event<void> initPostGL;

	static Event<void> preDraw;
	static Event<long> update;
};