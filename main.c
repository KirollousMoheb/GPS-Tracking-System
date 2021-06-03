#include "stdint.h"
#include "tm4c123gh6pm.h"
unsigned char segment1,segment2,segment3;
unsigned char values[10] = {0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
int distance=179;//dumy number 
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
void InitPortE(){      // Function to initialize port E
	
	SYSCTL_RCGCGPIO_R|= 0x10;  
	GPIO_PORTE_LOCK_R=0x4C4F434B;
	GPIO_PORTE_CR_R = 0x0E;                
	GPIO_PORTE_AMSEL_R = 0x00;        
	GPIO_PORTE_PCTL_R = 0x00;   
	GPIO_PORTE_DIR_R = 0x0E;     //PE3==>third 7segments  , PE2==>Second 7segments, PE1==>first 7segments  
	GPIO_PORTE_PUR_R = 0x00;              
	GPIO_PORTE_DEN_R = 0x0E;  

}


//PORTB initialization
void InitPortB(){
				
	SYSCTL_RCGCGPIO_R |= 0x02;
        GPIO_PORTB_LOCK_R=0x4C4F434B; 	
	GPIO_PORTB_CR_R = 0x7F;                 
	GPIO_PORTB_AMSEL_R = 0x00;        
	GPIO_PORTB_PCTL_R = 0x00;    
	GPIO_PORTB_DIR_R = 0x7F;          			
	GPIO_PORTB_PUR_R = 0x00;                
	GPIO_PORTB_DEN_R = 0x7F;     

}







void RedLedOn(){//Function to turn the red led once the distance exceeded 100m
	SYSCTL_RCGCGPIO_R|= 0x20;
	GPIO_PORTF_LOCK_R=0x4C4F434B;
	GPIO_PORTF_CR_R |= 0x0E;                 
	GPIO_PORTF_AMSEL_R = 0x00;        
	GPIO_PORTF_PCTL_R = 0x00;    
	GPIO_PORTF_DIR_R |= 0x0E;          
	GPIO_PORTF_PUR_R = 0x00;                
	GPIO_PORTF_DEN_R |= 0x0E;
	GPIO_PORTF_DATA_R|= 0x02;
}

void SplitDistance(int distance){
	segment1 = distance%10;		//Copies value in distance, divides it by 10 and then keeps remainder
	distance /= 10;			//Dividing value in distance by 10 shifts it by one decimal
	segment2 = distance%10;
	distance /= 10;
	segment3 = distance%10;		//Most left 7 Segments
	distance /= 10;
    
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
	
	while(1)
		 {
		
		SplitDistance(distance);
		show(8,segment3);
		show(4,segment2);
		show(2,segment1);
		
		if(distance>100){
		    RedLedOn();
		 }
		   }
		
}
