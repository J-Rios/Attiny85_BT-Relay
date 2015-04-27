/******************************************************************************************************************/
/* SISTEMA DE CONTROL DE SEÑAL DE POTENCIA POR CONMUTACION POR RELE, MEDIANTE COMUNICACION BLUETOOTH              */
/* MICROCONTROLADOR: ATTINY85 - BOOTLOADER A 8MHZ                                                                 */
/* VERSION: 2.0                                                                                                   */
/******************************************************************************************************************/

#include <SoftwareSerial.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
/******************************************************************************************************************/

#define RELE  0   // (PIN 5)
#define TX    1   // (PIN 6)
#define RX    2   // (PIN 7)
#define NC_1  3   // (PIN 2)
#define NC_2  4   // (PIN 3)
#define RESET 5   // (PIN 1)

/******************************************************************************************************************/

SoftwareSerial serie(RX, TX);
enum estados {ON, OFF};
estados estado;
char dato; // CHAR: 1 BYTE
volatile int wd_counter = 0;

ISR(WDT_vect)
{
    wd_counter++;
}

/******************************************************************************************************************/

char bluetooth_rx();
void accion(char opcion);
void rele_on();
void rele_off();
void rele_inicializar();

/******************************************************************************************************************/

void setup()
{
     serie.begin(9600);
     rele_inicializar();
}

void loop()
{
    dato = bluetooth_rx();
    accion(dato);
    wd_delay(250);
}

/******************************************************************************************************************/

char bluetooth_rx()
{
    char rx = 'z';
    
    if(serie.available())
        rx = serie.read();
    
    return rx;
}

void accion(char opcion)
{
    switch(opcion)
    {
        case 'a':
        {
            rele_on();
            opcion = 'z';
            break;
        }
        case 'b':
        {
            rele_off();
            opcion = 'z';
            break;
        }
        case 'z':
        {
            break;
        }
        default:
        {
            opcion = 'z';
            break;
        }
    }
}

void rele_inicializar()
{
    pinMode(RELE, OUTPUT);
    digitalWrite(RELE, LOW);
    estado = OFF;
}

void rele_on()
{
    if(estado == OFF)
    {
        digitalWrite(RELE, HIGH);
        estado = ON;
    }
}

void rele_off()
{
    if(estado == ON)
    {
        digitalWrite(RELE, LOW);
        estado = OFF;
    }
}

void wd_delay(unsigned long t_ms) // OJO: LA SENSIBILIDAD SERA DE 15MS (UN CAMBIO MENOR A 15MS NO SE DETECTA Y, EL VALOR MINIMO SERA DE 15MS)
{
    int num_veces = 0;
    
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    
    if(t_ms == 15)
    {
        setup_watchdog(WDTO_15MS);
        sleep_mode();
    }
    else if(t_ms == 30)
    {
        setup_watchdog(WDTO_30MS);
        sleep_mode();
    }
    else if(t_ms == 60)
    {
        setup_watchdog(WDTO_60MS);
        sleep_mode();
    }
    else if(t_ms == 120)
    {
        setup_watchdog(WDTO_120MS);
        sleep_mode();
    }
    else if(t_ms == 250)
    {
        setup_watchdog(WDTO_250MS);
        sleep_mode();
    }
    else if(t_ms == 500)
    {
        setup_watchdog(WDTO_500MS);
        sleep_mode();
    }
    else if(t_ms == 1000)
    {
        setup_watchdog(WDTO_1S);
        sleep_mode();
    }
    else if(t_ms == 2000)
    {
        setup_watchdog(WDTO_2S);
        sleep_mode();
    }
    else if(t_ms == 4000)
    {
        setup_watchdog(WDTO_4S);
        sleep_mode();
    }
    else if(t_ms == 8000)
    {
        setup_watchdog(WDTO_8S);
        sleep_mode();
    }
    else
    {
        num_veces = t_ms/15;
        while(wd_counter < num_veces)
        {
            setup_watchdog(WDTO_15MS);
            sleep_mode();
        }
    }
    
    wdt_disable();
    wd_counter = 0; 
}

void setup_watchdog(int timerPrescaler)
{
    if ((timerPrescaler < 0) || (timerPrescaler > 10))
        timerPrescaler = WDTO_15MS;
        
    byte bb = timerPrescaler & 7;    
    if(timerPrescaler > 7)
        bb |= (1<<5); // Establece los bits necesarios
    
    MCUSR &= ~(1<<WDRF); // Limpia el flag de reset del watchdog
    WDTCR |= (1<<WDCE) | (1<<WDE); // Activa el watchdog
    WDTCR = bb; // Establece el Timeout del watchdog
    WDTCR |= _BV(WDIE); // Activa la interrupcion del watchdog
}

/******************************************************************************************************************/
