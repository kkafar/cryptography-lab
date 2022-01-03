import unittest
import main


class CipherFunctionsTest(unittest.TestCase):
    def test_permute_bits(self):
        simple_permutation = (
            4, 0, 7, 3, 6, 2, 1, 5
        )
        bits = '10101010'
        expected = '01001011'

        assert len(simple_permutation) == len(bits), "Invalid test format. " \
                                                     "Permutation table length " \
                                                     "must me the same as length " \
                                                     "of bitset."

        permuted_bits = main.permute_bits(bits, simple_permutation)
        self.assertEqual(permuted_bits, expected)

    def test_expand_bits(self):
        simple_extension_func = (
            6, 2, 7, 1, 0, 4, 5, 0, 3, 1, 7, 4
        )
        bits = '10101010'
        expected = '110011010001'

        expanded_bits = main.expand_bits(bits, simple_extension_func)
        self.assertEqual(len(expanded_bits), len(expected))
        self.assertEqual(expanded_bits, expected)

    def test_apply_sbox(self):
        # 64 bit input
        sbox_input = '1100101100100110' * 4
        expected = '00101010100110000011110011111010'
        sbox_output = main.apply_sbox(sbox_input)

        self.assertEqual(sbox_output, expected)

    def test_nonlinear_f(self):
        # 32 bit input
        bit_input = '1100101100100110' * 2
        # 64 bit key
        key = '1011011100011010' * 4

        expected_bit_input_expanded = '0011011000101100010011011100101110010011011010110100110100101001'
        bit_input_expanded = main.expand_bits(bit_input, main.block_extension_map)

        self.assertEqual(bit_input_expanded, expected_bit_input_expanded)

        expected_key_xor_bits = '1000000100110110111110101101000100100100011100011111101000110011'
        key_xor_bits = main._xor_(bit_input_expanded, key)

        self.assertEqual(key_xor_bits, expected_key_xor_bits)















