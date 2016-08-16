#include <cstdint>
#include <cstdio>

#define GLUT_NO_LIB_PRAGMA
#include "lib/glut.h"

int argc = 1; 
const char* argv[] ={__FILE__};

void displayFunc();
void motionFunc( int, int );
void keyboardFunc( unsigned char, int, int );
void mouseFunc( int, int, int, int );
void timerFunc( int );
void menuFunc( int );

int main() {
  glutInit( &argc, const_cast<char**>( argv ) );
  glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_ALPHA );
  glutInitWindowSize( 640, 480 );

  uintptr_t win1 = glutCreateWindow("Test");
  glutSetWindow( win1 );
  uintptr_t menu1 = glutCreateMenu( menuFunc );
  glutSetMenu( menu1 );
  glutAddMenuEntry( "Test 1", 1 );
  glutAddMenuEntry( "Test 2", 2 );
  glutAttachMenu( GLUT_RIGHT_BUTTON );

  glutDisplayFunc( displayFunc );
  glutMotionFunc( motionFunc );
  glutKeyboardFunc( keyboardFunc );
  glutMouseFunc( mouseFunc );
  glutTimerFunc( 100, timerFunc, 0 );
  
  glutMainLoop();


return 0;
}

void displayFunc() {
  printf("Start\n");
  glClearColor( 0, 0, 0, 0);
  glClearDepth( 0 );
  glClearStencil( 0 );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
}
void menuFunc( int menuid ) {
  printf("Menu %d\n", menuid );
}
void motionFunc( int, int ) {}
void keyboardFunc( unsigned char, int, int ) {}
void mouseFunc( int, int, int, int ) {}

void timerFunc( int value ) {

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

  glBegin(GL_TRIANGLES);
  glVertex3f( 0, 0, 0 );
    glColor3f( 255,0,0);
    glVertex3f( 0, 1, 0 );
    glColor3f( 0,0,0);
    glVertex3f( 1, 1, 0 );
    glColor3f( 0,0,0);
  glEnd();

  glColor3f( 255, 255, 255 );
  glRasterPos2f( -1.f, -1.f );
  glutBitmapCharacter( GLUT_BITMAP_8_BY_13, 'A' );
  glutSwapBuffers();
  glutTimerFunc( 100, timerFunc, 0 );
}






