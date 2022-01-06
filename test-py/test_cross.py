import main
import unittest


class CrossTest(unittest.TestCase):
    def test_encode_decode_match(self):
        message = '01234567'

        key = "1110101101001010" * 4
        encoded_message = main.encode(message, key)

        key = "1110101101001010" * 4
        decoded_message = main.decode(encoded_message, key)

        self.assertEqual(message, decoded_message, 'Decoded message differ from input message')
