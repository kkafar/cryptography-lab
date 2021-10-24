# lepszy Cezar
plaintext = "Zorro fly zealotry zipper"

Ciphertext = str
Plaintext = str
Alphabet = list[str] # it should containt space char: " "
unicode_shift_lowercase = 97
unicode_shift_uppercase = 65

latin_alphabet = "abcdefghijklmnopqrstuvwxyz"

class CaesarUpgradedEncoder(object):
    def __init__(self, key: int = 3, in_alphabet: Alphabet = latin_alphabet, out_alphabet: Alphabet = None) -> None:
        super().__init__()
        self.in_alphabet = in_alphabet
        self.out_alphabet = out_alphabet if out_alphabet is not None else in_alphabet
        self.key = key
        
    def __call__(self, plaintext: Plaintext):
        ciphertext = ["" for _ in range(len(plaintext))]
        for i, char in enumerate(plaintext.lower().strip()):
            ciphertext[i] = self.out_alphabet[(ord(char) - unicode_shift_lowercase + self.key) % len(self.out_alphabet)]
        return "".join(ciphertext)


class CaesarUpgradedDecoder(object):
    def __init__(self, key: int = 3, in_alphabet: Alphabet = latin_alphabet, out_alphabet: Alphabet = None) -> None:
        super().__init__()
        self.in_alphabet = in_alphabet
        self.out_alphabet = out_alphabet if out_alphabet is not None else in_alphabet
        self.key = key

    def __call__(self, ciphertext: Ciphertext):
        plaintext = ["" for _ in range(len(ciphertext))]
        for i, char in enumerate(ciphertext.lower().strip()):
            plaintext[i] = self.in_alphabet[(ord(char) - unicode_shift_lowercase - self.key) % len(self.in_alphabet)]
        return "".join(plaintext)


class Cipher(object):
    def __init__(
        self, 
        encoder = CaesarUpgradedEncoder(), 
        decoder = CaesarUpgradedDecoder(),
        in_alphabet: Alphabet = latin_alphabet,
        secret_alphabet: Alphabet = None, 
    ) -> None:
        self.in_alphabet = in_alphabet
        self.secret_alphabet = secret_alphabet if secret_alphabet is not None else in_alphabet
        self.encoder = encoder
        self.decoder = decoder
        self.encoder.in_alphabet = self.in_alphabet
        self.encoder.out_alphabet = self.secret_alphabet
        self.decoder.in_alphabet = self.secret_alphabet
        self.decoder.out_alphabet = self.in_alphabet

    def encode(self, plaintext: Plaintext) -> Ciphertext:
        return self.encoder(plaintext)
    
    def decode(self, ciphertext: Ciphertext) -> Plaintext:
        return self.decoder(ciphertext)
    


class CaesarUpgraded(object):


def main():
    print(ord("a"))


if __name__ == "__main__":
    main()
