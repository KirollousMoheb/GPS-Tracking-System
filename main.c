#include "stdint.h"
#include "tm4c123gh6pm.h"
void SystemInit(){};


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




int main(void)
{			 	
	                         InitPortE();
			while(1)
				 {				
				 }
		
}
