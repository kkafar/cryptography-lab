/**
 * Szyfr będzie operował na słowie 64 bitowym, oraz kluczu 64 bitowym.
 * 
 * Wyrzucamy permutację początkową i końcową, ponieważ nie wpływają na 
 * bezpieczeństwo szyfru.
 * 
 * Funkcja nieliniowa f, będzie:
 * 1. dostawać na wejście słowo 32 bitowe i 64 bitowy klucz
 * 2. rozszerzać słowo 32 bitowe do 64
 * 
 * Przeprowadzam rozszerzenie słowa do 64 bitów w celu zwiększenia dyfuzji. 
 * Rezygnuję natomiast z pobierania fragmentu klucza danej rundy (zamiast tego
 * biorę cały).
 * 
 * Nieliniowość będzie wprowadzona poprzez sboxy. 
 * W odróżnieniu od DESa
 * 
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cstring>

typedef char byte_t;
typedef uint64_t key_t;

#define N_ROUNDS 16
#define BLOCK_SIZE_b 64
#define BLOCK_SIZE_B (BLOCK_SIZE_b / 8)
#define HALF_BLOCK_SIZE_B (BLOCK_SIZE_B / 2)

constexpr byte_t pc_1[] = {

};

constexpr byte_t block_extension_map[] = {
  27, 28, 29, 30, 31,  0,  1,  2,
   2,  3,  4,  5,  6,  7,  8,  9,
   9, 10, 11, 12, 13, 14, 15, 16,
  16, 17, 18, 19, 20, 21, 22, 23,
  23, 24, 25, 26, 27, 28, 29, 30,
  31,  0,  1,  3,  4,  5,  6,  7,
   8, 10, 11, 12, 13, 14, 15, 17,
  18, 19, 20, 21, 22, 24, 25, 26
};

constexpr byte_t sbox[16][16] = {
  {

  }
};

inline void xor_bytes(
  const byte_t bytes1[],
  const byte_t bytes2[],
  byte_t result[],
  size_t size
) {
  for (size_t i = 0; i < size; ++i) 
    result[i] = bytes1[i] ^ bytes2[i];
}

void generate_keys(key_t initial_key, key_t key_set[N_ROUNDS]) {


}

void key_to_bytes(key_t key, byte_t buffer[8]) {
  key_t mask = 0b11111111;
  for (int i = 7; i >= 0; --i) {
    buffer[i] = key & mask;
    mask <<= 8;
  }
}

byte_t * nonlinear_f(key_t key_fragment, const byte_t bytes[HALF_BLOCK_SIZE_B]) {
  byte_t ext_bytes[BLOCK_SIZE_B];
  for (size_t i = 0; i < BLOCK_SIZE_B; ++i)
    ext_bytes[i] = bytes[block_extension_map[i]];
  
  byte_t key[8];
  key_to_bytes(key_fragment, key);
  xor_bytes(ext_bytes, key, ext_bytes, BLOCK_SIZE_B);

}

void encode(
  const char * const plain_text, 
  size_t plain_text_len, 
  char * const cipher_text_buffer,
  key_t key
) {
  memset(cipher_text_buffer, 0, plain_text_len);

  size_t block_count = plain_text_len / BLOCK_SIZE_b;

  key_t key_set[N_ROUNDS];
  generate_keys(key, key_set);

  byte_t block_l[HALF_BLOCK_SIZE_B];
  byte_t block_r[HALF_BLOCK_SIZE_B];
  byte_t new_block_l[HALF_BLOCK_SIZE_B];
  byte_t new_block_r[HALF_BLOCK_SIZE_B];

  for (size_t block_i = 0; block_i < block_count; ++block_i) {
    // dzielę blok na pół
    memcpy(
      block_l, 
      plain_text + block_i * BLOCK_SIZE_B, 
      HALF_BLOCK_SIZE_B
    );
    memcpy(
      block_r, 
      plain_text + block_i * BLOCK_SIZE_B + HALF_BLOCK_SIZE_B, 
      HALF_BLOCK_SIZE_B
    );

    for (size_t round_i = 0; round_i < N_ROUNDS; ++round_i) {
      xor_bytes(
        block_l,
        nonlinear_f(
          key_set[round_i],
          block_r
        ),
        new_block_r,
        HALF_BLOCK_SIZE_B
        );
      memcpy(block_l, block_r, HALF_BLOCK_SIZE_B);
      memcpy(block_r, new_block_r, HALF_BLOCK_SIZE_B);
    }
    // skopiowanie do wyniku
    memcpy(
      cipher_text_buffer + block_i * BLOCK_SIZE_B,
      block_l,
      HALF_BLOCK_SIZE_B
    );
    memcpy(
      cipher_text_buffer + block_i * BLOCK_SIZE_B + HALF_BLOCK_SIZE_B,
      block_r,
      HALF_BLOCK_SIZE_B
    );
  }
}

void decode(
  const char * const cipher_text,
  size_t cipher_text_len,
  char * const plain_text_buffer,
  key_t key
) {
  memset(plain_text_buffer, 0, cipher_text_len);
}

inline void print_text(const char * const text, size_t len) {
  for (size_t i = 0; i < len; ++i)
    std::cout << text[i];

  std::cout << std::endl;
}

// inline void set


int main(int argc, char * argv[]) {
  constexpr int text_size = 512;

  srand(time(NULL));

  key_t key = 0b1011'1001'0000'1101'1111'1100'1010'0110'0010'1111'1011'1110'0001'0101'0111'0011;
  char * const plain_text = new char[text_size];
  char * const cipher_text = new char[text_size];
  char * const decoded_text = new char[text_size];

  for (int i = 0; i < text_size; ++i) {
    plain_text[i] = rand() % ('z' - 'a') + 'a';
  }
  std::cout << "Plaintext:\n";
  print_text(plain_text, text_size);

  encode(plain_text, text_size, cipher_text, key);

  std::cout << "Ciphertext:\n";
  print_text(cipher_text, text_size);

  decode(cipher_text, text_size, decoded_text, key);

  std::cout << "Decoded:\n";
  print_text(decoded_text, text_size);

  delete[] plain_text;
  delete[] cipher_text;
  delete[] decoded_text;
  return 0;
}
