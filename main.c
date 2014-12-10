/*
 * Chris Kiernan
 * ECE 382 - Lab 8 "Robot Maze"
 * Date - 9 Dec 14
 * Purpose - to program the robot to maneuver through
 * 			the maze using IR sensors
 */
#include <msp430.h> 

#define	RGTfarThreshold	0x0140

void moveFWD(void);
void turnRGT(void);
void turnLFTtank(void);
void turnRGTsmall(void);
void turnLFTsmall(void);
void getRGTsensorRead(void);
void getCTRsensorRead(void);
void isClosetoCTRwall(void);
void isClosetoRGTwall(void);
void clearVar(void);
void stop(void);

unsigned short CTRsample[16];
unsigned short RGTsample[16];
unsigned char i=0;
unsigned char j=0;
unsigned char k=0;

int main(void) {

	//  Initialize my sensor values
	//--------------------------------



    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    // Initialize the motor pins
    //--------------------------------

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

    P2DIR |= BIT0;							// P2.0 is the enable pin for L Motor
    P2OUT |= BIT0;

    P2DIR |= BIT5;							// P2.5 is the enable pin for R Motor
    P2OUT |= BIT5;

    P2DIR |= BIT1;							// P2.1 is the directional pin for L motor
    P2OUT |= BIT1;							// Directional pins are used to give either a (+) or (-) net voltage across the motor - resulting in FWD or BCK

    P2DIR |= BIT3;							// P2.3 is the directional pin for R motor
    P2OUT &= ~BIT3;

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period

    TA1CCR1 = 0x0050;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode - admittedly unneeded with a duty cycle of 50%, but included to be thorough

    TA1CCR2 = 0x0050;
    TA1CCTL2 = OUTMOD_3;					// set TACCTL2 to Set / Reset mode

    while(1){
    	getRGTsensorRead();
    	isClosetoRGTwall();
    	getCTRsensorRead();
    	isClosetoCTRwall();
    	stop();
    	clearVar();
    }
	
}

void getRGTsensorRead(void){
	k=0;
	ADC10CTL0 = 0;											// Turn off ADC to make changes
	ADC10CTL1 = INCH_5 | ADC10DIV_3;						// Use channel A5, CLK/4
	ADC10AE0 = BIT5;										// Enable P1.5 as ADC input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

	ADC10CTL0 |= ADC10SC;									// ADC conversion
	while(ADC10CTL1 & ADC10BUSY);
	RGTsample[k] = ADC10MEM;
}

void isClosetoRGTwall(void){
	if(RGTsample[k]>0x200){
		turnLFTsmall();
	}else if(RGTsample[k]<0x0200 && RGTsample[k]>RGTfarThreshold){
		turnRGTsmall();
	}else if(RGTsample[k]<RGTfarThreshold){
		turnRGT();
	}
}

void getCTRsensorRead(void){
	j=0;
	ADC10CTL0 = 0;											// Turn off ADC to make changes
	ADC10CTL1 = INCH_4 | ADC10DIV_3 ;						// Use channel A4, CLK/4
	ADC10AE0 = BIT4;		 								// Enable P1.4 as input
	ADC10CTL0 = SREF_0 | ADC10SHT_3 | ADC10ON | ENC;		// Vcc & Vss as reference

	ADC10CTL0 |= ADC10SC;									// ADC Conversion
	while(ADC10CTL1 & ADC10BUSY);							// Wait for conversion to complete
	CTRsample[j] = ADC10MEM;								// Collect value from ADC
}

void isClosetoCTRwall(void){
	while(CTRsample[j]>0x0160){
		turnLFTtank();
		getCTRsensorRead();
	}
	moveFWD();
}

void moveFWD(void){
	int i=0;

	while(i<0x0100){
		P2OUT |= BIT0;						// Redundant motor enables
	    P2OUT |= BIT5;

	    P2OUT |= BIT1;						// Confirm correct motor direction
	    P2OUT &= ~BIT3;

	    TA1CCTL1 = OUTMOD_7;				// To be thorough - not needed for duty cylce of 50% but otherwise is helpful

	    TA1CCTL2 = OUTMOD_3;

	    i++;
	}
}

void turnLFTsmall(void){
	int i=0;

    while(i<0x0100){
    	P2OUT |= BIT5;						// Enable right motor
    	P2OUT &= ~BIT0;						// Disable left motor

    	P2OUT &= ~BIT3;						// Set direction for R motor to FWD

    	TA1CCTL2 = OUTMOD_3;

    	i++;
    }
}

void turnRGTsmall(void){
	int i=0;

    while(i<0x0100){

    	P2OUT &= ~BIT5;						// Disable right motor
    	P2OUT |= BIT0;						// Enable left motor

    	P2OUT |= BIT1;						// Set direction for L motor to FWD

    	TA1CCTL1 = OUTMOD_7;

    	i++;
    }
}

void turnLFTtank(void){
	int i=0;

    while(i<0x0100){
    	P2OUT |= BIT5;						// Enable right motor
    	P2OUT |= BIT0;						// Disable left motor

    	P2OUT &= ~BIT3;						// Set direction for R motor to FWD
    	P2OUT &= ~BIT1;

    	TA1CCTL2 = OUTMOD_3;

    	i++;
    }
}

void turnRGT(void){
	int i=0;								// Similar to the turnLFT() function, this is about 450 degrees of turn

    while(i<0x4FFF){
    	P2OUT &= ~BIT5;						// Disable right motor
    	P2OUT |= BIT0;						// Enable left motor

    	P2OUT |= BIT1;						// Confirm direction for L motor is FWD

    	TA1CCTL1 = OUTMOD_7;

    	i++;
    }
    i=0;
    getRGTsensorRead();
    if(RGTsample[k]<RGTfarThreshold){

		while(i<0x2000){
			P2OUT |= BIT0;						// Redundant motor enables
			P2OUT |= BIT5;

			P2OUT |= BIT1;						// Confirm correct motor direction
			P2OUT &= ~BIT3;

			TA1CCTL1 = OUTMOD_7;				// To be thorough - not needed for duty cylce of 50% but otherwise is helpful

			TA1CCTL2 = OUTMOD_3;

			i++;
		}
		i=0;
		while(i<0x3FFF){
			P2OUT &= ~BIT5;						// Disable right motor
			P2OUT |= BIT0;						// Enable left motor

			P2OUT |= BIT1;						// Confirm direction for L motor is FWD

			TA1CCTL1 = OUTMOD_7;

			i++;
		}
    }
}

void clearVar(void){
	i=0;
	j=0;
	k=0;
}

void stop(void){
	int i=0;

	while(i<0x0100){
		P2OUT &= ~BIT0;
		P2OUT &= ~BIT5;

		i++;
	}
}
