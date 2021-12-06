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

typedef uint8_t byte_t;

#define N_ROUNDS 16
#define BLOCK_SIZE_b 64
#define BLOCK_SIZE_B (BLOCK_SIZE_b / 8)
#define HALF_BLOCK_SIZE_B (BLOCK_SIZE_B / 2)
#define KEY_SIZE_b 64
#define KEY_SIZE_B (KEY_SIZE_b / 8)

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

/**
 * 16 sboxów, mapujących 4 bitowe wejście na 2 bitowe wyjście każdy
 * 
 * Indeksacja dla wejścia <b0 b1 b2 b3>, 
 * <b1 b3> to numer wiersza, <b0 b2> to nr kolumny
 */
constexpr byte_t sbox[16][4][4] = {
	// sbox 0
  {
		{0, 3, 1, 2},
		{3, 2, 1, 0},
		{1, 2, 0, 3},
		{1, 2, 3, 0}
  },
	// sbox 1
	{
		{0, 1, 3, 2},
		{1, 3, 0, 2},
		{2, 1, 0, 3},
		{3, 2, 0, 1}
	},
	// sbox 2
	{
		{2, 0, 3, 1},
		{2, 0, 1, 3},
		{1, 3, 2, 0},
		{3, 1, 2, 0}
	},
	// sbox 3
	{
		{3, 1, 0, 2},
		{3, 0, 2, 1},
		{2, 3, 1, 0},
		{2, 3, 0, 1}
	},
	// sbox 4
	{
		{2, 3, 1, 0},
		{2, 1, 0, 3},
		{0, 1, 2, 3},
		{0, 2, 3, 1}
	},
	// sbox 5
	{
		{0, 3, 2, 1},
		{0, 1, 2, 3},
		{0, 2, 1, 3},
		{2, 1, 0, 3}
	},
	// sbox 6
	{
		{3, 2, 1, 0},
		{2, 1, 0, 3},
		{1, 2, 0, 3},
		{1, 3, 0, 2}
	},
	// sbox 7
	{
		{0, 3, 1, 2},
		{1, 2, 0, 3},
		{2, 3, 1, 0},
		{3, 0, 1, 2}
	},
	// sbox 8
	{
		{2, 1, 0, 3},
		{2, 0, 1, 3},
		{1, 2, 0, 3},
		{3, 2, 0, 1}
	},
	// sbox 9
	{
		{0, 3, 1, 2},
		{2, 3, 0, 1},
		{2, 0, 1, 3},
		{1, 3, 2, 0}
	},
	// sbox 10
	{
		{3, 1, 2, 0},
		{3, 0, 2, 1},
		{1, 0, 2, 3},
		{0, 2, 1, 3}
	},
	// sbox 11
	{
		{0, 3, 1, 2},
		{2, 1, 0, 3},
		{1, 2, 3, 0},
		{0, 2, 3, 1}
	},
	// sbox 12
	{
		{3, 0, 2, 1},
		{0, 3, 1, 2},
		{1, 0, 3, 2},
		{1, 3, 0, 2}
	},
	// sbox 13
	{
		{1, 2, 3, 0},
		{2, 3, 0, 1},
		{2, 0, 1, 3},
		{0, 3, 1, 2}
	},
	// sbox 14
	{
		{1, 2, 0, 3},
		{2, 3, 1, 0},
		{3, 1, 0, 2},
		{0, 2, 3, 1}
	},
	// sbox 15
	{
		{1, 0, 3, 2},
		{3, 1, 2, 0},
		{3, 0, 1, 2},
		{0, 2, 3, 1}
	}
};

inline void _XOR_(
  const byte_t bytes1[],
  const byte_t bytes2[],
  byte_t result[],
  size_t size
) {
  for (size_t i = 0; i < size; ++i) {
    result[i] = bytes1[i] ^ bytes2[i];
	}
}

void generate_keys(byte_t initial_key[KEY_SIZE_B], byte_t key_set[N_ROUNDS][KEY_SIZE_B]) {

}

inline void _AND_(const byte_t bytes[], const byte_t mask[], byte_t output[], size_t byte_count) {
	for (size_t i = 0; i < byte_count; ++i) {
		output[i] = bytes[i] & mask[i];
	}
}

inline void _OR_(const byte_t bytes[], const byte_t mask[], byte_t output[], size_t byte_count) {
	for (size_t i = 0; i < byte_count; ++i) {
		output[i] = bytes[i] | mask[i];
	}
}


void apply_sbox(byte_t input[BLOCK_SIZE_B], byte_t output[HALF_BLOCK_SIZE_B]) {
	memset(output, 0, HALF_BLOCK_SIZE_B);
	// dzielimy 64 bity wejściowe na 16 części
	byte_t mask_l = 0b1111'0000, mask_r = 0b0000'1111;
	byte_t halfbyte_ll, halfbyte_lr, halfbyte_rl, halfbyte_rr;
	byte_t mask_row = 0b0101, mask_col = 0b1010;
	byte_t twobit_ll, twobit_lr, twobit_rl, twobit_rr;

	for (size_t i = 0, j = 0; i < BLOCK_SIZE_B; i += 2, ++j) {
		halfbyte_ll = (input[i] & mask_l) >> 4;
		halfbyte_lr = input[i] & mask_r;
		halfbyte_rl = (input[i + 1] & mask_l) >> 4;
		halfbyte_rr = input[i + 1] & mask_r;

		twobit_ll = sbox[2 * i][halfbyte_ll & mask_row][halfbyte_ll & mask_col];
		twobit_lr = sbox[2 * i + 1][halfbyte_lr & mask_row][halfbyte_lr & mask_col];
		twobit_rl = sbox[2 * i + 2][halfbyte_rl & mask_row][halfbyte_rl & mask_col];
		twobit_rr = sbox[2 * i + 3][halfbyte_rr & mask_row][halfbyte_rr & mask_col];

		output[j] = (twobit_ll << 6) | (twobit_lr << 4) | (twobit_rl << 2) | (twobit_rr);
	}

	
}

void nonlinear_f(const byte_t key[KEY_SIZE_B], const byte_t bytes[HALF_BLOCK_SIZE_B], byte_t output[HALF_BLOCK_SIZE_B]) {
  byte_t ext_bytes[BLOCK_SIZE_B];
  for (size_t i = 0; i < BLOCK_SIZE_B; ++i)
    ext_bytes[i] = bytes[block_extension_map[i]];
  
  _XOR_(ext_bytes, key, ext_bytes, BLOCK_SIZE_B);

	apply_sbox(ext_bytes, output);
}


void encode(
  const char * const plain_text, 
  size_t plain_text_len, 
  char * const cipher_text_buffer,
  byte_t key[KEY_SIZE_B]
) {
  memset(cipher_text_buffer, 0, plain_text_len);

  size_t block_count = plain_text_len / BLOCK_SIZE_b;

  byte_t key_set[N_ROUNDS][KEY_SIZE_B];
  generate_keys(key, key_set);

  byte_t block_l[HALF_BLOCK_SIZE_B];
  byte_t block_r[HALF_BLOCK_SIZE_B];
  // byte_t new_block_l[HALF_BLOCK_SIZE_B];
  byte_t new_block_r[HALF_BLOCK_SIZE_B];

	byte_t f_output[HALF_BLOCK_SIZE_B];

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
			nonlinear_f(key_set[round_i], block_r, f_output);
      _XOR_(
        block_l,
				f_output,
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
  byte_t key[KEY_SIZE_B]
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

  byte_t key[KEY_SIZE_B] = {
		0b1011'1001,
		0b0000'1101,
		0b1111'1100,
		0b1010'0110,
		0b0010'1111,
		0b1011'1110,
		0b0001'0101,
		0b0111'0011
	};

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
