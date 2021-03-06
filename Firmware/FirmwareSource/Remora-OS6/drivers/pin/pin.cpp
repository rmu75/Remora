#include "mbed.h"

#include "pin.h"
#include <cstdio>
#include <cerrno>
#include <string>

#include "LPC17xx.h"

Pin::Pin(std::string portAndPin, int dir)
{
    printf("Creating Pin @\n");

    LPC_GPIO_TypeDef* gpios[5] ={LPC_GPIO0,LPC_GPIO1,LPC_GPIO2,LPC_GPIO3,LPC_GPIO4};

    // The method below to determine the port and pin from the string is taken from Smoothieware, thanks!

    // cs is the current position in the string
    const char* cs = portAndPin.c_str();

    // cn is the position of the next char after the number we just read
    char* cn = NULL;

    // grab first integer as port. pointer to first non-digit goes in cn
    this->portNumber = std::strtol(cs, &cn, 10);

    printf("  portNumber = %d\n", this->portNumber);

    // if cn > cs then strtol read at least one digit
    if ((cn > cs) && (this->portNumber <= 4))
    {
        // translate port index into something useful
        this->port = gpios[this->portNumber];

        // if the char after the first integer is a . then we should expect a pin index next
        if (*cn == '.')
        {
            // move pointer to first digit (hopefully) of pin index
            cs = ++cn;

            // grab pin index.
            this->pin = strtol(cs, &cn, 10);

            printf("  pin = %d\n", this->pin);

            // if strtol read some numbers, cn will point to the first non-digit
            if ((cn > cs) && (this->pin < 32) && (dir >= 0))
            {
                // configure pin direction: FIODIR
                if (dir == INPUT)
                {
                    this->port->FIODIR &= ~(1<<this->pin);
                }
                else
                {
                    this->port->FIODIR |= 1<<this->pin;
                }

                // configure
                this->port->FIOMASK &= ~(1 << this->pin);

                // TODO ADD - check for modifiers
            }
        }
    }
}

// Configure this pin as no pullup or pulldown
void Pin::pull_none()
{
    // Set the two bits for this pin as 10
    if( this->portNumber == 0 && this->pin < 16  ){ LPC_PINCON->PINMODE0 |= (2<<( this->pin*2)); LPC_PINCON->PINMODE0 &= ~(1<<( this->pin    *2)); }
    if( this->portNumber == 0 && this->pin >= 16 ){ LPC_PINCON->PINMODE1 |= (2<<( this->pin*2)); LPC_PINCON->PINMODE1 &= ~(1<<((this->pin-16)*2)); }
    if( this->portNumber == 1 && this->pin < 16  ){ LPC_PINCON->PINMODE2 |= (2<<( this->pin*2)); LPC_PINCON->PINMODE2 &= ~(1<<( this->pin    *2)); }
    if( this->portNumber == 1 && this->pin >= 16 ){ LPC_PINCON->PINMODE3 |= (2<<( this->pin*2)); LPC_PINCON->PINMODE3 &= ~(1<<((this->pin-16)*2)); }
    if( this->portNumber == 2 && this->pin < 16  ){ LPC_PINCON->PINMODE4 |= (2<<( this->pin*2)); LPC_PINCON->PINMODE4 &= ~(1<<( this->pin    *2)); }
    if( this->portNumber == 3 && this->pin >= 16 ){ LPC_PINCON->PINMODE7 |= (2<<( this->pin*2)); LPC_PINCON->PINMODE7 &= ~(1<<((this->pin-16)*2)); }
    if( this->portNumber == 4 && this->pin >= 16 ){ LPC_PINCON->PINMODE9 |= (2<<( this->pin*2)); LPC_PINCON->PINMODE9 &= ~(1<<((this->pin-16)*2)); }
}

// Configure this pin as a pullup
void Pin::pull_up()
{
    // Set the two bits for this pin as 00
    if( this->portNumber == 0 && this->pin < 16  ){ LPC_PINCON->PINMODE0 &= ~(3<<( this->pin    *2)); }
    if( this->portNumber == 0 && this->pin >= 16 ){ LPC_PINCON->PINMODE1 &= ~(3<<((this->pin-16)*2)); }
    if( this->portNumber == 1 && this->pin < 16  ){ LPC_PINCON->PINMODE2 &= ~(3<<( this->pin    *2)); }
    if( this->portNumber == 1 && this->pin >= 16 ){ LPC_PINCON->PINMODE3 &= ~(3<<((this->pin-16)*2)); }
    if( this->portNumber == 2 && this->pin < 16  ){ LPC_PINCON->PINMODE4 &= ~(3<<( this->pin    *2)); }
    if( this->portNumber == 3 && this->pin >= 16 ){ LPC_PINCON->PINMODE7 &= ~(3<<((this->pin-16)*2)); }
    if( this->portNumber == 4 && this->pin >= 16 ){ LPC_PINCON->PINMODE9 &= ~(3<<((this->pin-16)*2)); }
}

// Configure this pin as a pulldown
void Pin::pull_down()
{
    // Set the two bits for this pin as 11
    if( this->portNumber == 0 && this->pin < 16  ){ LPC_PINCON->PINMODE0 |= (3<<( this->pin    *2)); }
    if( this->portNumber == 0 && this->pin >= 16 ){ LPC_PINCON->PINMODE1 |= (3<<((this->pin-16)*2)); }
    if( this->portNumber == 1 && this->pin < 16  ){ LPC_PINCON->PINMODE2 |= (3<<( this->pin    *2)); }
    if( this->portNumber == 1 && this->pin >= 16 ){ LPC_PINCON->PINMODE3 |= (3<<((this->pin-16)*2)); }
    if( this->portNumber == 2 && this->pin < 16  ){ LPC_PINCON->PINMODE4 |= (3<<( this->pin    *2)); }
    if( this->portNumber == 3 && this->pin >= 16 ){ LPC_PINCON->PINMODE7 |= (3<<((this->pin-16)*2)); }
    if( this->portNumber == 4 && this->pin >= 16 ){ LPC_PINCON->PINMODE9 |= (3<<((this->pin-16)*2)); }
}

// Convert a PortAndPin into a mBed Pin
// allows use of standard mbed libraries, eg FastAnalogIn
PinName Pin::pinToPinName()
{
  if( this->port == LPC_GPIO0 && this->pin == 0 ) {
      return p9;
  } else if( this->port == LPC_GPIO0 && this->pin == 1 ) {
      return p10;
  } else if( this->port == LPC_GPIO0 && this->pin == 23 ) {
      return p15;
  } else if( this->port == LPC_GPIO0 && this->pin == 24 ) {
      return p16;
  } else if( this->port == LPC_GPIO0 && this->pin == 25 ) {
      return p17;
  } else if( this->port == LPC_GPIO0 && this->pin == 26 ) {
      return p18;
  } else if( this->port == LPC_GPIO1 && this->pin == 30 ) {
      return p19;
  } else if( this->port == LPC_GPIO1 && this->pin == 31 ) {
      return p20;
  } else {
      //TODO: Error
      return NC;
  }
}
