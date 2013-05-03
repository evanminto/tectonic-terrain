#include "glCanvas.h"
#include "camera.h"

#if _WIN32
#include <Winsock2.h>
#else
#include <sys/time.h>
#endif

#include "mesh.h"
#include "argparser.h"

#ifndef M_PI
#define M_PI 3.14159
#endif

// ========================================================
// static variables of GLCanvas class

int GLCanvas::mouseButton;
int GLCanvas::mouseX;
int GLCanvas::mouseY;
ArgParser* GLCanvas::args;
Mesh* GLCanvas::mesh;
Camera* GLCanvas::camera;
bool GLCanvas::controlPressed;
bool GLCanvas::animate;
GLhandleARB GLCanvas::program;
GLhandleARB GLCanvas::vertex_shader;
GLhandleARB GLCanvas::fragment_shader;

// ========================================================

void GLCanvas::InitLight() {
  // Set the last component of the position to 0 to indicate
  // a directional light source

  GLfloat position[4] = { 30,30,100, 1};
  GLfloat diffuse[4] = { 0.7,0.7,0.7,1};
  GLfloat specular[4] = { 0,0,0,1};
  GLfloat ambient[4] = { 0.3, 0.3, 0.3, 1.0 };

  GLfloat zero[4] = {0,0,0,0};
  glLightfv(GL_LIGHT1, GL_POSITION, position);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, zero);
  glEnable(GL_LIGHT1);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_COLOR_MATERIAL);
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

  GLfloat spec_mat[4] = {1,1,1,1};
  float glexponent = 30;
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec_mat);

  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  float back_color[] = { 0.1,0.1,0.1,1};
  glMaterialfv(GL_BACK, GL_AMBIENT_AND_DIFFUSE, back_color);
  glEnable(GL_LIGHT1);
}


void GLCanvas::display(void)
{
  // Clear the display buffer, set it to the background color
  glClearColor(1,1,1,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set the camera parameters
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  InitLight(); // light will be a headlamp!
  camera->glPlaceCamera();

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  mesh->drawVBOs();
     
  // Swap the back buffer with the front buffer to display
  // the scene
  glutSwapBuffers();
}

// ========================================================
// Callback function for window resize
// ========================================================

void GLCanvas::reshape(int w, int h) {
  // Set the OpenGL viewport to fill the entire window
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);

  // Set the camera parameters to reflect the changes
  camera->glInit(w, h);

  args->width = w;
  args->height = h;
}

// ========================================================
// Callback function for mouse click or release
// ========================================================

void GLCanvas::mouse(int button, int /*state*/, int x, int y) {
  // Save the current state of the mouse.  This will be
  // used by the 'motion' function
  mouseButton = button;
  mouseX = x;
  mouseY = y;
  controlPressed = (glutGetModifiers() & GLUT_ACTIVE_CTRL) != 0;
}

// ========================================================
// Callback function for mouse drag
// ========================================================

void GLCanvas::motion(int x, int y) {
  // Left button = rotation
  // (rotate camera around the up and horizontal vectors)
  if (mouseButton == GLUT_LEFT_BUTTON) {
    camera->rotateCamera(0.005*(mouseX-x), 0.005*(mouseY-y));
    mouseX = x;
    mouseY = y;
  }
  // Middle button = translation
  // (move camera perpendicular to the direction vector)
  else if (mouseButton == GLUT_MIDDLE_BUTTON) {
    camera->truckCamera(mouseX-x, y-mouseY);
    mouseX = x;
    mouseY = y;
  }
  // Right button = dolly or zoom
  // (move camera along the direction vector)
  else if (mouseButton == GLUT_RIGHT_BUTTON) {
    if (controlPressed) {
      camera->zoomCamera(mouseY-y);
    } else {
      camera->dollyCamera(mouseY-y);
    }
    mouseX = x;
    mouseY = y;
  }

  // Redraw the scene with the new camera parameters
  glutPostRedisplay();
}

// =============================================================
// =============================================================

// this function gets executed repeatedly and allows us to animate the
// light source.  stats on the frames per sec are also printed.
void GLCanvas::idle() {
#if _WIN32    
  static int last_tick_count;
  static int last_fps_count;
  static int frames = -1;
  if (frames == -1) {    
    last_fps_count = last_tick_count = GetTickCount();
    frames = 0;
  }
  if (animate) {
    frames++;
    int this_tick_count = GetTickCount();
    // difference in milliseconds
    double diff = 0.001*(this_tick_count-last_tick_count);
    last_tick_count = this_tick_count;
    args->timer += diff;
    double diff_fps_time = 0.001*(this_tick_count-last_fps_count);
    if (diff_fps_time > 1.00) {      
      float fps = frames / float(diff_fps_time);
      std::cout << "fps: " << fps << std::endl;
      frames = 0;
      last_fps_count = this_tick_count;
    }
    mesh->setupVBOs();
    glutPostRedisplay();
  } else {
    last_tick_count = last_fps_count = GetTickCount();
  }
#else
  static timeval last_time;
  static timeval last_fps_time;
  static timeval last_step_time;
  static int frames = -1;
  if (frames == -1) {
    gettimeofday(&last_time,NULL);
    last_fps_time = last_time;
    frames = 0;
  }
  if (animate) {
    frames++;
    timeval this_time;
    gettimeofday(&this_time,NULL);
    // compute the difference from last time
    double diff = this_time.tv_sec - last_time.tv_sec + 
      0.000001 * (this_time.tv_usec - last_time.tv_usec);
    double diff_fps_time = this_time.tv_sec - last_fps_time.tv_sec + 
      0.000001 * (this_time.tv_usec - last_fps_time.tv_usec);
    double diff_step_time = this_time.tv_sec - last_step_time.tv_sec + 
      0.000001 * (this_time.tv_usec - last_step_time.tv_usec);
    last_time = this_time;
    // print out stats on the FPS occasionally
    if (diff_fps_time > 1.00) {      
      float fps = frames / float(diff_fps_time);
      std::cout << "fps: " << fps << std::endl;
      frames = 0;
      last_fps_time = this_time;
    }
    if (diff_step_time > args->timestep/1000.0) {
      mesh->displaceVertices();
      last_step_time = this_time;
    }
    args->timer += diff;
    mesh->setupVBOs();
    glutPostRedisplay();

  } else {
    // if we aren't animating the light source, avoid busy-waiting!
    gettimeofday(&last_time, NULL);
    last_fps_time = last_time;
    frames = 0;
    usleep (100);
  }
#endif
}

// ========================================================
// Callback function for keyboard events
// ========================================================

void GLCanvas::keyboard(unsigned char key, int /*x*/, int /*y*/) {
  switch (key) {
  case 'd': case 'D': case ' ': 
    mesh->displaceVertices();
    mesh->setupVBOs();
    break;
  case 'a': case 'A':
    animate = !animate;
    break;
    /*
  case 'r': case 'R':
    delete mesh;
    args->timer = 0.0;
    mesh = new Mesh(args);
    mesh->Load();
    mesh->initializeVBOs();
    mesh->setupVBOs();
    break;
    */
  case 'x': case 'X':
    args->glsl_enabled = !args->glsl_enabled;
    if (args->glsl_enabled) {
      if (!args->glsl_initialized) {
	// initialize the shaders
	InitShaders();
	args->glsl_initialized = true;
      }
      LoadCompileLinkShaders();
    }
    mesh->setupVBOs();
    break;
    /*
  case 'l' : case 'L':
    if (args->glsl_enabled) {
      assert (args->glsl_initialized);
      LoadCompileLinkShaders();
      mesh->setupVBOs();
    }
    break;
    */
  case 'q':  case 'Q':
    exit(0);
    break;
  default:
    printf("UNKNOWN KEYBOARD INPUT  '%c'\n", key);
  }
  glutPostRedisplay();
}

// ========================================================
// Initialize all appropriate OpenGL variables, set
// callback functions, and start the main event loop.
// This function will not return but can be terminated
// by calling 'exit(0)'
// ========================================================

void GLCanvas::initialize(ArgParser *_args, Mesh *_mesh) {
  args = _args;
  mesh = _mesh;
  Vec3f camera_position = Vec3f(0,1,4);
  Vec3f point_of_interest = Vec3f(0,0.5,0);
  Vec3f up = Vec3f(0,1,0);
  camera = new PerspectiveCamera(camera_position, point_of_interest, up, 20 * M_PI/180.0);

  // Set window parameters
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB | GLUT_STENCIL );
  glutInitWindowSize(args->width, args->height);
  glutInitWindowPosition(100,100);
  glutCreateWindow("OpenGL Viewer");
  int num_bits;
  glGetIntegerv(GL_STENCIL_BITS,&num_bits);
  if (num_bits < 8) {
    std::cout << "WARNING: stencil buffer is only " << num_bits << " bits." << std::endl;
  }
#ifdef _WIN32
  GLenum err = glewInit();
  if (err != GLEW_OK) {
      fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
      exit(1);
  }
#endif

  glEnable(GL_DEPTH_TEST);

  // Set global lighting parameters
  glEnable(GL_LIGHTING);
  glShadeModel(GL_SMOOTH);

  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glEnable(GL_NORMALIZE);

  // Ambient light
  Vec3f ambColor = Vec3f(0.2,0.2,0.2); 
  GLfloat ambArr[] = { float(ambColor.x()), float(ambColor.y()), float(ambColor.z()), 1.0 };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambArr);

  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glCullFace(GL_BACK);
  glDisable(GL_CULL_FACE);
  //display_list_index = glGenLists(1);

  // Initialize callback functions
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutIdleFunc(idle);

  mesh->initializeVBOs();
  mesh->setupVBOs();

  // Enter the main rendering loop
  glutMainLoop();
}

// ========================================================
// ========================================================


void HandleGLError(std::string msg) {
  GLenum error;
  int i = 0;
  while ((error = glGetError()) != GL_NO_ERROR) {
    printf ("GL ERROR(%d):  %s\n", i, gluErrorString(error));
    i++;
  }
  if (i != 0) {
    std::cout << msg << std::endl;
    exit(0);
  }
}

