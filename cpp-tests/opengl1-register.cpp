bool func_reg_error{ 0 };

#define stringify(str) #str
#define pfnstr(str) PFN##str##PROC
#define glregister( func1, func2 ) func2 = (func1) wglGetProcAddress( stringify(func2) ); if(!func2) { printf("error registering %s\n", stringify(func2) ); func_reg_error = 1; } else (void) 0;
#define gldeclare( func1, func2 ) func1 func2;

#define glfunclist( xmacro ) \
  xmacro( pfnstr( GLGENBUFFERS ), glGenBuffers )\
  xmacro( pfnstr( GLDELETEBUFFERS ), glDeleteBuffers )\
  xmacro( pfnstr( GLBINDBUFFER ), glBindBuffer )\
  xmacro( pfnstr( GLBUFFERDATA ), glBufferData )\
  \
  xmacro( pfnstr( GLVERTEXATTRIBPOINTER ), glVertexAttribPointer )\
  xmacro( pfnstr( GLENABLEVERTEXATTRIBARRAY ), glEnableVertexAttribArray )\
  xmacro( pfnstr( GLDISABLEVERTEXATTRIBARRAY ), glDisableVertexAttribArray )\
  \
  xmacro( pfnstr( GLGENVERTEXARRAYS ), glGenVertexArrays )\
  xmacro( pfnstr( GLBINDVERTEXARRAY ), glBindVertexArray )\
  xmacro( pfnstr( GLDELETEVERTEXARRAYS ), glDeleteVertexArrays )\
  \
  xmacro( pfnstr( GLCREATEPROGRAM ), glCreateProgram )\
  xmacro( pfnstr( GLDELETEPROGRAM ), glDeleteProgram )\
  xmacro( pfnstr( GLUSEPROGRAM ), glUseProgram )\
  xmacro( pfnstr( GLLINKPROGRAM ), glLinkProgram )\
  xmacro( pfnstr( GLCREATESHADER ), glCreateShader )\
  xmacro( pfnstr( GLCOMPILESHADER ), glCompileShader )\
  xmacro( pfnstr( GLGETPROGRAMIV ), glGetProgramiv )\
  xmacro( pfnstr( GLATTACHSHADER ), glAttachShader )\
  xmacro( pfnstr( GLSHADERSOURCE ), glShaderSource )\
  xmacro( pfnstr( GLGETSHADERIV ), glGetShaderiv )\
  xmacro( pfnstr( GLGETSHADERINFOLOG ), glGetShaderInfoLog )\
  xmacro( pfnstr( GLGETPROGRAMINFOLOG ), glGetProgramInfoLog )\
  xmacro( pfnstr( GLDELETESHADER ), glDeleteShader )\
  xmacro( pfnstr( GLACTIVETEXTURE ), glActiveTexture )\
  \

/* 
  xmacro( pfnstr( GL ), gl )\
  xmacro( pfnstr( GL ), gl )\
  xmacro( pfnstr( GL ), gl )\
*/

glfunclist( gldeclare )

bool opengl_register() {
  glfunclist( glregister )
  if( func_reg_error ) { printf("Error func reg\n"); return false; }
  return true;
}

