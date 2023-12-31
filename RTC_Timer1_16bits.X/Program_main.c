

// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bits (Brown-out Reset disabled in hardware and software)
#pragma config BORV = 3         // Brown Out Reset Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-001FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-001FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-001FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.


#include <xc.h>
#include <stdio.h>
#include"LCD4b_EXSTO.h"
#define _XTAL_FREQ 8000000

int timer_seg = 0,   
    timer_min = 0,  
    timer_hor = 0;    

char bufferLCD[32];


void config_IO()
{
    TRISB = 0X00;
    PORTB = 0X01;       //Acompanha Contagem do TIMER
    ADCON1 = 0X0F;
}

void config_FOSC()
{
    OSCCON = 0X00;
    OSCTUNE = 0X00;
}

void config_INTERRUPT()
{
    INTCON = 0xC0;      /* GIE/GIEH = 1     - Habilita Interrup��o Global
                         * PEIE/GIEL = 1    - Habilita Interrup��o de perif�ricos
                         * TMR0IE = 0       - Habilita Interrup��o pelo TIMER0
                         * INT0IE = 0       - N�o uitilizado
                         * RBIE = 0         - N�o utilizado
                         * TMR0IF = 0       - Flag de overflow p/ o TIMER0
                         * INT0IF = 0       - N�o utilizado
                         * RBIE = 0         - N�o utilizado
                         */
    PIE1bits.TMR1IE = 1;  // Habilita interrup��o para o modulo TIMER1
}
void config_TIMER1()
{
    
    T1CON = 0X8F;       /* RD16 = 1         - Habilita modo 16bits                     
                         * T1RUN = 0        - Fonte de clock derivada de outra fonte
                         * T1CKPS1 e T1CKPS0 = 0   - Prescaler de 1:1
                         * T1OSCEN = 1      - Habilita Crystal Externo em RC0 e RC1
                         * T1SYNC = 1       - N�o de aplica
                         * TMR1CS = 1       - Fonte de clock externa
                         * TMR1ON = 1       - M�dulo do TIMER1 habilitado
                         */
    TMR1H = 0X80;       
    TMR1L = 0X00;       /* Valor inicial para o registrador TMR1  = 32768 = 0X8000 - Cristal Externo (32,765 kHz)          
                         * 
                         */
}

void __interrupt() intern_TMR1(void)
{
    if (PIR1bits.TMR1IF == 1)
    {
        timer_seg++;
        PORTBbits.RB0 =~ PORTBbits.RB0;
        if (timer_seg >= 60)
        {
            timer_seg = 0;
            timer_min++;           
            if (timer_min >= 60) 
            {
                timer_min = 0;
                timer_hor++; 
                if (timer_hor >= 24)
                {
                    timer_hor = 0;
                }
            }
        }
        TMR1H = 0X80;       
        TMR1L = 0X00;
        PIR1bits.TMR1IF = 0;
    }     
}

void main(void) 
{
    config_FOSC();
    config_IO();
    config_INTERRUPT();
    config_TIMER1();
    lcd_init();
    
    lcd_write(1,1, "REAL TIME CLOCK");
        
    while(1)
    {
        sprintf(bufferLCD, "%02d:%02d:%02d", timer_hor, timer_min, timer_seg);       
        lcd_write(2, 5, bufferLCD);
    }   
    return;
}
