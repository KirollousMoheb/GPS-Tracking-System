#include "stdint.h"
#include "tm4c123gh6pm.h"
#include <math.h>
#define pi 3.14159265358979323846
unsigned char segment1,segment2,segment3;
unsigned char values[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
double distance=179;//dumy number
double latitude1=0.0,longitude1=0.0,latitude2=0.0,longitude2=0.0;
void SystemInit(){};


void delay(void){
	
	unsigned long volatile time;
	time = 727240*200/91000; 		 // 0.1 ms
	while(time){
	time--;
  }
}
double getInDegree(double value) {
    int degree = (int)value / 100;
    double minutes = value-(double)degree*100;
    return (degree+ (minutes/60));


}

double degToRad(double degree) {
    return degree * (pi / 180.0);
}


// function to calculate distance between two points using data given from the GPS
double calculateDistance(double latitude1,double longitude1,double latitude2,double longitude2) {
    double R = 6371000;       //earth's radius in meters
    double lat1 = degToRad(getInDegree(latitude1));
    double long1 = degToRad(getInDegree(longitude1));
    double lat2 = degToRad(getInDegree(latitude2));
    double long2 = degToRad(getInDegree(longitude2));
    double latdiff = lat2 - lat1;
    double longdiff = long2 - long1;

    double a= pow(sin(latdiff/2),2)+cos(lat1)*cos(lat2)*pow(sin(longdiff/2),2); // Haversine formula: a = sin²(Δφ/2) + cos φ1 ⋅ cos φ2 ⋅ sin²(Δλ/2)
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return R*c;   //in meters
}



void InitPortE(){      // Function to initialize port E
	
	SYSCTL_RCGCGPIO_R|= 0x10;            //enable clock for portE
	GPIO_PORTE_LOCK_R=0x4C4F434B;
	GPIO_PORTE_CR_R = 0x0E;                
	GPIO_PORTE_AMSEL_R = 0x00;          
	GPIO_PORTE_PCTL_R = 0x00;   
	GPIO_PORTE_DIR_R = 0x0E;     //PE3==>third 7segments  , PE2==>Second 7segments, PE1==>first 7segments   // make ports PE1,PE2,PE3 output ports
	GPIO_PORTE_PUR_R = 0x00;              
	GPIO_PORTE_DEN_R = 0x0E;        //make ports PE1,PE2,PE3 digital ports

}


//PORTB initialization
//this port is used for initialization of 7-segments
// 7-segments consists of 7 LEDs each led is connected to a certain pin to on or off LED
// in our system pins is connected from B0 to B6
void InitPortB(){
				
	SYSCTL_RCGCGPIO_R |= 0x02;         //enable clock for PORTB
        GPIO_PORTB_LOCK_R=0x4C4F434B; 	   
	GPIO_PORTB_CR_R = 0x7F;            //enable changes to pins from B0 to B6     
	GPIO_PORTB_AMSEL_R = 0x00;         //disable using pins as analog input
	GPIO_PORTB_PCTL_R = 0x00;          
	GPIO_PORTB_DIR_R = 0x7F;           //set pins from B0 to B6 of PORTB to be output			
	GPIO_PORTB_PUR_R = 0x00;                
	GPIO_PORTB_DEN_R = 0x7F;           //set pins from B0 to B6 to be digital 

}







void InitPortF(){//Function to turn the red led once the distance exceeded 100m
	SYSCTL_RCGCGPIO_R|= 0x20;
	GPIO_PORTF_LOCK_R=0x4C4F434B;
	GPIO_PORTF_CR_R |= 0x0E;                 
	GPIO_PORTF_AMSEL_R = 0x00;        
	GPIO_PORTF_PCTL_R = 0x00;    
	GPIO_PORTF_DIR_R |= 0x0E;          
	GPIO_PORTF_PUR_R = 0x00;                
	GPIO_PORTF_DEN_R |= 0x0E;
	
}

void SplitDistance(double distance){
	int d= (int)distance;
	
	segment1 = d % 10;		//Copies value in distance, divides it by 10 and then keeps remainder in segment 1 ( most righ 7 segment)
	d /= 10;			//Dividing value in distance by 10 shifts it by one decimal
	segment2 = d % 10;              
	d /= 10;
	segment3 = d % 10;		//Most left 7 Segment
	d /= 10;
    
}

void show(int digit, char value){
	
	GPIO_PORTB_DATA_R = 0x00;		// Turns off LEDs
	GPIO_PORTE_DATA_R = digit;		// Selects segment to turn on
	GPIO_PORTB_DATA_R = ~values[value];	// Turns on number in selected digit
	delay();	
	 } 

int main(void)
{			 	
	 InitPortE();
	 InitPortB();
	 InitPortF();	
	while(1)
		 {
		//distance+=calculateDistance( latitude1, longitude1, latitude2, longitude2)
		SplitDistance(distance);
		show(8,segment3);
		show(4,segment2);
		show(2,segment1);
		
		if(distance>100){
		    GPIO_PORTF_DATA_R|= 0x02;
		 }
		   }
		
}
