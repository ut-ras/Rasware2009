

// Definitions of corners
#define FLAG 0
#define FAN 1 //couldn't remember name of this one
#define ELECTRIC 2
#define LIGHT 3
#define TREE -1


void travelInit(void);

//corner to goto, and boolean value for turning backwards to use fan
void stop(void);
void goForward(void);
void goBackward(void);
void goCorner(void);
void goWall(void);
void goRotate(signed char);
void goAlignWall(char,char);
void gotoDest(signed char,char);

void testSensors(void);


