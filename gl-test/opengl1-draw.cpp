#include "opengl1-shaders.cpp"

GLuint vbuffer, vao1, vindex1, program, shader, texture;
GLfloat rect[] = { -1, -1, 0,   -1, 1, 1,   1, -1, 0,   1, 1, 1 };
GLint indexes[] = {0,1,2,2,1,3};

//bool opengl_shader();
bool opengl_setup() {
  printf("Setup\n");
  glGenBuffers(  1, &vbuffer );
  glBindBuffer ( GL_ARRAY_BUFFER, vbuffer );
  glBufferData( GL_ARRAY_BUFFER, sizeof rect, rect, GL_STATIC_DRAW );

  glGenVertexArrays( 1, &vao1 );
  glBindVertexArray( vao1 );

  glGenBuffers(  1, &vindex1 );
  glBindBuffer ( GL_ELEMENT_ARRAY_BUFFER, vindex1 );
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof indexes, indexes, GL_STATIC_DRAW );

  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );
  glEnableVertexAttribArray( 0 );

  glBindBuffer( GL_ARRAY_BUFFER, 0 );

  glGenTextures( 1, &texture );
  glActiveTexture( GL_TEXTURE0 );
  glBindTexture( GL_TEXTURE_2D, texture );
  unsigned char imgdata[] = {0,0,0,255,  0,0,255,255,  0,0,255,255,  0,0,0,255};
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, 2, 2, 0, GL_BGRA, GL_UNSIGNED_BYTE, imgdata );  
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0 );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

  opengl_print_errors();
  return true;//opengl_shader();
}

void opengl_delshader() {
  glUseProgram( 0 );
  glDeleteProgram( program );
}

bool opengl_shader( const char* vertexshader, const char* fragmentshader ) {
  printf("Create program\nVertex shader\n%s\nFragment shader\n%s",vertexshader, fragmentshader);
  GLint status = 0;

  program = glCreateProgram();

  shader = glCreateShader( GL_VERTEX_SHADER );
  glShaderSource( shader, 1, &vertexshader, nullptr );
  glCompileShader( shader );

  glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
  if(status == GL_FALSE) {
    char msg[1024]; GLsizei len;
    glGetShaderInfoLog( shader, sizeof msg, &len, msg );
    printf("*vertex shader error, length = %d: %s", len, msg );
    return false;
  }
  glAttachShader( program, shader );
  glDeleteShader( shader );

  shader = glCreateShader( GL_FRAGMENT_SHADER );
  glShaderSource( shader, 1, &fragmentshader, nullptr );
  glCompileShader( shader );

  glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
  if(status == GL_FALSE) {
    char msg[1024]; GLsizei len;
    glGetShaderInfoLog( shader, sizeof msg, &len, msg );
    printf("*fragment shader error, length = %d: %s", len, msg );
    return false;
  }
  glAttachShader( program, shader );
  glDeleteShader( shader );

  glLinkProgram( program );
  glGetProgramiv( program, GL_LINK_STATUS, &status );  
  if(status == GL_FALSE) {
    char msg[1024]; GLsizei len;
    glGetProgramInfoLog( shader, sizeof msg, &len, msg );
    printf("*program error, length = %d: %s\n", len, msg );
    return false;
  }
  glUseProgram( program );
  
  return true;
}

void opengl_release() {
  printf("Release\n");
  opengl_delshader();
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
  glBindVertexArray( 0 );
  glDeleteVertexArrays( 1, &vao1 );
  glDeleteBuffers( 1, &vbuffer );
  glDeleteBuffers( 1, &vindex1 );
  glBindTexture( GL_TEXTURE_2D, 0 );
  glDeleteTextures( 1, &texture );
}

void process_opengl( HWND hwnd ) {
  printf("Draw!\n");

  auto hdc = raii( GetDC( hwnd ), [hwnd]( HDC hdc ) { ReleaseDC( hwnd, hdc ); } );
  glClearColor( .0f, .0f, .0f, .0f );  
  glClearDepth( 1.0f );
  glClearStencil( .0f );
  //glDisable( GL_BLEND );
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
  glColor3f( .5f, .5f, .5f );
  //glDrawArrays( GL_TRIANGLES, 0, 3 );
  glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0 );

  opengl_print_errors();

  SwapBuffers( hdc );
}



