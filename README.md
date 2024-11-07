# 3D rendering library
A simple library used to make 3D games.

__Disclaimer:__ Work in progress (most things are subject to change)

<p align="center">
  <img src="https://github.com/user-attachments/assets/0809d40b-d650-477a-816e-063c2c9e091c" alt="A screenshot from the demo project"/>
</p>


# Getting started
You can create your game by forking the repo and using the provided Engine class.
## A simple app
This is an example of an extremely simple app that displays a quad and allows the user to control the view with the keyboard and mouse.
- Fork the project and clone it
- Edit the `src/main.cpp` file (or just use the already existing demo code in the same file)
```
// Define global objects like lights and cameras
PerspectiveCamera perspCam = PerspectiveCamera(vec3(3, -3, 2), vec3(0, 0, 0), 1, M_PI / 3);
CameraController perspectiveController = CameraController(&perspCam);

// Make a directional light
Light light = { vec4(-1, -3, 3, 0), vec4(1,1,1,1) };
Scene scene = Scene(&orthoCam, light, vec4(0.1, 0.1, 0.1, 0.1));

// chess.com like texture
auto chessTex = std::make_unique<ChessTex>(vec4(184.0f/255, 135.0f/255, 98.0f/255, 1), vec4(233.0f/255, 211.0f/255, 173.0f/255, 1),25,25);

//create materials that uses the texture
Material chessMaterial = { vec4(1, 1, 1, 1), vec4(1, 0, 0, 0), vec4(1,1,1,1), 5.0f, chessTex.get()};

// the init function sets up the scene
// later it will be registered to be called after opengl loaded
void init() {
	scene.light = light;

  // make a quad and adjust its size
	auto ground = std::make_unique<Object>(std::make_unique<Quad>(vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, -1, 0), vec3(1, 1, 0)), chessMaterial);
	float sf = sqrtf(5.0f);
	ground->scale = vec3(sf, sf, sf);
	scene.addObject(std::move(ground));
	scene.addTexture(std::move(chessTex));

	scene.create();
}


int main(int argc, char* argv[]) {
	// call init after opengl has been initialized
	Engine::initPostGL.addObserver(std::make_shared<Observer<void>>(init));

	// start the program
	Engine::initialize(&argc, argv);
	Engine::activeScene = &scene;
	Engine::start();
	return 1;
}
```

## Building with CMake
If you haven't yet, install [cmake](https://cmake.org/).
Now enter the build directory:

`cd /path_to_project/build`

Prepare cmake files:

`cmake ..`

Build the project:

`cmake --build .`

Alternatively, if you are using the Visual Studio compiler, you can look for the solution file (.sln) in the build directory and open the project with Visual Studio.

(This project does not support platforms other than windows yet.)

# Acknowledgement
This project uses:
- __FreeGLUT__ for window creation and handling user inputs ([FreeGLUT website](https://freeglut.sourceforge.net/))
- __GLEW__ to make working with openGL much easier ([GLEW website](https://freeglut.sourceforge.net/))
