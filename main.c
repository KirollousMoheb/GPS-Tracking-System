#include "stdint.h"
#include "tm4c123gh6pm.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define pi 3.14159265358979323846
unsigned char segment1,segment2,segment3;
unsigned char values[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
double distance=0;
char parsedValue[12][20],GPS[100],c0,*token;
double latitude1=0.0,longitude1=0.0,latitude2=0.0,longitude2=0.0;
double speed=0.0;
char flag=1;
int i=0;
void SystemInit(){};
int counter = 0;

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

double degToRad(double degree) {    // function to convert from degree to radian
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
//function to initialize UART2 [PD6,PD7]
void UART2_Init(){  
		SYSCTL_RCGCUART_R|=SYSCTL_RCGCUART_R2;   // clock enable UART2  =>   |=1
		SYSCTL_RCGCGPIO_R|=SYSCTL_RCGCGPIO_R3;  //enable clock to PORTD  =>   |=1
	        GPIO_PORTD_LOCK_R=0x4C4F434B; 	        
		GPIO_PORTD_CR_R = 0xC0;  
		UART2_CTL_R&=~UART_CTL_UARTEN;          //disable UART2
	
		UART2_FBRD_R =11;			// fraction part
		UART2_IBRD_R=104;			//    16MHz/16 = 1MHz,    1MHz/104 = 9600 baud rate
	
		UART2_LCRH_R =(UART_LCRH_WLEN_8 |UART_LCRH_FEN);          //(0x60|0x10)
		UART2_CTL_R|=(UART_CTL_UARTEN| UART_CTL_TXE  | UART_CTL_RXE );     // enable UART2, TXE, RXE  =>  |=0x301
	
                GPIO_PORTD_AFSEL_R=0XC0;                // use PD6,PD7 alternate function 
		GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R&~0xFF000000)|(GPIO_PCTL_PD6_U2RX |GPIO_PCTL_PD7_U2TX );
		GPIO_PORTD_DEN_R|=0XC0;                //make PD6  , PD7 as digital

}
char charIsAvailable(){  //function to check if the fifo has received any characters

	return ((UART2_FR_R &UART_FR_RXFE)==UART_FR_RXFE)?0:1;
}
char GetCharNonBlocking(){
	if(charIsAvailable()==1){
		return (char)(UART2_DR_R &0XFF);
	}else{
	   return (char)(-1);
	}
		
}

void parseGpsValue(){
		int i=0;
		token=strtok(GPS,",");
		while(token!=NULL){
			strcpy(parsedValue[i],token);
			 token = strtok(NULL, ",");
			i++;	
		}
		if(strcmp(parsedValue[1],"A")==0){
			
				latitude1=atof(parsedValue[2]);
				longitude1=atof(parsedValue[4]);
				speed=atof(parsedValue[6]);
			
			if(strcmp(parsedValue[3],"S")==0){
			
					latitude1=-atof(parsedValue[2]);
			}
			if(strcmp(parsedValue[5],"W")==0){
			
					longitude1=-atof(parsedValue[4]);
			}	
			
			
		}
}

void readGPS(){
	char c1,c2,c3,c4,c5,c6,c7;
	while(!charIsAvailable()){};
   c0=GetCharNonBlocking();
    if(c0=='$'){
        while(!charIsAvailable()){};
           c1=GetCharNonBlocking();
        if(c1=='G'){
            while(!charIsAvailable()){};
              c2=GetCharNonBlocking();
            if(c2=='P'){
                while(!charIsAvailable()){};
                  c3=GetCharNonBlocking();
                if(c3=='R'){
                    while(!charIsAvailable()){};
                      c4=GetCharNonBlocking();
                    if(c4=='M'){
                        while(!charIsAvailable()){};
                          c5=GetCharNonBlocking();
                        if(c5=='C'){
                            while(!charIsAvailable()){};
                              c6=GetCharNonBlocking();
                            if(c6==','){
                                while(!charIsAvailable()){};
                                  c7=GetCharNonBlocking();
				  strcpy(GPS, "");
				  i = 0;
       				  while(c7!='*'){
        			  GPS[i]=c7;
        			  while(!charIsAvailable());
        			  c7=GetCharNonBlocking();
        			  i++;
																																																																	
		}}}}}}}
   	 }
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
//is used for initialization of 7-segments
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
	GPIO_PORTF_DIR_R |= 0x0E;//set all the three built in leds as outputs          
	GPIO_PORTF_PUR_R = 0x00;                
	GPIO_PORTF_DEN_R |= 0x0E;//set all the three built in leds to be digitally enabled if we wanted to turn any of them on
	
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
//initializing Systick-timer of tiva c
void Sys_tick_Init(){
	NVIC_ST_RELOAD_R=80000; //timer counter expressed in ticks each tick-> 62.5ns so we need 5ms->80000 
	NVIC_ST_CTRL_R=7; // bit0, bit1, bit2  are all set to enable interrupt and to make source of clock is that of tiva and enable timer
	__enable_irq(); //enable intterupt
}


void SysTick_Handler(){
	if(counter==0){
		show(8,segment3);
		counter++;
	}
	if(counter==1){
		show(4,segment2);
		counter++;
	}
	if(counter==2){
		show(2,segment1);
		counter=0 ;
	}
						
}




int main(void)
{			
	 Sys_tick_Init();
	 InitPortE();
	 InitPortB();
	 InitPortF();
	 UART2_Init();
	while(1)
		 {
		
		readGPS();
		
		if(flag==1){
			latitude2=latitude1;
			longitude2=longitude1;						 
				 }
		                                               //distance+=calculateDistance( latitude1, longitude1, latitude2, longitude2)
		SplitDistance(distance);
		
		if(speed>0.8){
			distance+=calculateDistance(latitude1,longitude1,latitude2,longitude2);
				}

		if(!(GPIO_PORTF_DATA_R & 0x10)&&distance>=100){ //turn on the led if the push button on pf4 pressed and the distance more than 100
			while(1) { 
		GPIO_PORTF_DATA_R|= 0x02;                       //Turn on the red led if the distance exceeded 100 meters

		 }
		   }
		flag=0;
		latitude2=latitude1;
		longitude2=longitude1;
		
	}
}
