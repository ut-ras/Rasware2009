

// Definitions of corners
#define FLAG 0
#define FAN 1 //couldn't remember name of this one
#define ELECTRIC 2
#define LIGHT 3
#define TREE -1


void travelInit(void);

//calculates PD based on constants given
signed char PID(unsigned char,unsigned char,signed short,signed short);

//corner to goto, and boolean value for turning backwards to use fan
void stop(void);
void goForward(void);
void goBackward(void);
void goEngageCorner(signed char);
void goWall(void);
void goRotate(signed char);
void goAlignWall(char,char);
void gotoDest(signed char,char);
void WallFollow(int mode, int time);
void testSensors(void);


