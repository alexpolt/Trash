#include <stdio.h>

bool play_flag{};
unsigned wave_type{};

#include "dsound1.cpp"

#include "dsound2rc.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>


INT_PTR CALLBACK dlgproc1(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  switch( msg ) {
    case WM_INITDIALOG:
      CheckDlgButton( hwnd, IDC_RADIO1, BST_CHECKED );
      return TRUE;

    case WM_CLOSE:
      DestroyWindow( hwnd );
      PostQuitMessage( 0 );
      break;

    case WM_HSCROLL:
      if( LOWORD( wparam ) == SB_THUMBPOSITION
            || LOWORD( wparam ) == SB_THUMBTRACK ) {
              SetDlgItemInt( hwnd, IDC_EDIT1, HIWORD( wparam ), FALSE);
              int freq;
              freq = GetDlgItemInt( hwnd, IDC_EDIT1, NULL, FALSE );
              dsound_play( freq );
      }
    break;

    case WM_COMMAND: {
      if( LOWORD(wparam) == IDC_BUTTON1 ) {
        play_flag = play_flag ^ 1;
        if( play_flag ) SetWindowText( (HWND) lparam, "Playing" );
        else SetWindowText( (HWND) lparam, "Stopped" );
      }
      if( LOWORD(wparam) == IDC_RADIO1 ) if( IsDlgButtonChecked( hwnd, IDC_RADIO1 ) == BST_CHECKED ) wave_type = 0;
      if( LOWORD(wparam) == IDC_RADIO2 ) if( IsDlgButtonChecked( hwnd, IDC_RADIO2 ) == BST_CHECKED )  wave_type = 1;
      if( LOWORD(wparam) == IDC_RADIO3 ) if( IsDlgButtonChecked( hwnd, IDC_RADIO3 ) == BST_CHECKED )  wave_type = 2;

      int freq;
      freq = GetDlgItemInt( hwnd, IDC_EDIT1, NULL, FALSE );
      HWND tb1 = GetDlgItem( hwnd, IDC_SLIDER1 );
      SendMessage( tb1, TBM_SETPOS, TRUE, freq );

      dsound_play( freq );
    }
    break;

    default:;
  }
return 0;
}

//int main () {
int CALLBACK WinMain( HINSTANCE, HINSTANCE, LPSTR cmdline, int cmdshow ) {
  InitCommonControls();
  if(!dsound_init() ) {
    printf("dsound init error %u\n", (unsigned)GetLastError() );
    return 1;
  }
  
  HINSTANCE hinst = GetModuleHandle(NULL);
  
  HWND dlg1 = CreateDialog( hinst, MAKEINTRESOURCE( IDD_DIALOG1 ), NULL, dlgproc1 );
  if( !dlg1 ) {
    printf("error dialog %x\n", (unsigned)GetLastError() );
    return 1;
  }
  
  HWND tb1 = GetDlgItem( dlg1, IDC_SLIDER1 );
  if( !tb1 ) {
    printf("error slider %u\n", (unsigned)GetLastError() );
    return 1;
  }
  
  SendMessage( tb1, TBM_SETRANGE, 1, MAKELONG(0,10000) );
  SendMessage( tb1, TBM_SETTICFREQ, 100, 0 );
  
  MSG msg;
  while( GetMessage( &msg, NULL, 0, 0 ) ) {
    IsDialogMessage( dlg1, &msg );
  }
  
  return 0;
}
