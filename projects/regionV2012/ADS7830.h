#define ADS7830 (0x90)

void ADS7830_Init( void );

void ADS7830_Read( char*, char );

void ADS7830_BurstRead( char* );
