//amazing cubes
//vertex shader
#define reg(arg0) register(arg0)

struct IN {
  float4 pos: POSITION;
  float4 normal: NORMAL;
};

struct OUT {
  float4 pos: TEXCOORD0;
  float4 normal: TEXCOORD1;
  float2 bar: TEXCOORD2;
};

row_major float4x4 proj : reg(c0);
float4 cube_pos : reg(c4);
row_major float4x4 rotx : reg(c5);
row_major float4x4 roty : reg(c9);
row_major float4x4 rotz : reg(c13);

static float2 bar[] = { {0,0}, {1,0}, {0,1} };

OUT main( IN inp, out float4 pos : POSITION ) {
  OUT outp;

  float vertex_id = inp.pos.w;
  outp.bar = bar[ vertex_id - 3 * floor(vertex_id/3) ];
  inp.pos.w = 1;

  //inp.pos.xyz = inp.normal.xyz / 5 + inp.pos.xyz;

  inp.pos = mul( rotz, mul( roty,  mul( rotx, inp.pos + cube_pos )  )  );
  inp.pos = mul( proj,  inp.pos );
  pos = float4( inp.pos.xy * ( 2.0 - inp.pos.z ), inp.pos.z, 1 );

  outp.normal = mul( rotz, mul( roty,  mul( rotx, inp.normal )  )  );
  outp.pos = cube_pos;

  return outp;
}


