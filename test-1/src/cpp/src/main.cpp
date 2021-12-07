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
#include <bitset>
#include <memory>


#define N_ROUNDS 16
#define BLOCK_SIZE_b 64
#define BLOCK_SIZE_B (BLOCK_SIZE_b / 8)
#define HALF_BLOCK_SIZE_B (BLOCK_SIZE_B / 2)
#define HALF_BLOCK_SIZE_b (BLOCK_SIZE_b / 2)
#define KEY_SIZE_b 64
#define KEY_SIZE_B (KEY_SIZE_b / 8)

typedef uint8_t byte_t;
// typedef std::bitset<KEY_SIZE_b> key_type;

constexpr byte_t key_permutation[] = {
	45, 31, 32,  1, 63, 19, 62, 53,
	 2,  9, 11, 21, 37, 51, 54, 18,
	34, 47, 16,  8,  0, 52, 43, 41,
	 6, 49, 33, 22, 27, 44, 24, 57,
	 3, 20, 28, 60, 61, 39, 50, 25,
	17, 10,  4,  5, 58, 14, 29, 38,
	12, 35, 59, 13, 46, 23, 42, 26,
	36, 56, 30,  7, 48, 55, 15, 40
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


inline void print_text(const byte_t * const text, size_t len) {
  for (size_t i = 0; i < len; ++i)
    std::cout << (char) text[i];

  std::cout << std::endl;
}

std::unique_ptr<std::bitset<KEY_SIZE_b>[]> generate_keys(const std::bitset<KEY_SIZE_b>& key) {
	std::unique_ptr<std::bitset<KEY_SIZE_b>[]> key_set(new std::bitset<KEY_SIZE_b>[N_ROUNDS]);

	for (size_t i = 0; i < N_ROUNDS; ++i) {
		for (size_t j = 0; j < KEY_SIZE_b; ++j) {
			// key_set[i][j] = key[key_permutation[j]];
			key_set[i][j] = key[j];
		}
	}
	return key_set;
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


std::bitset<HALF_BLOCK_SIZE_b> apply_sbox(
	const std::bitset<BLOCK_SIZE_b>& input
) {
	std::bitset<HALF_BLOCK_SIZE_b> output(0);
	std::bitset<4> input_block, row_mask(0b0101), col_mask(0b1010);
	byte_t sbox_out;
	for (size_t i = 0, j = 0; i < BLOCK_SIZE_b; i += 4, j += 2) {
		input_block[0] = input[i];
		input_block[1] = input[i + 1];
		input_block[2] = input[i + 2];
		input_block[3] = input[i + 3];

		sbox_out = sbox[j >> 1][(input_block & row_mask).to_ulong()][(input_block & col_mask).to_ulong()];
		output[j] = sbox_out & 2;
		output[j + 1] = sbox_out & 1;
	}
	return output;
}

std::bitset<HALF_BLOCK_SIZE_b> nonlinear_f(
	const std::bitset<KEY_SIZE_b>& key,
	const std::bitset<HALF_BLOCK_SIZE_b>& bits
) {
	std::bitset<BLOCK_SIZE_b> extbits;
	for (size_t i = 0; i < BLOCK_SIZE_b; ++i) {
		extbits[i] = bits[block_extension_map[i]];
	}

	extbits ^= key; 

	return apply_sbox(extbits);
}


void encode(
  const byte_t * const plain_text, 
  size_t plain_text_size_B, 
  byte_t * const cipher_text_buffer,
  const std::bitset<KEY_SIZE_b>& key
) {
  memset(cipher_text_buffer, 0, plain_text_size_B);

  size_t block_count = plain_text_size_B / BLOCK_SIZE_B;

	std::cout << "Liczba bloków do zakodowania: " << block_count << '\n';

	std::unique_ptr<std::bitset<KEY_SIZE_b>[]> key_set = generate_keys(key);

	std::cout << "key set:\n";
	for (int i = 0; i < N_ROUNDS; ++i) {
		std::cout << key_set[i] << '\n';
	}


	// std::bitset<HALF_BLOCK_SIZE_b> block_l;
	std::bitset<HALF_BLOCK_SIZE_b> block_r;
	std::bitset<HALF_BLOCK_SIZE_b> new_block_r;
	uint32_t block_value;

  for (size_t block_i = 0; block_i < block_count; ++block_i) {
    // dzielę blok na pół
		print_text(plain_text + block_i * BLOCK_SIZE_B, BLOCK_SIZE_B);
		memcpy(&block_value, plain_text + block_i * BLOCK_SIZE_B, HALF_BLOCK_SIZE_B);
		std::bitset<HALF_BLOCK_SIZE_b> block_l(block_value);
		memcpy(&block_value, plain_text + block_i * BLOCK_SIZE_B + HALF_BLOCK_SIZE_B, HALF_BLOCK_SIZE_B);
		std::bitset<HALF_BLOCK_SIZE_b> block_r(block_value);

		std::cout << "L_i: ";
		std::cout << block_l;
		std::cout << "   R_i: ";
		std::cout << block_r << '\n';

    for (size_t round_i = 0; round_i < N_ROUNDS; ++round_i) {
			std::bitset<HALF_BLOCK_SIZE_b> new_block_r = block_l ^ nonlinear_f(key_set[round_i], block_r);
			block_l = block_r;
			block_r = new_block_r;
    }
    // skopiowanie do wyniku
		block_value = block_l.to_ulong();
    memcpy(
      cipher_text_buffer + block_i * BLOCK_SIZE_B,
      &block_value,
      HALF_BLOCK_SIZE_B
    );
		block_value = block_r.to_ulong();
    memcpy(
      cipher_text_buffer + block_i * BLOCK_SIZE_B + HALF_BLOCK_SIZE_B,
      &block_value,
      HALF_BLOCK_SIZE_B
    );
  }
}

void decode(
  const byte_t * const cipher_text,
  size_t cipher_text_size_B,
  byte_t * const plain_text_buffer,
  std::bitset<KEY_SIZE_b>& key
) {
  memset(plain_text_buffer, 0, cipher_text_size_B);

  size_t block_count = cipher_text_size_B / BLOCK_SIZE_B;

	std::cout << "Liczba bloków do zakodowania: " << block_count << '\n';

	std::unique_ptr<std::bitset<KEY_SIZE_b>[]> key_set = generate_keys(key);

	std::cout << "key set:\n";
	for (int i = 0; i < N_ROUNDS; ++i) {
		std::cout << key_set[i] << '\n';
	}


	// std::bitset<HALF_BLOCK_SIZE_b> block_l;
	std::bitset<HALF_BLOCK_SIZE_b> block_r;
	std::bitset<HALF_BLOCK_SIZE_b> new_block_r;
	uint32_t block_value;

  for (size_t block_i = 0; block_i < block_count; ++block_i) {
    // dzielę blok na pół
		memcpy(&block_value, cipher_text + block_i * BLOCK_SIZE_B, HALF_BLOCK_SIZE_B);
		std::bitset<HALF_BLOCK_SIZE_b> block_l(block_value);
		memcpy(&block_value, cipher_text + block_i * BLOCK_SIZE_B + HALF_BLOCK_SIZE_B, HALF_BLOCK_SIZE_B);
		std::bitset<HALF_BLOCK_SIZE_b> block_r(block_value);

		std::cout << "L_i: ";
		std::cout << block_l;
		std::cout << "   R_i: ";
		std::cout << block_r << '\n';

    for (size_t round_i = 0; round_i < N_ROUNDS; ++round_i) {
			std::bitset<HALF_BLOCK_SIZE_b> new_block_r = block_l ^ nonlinear_f(key_set[N_ROUNDS - round_i - 1], block_r);
			block_l = block_r;
			block_r = new_block_r;
    }
    // skopiowanie do wyniku
		block_value = block_l.to_ulong();
    memcpy(
      plain_text_buffer + block_i * BLOCK_SIZE_B,
      &block_value,
      HALF_BLOCK_SIZE_B
    );
		block_value = block_r.to_ulong();
    memcpy(
      plain_text_buffer + block_i * BLOCK_SIZE_B + HALF_BLOCK_SIZE_B,
      &block_value,
      HALF_BLOCK_SIZE_B
    );
  }
}

int main(int argc, char * argv[]) {
  constexpr int text_size_B = 64;

  srand(time(NULL));

	std::bitset<KEY_SIZE_b> key (
		std::string("11010101101000111101100010010100110011111111111100110101010100011")
	);



	std::cout << "Klucz wyjściowy: ";
	std::cout << key << '\n' << key.size() << '\n';


  byte_t * const plain_text = new byte_t[text_size_B];
  byte_t * const cipher_text = new byte_t[text_size_B];
  byte_t * const decoded_text = new byte_t[text_size_B];

	memset(cipher_text, 0, text_size_B);
	memset(decoded_text, 0, text_size_B);

  for (int i = 0; i < text_size_B; ++i) {
    plain_text[i] = rand() % ('z' - 'a') + 'a';
  }

  std::cout << "Plaintext:\n";
  print_text(plain_text, text_size_B);

  encode(plain_text, text_size_B, cipher_text, key);

  std::cout << "Ciphertext:\n";
  print_text(cipher_text, text_size_B);

  decode(cipher_text, text_size_B, decoded_text, key);

  std::cout << "Decoded:\n";
  print_text(decoded_text, text_size_B);

  delete[] plain_text;
  delete[] cipher_text;
  delete[] decoded_text;
  return 0;
}
