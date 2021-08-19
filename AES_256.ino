#include <string.h>
#include <stdio.h>
#include <hwcrypto/aes.h>


/*static inline int32_t _getCycleCount(void) {
int32_t ccount;
asm volatile("rsr %0,ccount":"=a" (ccount));
return ccount;
}*/

char plaintext[16384];
char encrypted[16384];

int encodetest()
{
uint8_t key[32];
uint8_t iv[16];

//If you have cryptographically random data in the start of your payload, you do not need
//an IV. If you start a plaintext payload, you will need an IV.
memset( iv, 0, sizeof( iv ) );

//I am using a key of all zeroes. This should change. You should fill the key
//out with actual data. 
memset( key, 0, sizeof( key ) );

memset( plaintext, 0, sizeof( plaintext ) );
strcpy( plaintext, "Hello world" );

//you must be encrypting/decrypting data that is in BLOCKSIZE chunks!!!

esp_aes_context ctx;
esp_aes_init( &ctx );
esp_aes_setkey( &ctx, key, 256 );
//int32_t start = _getCycleCount();
esp_aes_crypt_cbc( &ctx, ESP_AES_ENCRYPT, sizeof(plaintext), iv, (uint8_t*)plaintext, (uint8_t*)encrypted );
//int32_t end = _getCycleCount();
//float enctime = (end-start)/240.0;
//Serial.printf( "Encryption time: %.2fus (%f MB/s)\n", enctime, (sizeof(plaintext)*1.0)/enctime );

//See encrypted payload, and wipe out plaintext.
memset( plaintext, 0, sizeof( plaintext ) );
int i;

Serial.println("Encrypted message:");  
for( i = 0; i < 128; i++ )
{
Serial.printf( "%c", encrypted[i] );
}
Serial.printf( "\n" );
//Must reset IV.
//XXX TODO: Research further: I found out if you don't reset the IV, the first block will fail
//but subsequent blocks will pass. 
//Serial.printf( "IV: %02x %02x\n", iv[0], iv[1] );
memset( iv, 0, sizeof( iv ) );

//Use the ESP32 to decrypt the CBC block.
esp_aes_crypt_cbc( &ctx, ESP_AES_DECRYPT, sizeof(encrypted), iv, (uint8_t*)encrypted, (uint8_t*)plaintext );

//Verify output

Serial.println("Decrypted message:");  
for( i = 0; i < 128; i++ )
{
Serial.printf( "%c", plaintext[i] );
}
Serial.printf( "\n" );

esp_aes_free( &ctx );
}
void setup() {
// put your setup code here, to run once:
Serial.begin(115200);
encodetest();
}

void loop() {
// put your main code here, to run repeatedly:

}
