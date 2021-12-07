def apply_permutation(bits, perm_table):
    permuted_bits = [None] * len(bits)
    for i, bit in enumerate(bits):
        permuted_bits[perm_table[i]] = bits[i]
    return permuted_bits


def split_in_half(bits):
    return bits[ : len(bits) // 2], bits[len(bits) // 2 : ]


def generate_key_schedule(reversed = False):
    pass

def round_function(bits, key_fragment):
    pass


def encode(message, key):
    pass


def decode(message, key):
    pass


