#include <stdint.h>
#include <cmath>
#include <initguid.h>
#include <dsound.h>
#include "raii.cpp"

void print_last_error() {
  static char buf[256];
  FormatMessageA( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), 0, buf, sizeof buf, NULL );
  printf("Error 0x%x, Message: %s\n", (unsigned int)GetLastError(), buf );
}

auto release_fn = []( IUnknown* p ) { printf("relese %p\n", (void*)p); p->Release(); };

auto ds = raii( (IDirectSound8*) 0, release_fn );
int turn{};
decltype(raii( (IDirectSoundBuffer8*) 0, release_fn )) dsb[2] = { raii( (IDirectSoundBuffer8*) 0, release_fn ), raii( (IDirectSoundBuffer8*) 0, release_fn ) };

bool dsound_init() { 
  HRESULT hr = DirectSoundCreate8( NULL, &ds.data(), NULL );
  if( FAILED(hr) ) {
    print_last_error();
    return false;
  }
  
  hr = ds->SetCooperativeLevel( GetDesktopWindow(), DSSCL_PRIORITY );
  if( FAILED(hr) ) {
    print_last_error();
    return false;
  }
  
  WAVEFORMATEX format = { WAVE_FORMAT_PCM, 2, 44100, 44100 * 4, 4, 16, sizeof( WAVEFORMATEX) };
  DSBUFFERDESC dsbdesc{};
  dsbdesc.dwSize = sizeof dsbdesc;
  dsbdesc.dwFlags = DSBCAPS_GLOBALFOCUS | DSBCAPS_GETCURRENTPOSITION2;
  dsbdesc.dwBufferBytes = 44100 * 4;
  dsbdesc.lpwfxFormat = &format;

  for( size_t i{0}; i<2; i++ )  {
    auto dsbtemp1 = raii( (IDirectSoundBuffer*) 0, release_fn );
    hr = ds->CreateSoundBuffer( &dsbdesc, &dsbtemp1.data(), NULL );
    if( FAILED(hr) ) {
      print_last_error();
      return 1;
    }
    
    hr = dsbtemp1->QueryInterface( IID_IDirectSoundBuffer8, (void**)&dsb[i].data() );
    if( FAILED(hr) ) {
      print_last_error();
      return false;
    }
  }  
  
  //size_t wfxsz{};
  //dsb8->GetFormat( &format, sizeof format, (LPDWORD)&wfxsz );
  //printf("nsamples = %d\n", (int)format.nSamplesPerSec);
return true;
}

void dsound_play(float freq) {  

  dsb[turn]->Stop();

  if( 0 == freq || !play_flag ) return;

  printf("play %f, turn = %d\n", freq, turn);

  turn = turn ^ 1;

  dsb[turn]->SetCurrentPosition(0);

  int32_t* buffer{};
  size_t buffersz{};
  HRESULT hr = dsb[turn]->Lock( 0, 0, (void**)&buffer, (LPDWORD)&buffersz, nullptr, nullptr, DSBLOCK_ENTIREBUFFER );
  if( FAILED( hr ) ) {
    if( DSERR_BUFFERLOST == hr ) { 
      printf("lost buffer\n");
      hr = dsb[turn]->Restore();
      if( FAILED( hr ) ) return;
      hr = dsb[turn]->Lock( 0, 0, (void**)&buffer, (LPDWORD)&buffersz, nullptr, nullptr, DSBLOCK_ENTIREBUFFER );
      if( FAILED( hr ) ) return;
    }
  }
  printf("locked %d\n", buffersz );

  float pi2 = 2.0f * 3.141592f;
  for(size_t i=0; i < 44100; i ++ ) { 
      float t = i * pi2 / 44100.0f;
      float vol = 2048.0f;
      uint16_t sample{};

      if( 0 == wave_type )
        sample = vol * sinf( t * freq );

      if( 1 == wave_type ) {
        float sin = sinf( t * freq );
        //if( sin < 0.05f && sin > -0.05f ) sin = sqrtf( 20.0f * sin );
        //else sin = sin > .0f ? 1.0f : -1.0f;
        float sin2 = (sqrtf(fabs(sin))) * (sin > .0f ? 1.0f : -1.0f);
        sample = vol * sin2;
      }

      if( 2 == wave_type ) {
        float frac = freq * t / pi2 - floor(freq * t / pi2) ;
        sample = vol * ( ( frac < 0.5f ? 4 * frac : 4 - 4 * frac ) - 1 );
      }

      buffer[ i ] = sample << 16 | sample;
  };

  dsb[turn]->Unlock( buffer, buffersz, nullptr, 0 );
  dsb[turn]->Play( 0, 0, DSBPLAY_LOOPING);
}


