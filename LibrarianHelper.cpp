//1215141 김자영
//1115125 차지은

#include "LibrarianHelper.h"

int direction_data1 = 0;
int S_DIN = 42, S_SCLK = 43, S_SYNCN = 44;
int IN_SEN_EN = 26;
int SensorA[8] = {A0,A1,A2,A3,A4,A5,A6,A7};
int SensorD[8] = {30,31,32,33,34,35,36,37};
int Motor[6] = {22,23,24,25,4,5};
unsigned int Buff_A[8] = {0,0,0,0,0,0,0,0};
unsigned int ADC_MAX[8] = {0,0,0,0,0,0,0,0};
unsigned int ADC_MIN[8] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
unsigned int ADC_MEAN[8] = {0,0,0,0,0,0,0,0};
unsigned char Sensor_data = 0;
int DAC_data = 0;

int Sensor_data2;	//compare with Sensor_data1
int Sensor_data1;	//accept the line data

bool b = false;	//make do LineTrace
int want = 0;	//point user want
int i = 0;		//counting points which user passed
int go;			//calculate distance from here to there
int a = 16; 	//compare with 'go'
int now = 0;	//where user is
int turn = 0;	//how many it turns


void setup()
{

	int z;
	pinMode(IN_SEN_EN, OUTPUT);
	pinMode(S_DIN, OUTPUT);
	pinMode(S_SYNCN, OUTPUT);
	pinMode(S_SCLK, OUTPUT);
	digitalWrite(S_SCLK, LOW);
	digitalWrite(S_SYNCN, HIGH);
	digitalWrite(IN_SEN_EN, HIGH);
	for(z=0; z<6; z++){
		pinMode(Motor[z],OUTPUT);
		digitalWrite(Motor[z],LOW);
	}
	for(z=0; z<8; z++){
		pinMode(SensorD[z],INPUT);
	}
	Serial.begin(115200);
	DAC_setting(0x9000);
	for(z=0; z<8; z++){
		DAC_CH_Write(z,255);
	}
	infrared_init();	//sense the line
}

void loop(){

	if(Serial.available()>0){		//get some data from the phone
		char command = Serial.read();

		Motor_Control('A');
		switch(command){
		case 'a':	//go to the first point
			want = 1;
			b = true;
			break;
		case 'b':	//go to the second point
			want = 2;
			b = true;
			break;
		case 'c':	//go to the third point
			want = 3;
			b = true;
			break;
		case 'd':
			want = 4;
			b = true;
			break;
		case 'e':
			want = 5;
			b = true;
			break;
		case 'f':
			want = 6;
			b = true;
			break;
		case 'g':
			want = 7;
			b = true;
			break;
		case 'h':
			want = 8;
			b = true;
			break;
		case 'i':
			want = 9;
			b = true;
			break;
		case 'j':
			want = 10;
			b = true;
			break;
		case 'k':
			want = 11;
			b = true;
			break;
		case 'l':
			want = 12;
			b = true;
			break;
		case 'm':
			want = 13;
			b = true;
			break;
		case 'n':
			want = 14;
			b = true;
			break;
		case 'o':
			want = 15;
			b = true;
			break;
		case 'p':
			want = 16;
			b = true;
			break;
		case 'q':
			want = 17;
			b = true;
			break;
		case 'r':
			want = 18;
			b = true;
			break;
		case 's':
			want = 19;
			b = true;
			break;
		case 't':
			want = 20;
			b = true;
			break;
		case 'u':
			want = 21;
			b = true;
			break;
		case 'v':
			want = 22;
			b = true;
			break;
		case 'w':
			want = 23;
			b = true;
			break;
		case 'x':
			want = 24;
			b = true;
			break;
		case 'y':
			want = 25;
			b = true;
			break;
		case 'z':
			want = 26;
			b = true;
			break;
		case 'A':
			want = 27;
			b = true;
			break;
		case 'B':
			want = 28;
			b = true;
			break;
		case 'C':
			want = 29;
			b = true;
			break;
		case 'D':
			want = 30;
			b = true;
			break;
		case 'Z':		//go to start point
			want = 0;
			b = true;
			break;
		default:
			Motor_mode(STOP);
			delay(5000);
			break;
		}
	}
	go = want - now;		//calculate distance from here to there
	if(go > a){				//if 'go' is bigger than 'a'(16)
		if(turn%2 == 0){	//the number of turns is even	direction is forward
			Motor_mode(RIGHT_U);	//Turn halfway
			delay(800);
			turn++;
			go = 31 + now - want;	//calculate distance it should go		//this way is much efficient
			LineTrace();
		}
		else{				//the number of turns is odd	direction is backward
			go = 31 + now - want;	//calculate distance it should go		//this way is much efficient
			LineTrace();
		}
	}
	else if(go > 0){		//if 'go' is bigger than 0
		if(turn%2 == 0){
			LineTrace();
		}
		else{
			Motor_mode(RIGHT_U);	//Turn halfway
			delay(800);
			turn++;
			LineTrace();		//just do what you do
		}

	}
	else if(go == 0){		//if 'go' is 0
		Motor_mode(STOP);	//stop
		delay(5000);
	}
	else if(go < (-1*a)){	//if 'go' is smaller than '-1 * a'
		if(turn%2 == 0){
			go = 31 + go;		//calculate the distance
			LineTrace();
		}
		else{
			Motor_mode(RIGHT_U);	//Turn halfway
			delay(800);
			turn++;
			go = 31 + go;		//calculate the distance
			LineTrace();		//just do what you do
		}

	}
	else {					//if 'go' is smaller than 0
		if(turn%2 == 0){
			Motor_mode(RIGHT_U);	//Turn halfway
			delay(800);
			turn++;
			go = go*(-1);	//calculate distance it should go		//this way is much efficient
			LineTrace();
		}
		else{
			go = go*(-1);	//calculate distance it should go		//this way is much efficient
			LineTrace();
		}
	}
	now = want;				//current site is updated
	i = 0;
}

void LineTrace()	//function of line trace
{
	while(b){
		int Speed_data_R = 0, Speed_data_L = 0, direction_data = 0;


		Sensor_data1 = SensorD_read();	//get data of line
		switch(0xFF^Sensor_data1){
		case 0x18:
		case 0x10:
		case 0x08:
		case 0x38:
		case 0x1C:
		case 0x3C:
			direction_data = FORWARD;
			Speed_data_L = 140;
			Speed_data_R = 140;
			break;
		case 0x0C:
		case 0x04:
		case 0x06:
		case 0x0E:
		case 0x1E:
		case 0x0F:
			direction_data = RIGHT;
			Speed_data_L = 200;
			Speed_data_R = 200;
			break;
		case 0x30:
		case 0x20:
		case 0x60:
		case 0x70:
		case 0x78:
		case 0xF0:
			direction_data = LEFT;
			Speed_data_L = 200;
			Speed_data_R = 200;
			break;
		case 0x07:
		case 0x03:
		case 0x02:
		case 0X01:
			direction_data = RIGHT_U;
			Speed_data_L = 200;
			Speed_data_R = 80;
			break;
		case 0xC0:
		case 0x40:
		case 0x80:
		case 0xE0:
			direction_data = LEFT_U;
			Speed_data_L = 80;
			Speed_data_R = 200;
			break;
		case 0:									//if there is no line
			if(Sensor_data1 != Sensor_data2)	//compare one data with just before data and if they are different
				i++;							//count number
			else{								//if they are same
				direction_data = FORWARD;		//just go forward
				Speed_data_L = 200;
				Speed_data_R = 200;
			}
			break;
		}
		Sensor_data2 = Sensor_data1;			//update the 'Sensor_data2' to 'Sensor_data1'

		if(direction_data1 != direction_data){	//if current direction differs with right before direction
			direction_data1 = direction_data;	//update direction
			Motor_Control('L',Speed_data_L);
			Motor_Control('R',Speed_data_R);
			Motor_mode(direction_data);			//go forward with that direction
		}
		if(i== go){								//if user is in right point
			Motor_mode(STOP);					//stop
			delay(5000);
			b = false;
		}
		delay(5);
	}
}

void ADC_Compare(){
	int z;
	for(z=0; z<8; z++){
		if(ADC_MAX[z]<Buff_A[z])
			ADC_MAX[z] = Buff_A[z];
		if(ADC_MIN[z]>Buff_A[z])
			ADC_MIN[z] = Buff_A[z];
	}
}

void SensorA_read(){
	int z;
	for(z=0; z<8; z++)
		Buff_A[z] = analogRead(SensorA[z]);
}

void DAC_CH_Write(unsigned int ch, unsigned int da){
	unsigned int data = ((ch << 12)&0x7000)|((da<<4)&0x0FF0);
	DAC_setting(data);
}

void DAC_setting(unsigned int data){
	int z;
	digitalWrite(S_SCLK, HIGH);
	delayMicroseconds(1);
	digitalWrite(S_SCLK, LOW);
	delayMicroseconds(1);
	digitalWrite(S_SYNCN, LOW);
	delayMicroseconds(1);
	for(z=16; z>0; z--){
		digitalWrite(S_DIN, (data>>(z-1))&0x1);
		digitalWrite(S_SCLK, HIGH);
		delayMicroseconds(1);
		digitalWrite(S_SCLK, LOW);
		delayMicroseconds(1);
	}
	digitalWrite(S_SYNCN, HIGH);
}

void infrared_init(){		//sense line
	int z, error = 0;
	Motor_Control('A',140);
	while(1){
		Motor_mode(LEFT_U);
		for(z=0; z<50; z++){
			SensorA_read();
			ADC_Compare();
			delay(8);
		}
		Motor_mode(RIGHT_U);
		for(z=0; z<90; z++){
			SensorA_read();
			ADC_Compare();
			delay(8);
		}
		Motor_mode(LEFT_U);
		delay(450);
		Motor_mode(STOP);


		delay(1000);
		for(z=0; z<8; z++){
			if((ADC_MAX[z]-ADC_MIN[z])<200)
				error++;
		}
		if(error==0){
			break;
		}
		else{
			error=0;
			for(z=0; z<8; z++){
				ADC_MAX[z] = 0;
				ADC_MIN[z] = 1023;
			}
		}
	}
	for(z=0; z<8; z++){
		ADC_MEAN[z] = (ADC_MAX[z]+ADC_MIN[z])/2;
		DAC_CH_Write(z, ADC_MEAN[z]/4);
	}
}

void Motor_mode(int da){
	int z;
	for(z=0; z<4; z++)
		digitalWrite(Motor[z],(da>>z)&0x01);
}

void Motor_Control(char da, unsigned int OC_value){
	switch(da){
	case 'L':
		analogWrite(Motor[4],OC_value);
		break;
	case 'R':
		analogWrite(Motor[5],OC_value);
		break;
	case 'A':
		analogWrite(Motor[4],OC_value);
		analogWrite(Motor[5],OC_value);
		break;
	}
}

void Motor_Control(char da)
{
	switch(da)
	{
	case 'L':
		digitalWrite(Motor[4],HIGH);
		digitalWrite(Motor[5],LOW);
		break;
	case 'R':
		digitalWrite(Motor[4],LOW);
		digitalWrite(Motor[5],HIGH);
		break;
	case 'A':
		digitalWrite(Motor[4],HIGH);
		digitalWrite(Motor[5],HIGH);
		break;
	default:
		digitalWrite(Motor[4],LOW);
		digitalWrite(Motor[5],LOW);
		break;
	}
}
unsigned char SensorD_read(void){
	unsigned char data = 0,z;
	for(z=0; z<8; z++){
		data >>= 1;
		if(digitalRead(SensorD[z]))
			data |= 0x80;
	}
	return data;
}
