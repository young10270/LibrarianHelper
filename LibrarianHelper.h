// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef Week09_H_
#define Week09_H_
#include "Arduino.h"
//add your includes for the project Week09 here

#define FORWARD 0x09
#define BACKWARD 0x06
#define STOP 0x00
#define LEFT 0x08
#define RIGHT 0x01
#define LEFT_U 0x0A
#define RIGHT_U 0x05

//end of add your includes here
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();
#ifdef __cplusplus
} // extern "C"
#endif

//add your function definitions for the project Week09 here

void LineTrace();
void ADC_Compare();
unsigned char SensorD_read();
void SensorA_read();
void DAC_CH_Write(unsigned int ch, unsigned int da);
void DAC_setting(unsigned int data);
void infrared_init();
void Motor_mode(int da);
void Motor_Control(char da, unsigned int OC_value);
void Motor_Control(char da);
unsigned char SensorD_read(void);

//Do not add code below this line
#endif /* Week09_H_ */
