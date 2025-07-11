/**
 * Copyright (c) 2013-2014 Tomas Dzetkulic
 * Copyright (c) 2013-2014 Pavol Rusnak
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdbool.h>
#include <string.h>

#include "bip39.h"
#include "bip39_english.h"
#include "hmac.h"
#include "memzero.h"
#include "options.h"
#include "pbkdf2.h"
#include "rand.h"
#include "sha2.h"

#if USE_BIP39_CACHE

static int bip39_cache_index = 0;

static CONFIDENTIAL struct {
  bool set;
  char mnemonic[256];
  char passphrase[64];
  uint8_t seed[512 / 8];
} bip39_cache[BIP39_CACHE_SIZE];

#endif

const char *mnemonic_generate(int strength) {
  if (strength % 32 || strength < 128 || strength > 256) {
    return 0;
  }
  uint8_t data[32];
  random_buffer(data, 32); // 使用随机数生成器生成一个长度为 32Bit 的随机序列
  const char *r = mnemonic_from_data(data, strength / 8); 
  memzero(data, sizeof(data));
  return r;
}

static CONFIDENTIAL char mnemo[24 * 10];

const char *mnemonic_from_data(const uint8_t *data, int len) {
  if (len % 4 || len < 16 || len > 32) {
    return 0;
  }

  uint8_t bits[32 + 1];

  sha256_Raw(data, len, bits); // 取其哈希值的前 len 位作为检验和连接于其后，就得到了一个长度为 33Bit 的序列
  // checksum
  bits[len] = bits[0];
  // data
  memcpy(bits, data, len);

  int mlen = len * 3 / 4;

  int i, j, idx;
  char *p = mnemo;
  for (i = 0; i < mlen; i++) {
    idx = 0;
    for (j = 0; j < 11; j++) {
      idx <<= 1;
      idx += (bits[(i * 11 + j) / 8] & (1 << (7 - ((i * 11 + j) % 8)))) > 0;
    }
    strcpy(p, wordlist[idx]);
    p += strlen(wordlist[idx]);
    *p = (i < mlen - 1) ? ' ' : 0;
    p++;
  }
  memzero(bits, sizeof(bits));

  return mnemo;
}

void mnemonic_clear(void) { memzero(mnemo, sizeof(mnemo)); }

int mnemonic_to_entropy(const char *mnemonic, uint8_t *entropy) {
  if (!mnemonic) {
    return 0;
  }

  uint32_t i = 0, n = 0;

  while (mnemonic[i]) {
    if (mnemonic[i] == ' ') {
      n++;
    }
    i++;
  }
  n++;

  // check number of words
  if (n != 12 && n != 18 && n != 24) {
    return 0;
  }

  char current_word[10];
  uint32_t j, k, ki, bi = 0;
  uint8_t bits[32 + 1];

  memzero(bits, sizeof(bits));
  i = 0;
  while (mnemonic[i]) {
    j = 0;
    while (mnemonic[i] != ' ' && mnemonic[i] != 0) {
      if (j >= sizeof(current_word) - 1) {
        return 0;
      }
      current_word[j] = mnemonic[i];
      i++;
      j++;
    }
    current_word[j] = 0;
    if (mnemonic[i] != 0) {
      i++;
    }
    k = 0;
    for (;;) {
      if (!wordlist[k]) {  // word not found
        return 0;
      }
      if (strcmp(current_word, wordlist[k]) == 0) {  // word found on index k
        for (ki = 0; ki < 11; ki++) {
          if (k & (1 << (10 - ki))) {
            bits[bi / 8] |= 1 << (7 - (bi % 8));
          }
          bi++;
        }
        break;
      }
      k++;
    }
  }
  if (bi != n * 11) {
    return 0;
  }
  memcpy(entropy, bits, sizeof(bits));
  return n * 11;
}

int mnemonic_check(const char *mnemonic) {
  uint8_t bits[32 + 1];
  int seed_len = mnemonic_to_entropy(mnemonic, bits);
  if (seed_len != (12 * 11) && seed_len != (18 * 11) && seed_len != (24 * 11)) {
    return 0;
  }
  int words = seed_len / 11;

  uint8_t checksum = bits[words * 4 / 3];
  sha256_Raw(bits, words * 4 / 3, bits);
  if (words == 12) {
    return (bits[0] & 0xF0) == (checksum & 0xF0);  // compare first 4 bits
  } else if (words == 18) {
    return (bits[0] & 0xFC) == (checksum & 0xFC);  // compare first 6 bits
  } else if (words == 24) {
    return bits[0] == checksum;  // compare 8 bits
  }
  return 0;
}

// passphrase must be at most 256 characters otherwise it would be truncated
void mnemonic_to_seed(const char *mnemonic, const char *passphrase, uint8_t seed[512 / 8], void (*progress_callback)(uint32_t current, uint32_t total)) {
  int mnemoniclen = strlen(mnemonic);
  int passphraselen = strlen(passphrase);
	if (passphraselen > 256) passphraselen  = 256;
#if USE_BIP39_CACHE
  // check cache
  if (mnemoniclen < 256 && passphraselen < 64) {
    for (int i = 0; i < BIP39_CACHE_SIZE; i++) {
      if (!bip39_cache[i].set) continue;
      if (strcmp(bip39_cache[i].mnemonic, mnemonic) != 0) continue;
      if (strcmp(bip39_cache[i].passphrase, passphrase) != 0) continue;
      // found the correct entry
      memcpy(seed, bip39_cache[i].seed, 512 / 8);
      return;
    }
  }
#endif
  uint8_t salt[8 + 256];
  memcpy(salt, "mnemonic", 8);
  memcpy(salt + 8, passphrase, passphraselen);
  static CONFIDENTIAL PBKDF2_HMAC_SHA512_CTX pctx;
  pbkdf2_hmac_sha512_Init(&pctx, (const uint8_t *)mnemonic, mnemoniclen, salt, passphraselen + 8, 1);
  if (progress_callback) {
    progress_callback(0, BIP39_PBKDF2_ROUNDS);
  }
  for (int i = 0; i < 16; i++) {
    pbkdf2_hmac_sha512_Update(&pctx, BIP39_PBKDF2_ROUNDS / 16);
    if (progress_callback) {
      progress_callback((i + 1) * BIP39_PBKDF2_ROUNDS / 16, BIP39_PBKDF2_ROUNDS);
    }
  }
  pbkdf2_hmac_sha512_Final(&pctx, seed);
  memzero(salt, sizeof(salt));
#if USE_BIP39_CACHE
  // store to cache
  if (mnemoniclen < 256 && passphraselen < 64) {
    bip39_cache[bip39_cache_index].set = true;
    strcpy(bip39_cache[bip39_cache_index].mnemonic, mnemonic);
    strcpy(bip39_cache[bip39_cache_index].passphrase, passphrase);
    memcpy(bip39_cache[bip39_cache_index].seed, seed, 512 / 8);
    bip39_cache_index = (bip39_cache_index + 1) % BIP39_CACHE_SIZE;
  }
#endif
}

const char *const *mnemonic_wordlist(void) { return wordlist; }
