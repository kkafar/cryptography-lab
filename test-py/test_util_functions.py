from unittest import suite
import main
import unittest


class UtilTest(unittest.TestCase):
    def test_xor(self):
        bits1 = '1100'
        bits2 = '1010'

        self.assertEqual(
            main._xor_(bits1, bits2),
            '0110'
        )

    def test_xor_raises(self):
        bits1 = '1111'
        bits2 = '111100'

        self.assertRaises(
            AssertionError,
            main._xor_,
            bits1,
            bits2
        )

    def test_or(self):
        bits1 = '1100'
        bits2 = '1010'

        self.assertEqual(
            main._or_mask_(bits1, bits2),
            '1110'
        )

    def test_or_raises(self):
        bits1 = '1111'
        bits2 = '111100'

        self.assertRaises(
            AssertionError,
            main._or_mask_,
            bits1,
            bits2
        )

    def test_and(self):
        bits1 = '1100'
        bits2 = '1010'

        self.assertEqual(
            main._and_mask_(bits1, bits2),
            '1000'
        )

    def test_and_raises(self):
        bits1, bits2 = '1111', '111100'

        self.assertRaises(
            AssertionError,
            main._and_mask_,
            bits1,
            bits2
        )

    def test_split_in_half(self):
        bits = '1100'
        left, right = main.split_in_half(bits)
        self.assertEqual(left, '11')
        self.assertEqual(right, '00')

    def test_shift_left_circle(self):
        bits = '1101'
        expected = '0111'
        shifted = main.shift_left_circle(bits, 2)
        self.assertEqual(shifted, expected)

    def test_to_bitset(self):
        bits = main.to_bitset(ord('a'), width=8)
        self.assertEqual(len(bits), 8)
        self.assertEqual(bits, '01100001')

        bits = main.to_bitset(ord('z'), width=7)
        self.assertEqual(len(bits), 7)
        self.assertEqual(bits, '1111010')

    def test_int_from_bitset(self):
        bits = '110111'
        int_from_bits = main.int_from_bitset(bits)
        self.assertEqual(int_from_bits, 55)

    def test_convert_message_to_bits(self):
        message = 'abc'
        message_as_bits = '01100001' + '01100010' + '01100011'
        self.assertEqual(main.convert_message_to_bits(message), message_as_bits)

    def test_convert_message_from_bits(self):
        message = 'abc'
        message_as_bits = '01100001' + '01100010' + '01100011'
        self.assertEqual(message, main.convert_message_from_bits(message_as_bits))

    def test_conversion_invertible(self):
        messages = 'abacaba', '01234567', 'omegle', 'test case' 'UPPER TEST CASE'
        for message in messages:
            with self.subTest(message=message):
                self.assertEqual(
                    message,
                    main.convert_message_from_bits(main.convert_message_to_bits(message))
                )

    def test_block_gen(self):
        base_message = 'abac'
        message = base_message * 8
        assert len(message) % 5 != 0, "Invalid test configuration. " \
                                      "Make sure message length is not " \
                                      "divisible by 5."

        generator = main.block_gen(message, len(base_message))
        for block in generator:
            with self.subTest(block=block):
                self.assertEqual(block, base_message)

    def test_get_col(self):
        bitsets = ('1111', '0101', '1100', '1010', '0001')
        cols = (3, 3, 2, 0, 1)

        for bits, col in zip(bitsets, cols):
            with self.subTest(bits=bits, col=col):
                self.assertEqual(main.get_col(bits), col)

    def test_get_row(self):
        bitsets = ('1111', '0101', '1100', '1010', '0001')
        rows = (3, 0, 2, 3, 0)

        for bits, row in zip(bitsets, rows):
            with self.subTest(bits=bits, row=row):
                self.assertEqual(main.get_row(bits), row)
