#include <ctype.h>
#include <math.h>
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

double frequency_of_letter(char letter) {
  double lookup[] = {
      0.1463,  // A
      0.0104,  // B
      0.0388,  // C
      0.0501,  // D
      0.1257,  // E
      0.0102,  // F
      0.0130,  // G
      0.0128,  // H
      0.0618,  // I
      0.0040,  // J
      0.0002,  // K
      0.0278,  // L
      0.0474,  // M
      0.0505,  // N
      0.1073,  // O
      0.0252,  // P
      0.0120,  // Q
      0.0653,  // R
      0.0781,  // S
      0.0434,  // T
      0.0463,  // U
      0.0167,  // V
      0.0001,  // W
      0.0021,  // X
      0.0001,  // Y
      0.0047,  // Z
  };

  letter = toupper(letter);
  if (letter > 'Z' || letter < 'A') return 0;
  return lookup[letter - 'A'];
}

double sinkov(char* str) {
  double sum = 0;
  for (int i = 0; str[i]; i++) {
    sum += log10(1000 * frequency_of_letter(str[i]));
  }
  return sum;
}

void teste() {
  char* str = "MXOXYBKPSLZBZLKPBDRFRABZFCOXOXJBKPXDBJ";
  char* key = malloc(2);
  char* best_key = malloc(2);

  key[1] = '\0';
  best_key[1] = '\0';

  double best_prob = 0;

  for (int i = 'A'; i <= 'Z'; i++) {
    key[0] = i;
    char* result = vigenere(str, key, true);
    double prob = sinkov(result);

    if (prob > best_prob) {
      best_prob = prob;
      best_key[0] = i;
    }
    free(result);
  }

  char* result = vigenere(str, best_key, true);
  printf("Conteudo de %s: %s (chave %s, LW score %lf)\n", str, result, best_key,
         sinkov(result));
  free(result);
}

int main() {
  srand(time(NULL));

  teste();

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