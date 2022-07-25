
void getCharHex(uint8_t B, char * M, char * L) {
  *M = ((B >> 4) & 0x0f) + '0';
  *L = (B & 0x0f) + '0';
}
uint8_t getHexFromChars(unsigned char M, unsigned char L) {
  return (uint8_t)(((M - '0')<<4) | (L - '0'));
}
