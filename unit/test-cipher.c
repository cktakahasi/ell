/*
 *
 *  Embedded Linux library
 *
 *  Copyright (C) 2015  Intel Corporation. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <assert.h>

#include <ell/ell.h>
#include <ell/cipher-private.h>

#define FIXED_STR  "The quick brown fox jumps over the lazy dog. " \
		   "Jackdaws love my big sphinx of quartz. "       \
		   "Pack my box with five dozen liquor jugs. "     \
		   "How razorback-jumping frogs can level six piqued gymnasts!"
#define FIXED_LEN  (strlen(FIXED_STR))

#define KEY_STR "This key has exactly _32_ bytes!"
#define KEY_LEN (strlen(KEY_STR))

static void test_unsupported(const void *data)
{
	struct l_cipher *cipher;

	cipher = l_cipher_new(42, KEY_STR, KEY_LEN);
	assert(!cipher);
}

static void test_aes(const void *data)
{
	struct l_cipher *cipher;
	char buf[256];
	int r;

	cipher = l_cipher_new(L_CIPHER_AES, KEY_STR, KEY_LEN);
	assert(cipher);

	memcpy(buf, FIXED_STR, FIXED_LEN);

	l_cipher_encrypt(cipher, buf, buf, FIXED_LEN);

	r = memcmp(buf, FIXED_STR, FIXED_LEN);
	assert(r);

	l_cipher_decrypt(cipher, buf, buf, FIXED_LEN);

	r = memcmp(buf, FIXED_STR, FIXED_LEN);
	assert(!r);

	l_cipher_free(cipher);
}

static void test_arc4(const void *data)
{
	struct l_cipher *cipher;
	char buf[256];
	int r;

	static const unsigned char expect_plaintext[] = {
		0xbb, 0xf3, 0x16, 0xe8, 0xd9, 0x40, 0xaf, 0x0a, 0xd3,
	};
	static const unsigned char expect_pedia[] = {
		0x10, 0x21, 0xbf, 0x04, 0x20,
	};
	static const unsigned char expect_attack[] = {
		0x45, 0xa0, 0x1f, 0x64, 0x5f, 0xc3, 0x5b, 0x38, 0x35, 0x52,
		0x54, 0x4b, 0x9b, 0xf5,
	};

	cipher = l_cipher_new(L_CIPHER_ARC4, "Key", 3);
	assert(cipher);
	l_cipher_encrypt(cipher, "Plaintext", buf, 9);
	assert(!memcmp(buf, expect_plaintext, 9));
	l_cipher_free(cipher);

	cipher = l_cipher_new(L_CIPHER_ARC4, "Wiki", 4);
	assert(cipher);
	l_cipher_encrypt(cipher, "pedia", buf, 5);
	assert(!memcmp(buf, expect_pedia, 5));
	l_cipher_free(cipher);

	cipher = l_cipher_new(L_CIPHER_ARC4, "Secret", 6);
	assert(cipher);
	l_cipher_encrypt(cipher, "Attack at dawn", buf, 14);
	assert(!memcmp(buf, expect_attack, 14));
	l_cipher_free(cipher);

	cipher = l_cipher_new(L_CIPHER_ARC4, KEY_STR, KEY_LEN);
	assert(cipher);

	memcpy(buf, FIXED_STR, FIXED_LEN);

	l_cipher_encrypt(cipher, buf, buf, FIXED_LEN);

	r = memcmp(buf, FIXED_STR, FIXED_LEN);
	assert(r);

	l_cipher_decrypt(cipher, buf, buf, FIXED_LEN);

	r = memcmp(buf, FIXED_STR, FIXED_LEN);
	assert(!r);

	l_cipher_free(cipher);
}

/*
 * openssl genrsa 1024 | openssl rsa -outform DER | xxd -i
 */
static uint8_t rsa_priv_key[] = {
	0x30, 0x82, 0x02, 0x5e, 0x02, 0x01, 0x00, 0x02, 0x81, 0x81, 0x00, 0xe1,
	0xec, 0x78, 0x3c, 0x5f, 0x62, 0x74, 0x1e, 0x6d, 0x1d, 0x44, 0xac, 0x40,
	0xb3, 0xec, 0x01, 0x96, 0x01, 0x8a, 0xfe, 0xcf, 0x5d, 0xc5, 0xe6, 0x0c,
	0x36, 0x03, 0x2c, 0x4e, 0x84, 0x8f, 0x51, 0xf3, 0xc5, 0x32, 0x4f, 0xc4,
	0x73, 0x22, 0x92, 0x30, 0x7c, 0x75, 0xd7, 0x4b, 0xae, 0xc6, 0xd0, 0x59,
	0x6b, 0xd8, 0x46, 0x79, 0xbc, 0x6a, 0x6e, 0xde, 0x27, 0x11, 0x2f, 0xde,
	0x84, 0xe3, 0x64, 0x84, 0x07, 0x82, 0x83, 0xbf, 0x90, 0xf5, 0x80, 0x6f,
	0x63, 0x3a, 0xd1, 0x74, 0xd5, 0x6d, 0x2f, 0xde, 0xdc, 0xea, 0xab, 0xe5,
	0x20, 0x7d, 0x26, 0x3e, 0x20, 0x99, 0x97, 0x41, 0x47, 0x81, 0x04, 0x7e,
	0x53, 0x5c, 0xb2, 0xa9, 0xe0, 0x3d, 0x72, 0x37, 0x85, 0xcc, 0x5c, 0xda,
	0x04, 0x96, 0xfa, 0x02, 0xc2, 0x23, 0x8b, 0x20, 0x5d, 0xe1, 0x2a, 0x69,
	0xec, 0xcd, 0xce, 0x85, 0xc2, 0xf5, 0x49, 0x02, 0x03, 0x01, 0x00, 0x01,
	0x02, 0x81, 0x81, 0x00, 0xa5, 0x31, 0x72, 0xf9, 0x32, 0x05, 0x9b, 0x42,
	0x64, 0x26, 0x72, 0x80, 0x41, 0x0f, 0x4e, 0x12, 0x1a, 0xcd, 0x26, 0x05,
	0x0b, 0x3b, 0x55, 0xe8, 0xd0, 0x24, 0xee, 0x4d, 0x07, 0x5c, 0x86, 0x2f,
	0x36, 0x3f, 0x8a, 0x7a, 0x28, 0xfa, 0xc6, 0xdc, 0x7d, 0xf7, 0x83, 0x72,
	0xd9, 0x34, 0x02, 0xcb, 0x75, 0x97, 0x15, 0x9c, 0xf2, 0x86, 0x82, 0x8c,
	0x6e, 0x83, 0xc2, 0x5d, 0x6e, 0x27, 0x5c, 0xdc, 0x52, 0xb8, 0x8d, 0xa8,
	0x0d, 0x09, 0xcf, 0x69, 0xae, 0x61, 0x0e, 0xcb, 0x6a, 0x76, 0xac, 0xdd,
	0x85, 0xda, 0x9c, 0xac, 0x2b, 0xf0, 0xf6, 0x2e, 0x2e, 0x4d, 0x9b, 0xc7,
	0x67, 0xc2, 0xfa, 0x7b, 0x0e, 0x68, 0xf7, 0x1e, 0x03, 0x28, 0xea, 0x0e,
	0x9a, 0xd6, 0xc3, 0x28, 0x3d, 0xde, 0x11, 0x26, 0xb1, 0x95, 0xf6, 0x10,
	0x2f, 0x81, 0xa5, 0x60, 0x2c, 0x4f, 0x37, 0x5c, 0x2a, 0xd2, 0x30, 0x01,
	0x02, 0x41, 0x00, 0xf2, 0x80, 0xa2, 0x57, 0x5c, 0xe0, 0x41, 0x82, 0x00,
	0xac, 0x0b, 0xbd, 0xad, 0x98, 0x04, 0x33, 0x49, 0x64, 0x0b, 0x94, 0x94,
	0xc3, 0xd7, 0xd9, 0xfe, 0x1f, 0xa3, 0xd1, 0x83, 0x42, 0x3a, 0x2d, 0xaf,
	0xc5, 0x4c, 0xa4, 0x1b, 0xe4, 0x1c, 0x9c, 0x17, 0x8e, 0x28, 0xe9, 0xa5,
	0xd4, 0xbd, 0x9a, 0xce, 0x6e, 0x33, 0xb4, 0xaf, 0xce, 0x13, 0xd2, 0xab,
	0x0c, 0x4b, 0x34, 0x0d, 0x03, 0x87, 0xa1, 0x02, 0x41, 0x00, 0xee, 0x7f,
	0x9b, 0xb4, 0x3c, 0x21, 0x76, 0xf2, 0x0c, 0xdf, 0xb6, 0xea, 0xc9, 0x31,
	0xd4, 0xeb, 0x8f, 0x46, 0x41, 0x9b, 0xc1, 0x60, 0x4f, 0x50, 0x54, 0x32,
	0xd2, 0xf4, 0xfd, 0xd0, 0xc8, 0x58, 0x6d, 0x17, 0x4e, 0xac, 0x5f, 0x9e,
	0xb7, 0xd4, 0xfc, 0xce, 0xe0, 0x92, 0x0e, 0x1d, 0xd1, 0xa7, 0x54, 0xd3,
	0x98, 0xca, 0x5b, 0x9c, 0x41, 0x68, 0xbf, 0x0d, 0x1b, 0xe2, 0xdb, 0xa6,
	0xec, 0xa9, 0x02, 0x40, 0x0b, 0xc1, 0x72, 0x9d, 0x3b, 0x92, 0x5f, 0x7a,
	0x96, 0xdf, 0xc0, 0x3d, 0xf4, 0xb1, 0x5e, 0xda, 0xc1, 0x9f, 0x08, 0xf4,
	0xad, 0xf5, 0x84, 0x7c, 0x3b, 0xd6, 0x7a, 0xd1, 0x88, 0x44, 0x68, 0x9f,
	0x98, 0x5a, 0xbf, 0x29, 0x61, 0x74, 0xc0, 0x72, 0x4c, 0xae, 0x06, 0x8b,
	0xb5, 0x0f, 0x48, 0x15, 0xbe, 0x16, 0x17, 0x89, 0x95, 0xd0, 0x2e, 0xa3,
	0xd2, 0xc8, 0xe8, 0xc8, 0x60, 0x2d, 0x20, 0xa1, 0x02, 0x41, 0x00, 0xdb,
	0x39, 0xbf, 0x14, 0xf8, 0x24, 0xc6, 0xa2, 0x0d, 0xc5, 0x61, 0xed, 0x05,
	0x0d, 0x62, 0x2b, 0x38, 0xe2, 0x9a, 0x92, 0x22, 0x39, 0x76, 0x0e, 0x5f,
	0xa6, 0xec, 0x14, 0xb8, 0x6e, 0x3e, 0x8a, 0x51, 0x94, 0x98, 0x03, 0x88,
	0x4d, 0x6b, 0xab, 0x42, 0xca, 0xa2, 0xd0, 0x7e, 0x5b, 0x58, 0x88, 0x98,
	0x47, 0x7b, 0xed, 0x9e, 0x31, 0xce, 0x4a, 0x0b, 0x3b, 0x70, 0x83, 0xa1,
	0xe6, 0x19, 0x29, 0x02, 0x41, 0x00, 0x9c, 0x88, 0xbb, 0x56, 0x6b, 0x4a,
	0x81, 0x2c, 0xb3, 0x70, 0xdc, 0xf5, 0x65, 0x45, 0xd4, 0xed, 0xdd, 0xc3,
	0xdc, 0xc5, 0x27, 0xa3, 0xa0, 0x66, 0x5c, 0x51, 0xeb, 0x52, 0x8c, 0x8d,
	0x4e, 0xa6, 0x8f, 0x42, 0x5d, 0xb8, 0xa4, 0xa4, 0x26, 0xf3, 0xd6, 0xe5,
	0x01, 0x6b, 0x51, 0x8a, 0xa4, 0xee, 0xec, 0xff, 0x71, 0x8c, 0xbb, 0xba,
	0x05, 0x3e, 0x55, 0x14, 0xd9, 0xe4, 0xa4, 0x7f, 0xb7, 0x4f
};

static void test_rsa(const void *data)
{
	struct l_asymmetric_cipher *cipher;
	char buf[128];

	cipher = l_asymmetric_cipher_new(L_CIPHER_RSA_PKCS1_V1_5,
						rsa_priv_key,
						sizeof(rsa_priv_key), false);
	assert(cipher);
	assert(l_asymmetric_cipher_encrypt(cipher, FIXED_STR, buf, 100, 128));

	assert(memcmp(FIXED_STR, buf, 100));

	assert(l_asymmetric_cipher_decrypt(cipher, buf, buf, 128, 128));

	assert(!memcmp(FIXED_STR, buf, 100));

	l_asymmetric_cipher_free(cipher);
}

int main(int argc, char *argv[])
{
	l_test_init(&argc, &argv);

	l_test_add("unsupported", test_unsupported, NULL);

	l_test_add("aes", test_aes, NULL);

	l_test_add("arc4", test_arc4, NULL);

	l_test_add("rsa", test_rsa, NULL);

	return l_test_run();
}
