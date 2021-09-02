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

int hamming_char(char a, char b) {
  int sum = 0;
  for (int i = 0; i < 8; i++) {
    sum += ((a ^ b) >> i) & 1;
  }
  return sum;
}

int hamming_str(char* a, char* b) {
  int sum = 0;
  for (int i = 0; a[i] && b[i]; i++) {
    sum += hamming_char(a[i], b[i]);
  }
  return sum;
}

char break_single_char_key(char* chypher) {
  char best_key;
  char* key = malloc(2);

  key[1] = '\0';

  double best_prob = 0;

  for (int i = 'A'; i <= 'Z'; i++) {
    key[0] = i;
    char* result = vigenere(chypher, key, true);
    double prob = sinkov(result);
    free(result);

    if (prob > best_prob) {
      best_prob = prob;
      best_key = i;
    }
  }

  free(key);
  return best_key;
}

double normalized_distance(char* cypher, int key_size, int skip) {
  char* a = malloc(key_size + 1);
  char* b = malloc(key_size + 1);

  a[key_size] = '\0';
  b[key_size] = '\0';

  for (int i = 0; i < key_size; i++) {
    a[i] = cypher[i + skip];
    b[i] = cypher[i + skip + key_size];
  }
  int dist = hamming_str(a, b);

  free(a);
  free(b);

  return (double)dist / (key_size - 1);
}

char* break_cypher(char* chyper, int key_size) {
  int block_size = key_size;

  char* probable_key = malloc(key_size + 1);
  char* block_str = malloc(block_size + 1);

  probable_key[key_size] = '\0';
  block_str[block_size] = '\0';

  for (int block_idx = 0; block_idx < key_size; block_idx++) {
    for (int i = 0; i < block_size; i++) {
      block_str[i] = chyper[i * block_size + block_idx];
      // block_str[i] = chyper[i + block_size * block_idx];
    }
    probable_key[block_idx] = break_single_char_key(block_str);
    // printf("%s, %c\n", block_str, probable_key[block_idx]);
  }

  printf("Possivel chave: %s\n", probable_key);
  char* result = vigenere(chyper, probable_key, true);
  free(probable_key);
  free(block_str);
  return result;
}

void etapa_3() {
  char* cypher =
      "UVORVPSMAKSWLHWJGQIUAARNRLRAWTGXIFCEUARRWCKKOXEEMLIYJIIORYSRNDBYEJRYPPSV"
      "IFUXSNIRVNHQJMGIQHIOIHJMWSXEFVVPWFLGYSPSHEMVGRAIYGEXRGYMMRFBHWWYVJDNMQGA"
      "EQUSHGLQEOJGUHYHJWPKAITIWZKCIMSFIYJILBXSXDEUEMIGGVRJWMZVEYORETKIGOARLFQD"
      "FWUAVPHJWYSFVLYAHUDIXJWPNFSUWPEGFTRFLSJVYPRAWWLIWSVIADQLQJSKDXDWHIADFDFU"
      "SWLIYSDIPVRHGKIOEJRASVWLIDDJSBVMWSSZGQEQHWHGZRICEEXVUXSDIBVQHIWQGZPDHWGG"
      "CGXHSHKGSLGVIADLRHDMTBRXAKMZVHHVWPYZRTIWTGIEDPSWZVGHFWYWLIUCWRZIEUBSVKUI"
      "SFGQUMIUIEHKSEWSBYTKEUJAEOEXHFFIZLQJFMTUUIWWWXKJHHQGRTVGWWUYZVYTIWVUVRWF"
      "SVTRVHRWTXFQRJWVADHHPSXKAYQHSVBZELBLIXEIWIEKXLTRRWXOVXHGVIIFRQSUXOTYWRWG"
      "UERHQLMILXGSSGKJWDFGGNVJHRSQGTQLZAGORHHAAPGFYPVSGQVVPOXMUJSDQSFGUIVCDXGI"
      "YPJAVAJTDFSEZRGDFGWVISJFSQGJRRXSTGFIXEMIXFIQHJEXEEUSVIVRVDQGRZRGWOJSYCEU"
      "SKHUEISODSYSEUSKHUXEEOGUAVSFVWJKUESCDMIZEFOJMUTEDJAWGGIOCUIRLPDFIYKCEQOH"
      "VGRSQNWXKDYPJAHKFTREMIXGEUOKIPFKDFUSSGSVWUEUUIJWDFKIXRUAP";

  int best_keysize_so_far = 0;
  double best_normalize_so_far = 6000;

  for (int i = 2; i < 30; i++) {
    int blocks = 20;
    double normalize = 0;
    for (int skip = 0; skip <= blocks; skip += i) {
      normalize += normalized_distance(cypher, i, skip);
    }
    normalize /= blocks;
    if (normalize < best_normalize_so_far) {
      best_normalize_so_far = normalize;
      best_keysize_so_far = i;
    }
  }

  printf("\nKeysize mais provavel: %d - %lf\n", best_keysize_so_far,
         best_normalize_so_far);

  char* plain_text = break_cypher(cypher, best_keysize_so_far);
  printf("Conteudo: %s\n\n", plain_text);
}

void etapa_1() {
  char* chypher = "MXOXYBKPSLZBZLKPBDRFRABZFCOXOXJBKPXDBJ";
  char* best_key = malloc(2);

  best_key[0] = break_single_char_key(chypher);
  best_key[1] = '\0';

  char* result = vigenere(chypher, best_key, true);
  printf("Conteudo de %s: %s (chave %s, LW score %lf)\n", chypher, result,
         best_key, sinkov(result));
  free(result);
  free(best_key);
}

int main() {
  srand(time(NULL));

  etapa_1();
  etapa_3();

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