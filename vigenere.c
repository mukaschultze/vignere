#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const int ALPHABET_LEN = ('z' + 1 - 'a');

char recta(char a, char b, int op) {
  bool upper = isupper(a);
  a = tolower(a);
  b = tolower(b);

  if (a < 'a' || a > 'z') return a;

  a -= 'a';
  b -= 'a';

  char r = 'a' + (a + (b * op) + ALPHABET_LEN) % ALPHABET_LEN;

  return upper ? toupper(r) : r;
}

char* vigenere(char* str, char* key, bool decrypt) {
  int len = strlen(str);
  int lenKey = strlen(key);

  char* result = malloc(len);
  int op = decrypt ? -1 : 1;

  for (int i = 0; i < len; i++) {
    result[i] = recta(str[i], key[i % lenKey], op);
  }

  return result;
}

char* randomKey(int len) {
  char* buffer = malloc(len);
  for (int i = 0; i < len; i++) {
    buffer[i] = (rand() % ALPHABET_LEN) + 'a';
  }
  return buffer;
}

char* trim(char* s) {
  char* old = s;
  while (isspace(*s)) s++;
  char* back = s + strlen(s);
  while (isspace(*--back))
    ;
  *(back + 1) = '\0';
  char* new = strdup(s);
  free(old);
  return new;
}

int main() {
  srand(time(NULL));

  char* str = NULL;
  char* key = NULL;
  size_t size = 0;

  printf("Texto: ");
  getline(&str, &size, stdin);
  str = trim(str);

  printf("Chave (vazio para gerar automaticamente): ");
  getline(&key, &size, stdin);
  key = trim(key);

  if (!strlen(key)) {
    printf("Usando chave aleatória\n");
    free(key);
    key = randomKey(strlen(str));
  }

  char* result = vigenere(str, key, false);
  char* inverse = vigenere(result, key, true);

  printf("Texto:\t%s\n", str);
  printf("Chave:\t%s\n", key);
  printf("Cripto:\t%s\n", result);
  printf("Decrip:\t%s\n", inverse);
}