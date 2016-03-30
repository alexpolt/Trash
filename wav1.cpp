#include <cstdio>
#include <cmath>
#include <cstdlib>

struct wave_format {
  short wFormatTag;
  short nChannels;
  int   nSamplesPerSec;
  int   nAvgBytesPerSec;
  short nBlockAlign;
  short wBitsPerSample;
  short cbSize;
};

const int wf_pcm = 0x0001;

struct riff_chunk {
  char id[4];
  int size;
};

struct riff_header {
  riff_chunk header_chunk;
  char data_type[4];
  riff_chunk wfx_chunk;
  wave_format wfx;
  riff_chunk data_chunk;
};

const float base_freq = 440.0f;
const float pi2 = 2.0f * 3.141592f;
const float maxvol = 32767.0f;
const int sec = 10;
const int sample_rate = 44100;
const int channels = 1;
const int bytes_per_sample = sizeof( short );
const int samples = sample_rate * sec;
const int samples_bytes = samples * bytes_per_sample;

int main() {

  short *data = (short*) malloc( samples_bytes );

  riff_header header{ { {'R','I','F','F'}, sizeof( riff_header ) + sizeof( data ) },
                        {'W','A','V','E'},
                      { {'f','m','t',' '}, sizeof( wave_format ) },
                      { wf_pcm, channels, sample_rate, sample_rate * channels * bytes_per_sample,
                        channels * bytes_per_sample, bytes_per_sample * 8, 0 },
                      { {'d','a','t','a'}, samples_bytes }
                    };

  for(int i=0; i < samples; i++ ) {
    float t = float ( sec * i ) / samples;
    float pi2t = t * pi2;
    float result;
    result  = sinf( 440 * pi2t  + 100 * sinf( 2.0f * pi2t ) );
    result += sinf( 880 * pi2t  + 100 * sinf( 4.0f * pi2t ) );
    result /= 2.0f;
    result += (2.0f*t - int(2.0*t)) * (sinf( 220 * pi2t )>=0?1.0f:-1.0f);
    result /= 2.0f;
    data[i] = maxvol * result;
  }

  FILE* f = fopen( "wav1.wav", "wb");
  fwrite( &header, 1, sizeof( riff_header ), f );
  fwrite( data, 1, samples_bytes, f );
  fclose( f );

}


