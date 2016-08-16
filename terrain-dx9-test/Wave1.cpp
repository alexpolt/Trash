// Wave1.cpp : Defines the entry point for the console application.
//
#define WIN32_LEAN_AND_MEAN
#define _UNICODE
#define UNICODE

#include "stdafx.h"

#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>

#include <math.h>

#define P printf
#define PW _tprintf

LPDIRECT3D9 pD3D;
LPDIRECT3DDEVICE9 pDevice;
D3DPRESENT_PARAMETERS stPresentParameters;
LPDIRECT3DSURFACE9 g_pTempSurface1;
RECT clientRect;
int clientWidth, clientHeight;
HCURSOR hCurArrow;

LPCTSTR szAppTitle = TEXT("DxTest1");
LPCTSTR szOk = TEXT("Ok.\n");
int g_iRun = TRUE, g_iRotate = 1, g_iRotX, g_iRotY, g_iPosZ, g_iCamY, g_iCamX, g_iCamZ, g_iCamRotY, g_iCamRotX,  g_iWireframe = 0, g_iAutorotate = 0;
int gravity_b = 1;
D3DXVECTOR3 g_vCamLook(0,0,-1), g_vCamPos(0,1000,0);

#define OK PW( szOk );
#define HR( line, hr ) if( FAILED(hr) ) ReportDXError( line, hr );
#define HR1( func ) hr = func; if( FAILED(hr) ) ReportDXError( __LINE__, hr );

void ReportDXError( int line,  HRESULT hr ) { 
	//PW(_TEXT("Line: %d, "), line ); PW( DXGetErrorString9( hr ) ); PW(_TEXT(" : ")); PW( DXGetErrorDescription9( hr ) ); PW(_TEXT("\n")); 
}	
void ReportLastError( ) { LPTSTR pszStr; FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS, 
														NULL, GetLastError(), 0, (LPTSTR) &pszStr, 0, NULL ); PW( pszStr ); LocalFree( pszStr ); }	

LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam  );
void RENDER();
void SetRenderState();
LPDIRECT3DSURFACE9 CreateTempSurface();

void printBinary( int binary ) {
	int t = 0;
	while( t < sizeof binary * 8 ) {
		P("%d", binary&(1<<t)?1:0 );
		t++;
	}
	P("\n");
}

int _tmain(int argc, char* argv[])
{
	/*
	int exp_ = (150<<23) + (1<<22);
	float exp = *(float*)&exp_;
	float a = -1.0f; int f = *(int*)&a;
	a = 1.0f;
	printBinary(*(int*)&a);
	a = a + exp;
	printBinary(*(int*)&a);
	f = *(int*)&a;
	printBinary(f-( (150<<23)+(1<<22) )  );
	P("\n");
	a = -1.0f;
	printBinary(*(int*)&a);
	a = a + exp;
	printBinary(*(int*)&a);
	f = *(int*)&a;
	printBinary(f-((150<<23)+(1<<22)));
	//P("0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0\n");
	exit(0);
	*/

	WNDCLASS stWndClass;
	HWND hWnd;

	hCurArrow = LoadCursor( NULL, IDC_ARROW );

	stWndClass.hInstance = GetModuleHandle(NULL);
	stWndClass.lpszClassName = szAppTitle;
	stWndClass.lpszMenuName = NULL;
	stWndClass.hIcon = NULL;
	stWndClass.hCursor = NULL;
	stWndClass.hbrBackground = GetSysColorBrush( COLOR_BTNFACE );
	stWndClass.cbClsExtra = 0;
	stWndClass.cbWndExtra = 0;
	stWndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	stWndClass.lpfnWndProc = WndProc;

	if(! RegisterClass( &stWndClass ) ) {
		ReportLastError();
		_exit(0);
	}

	OK

	hWnd = CreateWindow( szAppTitle, szAppTitle, WS_VISIBLE|WS_OVERLAPPEDWINDOW, 
							CW_USEDEFAULT, CW_USEDEFAULT, 1024, 768, NULL, NULL, GetModuleHandle(NULL), 0 );
	UpdateWindow( hWnd );
	GetClientRect( hWnd, &clientRect );
	clientWidth = clientRect.right - clientRect.left, clientHeight = clientRect.bottom - clientRect.top;
	P("ClientWidth = %d, ClientHeight = %d\n", clientWidth, clientHeight );


	if(! hWnd ) {
		ReportLastError();
		_exit(0);
	}

	PW( _TEXT("Start D3D9!\n") );

	pD3D = Direct3DCreate9 ( D3D_SDK_VERSION );
	if( ! pD3D ) { ReportLastError(); _exit(0); }

	stPresentParameters.BackBufferCount = 0;
	stPresentParameters.BackBufferWidth = 0;
	stPresentParameters.BackBufferHeight = 0;
	stPresentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
	stPresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	//stPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	stPresentParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	stPresentParameters.Flags = 0;
	stPresentParameters.FullScreen_RefreshRateInHz = 0;
	stPresentParameters.EnableAutoDepthStencil = TRUE;
	stPresentParameters.AutoDepthStencilFormat = D3DFMT_D24X8;
	//stPresentParameters.AutoDepthStencilFormat = D3DFMT_D16;

	stPresentParameters.MultiSampleType = D3DMULTISAMPLE_8_SAMPLES;
	stPresentParameters.MultiSampleQuality = 0;
	//stPresentParameters.MultiSampleType = D3DMULTISAMPLE_NONE;

	stPresentParameters.hDeviceWindow = hWnd;
	stPresentParameters.Windowed = TRUE;
		

	HRESULT hr = pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, 
										D3DCREATE_HARDWARE_VERTEXPROCESSING, &stPresentParameters, &pDevice );
	if( FAILED(hr) ) {
		ReportDXError( __LINE__,  hr );
		pD3D->Release();
		_exit(0);
	}

	g_pTempSurface1 = CreateTempSurface();

	SetRenderState();

	//ShowCursor( FALSE );

	MSG msg;
	ZeroMemory( &msg, sizeof MSG );

	for(;;) {

		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) {
				GetMessage( &msg, NULL, 0, 0);
				TranslateMessage( &msg );
				DispatchMessage( &msg );
		}

		if( msg.message == WM_QUIT ) break;

		if( g_iRun ) {
			RENDER();
			Sleep(0);
		}
	} 

	//ShowCursor( TRUE );

	g_pTempSurface1->Release();
	pDevice->Release();
	pD3D->Release();

	return 0;
}



LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ) {
	static POINTS stOldPos = {clientWidth>>1,clientHeight>>1};
	static int oldPosZ = 0;
	HDC hDC = NULL;
	D3DXVECTOR3 vecZ(0,1,0);

	switch( uMsg ) {
		case WM_KEYDOWN: 
			//P("KEY DOWN %d\n", wParam );
			if( wParam == VK_ESCAPE ) PostQuitMessage(0); 
			if( wParam == VK_LEFT ) g_iRotX-=10;
			if( wParam == VK_RIGHT ) g_iRotX+=10;
			if( wParam == VK_UP ) g_iPosZ+=100; 
			if( wParam == VK_DOWN ) g_iPosZ-=100;

			if( wParam == 65 ) { D3DXVec3Cross( &vecZ, &vecZ, &g_vCamLook ); D3DXVec3Normalize(&vecZ,&vecZ); g_vCamPos = g_vCamPos - 100 * vecZ; }
			if( wParam == 68 ) { D3DXVec3Cross( &vecZ, &vecZ, &g_vCamLook ); D3DXVec3Normalize(&vecZ,&vecZ); g_vCamPos = g_vCamPos + 100 * vecZ; }
			if( wParam == 87 ) { g_vCamPos = g_vCamPos + g_vCamLook * 200; }
			if( wParam == 83 ) { g_vCamPos = g_vCamPos - g_vCamLook * 200; }

			if( LOWORD(wParam) == 57 ) g_iPosZ+=10;
			if( LOWORD(wParam) == 53 ) g_iPosZ-=10;
			if( LOWORD(wParam) == 41 ) g_iRotY-=10;
			if( LOWORD(wParam) == 44 ) g_iRotY+=10;
			return 0;
		case WM_KEYUP: 
			if( wParam == VK_SPACE ) 
				g_iAutorotate = 1 - g_iAutorotate;
			if( wParam == VK_RETURN ) 
				gravity_b = 1 - gravity_b;
			
			if( wParam == VK_NEXT ) {
				if( !g_iWireframe )
					pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
				else
					pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
				g_iWireframe = 1 - g_iWireframe;
			}
			return 0;
		case WM_CREATE: return 0;
			break;
		case WM_LBUTTONUP:
			ReleaseCapture();
			return 0;
		case WM_LBUTTONDOWN:
			SetCapture( hWnd );
			POINT stPoint;
			GetCursorPos( &stPoint );
			ScreenToClient( hWnd, &stPoint );
			stOldPos.x = (short)stPoint.x;
			stOldPos.y = (short)stPoint.y;
			return 0;
		case WM_MOUSEMOVE:
			SetCursor( hCurArrow );
			if( ! GetCapture() ) return 0;
			POINTS stCurPos = MAKEPOINTS( lParam );
			g_iCamRotY += stOldPos.x - stCurPos.x;
			g_iCamRotX += stOldPos.y - stCurPos.y;

			stOldPos = stCurPos;
			break;
		case WM_DESTROY: g_iRun = 0; PostQuitMessage(0); 
		default: ;
	}

	D3DXMATRIX matCam, matRot;
	D3DXVECTOR4 look_gv4;
	if( g_iCamRotX ) {
		//D3DXMatrixRotationX( &matRot, -(float)g_iCamRotX / 512 );
		//D3DXVec3Transform( &look_gv4, &g_vCamLook, &matRot );
		//g_vCamLook.x = look_gv4.x;
		//g_vCamLook.y = look_gv4.y;
		//g_vCamLook.z = look_gv4.z;
		D3DXVec3Cross( &vecZ, &vecZ, &g_vCamLook ); 
		D3DXVec3Normalize( &vecZ, &vecZ ); 
		D3DXVec3Cross( &vecZ, &g_vCamLook, &vecZ ); 
		D3DXVec3Normalize( &vecZ, &vecZ ); 
		g_vCamLook = g_vCamLook + g_iCamRotX * 0.005 * vecZ ;
		D3DXVec3Normalize( &g_vCamLook, &g_vCamLook); 
		g_iCamRotX = 0;
	}
	if( g_iCamRotY ) {
		//D3DXMatrixRotationY( &matRot, -(float)g_iCamRotY / 512 );
		//D3DXVec3Transform( &look_gv4, &g_vCamLook, &matRot );
		//g_vCamLook.x = look_gv4.x;
		//g_vCamLook.y = look_gv4.y;
		//g_vCamLook.z = look_gv4.z;
		D3DXVec3Cross( &vecZ, &vecZ, &g_vCamLook ); 
		D3DXVec3Normalize( &vecZ, &vecZ ); 
		g_vCamLook = g_vCamLook - g_iCamRotY * 0.005 * vecZ;
		D3DXVec3Normalize( &g_vCamLook, &g_vCamLook); 
		g_iCamRotY = 0;
	}

	return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

LPDIRECT3DSURFACE9 CreateTempSurface() {
	LPDIRECT3DSURFACE9 pBackBuffer;
	D3DSURFACE_DESC stSurfaceDesc; 

	//HRESULT hr = pDevice->GetRenderTarget( 0, &pBackBuffer );
	HRESULT hr = pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	if( FAILED(hr) ) ReportDXError( __LINE__,  hr );
	pBackBuffer->GetDesc( &stSurfaceDesc );
	pBackBuffer->Release();

	hr = pDevice->CreateOffscreenPlainSurface( clientWidth, clientHeight, stSurfaceDesc.Format, D3DPOOL_SYSTEMMEM, &pBackBuffer, NULL );
	if( FAILED(hr) ) ReportDXError( __LINE__,  hr );
	return pBackBuffer;
}

void SetRenderState() {
	D3DXMATRIX matPerspective, matPerspective2, matIdentity, matView;

	D3DXMatrixPerspectiveFovLH( &matPerspective, 3.11/4, (double)clientWidth/clientHeight, 1.0, 1000000.0 );
	D3DXMatrixIdentity( &matIdentity );
	D3DXMatrixTranslation( &matView, 0, -1000, 6000 );
	
	matPerspective2 = matIdentity;
	matPerspective2._11 = 1;
	matPerspective2._33 = 0;
	matPerspective2._34 = 0;
	matPerspective2._43 = 1;
	matPerspective2._44 = 10000;
	matPerspective._34 = 1;
	//pDevice->SetTransform( D3DTS_PROJECTION, &matPerspective2 );
	pDevice->SetTransform( D3DTS_PROJECTION, &matPerspective );
	pDevice->SetTransform( D3DTS_WORLD, &matIdentity );
	pDevice->SetTransform( D3DTS_VIEW, &matView );

	pDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
	//pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	pDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
	pDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	pDevice->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_ARGB(0xff,0xff,0xff,0xff) );
	pDevice->SetRenderState( D3DRS_LIGHTING, 0 );

	pDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
	pDevice->SetRenderState( D3DRS_FOGCOLOR, D3DCOLOR_ARGB(0,128,128,140));
	static float fogStart = 3000.0f, fogEnd = 60000.0f, fogDensity = 1.0f;
	pDevice->SetRenderState( D3DRS_FOGSTART, *(DWORD*) &fogStart );
	pDevice->SetRenderState( D3DRS_FOGEND, *(DWORD*) &fogEnd );
	pDevice->SetRenderState( D3DRS_FOGDENSITY, *(DWORD*) &fogDensity );
	pDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_LINEAR );
	pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_NONE );

	pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	//pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_DESTALPHA );
	//pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO );
	//pDevice->SetRenderState( D3DRS_, D3D_ );
	//pDevice->SetRenderState( D3DRS_, D3D_ );

	//pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1 );
	pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );

	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );

	//pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE );

	//pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_CONSTANT );
	//pDevice->SetTextureStageState( 0, D3DTSS_CONSTANT, D3DCOLOR_ARGB(0xFF,0xFF,0,0 ) );
	//pDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR );
	pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
	pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
	//pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER );
	//pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER );
	//pDevice->SetSamplerState( 0, D3DSAMP_BORDERCOLOR, D3DCOLOR_ARGB(0xFF,0xFF,0xFF,0xFF ) );

	//D3DVIEWPORT9 stViewport = { 0, 0, clientWidth,clientHeight, 0, 65535.0 }; 
	D3DVIEWPORT9 stViewport = { 0, 0, clientWidth,clientHeight, 0, 1.0 }; 
	pDevice->SetViewport( &stViewport );

}

void RenderCube() {
	//static float[24];
	static LPD3DXMESH pMesh = 0;
	static LPDIRECT3DVERTEXBUFFER9 pVBSphere = 0;
	static LPDIRECT3DINDEXBUFFER9 pIBSphere = 0;
	static D3DXMATRIX matRotation;
	static LPDIRECT3DTEXTURE9 pTexture1;

	HRESULT hr;

	if( ! pMesh ) {
		HR1( D3DXCreateSphere( pDevice, 120, 20, 20, &pMesh, NULL ) )
		//HR1( D3DXCreateCylinder( pDevice, 40, 10, 100, 20, 20, &pMesh, NULL ) )
		//HR1( D3DXCreateTeapot( pDevice, &pMesh, NULL ) )
		HR1( pMesh->GetVertexBuffer( &pVBSphere ) )
		HR1( pMesh->GetIndexBuffer( &pIBSphere ) )
		D3DXMatrixIdentity( &matRotation );
		HR1( D3DXCreateTextureFromFile( pDevice, TEXT("tex1.png"), &pTexture1 ) )
	}

	D3DXMATRIX matRotY, matRotX, matPosZ;
	if( g_iRotate ) {
		if( g_iAutorotate ) g_iRotX++;
		D3DXMatrixRotationY( &matRotX, g_iRotX * 3.14 / 512 );
		D3DXMatrixRotationX( &matRotY, g_iRotY * 3.14 / 512 );
		D3DXMatrixTranslation( &matPosZ, 0, 0, g_iPosZ );
		matRotation = matRotX * matRotY * matPosZ;
	} 
	HR1( pDevice->SetTransform( D3DTS_WORLD, &matRotation ) )

	HR1( pDevice->SetTexture( 0, pTexture1 ) )
	HR1( pDevice->SetFVF( pMesh->GetFVF() ) )
	HR1( pDevice->SetStreamSource( 0, pVBSphere, 0, 24 ) )
	HR1( pDevice->SetIndices( pIBSphere ) )
	HR1( pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, pMesh->GetNumVertices(), 0, pMesh->GetNumFaces() ) )

}

void RenderTerrain() {
	struct TerrainMesh { float x,y,z; char color[4]; float u,v;  };
	static struct TerrainMesh *mesh = 0;
	static short * indexes = 0;
	static LPDIRECT3DVERTEXBUFFER9 pVB = 0;
	static LPDIRECT3DINDEXBUFFER9 pIB = 0;
	static D3DXMATRIX matRotation;
	static LPDIRECT3DTEXTURE9 pTexture1;
	HRESULT hr;

	if( ! mesh ) {
		HR1( D3DXCreateTextureFromFile( pDevice, TEXT("tex3.jpg"), &pTexture1 ) )

		srand( timeGetTime() );

		mesh = (struct TerrainMesh*) malloc( sizeof TerrainMesh * 200 * 200 );
		indexes = (short*)malloc( sizeof (short) * 199 * 199 * 6 );

		for(int y=0; y<200; y++ )
			for(int x=0; x<200; x++ ) {
				mesh[y*200+x].u = (float)x/20;
				mesh[y*200+x].v = 1.0-(float)y/20;

				//mesh[y*200+x].x = ((float)x+y-200)*50;
				//mesh[y*200+x].z = (float)((x-100)*(y-100))*2;
				mesh[y*200+x].x = ((float)x-100)*400;
				mesh[y*200+x].z = ((float)y-100)*400;
				//mesh[y*200+x].y = (y-100)*40; 
				//mesh[y*50+x].y = (float) (rand()%100);

				float angle = (float)x/200*2*6.28;
				float angle2 = (float)y/200*6.28;
				mesh[y*200+x].y = (float) 400 * ( sin( angle*angle2*0.5 ) + 2*sin( angle + angle2 * 3 + 3.14 )+4);
				//mesh[y*200+x].y = (float) (rand()%100) + 400 * ( sin( angle*angle2*0.5 ) + sin( angle + angle2 * 3 + 3.14 )+2);

				char color = (mesh[y*200+x].y+0.5) / 4000 * 255;
				//char color = 200;
				mesh[y*200+x].color[0] = color;
				mesh[y*200+x].color[1] = color;
				mesh[y*200+x].color[2] = color;
				mesh[y*200+x].color[3] = 0;

				if( y == 199 || x == 199 ) continue;
				indexes[y*199*6+x*6+0] = (y+0)*200+x+0;
				indexes[y*199*6+x*6+1] = (y+1)*200+x+0;
				indexes[y*199*6+x*6+2] = (y+1)*200+x+1;
				indexes[y*199*6+x*6+3] = (y+0)*200+x+0;
				indexes[y*199*6+x*6+4] = (y+1)*200+x+1;
				indexes[y*199*6+x*6+5] = (y+0)*200+x+1;
			}
		HR1( pDevice->CreateVertexBuffer( sizeof TerrainMesh * 40000, D3DUSAGE_WRITEONLY, 
											D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &pVB, NULL ) )
		HR1( pDevice->CreateIndexBuffer( sizeof (short) * 199 * 199 * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
											D3DPOOL_DEFAULT, &pIB, NULL ) )
		void *bytes;

		pVB->Lock( 0, 0, &bytes, D3DLOCK_DISCARD );
		memcpy( bytes, mesh, sizeof TerrainMesh*40000 );
		pVB->Unlock();

		pIB->Lock( 0, 0, &bytes, D3DLOCK_DISCARD );
		memcpy( bytes, indexes, sizeof (short) * 199*199*6 );
		pIB->Unlock();
	}

	D3DXMATRIX matRotY, matRotX, matPosZ;
	if( g_iRotate ) {
		static int autoRot = 0;
		if( g_iAutorotate ) { autoRot++; g_iRotX += (autoRot>>4);autoRot=autoRot&0xf; }
		D3DXMatrixRotationY( &matRotX, g_iRotX * 3.14 / 512 );
		D3DXMatrixRotationX( &matRotY, g_iRotY * 3.14 / 512 );
		D3DXMatrixTranslation( &matPosZ, 0, 0, g_iPosZ );
		matRotation = matRotX * matRotY * matPosZ;
	} 
	HR1( pDevice->SetTransform( D3DTS_WORLD, &matRotation ) )
	//HR1( pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO ) )
	//HR1( pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ ) )

	pDevice->SetFVF( D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_DIFFUSE );
	HR1( pDevice->SetTexture( 0, pTexture1 ) )
	HR1( pDevice->SetStreamSource( 0, pVB, 0, 24 ) )
	HR1( pDevice->SetIndices( pIB ) )
	HR1( pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 40000, 0, 199*199*2 ) )

	int camArrZ = int(g_vCamPos.z/400 + 100);
	int camArrX = int(g_vCamPos.x/400 + 100);
	//P("Z=%d,X=%d\n", camArrZ,camArrX);
	if( gravity_b && camArrZ >= 0 && camArrZ < 200 && camArrX >= 0 && camArrX < 200) {
		g_vCamPos.y = mesh[ camArrZ*200 + camArrX ].y + 1000;
	}
}


static float* velocities = (float*) malloc( sizeof (float) * 2000 );

void RenderParticle() {
	struct Particle { float x,y,z; float u,v;  };
	static struct Particle *mesh = 0;
	static short * indexes = 0;
	static LPDIRECT3DVERTEXBUFFER9 pVB = 0;
	static LPDIRECT3DINDEXBUFFER9 pIB = 0;
	static D3DXMATRIX matRotation;
	static LPDIRECT3DTEXTURE9 pTexture1;
	HRESULT hr;

	if( ! mesh ) {
		HR1( D3DXCreateTextureFromFile( pDevice, TEXT("tex2.png"), &pTexture1 ) )

		srand( timeGetTime() );

		mesh = (struct Particle*) malloc( sizeof Particle * 2000 * 4 );
		indexes = (short*)malloc( sizeof (short) * 2000 * 6 );

		for(int i=0; i<2000; i++ ) {
				velocities[ i ] = 0.6f + float(rand()%100)/50;

				int h = 800;
				int x = (i % 50 - 25) * 2000;
				int z = (i / 50 - 25) * 2000;

				mesh[i*4+0].x = x - 200;
				mesh[i*4+0].y = h - 200;
				mesh[i*4+0].z = z;
				mesh[i*4+0].u = 0.0f;
				mesh[i*4+0].v = 1.0f;

				mesh[i*4+1].x = x + 200;
				mesh[i*4+1].y = h - 200;
				mesh[i*4+1].z = z;
				mesh[i*4+1].u = 1.0f;
				mesh[i*4+1].v = 1.0f;

				mesh[i*4+2].x = x - 200;
				mesh[i*4+2].y = h + 200;
				mesh[i*4+2].z = z;
				mesh[i*4+2].u = 0;
				mesh[i*4+2].v = 0;

				mesh[i*4+3].x = x + 200;
				mesh[i*4+3].y = h + 200;
				mesh[i*4+3].z = z;
				mesh[i*4+3].u = 1.0f;
				mesh[i*4+3].v = 0.0f;

				indexes[i*6+0] = i*4+0;
				indexes[i*6+1] = i*4+1;
				indexes[i*6+2] = i*4+3;
				indexes[i*6+3] = i*4+0;
				indexes[i*6+4] = i*4+3;
				indexes[i*6+5] = i*4+2;
		}
		HR1( pDevice->CreateVertexBuffer( sizeof Particle * 8000, D3DUSAGE_DYNAMIC, 
											D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &pVB, NULL ) )
		HR1( pDevice->CreateIndexBuffer( sizeof (short) * 2000 * 6, D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, 
											D3DPOOL_DEFAULT, &pIB, NULL ) )
		void *bytes;

		pVB->Lock( 0, 0, &bytes, D3DLOCK_DISCARD );
		memcpy( bytes, mesh, sizeof Particle*8000 );
		pVB->Unlock();

		pIB->Lock( 0, 0, &bytes, D3DLOCK_DISCARD );
		memcpy( bytes, indexes, sizeof (short) * 2000 * 6 );
		pIB->Unlock();
	}

	D3DXVECTOR3 vecZ(0,1,0); D3DXVECTOR3 vecCenter(0,0,0);
	D3DXVec3Cross( &vecZ, &vecZ, &g_vCamLook ); 
	D3DXVec3Normalize( &vecZ, &vecZ ); 

	for(int i=0; i<2000; i++ ) {
			vecCenter.x = mesh[i*4+0].x + (mesh[i*4+1].x - mesh[i*4+0].x) / 2;
			vecCenter.z = mesh[i*4+0].z + (mesh[i*4+1].z - mesh[i*4+0].z) / 2;
			mesh[i*4+0].x = vecCenter.x - vecZ.x*200;
			mesh[i*4+0].z = vecCenter.z - vecZ.z*200;
			mesh[i*4+1].x = vecCenter.x + vecZ.x*200;
			mesh[i*4+1].z = vecCenter.z + vecZ.z*200;
			mesh[i*4+2].x = vecCenter.x - vecZ.x*200;
			mesh[i*4+2].z = vecCenter.z - vecZ.z*200;
			mesh[i*4+3].x = vecCenter.x + vecZ.x*200;
			mesh[i*4+3].z = vecCenter.z + vecZ.z*200;

			mesh[i*4+0].y += velocities[ i ];
			mesh[i*4+1].y += velocities[ i ];
			mesh[i*4+2].y += velocities[ i ];
			mesh[i*4+3].y += velocities[ i ];
			if( mesh[i*4+3].y > 30000 ) velocities[ i ] = -0.6f - float(rand()%100)/50;
			if( mesh[i*4+3].y < 1000 ) velocities[ i ] = 0.6f + float(rand()%100)/50;
	}
	void *bytes;
	HR1( pVB->Lock( 0, 0, &bytes, D3DLOCK_DISCARD ) )
	memcpy( bytes, mesh, sizeof Particle*8000 );
	pVB->Unlock();

	D3DXMATRIX matIdentity, matWorld; 
	D3DXMatrixIdentity( &matIdentity );
	HR1( pDevice->GetTransform( D3DTS_WORLD, &matWorld ) )
	HR1( pDevice->SetTransform( D3DTS_WORLD, &matIdentity ) )

	HR1( pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_DESTALPHA ) )

	//pDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );

	pDevice->SetFVF( D3DFVF_XYZ | D3DFVF_TEX1 );
	HR1( pDevice->SetTexture( 0, pTexture1 ) )
	HR1( pDevice->SetStreamSource( 0, pVB, 0, 20 ) )
	HR1( pDevice->SetIndices( pIB ) )
	HR1( pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, 8000, 0, 2000*2 ) )

	HR1( pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO ) )
	//pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );
	//HR1( pDevice->SetTransform( D3DTS_WORLD, &matWorld ) )
}

static float* quad = (float*) malloc( sizeof (float) * 36 );

void DrawQuad() {
	static LPDIRECT3DVERTEXBUFFER9 pVB = 0;
	static LPDIRECT3DTEXTURE9 pTex1;
	HRESULT hr;
	if( !pVB ) {
		quad[0] = -1.0f; quad[1] = -1.0f;	quad[2] = 1.0f;	 *(DWORD*)&quad[3] = D3DCOLOR_ARGB(255,100,80,80);	
		quad[4] = 0.0f; quad[5] = 1.0f;
		quad[6] = 1.0f; quad[7] = 1.0f;	quad[8] = 1.0f;	 *(DWORD*)&quad[9] = D3DCOLOR_ARGB(255,100,80,80);	
		quad[10] = 1.0f; quad[11] = 0.0f;
		quad[12] = 1.0f; quad[13] = -1.0f;	quad[14] = 1.0f; *(DWORD*)&quad[15] = D3DCOLOR_ARGB(255,100,80,80);
		quad[16] = 1.0f; quad[17] = 1.0f;

		quad[18] = -1; quad[19] = -1; quad[20] = 1.0f; *(DWORD*)&quad[21] = D3DCOLOR_ARGB(255,100,80,80);	
		quad[22] = 0.0f; quad[23] = 1.0f;
		quad[24] = -1; quad[25] = 1; quad[26] = 1.0f; *(DWORD*)&quad[27] = D3DCOLOR_ARGB(255,100,80,80);	
		quad[28] = 0.0f; quad[29] = 0.0f;
		quad[30] = 1; quad[31] = 1; quad[32] = 1.0f; *(DWORD*)&quad[33] = D3DCOLOR_ARGB(255,100,80,80);
		quad[34] = 1.0f; quad[35] = 0.0f;

		HR1( pDevice->CreateVertexBuffer( 36*4, D3DUSAGE_WRITEONLY, D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_DIFFUSE, D3DPOOL_DEFAULT,
										&pVB, NULL ) )
		void *data;
		HR1( pVB->Lock( 0, 0, &data, D3DLOCK_DISCARD ) )
		memcpy( data, quad, 36*4 );
		pVB->Unlock();

		HR1( D3DXCreateTextureFromFile( pDevice, TEXT("tex1.jpg"), &pTex1 ) )
	}
	D3DXMATRIX matIdentity, matPersp, matView; 
	D3DXMatrixIdentity( &matIdentity );
	HR1( pDevice->GetTransform( D3DTS_PROJECTION, &matPersp ) )
	HR1( pDevice->GetTransform( D3DTS_VIEW, &matView ) )
	HR1( pDevice->SetTransform( D3DTS_WORLD, &matIdentity ) )
	HR1( pDevice->SetTransform( D3DTS_VIEW, &matIdentity ) )
	matIdentity._34 = 0;
	matIdentity._44 = 1.0;
	HR1( pDevice->SetTransform( D3DTS_PROJECTION, &matIdentity ) )

	//pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
	//HR1( pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE ) )
	//HR1( pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_DESTALPHA ) )

	HR1( pDevice->SetTexture( 0, pTex1 ) )
	HR1( pDevice->SetFVF( D3DFVF_XYZ|D3DFVF_TEX1|D3DFVF_DIFFUSE ) )
	HR1( pDevice->SetStreamSource( 0, pVB, 0, 24) )
	HR1( pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 2 ) )
	//HR1( pDevice-> )
	//HR1( pDevice-> )

	HR1( pDevice->SetTransform( D3DTS_PROJECTION, &matPersp ) )
	HR1( pDevice->SetTransform( D3DTS_VIEW, &matView ) )
	//pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
	//HR1( pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA ) )

}


void RENDER() {
	LPDIRECT3DSURFACE9 pBackBuffer;
	D3DLOCKED_RECT stRect;
	HRESULT hr;

/*
	if( g_pTempSurface1 ) {
		g_pTempSurface1->LockRect( &stRect, NULL, 0 );
		PCHAR pbits = (PCHAR) stRect.pBits;
		for( int i = 0; i < clientWidth; i++ ) {
			*(int*)&pbits[ 100*stRect.Pitch + i*4 ] = (int) D3DCOLOR_ARGB(0,0,0,255);
		}
		g_pTempSurface1->UnlockRect( );
	}
*/
	pDevice->BeginScene();

	hr = pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
									D3DCOLOR_ARGB(0,0,0,0), 1.0, 0 );
	//hr = pDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 
	//								D3DCOLOR_ARGB(255,255,255,255), 1.0, 0 );
	if( FAILED(hr) ) ReportDXError( __LINE__,  hr );
/*
	if( g_pTempSurface1 ) {
		hr = pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
		//hr = pDevice->GetRenderTarget( 0, &pBackBuffer );
		if( FAILED(hr) ) ReportDXError( __LINE__,  hr );
		else {
			hr = pDevice->UpdateSurface( g_pTempSurface1, NULL, pBackBuffer, NULL );
			if( FAILED(hr) ) ReportDXError( __LINE__,  hr );
			pBackBuffer->Release();
		}
	}
*/

	D3DXMATRIX matCam;
	D3DXMatrixLookAtLH( &matCam, &g_vCamPos, &( g_vCamPos + g_vCamLook ), &D3DXVECTOR3(0,1,0)  );
	HR1( pDevice->SetTransform( D3DTS_VIEW, &matCam ) )
	
	//RenderCube();
	DrawQuad();
	RenderTerrain();
	RenderParticle();

	pDevice->EndScene();

	hr = pDevice->Present( NULL, NULL, NULL, NULL );


	if( FAILED(hr) ) ReportDXError( __LINE__,  hr );
	if( hr == D3DERR_DEVICELOST ) {
		hr = pDevice->Reset( &stPresentParameters );
		if( FAILED(hr) ) ReportDXError( __LINE__,  hr );
	}

}



