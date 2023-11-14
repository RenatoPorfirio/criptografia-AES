#include <string.h>
#include <locale.h>
#include <pthread.h>
#include <unistd.h>
#include "../lib/aes.h"

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

void decipher(char *s, char *d, uint8_t *k, uint8_t n){
  FILE *in, *out;
  uint8_t buffer[16];
  uint8_t tmp[16];
  long sizein;
  long count = 0;
  char done = 0;
  pthread_t thread;
  void* args[] = {&count, &sizein, &done};
  
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
  pthread_create(&thread, NULL, (void*)report, (void*)args);
  size_t qnt;
  qnt = fread(buffer, 1, 16, in);
  while(!feof(in)){
    deciph(tmp, buffer, k, n);
    fwrite(tmp, 1, qnt, out);
    count += 16;
    qnt = fread(buffer, 1, 16, in);
  }
  deciph(tmp, buffer, k, n);
  fwrite(tmp, 1, buffer[15], out);
  count += 16;
  done = 1;
  pthread_join(thread, NULL);
  printf("PROCESSO: 100%% CONCLUIDO\n\n");
  fclose(in);
  fclose(out);
}

int main(int argc, char **argv){
  setlocale(LC_ALL, "portuguese");
  decipher(argv[1], argv[2], (uint8_t*)argv[3], strlen(argv[3])>>2);
  return 0;
}