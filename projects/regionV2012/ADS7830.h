#define ADS7830 (0x90)

extern unsigned char ADS7830_Values[8];

void ADS7830_Init( void );

void ADS7830_Read(unsigned char*,unsigned char );

void ADS7830_BurstRead(unsigned char* );

void I2CIntHandler(void);
