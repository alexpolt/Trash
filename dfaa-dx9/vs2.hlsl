//amazing cubes
//vertex shader
#define reg(arg0) register(arg0)

struct IN {
  float4 pos: POSITION;
  float2 uv: TEXCOORD0;
};

struct OUT {
  float2 screen: TEXCOORD0;
};


OUT main( IN i, out float4 pos : POSITION ) {
  OUT o;
  pos = i.pos;
  o.screen = i.uv;
  return o;
}


