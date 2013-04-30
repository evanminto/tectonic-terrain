// Inspired by Sumanta Guha
// Computer Graphics Through OpenGL: From Theory to Experiments
// 2011

#include <fstream>
#include "bmp.h"

// Routine to read a bitmap file.
// Works only for uncompressed bmp files of 24-bit color.
BitMapFile *getBMPData(std::string filename)
{
  BitMapFile *bmp = new BitMapFile;
  unsigned int size, offset, headerSize;
  
  // Read input file name.
  std::ifstream infile(filename.c_str(), std::ios::binary);
 
  // Get the starting point of the image data.
  infile.seekg(10);
  infile.read((char *) &offset, 4); 
  
  // Get the header size of the bitmap.
  infile.read((char *) &headerSize,4);
  
  // Get width and height values in the bitmap header.
  infile.seekg(18);
  infile.read( (char *) &bmp->sizeX, 4);
  infile.read( (char *) &bmp->sizeY, 4);
  
  // Allocate buffer for the image.
  size = bmp->sizeX * bmp->sizeY * 24;
  bmp->data = new unsigned char[size];

  // Read bitmap data.
  infile.seekg(offset);
  infile.read((char *) bmp->data , size);
  
  // Reverse color from bgr to rgb.
  int temp;
  for (unsigned int i = 0; i < size; i += 3)
  { 
    temp = bmp->data[i];
	  bmp->data[i] = bmp->data[i+2];
	  bmp->data[i+2] = temp;
  }

  return bmp;
}

/*void loadExternalTextures()			
{
  // Local storage for bmp image data.
  BitMapFile *image[6];

  // Load the textures.
  image[0] = getBMPData("Textures/front.bmp");
  image[1] = getBMPData("Textures/left.bmp");
  image[2] = getBMPData("Textures/back.bmp");
  image[3] = getBMPData("Textures/right.bmp");
  image[4] = getBMPData("Textures/top.bmp");
  image[5] = getBMPData("Textures/ground.bmp");

  // Bind front image to texture index[0]. 
  glBindTexture(GL_TEXTURE_2D, texture[0]); 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[0]->sizeX, image[0]->sizeY, 0, 
    GL_RGB, GL_UNSIGNED_BYTE, image[0]->data);

  // Bind left image to texture index[1]
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[1]->sizeX, image[1]->sizeY, 0, 
    GL_RGB, GL_UNSIGNED_BYTE, image[1]->data);				

	// Bind back image to texture index[2]
  glBindTexture(GL_TEXTURE_2D, texture[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[2]->sizeX, image[2]->sizeY, 0, 
    GL_RGB, GL_UNSIGNED_BYTE, image[2]->data);	

  // Bind right image to texture index[3]
  glBindTexture(GL_TEXTURE_2D, texture[3]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[3]->sizeX, image[3]->sizeY, 0, 
    GL_RGB, GL_UNSIGNED_BYTE, image[3]->data);

  // Bind top image to texture index[4]
  glBindTexture(GL_TEXTURE_2D, texture[4]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image[4]->sizeX, image[4]->sizeY, 0, 
    GL_RGB, GL_UNSIGNED_BYTE, image[4]->data);

  // Bind ground image to texture index[5]
  glBindTexture(GL_TEXTURE_2D, texture[5]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, image[5]->sizeX, image[5]->sizeY, 
	  GL_RGB, GL_UNSIGNED_BYTE, image[5]->data);
}

// Initialization routine
void setup(void)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);

  // Create texture index array.
  glGenTextures(6, texture);

  // Load external textures.
  loadExternalTextures();

  glEnable(GL_TEXTURE_2D);
}

// Routine to draw the skybox
void drawSkybox(void)
{
  // Store the current matrix
  glPushMatrix();

  // Reset and transform the matrix
  glLoadIdentity();
  gluLookAt(0, 0.2, 0, cameraX, cameraY, cameraZ, 0, 1, 0);

  // Enable and disable features
  glPushAttrib(GL_ENABLE_BIT);
  glEnable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  // Set vertices to white
  glColor4f(1,1,1,1);

  // Render the front quad
  glBindTexture(GL_TEXTURE_2D, texture[0]);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f( 0.5f, -0.5f, -0.5f);
    glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1, 1); glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(0, 1); glVertex3f( 0.5f,  0.5f, -0.5f);
  glEnd();

  // Render the left quad
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(1, 0); glVertex3f( 0.5f, -0.5f, -0.5f);
    glTexCoord2f(1, 1); glVertex3f( 0.5f,  0.5f, -0.5f);
    glTexCoord2f(0, 1); glVertex3f( 0.5f,  0.5f,  0.5f);
  glEnd();

  // Render the back quad
  glBindTexture(GL_TEXTURE_2D, texture[2]);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1, 0); glVertex3f( 0.5f, -0.5f,  0.5f);
    glTexCoord2f(1, 1); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(0, 1); glVertex3f(-0.5f,  0.5f,  0.5f);
  glEnd();

  // Render the right quad
  glBindTexture(GL_TEXTURE_2D, texture[3]);
  glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f,  0.5f);
    glTexCoord2f(1, 1); glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(0, 1); glVertex3f(-0.5f,  0.5f, -0.5f);
  glEnd();

  // Render the top quad
  glBindTexture(GL_TEXTURE_2D, texture[4]);
  glBegin(GL_QUADS);
    glTexCoord2f(1, 0); glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(1, 1); glVertex3f(-0.5f,  0.5f,  0.5f);
    glTexCoord2f(0, 1); glVertex3f( 0.5f,  0.5f,  0.5f);
    glTexCoord2f(0, 0); glVertex3f( 0.5f,  0.5f, -0.5f);
  glEnd();

  // Render the bottom quad
  glColor3f(0,0,0);
  glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
  glEnd();
  
  glPopAttrib();
  glPopMatrix();
}

// Routine to draw the ground plane
void drawGround(void)
{
  glPushMatrix();
  glLoadIdentity();
  gluLookAt(0, 0.2, 0, cameraX, cameraY, cameraZ, 0, 1, 0);

  glColor4f(1,1,1,1);
    
  glEnable(GL_TEXTURE_2D);

  // Render the ground plane
  glBindTexture(GL_TEXTURE_2D, texture[5]);        
  glBegin(GL_POLYGON);
    glTexCoord2f( 0.0,  0.0); glVertex3f(-5.0, 0.0,  5.0);
    glTexCoord2f(50.0,  0.0); glVertex3f( 5.0, 0.0,  5.0);
    glTexCoord2f(50.0, 50.0); glVertex3f( 5.0, 0.0, -5.0);
    glTexCoord2f( 0.0, 50.0); glVertex3f(-5.0, 0.0, -5.0);
  glEnd();

  glPopMatrix();
}

// Drawing routine
void drawScene(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawSkybox();
  drawGround();

  glutSwapBuffers();
}

// OpenGL window reshape routine
void resize(int w, int h)
{
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-0.1, 0.1, -0.1, 0.1, 0.1, 1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

// Keyboard input processing routine
void keyInput(unsigned char key, int x, int y)
{
  switch(key)
  {
  case 27:
    exit(0);
    break;
  case 'x':
    cameraX -= 0.1;
    cameraZ -= 0.1;
    glutPostRedisplay();
    break;
  case 'X':
    cameraX += 0.1;
    cameraZ += 0.1;
    glutPostRedisplay();
    break;
  default:
    break;
  }
}

// Routine to output interaction instructions to the terminal
void printInteraction(void)
{
  cout << "Interaction:" << endl;
}

// Main routine
int main(int argc, char **argv)
{
  printInteraction();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(300, 500);
  glutCreateWindow("Ferris Wheel");
  setup();
  glutDisplayFunc(drawScene);
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyInput);
  glutMainLoop();

  return 0;
}*/
