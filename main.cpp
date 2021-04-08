/** Beispiel RGB LED Strip 5 Volt Variante mit einer Leitung pro Farbe
*/
#include "mbed.h"
#include <time.h>
#include "platform/mbed_thread.h"

// MOSFET controlled LED strip
PwmOut green( MBED_CONF_IOTKIT_MOSFET1 );
PwmOut red( MBED_CONF_IOTKIT_MOSFET2 );
PwmOut blue( MBED_CONF_IOTKIT_MOSFET3 );

// SPI controlled LED strip
SPI spi( MBED_CONF_IOTKIT_LED_SPI_MOSI, NC, MBED_CONF_IOTKIT_LED_SPI_SCLK ); // mosi, miso, sclk

Thread thread_mosfetled;
Thread thread_spiled;

void off()
{
    printf( "off \n" );
    red = 0;
    green = 0;
    blue = 0;
    thread_sleep_for( 1000 );
}

/** 3 x 3 Werte */
unsigned int strip[9];

void writeLED()
{
    for ( int p = 0; p < 9; p++ )
        spi.write( strip[p] );
}

void clearLED()
{
    for ( int p = 0; p < 9; p++ ) 
    {
        strip[p] = 0;
        spi.write( strip[p] );
    }
}

void dim( PwmOut& pin )
{
    printf( "dim\n" );
    for ( float i = 0.0f; i < 1.0f; i += .01f )
    {
        pin = i;
        thread_sleep_for( 200 );
    }
    thread_sleep_for( 1000 );
    
}

void thread_mosfetled_func() {
    while(true) {
        dim( red );
        off();
        dim( green );
        off();
        dim( blue );
        off();
        
        red = 1;
        thread_sleep_for( 1000 );
        off();
        
        green = 1;
        thread_sleep_for( 1000 );
        off();
        
        blue = 1;
        thread_sleep_for( 1000 );
        off();
        
        red = 1;
        blue = 1;
        green = 1;
        thread_sleep_for( 1000 );
        off();
    }
}

void thread_spiled_func() {
    while(true) {
        for ( int i = 0; i < 9; i++ )
            strip[i] = rand() % 64 + 1;
            
        writeLED();
        thread_sleep_for( 200 );
    }
}

int main() 
{
    // start init SPI LED Strip
    printf( "LED Strip Test \n" );
     
    spi.format( 8,0 );
    spi.frequency( 800000 );

    clearLED();
    time_t t;
    time(&t);
    srand( (unsigned int)t );              /* Zufallsgenerator initialisieren */
    // End SPI LED strip init

    // Start threads for spi and mosfet leds
    thread_spiled.start(thread_spiled_func);
    thread_mosfetled.start(thread_mosfetled_func);

    while(1);
}