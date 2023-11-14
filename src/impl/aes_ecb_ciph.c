#include <string.h>
#include <locale.h>
#include <pthread.h>
#include "../lib/aes.h"

#ifdef __unix__         
  #include <unistd.h>
  #define NCORE sysconf(_SC_NPROCESSORS_CONF)

#elif defined(_WIN32) || defined(WIN32) 
  #define OS_Windows
  #include <windows.h>
  LPSYSTEM_INFO lpsysteminfo;
  GetSystemInfo(lpsysteminfo);
  #define NCORE lpsysteminfo->dwNumberOfProcessors

#endif

void report(void *args){
  size_t *count, *size;
  char *done;
  void **aux_args = (void**)args;
  count = aux_args[0];
  size = aux_args[1];
  done = aux_args[2];
  while(!(*done)){
    printf("PROCESSO: %ld%% CONCLUIDO\r", 100*(*count)/(*size));
    usleep(750);
  }
}

void no_threads(size_t sizein, FILE* in, FILE* out, uint8_t *k, uint8_t n){
  uint8_t buffer[16];
  uint8_t tmp[16];
  size_t count = 0;
  do{
    size_t qnt;
    qnt = fread(&buffer, 1, 16, in);
    if(feof(in))
      buffer[15] = qnt;
    ciph(tmp, buffer, k, n);
    fwrite(tmp, 1, 16, out);
    count += 16;
  } while(!feof(in));
}

void only_reporter(size_t sizein, FILE* in, FILE* out, uint8_t *k, uint8_t n){
  uint8_t buffer[16];
  uint8_t tmp[16];
  size_t count = 0;
  char done = 0;
  pthread_t thread;
  void* args[] = {&count, &sizein, &done};
  
  pthread_create(&thread, NULL, (void*)report, (void*)args);
  do{
    size_t qnt;
    qnt = fread(&buffer, 1, 16, in);
    if(feof(in))
      buffer[15] = qnt;
    ciph(tmp, buffer, k, n);
    fwrite(tmp, 1, 16, out);
    count += 16;
  } while(!feof(in));
  done = 1;
  pthread_join(thread, NULL);
}

void with_threads(size_t sizein, FILE* in, FILE* out, uint8_t *k, uint8_t n){
  
}

void cipher(char *s, char *d, uint8_t *k, uint8_t n){
  FILE *in, *out;
  size_t sizein;
  
  printf("ORIGEM: %s\nDESTINO: %s\n", s, d);
  in = fopen(s, "rb");
  out = fopen(d, "wb");
  if(!in){
    printf("ERRO: O arquivo %s não pôde ser aberto.\n", s);
    exit(EXIT_FAILURE);
  }
  if(!out){
    printf("ERRO: O arquivo %s não pôde ser aberto.\n", d);
    exit(EXIT_FAILURE);
  }
  
  fseek(in, 0, SEEK_END);
  sizein = ftell(in);
  fseek(in, 0, SEEK_SET);
  printf("PROCESSO: 0%% CONCLUIDO\r");
  switch(NCORE){
    case 0:
      exit(1);
    case 1:
      no_threads(sizein, in, out, k, n);
      break;
    case 2:
    case 3:
    case 4:
      only_reporter(sizein, in, out, k, n);
      break;
    default:
      only_reporter(sizein, in, out, k, n);
      break;
  }
  printf("PROCESSO: 100%% CONCLUIDO\n\n");
  fclose(in);
  fclose(out);
}

int main(int argc, char **argv){
  size_t n = strlen(argv[3]);
  setlocale(LC_ALL, "portuguese");
  
  if(n != 16 && n !=24 && n != 32){
    printf("ERRO: A chave deve ter 16, 24, ou 32 digitos!\n");
    exit(EXIT_FAILURE);
  }
  cipher(argv[1], argv[2], (uint8_t*)argv[3], strlen(argv[3])>>2);
  return 0;
}