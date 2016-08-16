#include <d3d9.h>
#include <dxerr9.h>
#include <cmath>
#include "d3dformat_desc.h"

//#pragma comment(lib,"d3d9.lib")
//#pragma comment(lib,"dxerr.lib")

comptr_t<IDirect3DDevice9> dev{};
comptr_t<IDirect3DSurface9> rt1{};
comptr_t<IDirect3DTexture9> tex0{}, tex1{}, tex2{};


void init_cubes();
void draw_cubes( LONG mouse_x, LONG mouse_y );
void animate_cubes();

#define $dxwarn( expr ) if( FAILED( expr ) ) { $fl; print_dx_error( expr ); }
#define $dxerror( expr ) if( FAILED( expr ) ) { $fl; print_dx_error( expr ); return null; }
#define $dxfatal( expr ) if( FAILED(expr) ) { $fl; print_dx_error( expr ); exit( 1 ); }
#define $dxwarn2( expr, msg ) if( FAILED( expr ) ) { $fl; print_dx_error( expr, msg ); }
#define $dxerror2( expr, msg ) if( FAILED( expr ) ) { $fl; print_dx_error( expr, msg ); return null; }
#define $dxfatal2( expr, msg ) if( FAILED( expr ) ) { $fl; print_dx_error( expr, msg ); exit( 1 ); }

#define $dxwarnall( VAR, ... ) $check( $dxwarn2, VAR, __VA_ARGS__ )
#define $dxerrorall( VAR, ... ) $check( $dxerror2, VAR, __VA_ARGS__ )
#define $dxfatalall( VAR, ... ) $check( $dxfatal2, VAR, __VA_ARGS__ )

void print_dx_error( hr_t hr, cstr_ msg = nullptr ) { 
  pf( "%s, %#8X\n", DXGetErrorDescription9(hr), hr ); 
  if( msg ) p( msg );
}

struct vertex_t {
  using v_t = vertex_t;
  const static dword_t fvf = D3DFVF_XYZW;
  float x; float y; float z; float w;

  vertex_t( float x, float y, float z, float w ) : x{x}, y{y}, z{z}, w{w} {}
  vertex_t() : x{},y{},z{} {}

  operator float*() { return (float*) this; }

  //vertex_t( std::initializer_list<float> list ) {
  //  size_t i{}; float* data = (float*) this;
  //  for( float f : list ) data[i++] = f;
  //}

  float dot( const v_t o ) const { return $.x * o.x + $.y * o.y + $.z * o.z + $.w * o.w; }
  v_t add( const v_t o ) const { return v_t{ $.x + o.x, $.y + o.y, $.z + o.z, $.w + o.w }; }
  v_t mul( const float n ) const { return v_t{ n * $.x , n * $.y, n * $.z, n * $.w }; }
};
using v_t = vertex_t;
v_t operator+( const v_t l, const v_t r ) { return l.add( r ); }
v_t operator*( const v_t l, const float r ) { return l.mul( r ); }
float dot( const v_t l, const v_t r ) { return l.dot( r ); }

float s = 1.0f/5.0f;
float s2 = 1.0 / 25.0;

v_t proj_matrix[] {
  { s, 0, 0, 0 }, 
  { 0, s, 0, 0 }, 
  { 0, 0, s/4.0f, 0.5f },
  { 0, 0, 0, 1.0 }, 
};

v_t rot_matrix_x[] {
  { 1, 0, 0, 0 }, 
  { 0, 1, 0, 0 }, 
  { 0, 0, 1, 0 }, 
  { 0, 0, 0, 1 }, 
};

v_t rot_matrix_y[] {
  { 1, 0, 0, 0 }, 
  { 0, 1, 0, 0 }, 
  { 0, 0, 1, 0 }, 
  { 0, 0, 0, 1 }, 
};

v_t rot_matrix_z[] {
  { 1, 0, 0, 0 }, 
  { 0, 1, 0, 0 }, 
  { 0, 0, 1, 0 }, 
  { 0, 0, 0, 1 }, 
};

void rot_x( double t ) {
  double a = 3.141592;
  double s = sin( a * t ), c = cos( a * t );
  rot_matrix_x[1][1] = c;
  rot_matrix_x[1][2] = s;
  rot_matrix_x[2][1] = -s;
  rot_matrix_x[2][2] = c;
}

void rot_y( double t ) {
  double a = 3.141592;
  double s = sin( a * t ), c = cos( a * t );
  rot_matrix_y[0][0] = c;
  rot_matrix_y[0][2] = -s;
  rot_matrix_y[2][0] = s;
  rot_matrix_y[2][2] = c;
}

void rot_z( double t ) {
  double a = 3.141592;
  double s = sin( a * t ), c = cos( a * t );
  rot_matrix_z[0][0] = c;
  rot_matrix_z[0][1] = -s;
  rot_matrix_z[1][0] = s;
  rot_matrix_z[1][1] = c;
}


D3DDISPLAYMODE dm{};
RECT wr;
float ar;

ret_t init_directx( hi_t hi, hwnd_t hwnd ) {
  $fn;
  hr_t hr;

  auto d3d = comptr( Direct3DCreate9( D3D_SDK_VERSION ) );
  $error( d3d );

  D3DPRESENT_PARAMETERS pp{};
  
  d3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &dm );
  $report("display mode w = %d, h = %d, rr = %d, format = %s", dm.Width, dm.Height, dm.RefreshRate, d3dformat_to_string( dm.Format ) );

  //pp.BackBufferWidth = dm.Width;
  //pp.BackBufferHeight = dm.Height;
  //pp.BackBufferFormat = dm.Format;
  //pp.BackBufferCount = 1;
  pp.hDeviceWindow = hwnd;
  //pp.Windowed = FALSE;
  pp.Windowed = TRUE;
  pp.EnableAutoDepthStencil = TRUE;
  pp.AutoDepthStencilFormat = D3DFMT_D16;
  //pp.SwapEffect = D3DSWAPEFFECT_FLIP;
  pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  //pp.SwapEffect = D3DSWAPEFFECT_COPY;
  pp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
  //pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

  dword_t flags = D3DCREATE_FPU_PRESERVE | D3DCREATE_HARDWARE_VERTEXPROCESSING;

  GetClientRect( hwnd, &wr );
  //wr.right = dm.Width; wr.bottom = dm.Height;

  ar = float(wr.bottom) / wr.right;
  proj_matrix[0][0] = proj_matrix[0][0] * ar;
  $report("ar = %f",ar);

  $dxfatalall( hr,
    d3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, flags, &pp, &dev )
    ,dev->SetRenderState( D3DRS_LIGHTING, FALSE )
    ,dev->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE )
    ,dev->CreateRenderTarget( dm.Width, dm.Height,  D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &rt1, nullptr )
  );

  $dxfatalall( hr
    ,dev->CreateTexture( wr.right, wr.bottom, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &tex0, nullptr )
    ,dev->CreateTexture( wr.right, wr.bottom, 1, D3DUSAGE_RENDERTARGET, D3DFMT_X8R8G8B8, D3DPOOL_DEFAULT, &tex1, nullptr )
    //,dev->CreateTexture( wr.right, wr.bottom, 1, D3DUSAGE_RENDERTARGET, D3DFMT_G16R16F, D3DPOOL_DEFAULT, &tex1, nullptr )
    //,dev->CreateTexture( wr.right, wr.bottom, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A16B16G16R16F, D3DPOOL_DEFAULT, &tex2, nullptr )
  );

  $report( "Custom Render Target = %#.8X wr.right = %d. wr.bottom = %d", *&rt1, wr.right, wr.bottom ); 
  $report( "device %#X, BackbufferCount %d, BackBufferWidth %d", (void*) dev, pp.BackBufferCount, pp.BackBufferWidth );


  IDirect3DSurface9 *sf1{};
  $dxfatalall( hr
    //,dev->CreateRenderTarget( wr.right, wr.bottom, D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, false, &rt2, nullptr )
    ,dev->CreateOffscreenPlainSurface( dm.Width, dm.Height, D3DFMT_A8R8G8B8, D3DPOOL_SYSTEMMEM, &sf1, nullptr )
    ,dev->GetFrontBufferData( 0, sf1 )
    ,dev->UpdateSurface( sf1, nullptr, rt1, nullptr )
  );
  sf1->Release();


  //IDirect3DSurface9 *bb1{};
  //$dxwarnall( hr, dev->GetBackBuffer( 0, 2, D3DBACKBUFFER_TYPE_MONO, &bb1 ) );
  //$report( "BackBuffer2 = %#.8X\n", bb1 );
  //dev->SetRenderTarget( 0, bb1 );
  //$dxwarnall( hr, dev->SetRenderTarget( 0, bb1 ) );
  //$report( "RenderTarget0 = %#.8X\n", bb1 );
  //bb1->Release();

  init_cubes();

  $ret true;
}

#include "vs1.h"
#include "ps1.h"
#include "vs2.h"
#include "ps2.h"

struct mesh_t {
  size_t size;
  vertex_t* data;
  vertex_t* normal;
  comptr_t<IDirect3DVertexBuffer9> vb;
  comptr_t<IDirect3DVertexBuffer9> vn;
  comptr_t<IDirect3DVertexDeclaration9> vdecl;
  bool init{false};

  mesh_t( size_t size, v_t* data, v_t* normal  ) : size{size}, data{data}, normal{normal} {}

  ret_t initialize() {
    hr_t hr;
    v_t* data;

    $dxfatalall( hr,
      dev->CreateVertexBuffer( size * sizeof(data[0]), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vb, nullptr ),
      $.vb->Lock( 0, size * sizeof(data[0]), (void**)&data, 0 )
    );
    $report("vertex buffer %p", *&vb);
    for(size_t i = 0; i < $.size; i++) data[i] = $.data[i];
    $.vb->Unlock();

    $dxfatalall( hr,
      dev->CreateVertexBuffer( size * sizeof(normal[0]), D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &vn, nullptr ),
      $.vn->Lock( 0, size * sizeof(normal[0]), (void**)&data, 0 )
    );
    $report("normal buffer %p", *&vn);
    for(size_t i = 0; i < $.size; i++) data[i] = $.normal[i];
    $.vn->Unlock();

    D3DVERTEXELEMENT9 decl[] {
      {0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
      {1, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
      D3DDECL_END()
    };

    $dxfatalall( hr
      ,dev->CreateVertexDeclaration( decl, &vdecl )
    );

    $.init = true;
    $ret true;
  }

  void set_constants() { 
    hr_t hr;

    $dxwarnall( hr
      ,dev->SetVertexDeclaration( vdecl )
      ,dev->SetStreamSource( 0, vb, 0, sizeof(data[0]) )
      ,dev->SetStreamSource( 1, vn, 0, sizeof(data[0]) )
    );
/*
    hr_t hr;
    v_t* data;
    $dxwarnall( hr,
      $.vb->Lock( 0, 0, (void**)&data, 0 )
    );

    for(size_t i = 0; i < $.size; i++) data[i] = $.data[i];

    $.vb->Unlock();
*/
  }

  ret_t draw() {
    //if( ! $.init ) initialize();
    set_constants();
    hr_t hr;
    $dxerrorall( hr,
      dev->DrawPrimitive( D3DPT_TRIANGLELIST, 0, $.size/3 )
    );
    $ret true;
  }

};

struct material_t {
  v_t color;
  comptr_t<IDirect3DPixelShader9> ps;
  bool init{false};

  material_t( v_t color ) : color{color} {}

  void initialize() {
    hr_t hr = dev->CreatePixelShader( (dword_t*)g_ps30_main, &ps );
    $dxfatal( hr );
    $.init = true;
  }
  void set_constants() {
    hr_t hr;
    hr = dev->SetPixelShader( ps );
    $dxfatal( hr );
    dev->SetPixelShaderConstantB( 0, (BOOL const*)&DFAA, 1 );
    hr = dev->SetPixelShaderConstantF( 0, color, 1 );
    $dxwarn( hr );
  }
  ret_t draw() {
    //if( ! $.init ) initialize();
    set_constants();
    $ret true;
  }
};


struct cube_t {
  vertex_t pos;
  vertex_t origpos;
  mesh_t mesh;
  material_t material;
  comptr_t<IDirect3DVertexShader9> vs;
  bool init{false};

  tp<tn A, tn B, tn C>
  cube_t( A&& pos, B&& mesh, C&& material ) : 
    pos( std::forward<A>(pos) ), 
    origpos( pos ), 
    mesh( std::forward<B>(mesh) ),
    material( std::forward<C>(material) )
   { }

  void set_pos( v_t pos ) { $.pos = pos; };
  void set_size( float size ) { $.pos.z = size; };
  void initialize() {
    $.mesh.initialize();
    $.material.initialize();
    hr_t hr = dev->CreateVertexShader( (dword_t*) g_vs30_main, &vs );
    $dxfatal( hr );
    $.init = true;
  }

  ret_t set_constants( LONG mouse_x, LONG mouse_y ) { 
    hr_t hr;

    float mouse[]{ float( mouse_x - wr.right / 2 ) * 2.0f / dm.Width, -float( mouse_y - wr.bottom / 2 )  * 2.0f / dm.Height, 0, 0 };
    //$report("mouse %.3f %.3f", mouse[0], mouse[1]);

    hr = dev->SetVertexShader( vs );
    $dxfatal( hr );

    $dxerrorall( hr, 
      dev->SetVertexShaderConstantF( 4, pos, 1 ); $dxwarn( hr )
      ,dev->SetVertexShaderConstantF( 0, (float*) proj_matrix, 4 )
      ,dev->SetVertexShaderConstantF( 5, (float*) rot_matrix_x, 4 )
      ,dev->SetVertexShaderConstantF( 9, (float*) rot_matrix_y, 4 )
      ,dev->SetVertexShaderConstantF( 13, (float*) rot_matrix_z, 4 )
    );
    $dxerrorall( hr, 
       dev->SetPixelShaderConstantF( 1, mouse, 1 )
    );
    $ret true;
  }

  ret_t draw( LONG mouse_x, LONG mouse_y ) { 
    if( ! $.init ) initialize();
    set_constants( mouse_x, mouse_y );
    //hr_t hr = dev->SetVertexShader( vs );
    //$dxerror( hr );
    $.material.draw();
    $.mesh.draw(); 
    $ret true;
  }
};

void display_backbuffers_begin();
void display_backbuffers_end();
comptr_t<IDirect3DVertexShader9> vs2;
comptr_t<IDirect3DPixelShader9> ps2;

float mesh_quad[] = { 
  -1,-1,0,1, 0,1,  -1,1,0,1, 0,0,   1,-1,0,1, 1,1,
   1,-1,0,1, 1,1,  -1,1,0,1, 0,0,   1, 1,0,1, 1,0,  
};

ret_t draw_loop( LONG mouse_x, LONG mouse_y ) {
  static unsigned frame{};
  frame++;
  hr_t hr;

  animate_cubes();

  IDirect3DSurface9 *rt1{}, *tex0sf{}, *rt2{}, *tex1sf{};
  
  $dxwarnall( hr
    ,dev->GetRenderTarget( 0, &rt1 ) 
  );
  //dev->GetRenderTarget( 1, &rt2 );

  $dxwarnall( hr
    ,tex0->GetSurfaceLevel( 0, &tex0sf ) 
    ,tex1->GetSurfaceLevel( 0, &tex1sf ) 
  );

  dev->BeginScene();

  $dxwarnall( hr
    ,dev->SetRenderTarget( 0, tex0sf )
    ,dev->SetRenderTarget( 1, tex1sf )
  );

  dev->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255,0,0,0 ), 1.0f, 0 );
  //dev->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255, 200,200,200 ), 1.0f, 0 );


  //display_backbuffers_begin();
  draw_cubes( mouse_x, mouse_y );


  //dev->EndScene();

  dev->SetRenderTarget( 0, rt1 );
  dev->SetRenderTarget( 1, rt2 );

  if(rt1) rt1->Release();
  if(rt2) rt2->Release();

  tex0sf->Release();
  tex1sf->Release();

  if( frame == 1 ) {

    hr_t hr = dev->CreateVertexShader( (dword_t*) vs2_binary, &vs2 );
    $dxfatal( hr );

    hr = dev->CreatePixelShader( (dword_t*) ps2_binary, &ps2 );
    $dxfatal( hr );

    float offx = 1.0f / wr.right;
    float offy = 1.0f / wr.bottom;
    for(int i=0; i<6; i++) {
      mesh_quad[ i * 6 ] -= offx;
      mesh_quad[ i * 6 + 1] += offy;
    }
  }


  //dev->BeginScene();

  $dxerrorall( hr
    ,dev->SetVertexShader( vs2 )
    ,dev->SetPixelShader( ps2 )
    ,dev->SetPixelShaderConstantB( 0, (BOOL const*)&DFAA, 1 )
    ,dev->SetFVF( D3DFVF_XYZW | D3DFVF_TEX1 )
  );

  $dxerrorall( hr
    ,dev->SetTexture( 0, tex0 )
    ,dev->SetTexture( 1, tex1 )
  );
  
/*
  $dxerrorall( hr
    ,dev->SetSamplerState( 0, D3DSAMP_BORDERCOLOR, D3DCOLOR_ARGB( 255, 0, 0, 0 ) )
    ,dev->SetSamplerState( 0, D3DSAMP_MINFILTER,   D3DTEXF_POINT )
    ,dev->SetSamplerState( 0, D3DSAMP_MAGFILTER,   D3DTEXF_POINT )
    ,dev->SetSamplerState( 0, D3DSAMP_MIPFILTER,   D3DTEXF_POINT )
  );
*/
  dev->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255, 0, 0, 0 ), 1.0f, 0 );
  hr = dev->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, mesh_quad, 6 * sizeof( float ) );
  $dxerror( hr );
  
  $dxerrorall( hr
    ,dev->SetTexture( 0, NULL )
    ,dev->SetTexture( 1, NULL )
  );
 
  if(pass2) {
    IDirect3DSurface9 *rt{};
    $dxwarnall( hr
      ,dev->GetRenderTarget( 0, &rt ) 
      ,tex0->GetSurfaceLevel( 0, &tex0sf ) ;
      ,dev->StretchRect( rt, nullptr, tex0sf, nullptr, D3DTEXF_NONE ) 
    );
    tex0sf->Release();
    rt->Release();

    dev->Clear( 0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB( 255, 0, 0, 0 ), 1.0f, 0 );
    hr = dev->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 2, mesh_quad, 6 * sizeof( float ) );
    $dxerror( hr );
  }

  if(zoom) display_backbuffers_end();

  dev->EndScene();


  hr = dev->Present( nullptr, nullptr, nullptr, nullptr );
  $dxerror( hr );

  $ret true;
}


v_t pyramid[]{
  {-1,-1,-1, 0}, {0, 1, 0, 1 }, { 1,-1,-1, 2 },
  { 0,-1, 1, 3}, {0, 1, 0, 4 }, {-1,-1,-1, 5 },
  { 1,-1,-1, 6}, {0, 1, 0, 7 }, { 0,-1, 1, 8 },
  {-1,-1,-1, 9}, {0,-1, 1, 10}, { 1,-1,-1, 11},
};
v_t pyramidnormal[] {
  {0, 1.0/1.4, -1.0/1.4, 0}, {0, 1.0/1.4, -1.0/1.4, 0}, {0, 1.0/1.4, -1.0/1.4, 0}, 
  {-1.0/1.7, 1.0/1.7, 1.0/1.7, 0}, {-1.0/1.7, 1.0/1.7, 1.0/1.7, 0}, {-1.0/1.7, 1.0/1.7, 1.0/1.7, 0}, 
  {1.0/1.7, 1.0/1.7, 1.0/1.7, 0}, {1.0/1.7, 1.0/1.7, 1.0/1.7, 0}, {1.0/1.7, 1.0/1.7, 1.0/1.7, 0}, 
  {0,-1,0,0}, {0,-1,0,0}, {0,-1,0,0}, 
};
v_t cubevertex[36]{
  {-1,-1,-1,0 }, {-1,+1,-1,1 }, {+1,-1,-1,2 }, {+1,-1,-1,3 }, {-1,+1,-1,4 }, {+1,+1,-1,5 }, 
  {-1,-1,+1,6 }, {+1,-1,+1,7 }, {-1,+1,+1,8 }, {-1,+1,+1,9 }, {+1,-1,+1,10}, {+1,+1,+1,11},
  {+1,-1,-1,12}, {+1,+1,-1,13}, {+1,-1,+1,14}, {+1,-1,+1,15}, {+1,+1,-1,16}, {+1,+1,+1,17}, 
  {-1,-1,-1,18}, {-1,-1,+1,19}, {-1,+1,-1,20}, {-1,+1,-1,21}, {-1,-1,+1,22}, {-1,+1,+1,23}, 
  {-1,+1,-1,24}, {-1,+1,+1,25}, {+1,+1,-1,26}, {+1,+1,-1,27}, {-1,+1,+1,28}, {+1,+1,+1,29}, 
  {-1,-1,-1,30}, {+1,-1,-1,31}, {-1,-1,+1,32}, {-1,-1,+1,33}, {+1,-1,-1,34}, {+1,-1,+1,35}, 
  
};
v_t cubenormal[36]{
  { 0, 0,-1,0}, { 0, 0,-1,0}, { 0, 0,-1,0}, { 0, 0,-1,0}, { 0, 0,-1,0}, { 0, 0,-1,0},  
  { 0, 0,+1,0}, { 0, 0,+1,0}, { 0, 0,+1,0}, { 0, 0,+1,0}, { 0, 0,+1,0}, { 0, 0,+1,0}, 
  {+1, 0, 0,0}, {+1, 0, 0,0}, {+1, 0, 0,0}, {+1, 0, 0,0}, {+1, 0, 0,0}, {+1, 0, 0,0}, 
  {-1, 0, 0,0}, {-1, 0, 0,0}, {-1, 0, 0,0}, {-1, 0, 0,0}, {-1, 0, 0,0}, {-1, 0, 0,0}, 
  { 0,+1, 0,0}, { 0,+1, 0,0}, { 0,+1, 0,0}, { 0,+1, 0,0}, { 0,+1, 0,0}, { 0,+1, 0,0}, 
  { 0,-1, 0,0}, { 0,-1, 0,0}, { 0,-1, 0,0}, { 0,-1, 0,0}, { 0,-1, 0,0}, { 0,-1, 0,0}, 
};

cube_t cubes[] {
  { v_t{  2.0f,  2.0f,  2.0f, 0.0f }, mesh_t{ size(cubevertex), cubevertex, cubenormal }, material_t{ {1.0f, .9f, .6f, 1.0f} } },
  { v_t{ -3.0f,  2.0f, -2.0f, 0.0f }, mesh_t{ size(cubevertex), cubevertex, cubenormal }, material_t{ {.7f, 1.0f, .7f, 1.0f} } },
  { v_t{ -1.0f, -2.0f,  3.0f, 0.0f }, mesh_t{ size(cubevertex), cubevertex, cubenormal }, material_t{ {.5f, .8f, 1.0f, 1.0f} } },
  { v_t{  3.0f, -1.0f, -1.0f, 0.0f }, mesh_t{ size(pyramid), pyramid, pyramidnormal }, material_t{ {1.0f, .7f, 1.0f, 1.0f} } },
};

void init_cubes() {}

void animate_cubes() {
  static LARGE_INTEGER time{};
  static LARGE_INTEGER freq{};
  static double oldtime{1.0}, t_fps{0};
  static int paused{};
  static unsigned frame = 0;
  frame++;

  if( ! time.LowPart ) {
    QueryPerformanceCounter( &time );
    QueryPerformanceFrequency( &freq );
  }

  LARGE_INTEGER newtime;
  QueryPerformanceCounter( &newtime );

  if( paused != pause && ! pause ) { 
    paused = pause;
    time = newtime;
    frame=1;
  }

  double delta = double( newtime.QuadPart - time.QuadPart ) / freq.QuadPart;
  double t = oldtime + delta;
  double t_slow = oldtime + 0.5 * 0.125 * delta;

  if( fps && frame % 1000 == 0 ) {
    $report("%.2ffps",frame/(t-t_fps));
    frame = 0;
    t_fps = t;
  }

  if( paused != pause && pause ) { 
    paused = pause;
    oldtime = t_slow; 
  }

  if( pause ) return;

  //$report( "time %.3f, sinf %.3f", t, sinf( t ) );
  int c{};
  for( cube_t& cube : cubes ) {
    v_t dir = cube.origpos * ( 1.0f / sqrtf( cube.origpos.dot( cube.origpos ) ) );
    dir = dir * ( 0.5 + 1.5 * sin( 2.0 * t_slow + 3.141593 / 3.0 * c ) );
    cube.pos = cube.origpos + dir * ( 1.0f + 0.5f * sin( t_slow ) );
    c++;
  }

  rot_z( 0.5*t_slow );
  rot_x( t_slow + 1.0 );
  rot_y( 0.75*t_slow + 1.0 );
}

void draw_cubes( LONG mouse_x, LONG mouse_y ) {
  for( cube_t& cube : cubes ) {
    cube.draw( mouse_x, mouse_y );
  }
}

void display_backbuffers_begin() {
  //$report( "display_backbuffers_begin" );
  IDirect3DSurface9 *rt{};
  HRESULT hr{};
  $dxwarnall( hr, dev->GetRenderTarget( 0, &rt ) );
  $dxwarnall( hr, dev->StretchRect( rt1, nullptr, rt  , nullptr, D3DTEXF_NONE ) );
  //$dxwarnall( hr, dev->StretchRect( rt, nullptr, rt1, nullptr, D3DTEXF_NONE ) );
  rt->Release();
}

void display_backbuffers_end() {

  IDirect3DSurface9 *rt{};
  HRESULT hr{};
  $dxwarnall( hr, dev->GetRenderTarget( 0, &rt ) );
  int const w = 200, h = w * ar;
  RECT rc{ wr.right/2-w, wr.bottom/2-w, wr.right/2+h, wr.bottom/2+h };
  $dxwarnall( hr, dev->StretchRect( rt, &rc, rt1, nullptr, D3DTEXF_NONE ) );
  $dxwarnall( hr, dev->StretchRect( rt1, nullptr, rt, nullptr, D3DTEXF_NONE ) );
  rt->Release();
  
  return;
  //$report( "display_backbuffers_end" );

  /*{
  hr_t hr;
  IDirect3DSurface9 *bb1{}, *sf1{};
  dev->GetRenderTarget( 0, &bb1 );
  $dxfatalall( hr
    ,dev->CreateOffscreenPlainSurface( wr.right, wr.bottom, dm.Format, D3DPOOL_SYSTEMMEM, &sf1, nullptr )
    ,dev->GetRenderTargetData( bb1, sf1 )
    ,dev->UpdateSurface( sf1, nullptr, rt1, nullptr )
  );
  bb1->Release();
  sf1->Release();
  }*/ 

/*
  IDirect3DSurface9 *rt{};
  HRESULT hr{};
  $dxwarnall( hr, dev->GetRenderTarget( 0, &rt ) );
  //$report( "RenderTarget0 = %#.8X", rt );

  for(int bb=1; bb<2; bb++) {
    IDirect3DSurface9 *bb1{};
    $dxwarnall( hr, dev->GetBackBuffer( 0, bb, D3DBACKBUFFER_TYPE_MONO, &bb1 ) );
    //$report( "BackBuffer%d = %#.8X", bb, bb1 );
    RECT rc{ bb * 160, 10, rc.left+160, rc.top+120 };
    $dxwarnall( hr, dev->StretchRect( bb1, nullptr, rt, &rc, D3DTEXF_LINEAR ) );
    bb1->Release();
  }

  RECT rc{ 0, 10, rc.left+160, rc.top+120 };
  $dxwarnall( hr, dev->StretchRect( rt1, nullptr, rt, &rc, D3DTEXF_LINEAR ) );

  rt->Release();
*/
}







