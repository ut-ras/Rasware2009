

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
void goCorner(char source);
void goWall(void);
void gotoDest(char,bool);

void testSensors(void);


