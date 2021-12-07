# Szyfr: 
# będzie operował na słowie 64 bitowym
# będzie wykorzystywał klucz 64 bitowy

# NOTE
# Możliwe, że do uruchomienia kodu wymagany jest python 10 (type unions)

Bitset = str

round_n = 16
block_s = 64 # in bits
key_s = 64 # in bits

block_extension_map = (
  27, 28, 29, 30, 31,  0,  1,  2,
   2,  3,  4,  5,  6,  7,  8,  9,
   9, 10, 11, 12, 13, 14, 15, 16,
  16, 17, 18, 19, 20, 21, 22, 23,
  23, 24, 25, 26, 27, 28, 29, 30,
  31,  0,  1,  3,  4,  5,  6,  7,
   8, 10, 11, 12, 13, 14, 15, 17,
  18, 19, 20, 21, 22, 24, 25, 26
)

sbox = (
	# sbox 0
  (
		(0, 3, 1, 2),
		(3, 2, 1, 0),
		(1, 2, 0, 3),
		(1, 2, 3, 0)
  ),
	# sbox 1
	(
		(0, 1, 3, 2),
		(1, 3, 0, 2),
		(2, 1, 0, 3),
		(3, 2, 0, 1)
	),
	# sbox 2
	(
		(2, 0, 3, 1),
		(2, 0, 1, 3),
		(1, 3, 2, 0),
		(3, 1, 2, 0)
	),
	# sbox 3
	(
		(3, 1, 0, 2),
		(3, 0, 2, 1),
		(2, 3, 1, 0),
		(2, 3, 0, 1)
	),
	# sbox 4
	(
		(2, 3, 1, 0),
		(2, 1, 0, 3),
		(0, 1, 2, 3),
		(0, 2, 3, 1)
	),
	# sbox 5
	(
		(0, 3, 2, 1),
		(0, 1, 2, 3),
		(0, 2, 1, 3),
		(2, 1, 0, 3)
	),
	# sbox 6
	(
		(3, 2, 1, 0),
		(2, 1, 0, 3),
		(1, 2, 0, 3),
		(1, 3, 0, 2)
	),
	# sbox 7
	(
		(0, 3, 1, 2),
		(1, 2, 0, 3),
		(2, 3, 1, 0),
		(3, 0, 1, 2)
	),
	# sbox 8
	(
		(2, 1, 0, 3),
		(2, 0, 1, 3),
		(1, 2, 0, 3),
		(3, 2, 0, 1)
	),
	# sbox 9
	(
		(0, 3, 1, 2),
		(2, 3, 0, 1),
		(2, 0, 1, 3),
		(1, 3, 2, 0)
	),
	# sbox 10
	(
		(3, 1, 2, 0),
		(3, 0, 2, 1),
		(1, 0, 2, 3),
		(0, 2, 1, 3)
	),
	# sbox 11
	(
		(0, 3, 1, 2),
		(2, 1, 0, 3),
		(1, 2, 3, 0),
		(0, 2, 3, 1)
	),
	# sbox 12
	(
		(3, 0, 2, 1),
		(0, 3, 1, 2),
		(1, 0, 3, 2),
		(1, 3, 0, 2)
	),
	# sbox 13
	(
		(1, 2, 3, 0),
		(2, 3, 0, 1),
		(2, 0, 1, 3),
		(0, 3, 1, 2)
	),
	# sbox 14
	(
		(1, 2, 0, 3),
		(2, 3, 1, 0),
		(3, 1, 0, 2),
		(0, 2, 3, 1)
	),
	# sbox 15
	(
		(1, 0, 3, 2),
		(3, 1, 2, 0),
		(3, 0, 1, 2),
		(0, 2, 3, 1)
	)
)


def _xor_(b1: Bitset, b2: Bitset) -> Bitset:
  assert len(b1) == len(b2), "Not equal lengths of bitsets in xor op."
  result = [' '] * len(b1)
  for i, (a, b) in enumerate(zip(b1, b2)):
    result[i] = '1' if a != b else '0'
  return ''.join(result)


def apply_permutation(bits: Bitset, perm_table) -> Bitset:
  permuted_bits = [None] * len(bits)
  for i, bit in enumerate(bits):
    permuted_bits[perm_table[i]] = bits[i]
  return permuted_bits


def split_in_half(bits: Bitset) -> tuple[Bitset, Bitset]:
  return bits[ : len(bits) // 2], bits[len(bits) // 2 : ]


def generate_key_schedule(key: Bitset, reversed = False):
  pass


def round_function(bits: Bitset, key: Bitset) -> Bitset:
  pass


def block_gen(message: Bitset) -> Bitset:
  blocks_total_n = len(message) // block_s
  block_i = 0
  while block_i < blocks_total_n:
    yield message[block_i * block_s : (block_i + 1) * block_s]


def expand_bits(bits: Bitset, expansion_map: tuple[int]) -> Bitset:
  output = [None] * len(expansion_map)
  for i, mapped_i in enumerate(expansion_map):
    output[i] = bits[mapped_i]
  return ''.join(output)


def nonlinear_f(bits: Bitset, key: Bitset) -> Bitset:
  assert len(bits) == block_s // 2, f"Invalid length of bit block: {len(bits)}. Expected: {block_s // 2}."

  output = [None] * len(bits)

  bits = expand_bits(bits, block_extension_map)


def to_bitset(numb: int, width = 8) -> Bitset:
  as_str = str(bin(numb)[2:])
  assert len(as_str) <= width, "Overflow"

  if len(as_str) < width:
    return '0' * (width - len(as_str)) + as_str
  else:
    return as_str

def int_from_bitset(bits: Bitset) -> int:
  return int(bits, 2)


def convert_message_to_bits(message: str, width_per_char: int = 8) -> Bitset:
  codes = []
  for letter in message:
    codes.append(to_bitset(ord(letter), width=width_per_char))

  return ''.join(codes)


def convert_message_from_bits(message: Bitset, width_per_char: int = 8) -> str:
  assert len(message) % width_per_char == 0, f"Message of len: {len(message)} can not be split into blocks of len: {width_per_char}."
  block_n = len(message) // width_per_char

  converted_message = []
  for i in range(block_n):
    converted_message.append(chr(int_from_bitset(message[i * width_per_char : (i + 1) * width_per_char])))
    
  return ''.join(converted_message)


def codec(message: Bitset, key: Bitset, decode: bool, width_per_char: int = 8) -> Bitset:
  message = convert_message_to_bits(message)
  key_schedule = generate_key_schedule(key, reversed=decode)
  assert len(key_schedule) == round_n,\
    f"Invalid number of keys in schedule. Expected {round_n}. Received: {len(key_schedule)}."

  encoded_message = [ ]
  encoded_block = [ ] 

  for message_block in block_gen(message):
    block_l, block_r = split_in_half(message_block)
    for i, round_key in enumerate(key_schedule):
      new_block_l = block_r
      new_block_r = _xor_(block_l, nonlinear_f(block_r, round_key))
      block_l, block_r = new_block_l, new_block_r
    
    encoded_message.append(block_l + block_r)
  
  return ''.join(encoded_message)
  


def encode(message: str, key: Bitset, width_per_char: int = 8) -> Bitset:
  return codec(message, key, False, width_per_char=width_per_char)


def decode(message: Bitset, key: Bitset, width_per_char: int = 8) -> str:
  return convert_message_from_bits(codec(message, key, True, width_per_char=width_per_char))




def test():
  message = "Ala ma k" * 8 # 64 znaki
  print("Wyjściowa wiadomość")
  print(message)

  key = "1110101101001010" * 8

  encoded_message = encode(message, key)

  print("Zakodowana wiadomość")
  print(encoded_message)

  decoded_message = decode(encoded_message, key)

  print("Zdekodowana wiadomość")
  print(decoded_message)

test()
