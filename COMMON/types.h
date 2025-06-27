#ifndef __types_h
#define __types_h

#include <stdint.h>

typedef   signed           char INT8;
typedef   signed short     int  INT16;
typedef   signed           int  INT32;

/* exact-width unsigned integer types */
typedef uint8_t  UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
/* exact-width unsigned integer types */
typedef unsigned           char UINT8;
typedef unsigned short     int  UINT16;
typedef unsigned           int  UINT32;

typedef unsigned char 			u8;
typedef unsigned short int		u16;
typedef unsigned int 			u32;


typedef unsigned char 			uint8;
typedef unsigned short int		uint16;
typedef unsigned int 			uint32;
typedef signed char 			int8;
typedef signed short int		int16;
typedef signed int				int32;

typedef unsigned           char BYTE;
typedef unsigned short     int  WORD;
typedef unsigned           int  DWORD;
typedef unsigned           char * PBYTE;
typedef unsigned short     int  * PWORD;
typedef unsigned           int  * PDWORD;

typedef void (*driver_callback_t)(int rx, UINT8 * const in_data, UINT8 * out_data, int * tx, UINT32 * flags, int capacity, void * user_data);

typedef int (*interruption_callback_t)(void * user_data);

typedef UINT8 sign_flag_t;
#define FLAG_HARDWARE                        0x01
#define FLAG_CHECK_CANONICAL                 0x01 << 1
#define FLAG_CHECK_CANONICAL_STRICTLY        0x01 << 2

typedef enum {
    SECP256K1 = 0,
    SECP256R1,
    ED25519,
    ED25519_CARDANO
} ecc_curve_t;

typedef UINT8 hash_flag_t;
#define FLAG_HASH_INIT    0x01
#define FLAG_HASH_UPDATE  0x02
#define FLAG_HASH_FINAL   0x04
#define FLAG_HASH_RESET   0x08
#define FLAG_HASH_RAW     0x10

typedef enum {
    SHA1,
    SHA2 = 1,
    SHA2D,
    SHA2_512,
    SHA2_RIPEMD,
    SHA3,  // represent SHA3_256
    SHA3_KECCAK,
    SHA3_224,
    SHA3_384,
    SHA3_512,
    BLAKE,
    BLAKED,
    BLAKE_RIPEMD,
    GROESTLD_TRUNC,
    BLAKE2B,
    BLAKE2B_PERSONAL,
} hash_type_t;

typedef enum {
    EN = 1,
    ZH_HANS,
    MAX_LANGUAGE_INDEX
} language_t;

typedef enum {
    IS_WALLET_INIT = 1,
    WALLET_INIT,
    RESET_DEVICE,
} device_type_t;

typedef enum {
    GET_MNEMONIC = 1,
    CHECK_MNEMONIC_GET_SEED,
    SET_SEED,
} seed_type_t;

typedef enum {
    AUTH_TYPE_PIN = 1,
    AUTH_TYPE_TOUCH
} auth_type_t;

typedef enum {
    VERIFY = 0,
    AUTHENICATION
} auth_flag_t;

typedef enum {
    PIN_TYPE_SET = 0,
    PIN_TYPE_RESET
} pin_type_t;

typedef enum {
    FP_TYPE_CAPTURE = 0,
    FP_TYPE_REGISTER,
    FP_TYPE_UNREGISTER,
    MP_TEST_REGISTER,
    MP_TEST_VERIFY
} fp_type_t;



#define TRUE  1
#define FALSE 0
#define true  1
#define false 0


typedef unsigned char 	BOOL;
#define bool BOOL


#endif 




