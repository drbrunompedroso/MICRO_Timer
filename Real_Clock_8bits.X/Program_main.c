
// PIC18F4520 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = INTIO67    // Oscillator Selection bits (Internal oscillator block, port function on RA6 and RA7)
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

int timer_500ms = 0,
    timer_1000ms = 0,
    counter = 0;

char bufferLCD[16];


void config_IO()
{
    TRISB = 0X00;
    PORTB = 0X01;       //Acompanha Contagem do TIMER
    ADCON1 = 0X0F;
}

void config_FOSC()
{
    OSCCON = 0X72;
    OSCTUNE = 0X00;
}

void config_INTERRUPT()
{
    INTCON = 0xA0;      /* GIE/GIEH = 1     - Habilita Interrupção Global
                         * PEIE/GIEL = 0    - Não utilizado 
                         * TMR0IE = 1       - Habilita Interrupção pelo TIMER0
                         * INT0IE = 0       - Não uitilizado
                         * RBIE = 0         - Não utilizado
                         * TMR0IF = 0       - Flag de overflow p/ o TIMER0
                         * INT0IF = 0       - Não utilizado
                         * RBIE = 0         - Não utilizado
                         */
}
void config_TIMER0()
{
    
    T0CON = 0XC7;       /* TMR0ON = 1       - Habilita TIMER0                       
                         * T08BIT = 1       - Modo de operação em 8 bits
                         * T0CS = 0         - Incremento pelo ciclo de máquina
                         * T0SE = 0         - Nào de aplica
                         * PSA = 0          - Prescaler habilitado
                         * PS2, PS1, PS0 = 1  - Prescaler de 1:256
                         */
    
    TMR0 = 61;          /* Valor inicial para o registrador TMR0
                         * 
                         * Cálculo do TIMER 8bits
                         * tempo desejado = 256 * prescaler * ciclo de máquina = 256 * 256 * [1/(FOSC/4)] = 32,768mSeg
                         * 
                         * 1000ms / 32.768mseg = 30.5175 mseg  (quebrado)
                         * 1000ms / 25mseg = 40 (ok)
                         * 
                         * TMR0 = 256 - (tempo desejado / (ciclo de maquina * prescaler))
                         * TMR0 = 256 - (25mseg / (0.5useg * 256))
                         * TMR0 = 60.69, portanto TMR0 = 61                      
                         */
}

void __interrupt(low_priority) intern_TMR0(void)
{
    if (INTCONbits.TMR0IF == 1)
    {
        timer_500ms++;
        if (timer_500ms >= 20)
        {
            timer_500ms = 0;
            PORTBbits.RB0 = ~PORTBbits.RB0;
            timer_1000ms++;
            if (timer_1000ms >= 2)
            {
                counter++;
                if (counter > 60) counter = 0;
                timer_1000ms = 0;
            }
        }
        TMR0 = 61;
        INTCONbits.TMR0IF = 0;
    }     
}

void main(void) 
{
    config_FOSC();
    config_IO();
    config_INTERRUPT();
    config_TIMER0();
    lcd_init();
    
    lcd_write(1,6, "TIMER0");
    lcd_write(2,3, "CONTAGEM: 00");
    
    while(1)
    {
        sprintf(bufferLCD, "%02d", counter);
        lcd_write(2, 13, bufferLCD);
    }   
    return;
}
