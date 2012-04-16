#define ADS7830 (0x90)

extern unsigned char ADS7830_Values[8];

void ADS7830_Init( void );

void ADS7830_Read(void);

void I2CIntHandler(void);
