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


def _and_mask_(bits: Bitset, mask: Bitset) -> Bitset:
  assert len(bits) == len(mask), "Not equal lengths of bitsets in and mask op."

  result = [ ]
  for b, m in zip(bits, mask):
    result.append('1' if (b == m and b == '1') else '0')
  return ''.join(result)


def _or_mask_(bits: Bitset, mask: Bitset) -> Bitset:
  assert len(bits) == len(mask), "Not equal lengths of bitsets in or mask op."

  result = [ ]
  for b, m in zip(bits, mask):
    result.append('1' if (b == '1' or m == '1') else '0')
  return ''.join(result)


def apply_permutation(bits: Bitset, perm_table) -> Bitset:
  permuted_bits = [None] * len(bits)
  for i, bit in enumerate(bits):
    permuted_bits[perm_table[i]] = bits[i]
  return permuted_bits


def split_in_half(bits: Bitset) -> tuple[Bitset, Bitset]:
  return bits[ : len(bits) // 2], bits[len(bits) // 2 : ]


def generate_key_schedule(key: Bitset, _reversed = False):
  key_schedule = [key for _ in range(round_n)]
  return key_schedule


def round_function(block_l: Bitset, block_r: Bitset, key: Bitset) -> Bitset:
  new_block_l = block_r
  new_block_r = _xor_(block_l, nonlinear_f(block_r, key))
  return new_block_l, new_block_r


def block_gen(message: Bitset, block_size) -> Bitset:
  assert len(message) % block_size == 0, f"Message of len: {len(message)} can not be split into blocks of len: {block_size}."
  blocks_total_n = len(message) // block_size
  block_i = 0
  while block_i < blocks_total_n:
    yield message[block_i * block_size : (block_i + 1) * block_size]
    block_i += 1


def expand_bits(bits: Bitset, expansion_map: tuple[int]) -> Bitset:
  output = [None] * len(expansion_map)
  for i, mapped_i in enumerate(expansion_map):
    output[i] = bits[mapped_i]
  # print('Expanded bit len', len(output))
  return ''.join(output)


def get_row(block: Bitset) -> int:
  row = _and_mask_(block, '1010').replace('0', '')
  if len(row) == 0: return 0
  else:
    return int(row, 2)


def get_col(block: Bitset) -> int:
  col = _and_mask_(block, '0101').replace('0', '')
  if len(col) == 0: return 0
  else:
    return int(col, 2)


def apply_sbox(bits: Bitset) -> Bitset:
  output = [ ]
  for i, blk in enumerate(block_gen(bits, 8)):
    left, right = split_in_half(blk)
    # print(i, 2*i, 2*i+1, 'lrow', get_row(left), 'lcol', get_col(left), 'rrow', get_row(right), 'rcol', get_col(right))
    output.append(
      to_bitset(sbox[2 * i][get_row(left)][get_col(left)], 2) 
      +
      to_bitset(sbox[2 * i + 1][get_row(right)][get_col(right)], 2)
    )
  return ''.join(output)


def nonlinear_f(bits: Bitset, key: Bitset) -> Bitset:
  assert len(bits) == block_s // 2, f"Invalid length of bit block: {len(bits)}. Expected: {block_s // 2}."

  output = [None] * len(bits)
  bits = expand_bits(bits, block_extension_map)
  bits = _xor_(bits, key)

  return apply_sbox(bits)


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
  print('convert')
  assert len(message) % width_per_char == 0, f"Message of len: {len(message)} can not be split into blocks of len: {width_per_char}."

  converted_message = []
  print(message)
  # i = 0
  for block in block_gen(message, width_per_char):
    # print('iteration', i)
    # print(block)
    # print(int_from_bitset(block))
    # print(chr(int_from_bitset(block)))
    # print(chr(int_from_bitset(block)))
    converted_message.append(chr(int_from_bitset(block)))
    # i += 1
    
  return ''.join(converted_message)


def codec(message: Bitset, key: Bitset, decode: bool, width_per_char: int = 8) -> Bitset:
  if not decode:
    print('Encoding')
  else:
    print('Decoding')
  print(message)

  key_schedule = generate_key_schedule(key, _reversed=decode)
  assert len(key_schedule) == round_n,\
    f"Invalid number of keys in schedule. Expected {round_n}. Received: {len(key_schedule)}."

  encoded_message = [ ]

  for message_block in block_gen(message, block_s):
    # print('Message block')
    # print(message_block)
    block_l, block_r = split_in_half(message_block)
    for i, round_key in enumerate(key_schedule):
      block_l, block_r = round_function(block_l, block_r, round_key)
    
    encoded_message.append(block_l + block_r)
  print('Return from codec')
  print(''.join(encoded_message))
  return ''.join(encoded_message)
  

def encode(message: str, key: Bitset, width_per_char: int = 8) -> Bitset:
  return codec(convert_message_to_bits(message), key, False, width_per_char=8)


def decode(message: Bitset, key: Bitset, width_per_char: int = 8) -> str:
  return convert_message_from_bits(codec(message, key, True, width_per_char=8))




def test():
  message = "01234567899876543210024681357901"
  print("Wyjściowa wiadomość", len(message))
  print(message)

  key = "1110101101001010" * 4

  encoded_message = encode(message, key)

  print("Zakodowana wiadomość", len(message))
  print(encoded_message)

  key = "1110101101001010" * 4
  decoded_message = decode(encoded_message, key)

  print("Zdekodowana wiadomość", len(message))
  print(decoded_message)

test()