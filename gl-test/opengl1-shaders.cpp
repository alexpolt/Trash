
const char* vertexshader = 
  "#version 140\r\n"
  "in vec3 position_in;\r\n"
  "out vec3 pos;\r\n"
  "\r\n"
  "void main(void) {\r\n"
  "  gl_Position = vec4( position_in, 1.0 );\r\n"
  "  pos = position_in;\r\n"
  "}\r\n";

const char* fragmentshader = 
  "#version 140\r\n"
  "in vec3 pos;\r\n"
  "float translate = 0.5;\r\n"
  "uniform sampler2D s0;\r\n"
  "void main(void) {\r\n"
  "  gl_FragColor = texture( s0, (pos * translate + translate).xy );\r\n"
  "  //gl_FragColor = vec4( pos * translate + translate, 1 );\r\n"
  "}\r\n";
