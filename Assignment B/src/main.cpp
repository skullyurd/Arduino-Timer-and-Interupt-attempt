#include <Arduino.h>
//implement interrupt

// Pin definitions
#define BUTTON_10_PIN PINB2  // D10
#define BUTTON_11_PIN PINB3  // D11
#define LED_5_PIN PIND5      // D5
#define LED_6_PIN PIND6      // D6
#define LED_8_PIN PIND8      // D8
#define LED_9_PIN PIND9      // D9

// Button states
#define BUTTON_NOT_PRESSED HIGH
#define BUTTON_PRESSED LOW

// Variables
volatile boolean button10State = BUTTON_NOT_PRESSED;
volatile boolean button11State = BUTTON_NOT_PRESSED;

// Interrupt service routine for button 10
void button10ISR()
{
  Serial.println("Button 10 pressed");
  button10State = BUTTON_PRESSED;
}

// Interrupt service routine for button 11
void button11ISR() {
  Serial.println("Button 11 pressed");
  button11State = BUTTON_PRESSED;
}


void setup() {
  // Set button pins as inputs
  DDRB &= ~_BV(BUTTON_10_PIN);
  DDRB &= ~_BV(BUTTON_11_PIN);

  // Enable internal pull-up resistors for the buttons
  PORTB |= _BV(BUTTON_10_PIN);
  PORTB |= _BV(BUTTON_11_PIN);

  // Set LED pins as outputs
  DDRD |= _BV(LED_5_PIN);
  DDRD |= _BV(LED_6_PIN);

  // Initialize Serial communication
  Serial.begin(9600);

  PCICR |= (1 << PCIE0);            // Enable Pin Change Interrupt 0
  PCMSK0 |= (1 << PCINT2) | (1 << PCINT3);   // Enable interrupt for BUTTON_10_PIN and BUTTON_11_PIN
}

void loop() {
  // Read button states
  boolean button10Pressed = (PINB & _BV(BUTTON_10_PIN)) == BUTTON_PRESSED;
  boolean button11Pressed = (PINB & _BV(BUTTON_11_PIN)) == BUTTON_PRESSED;

  // Turn on LED 5 if button 10 is pressed
  if (button10Pressed == true)
  {
    PORTD |= _BV(LED_5_PIN); // Turn on LED 5
  }
  else
  {
    PORTD &= ~_BV(LED_5_PIN); // Turn off LED 5
  }

  // Turn on LED 6 if button 11 is pressed
  if (button11Pressed == true)
  {
    PORTD |= _BV(LED_6_PIN); // Turn on LED 6
  }
  else
  {
    PORTD &= ~_BV(LED_6_PIN); //  Turn off LED 6
  }

  if (button10Pressed == true && button11Pressed == true)
  {
    PORTD |= _BV(LED_5_PIN);
    PORTD &= ~_BV(LED_6_PIN);
    delay(500);
    PORTD |= _BV(LED_6_PIN);
    PORTD &= ~_BV(LED_5_PIN);
    delay(500);
  }
  else if (button10Pressed == false && button11Pressed == false)
  {
    PORTD &= ~_BV(LED_5_PIN);
    PORTD &= ~_BV(LED_6_PIN);
  }

  
}

// Interrupt service routine for button 10
ISR(PCINT0_vect) {
  boolean button10Pressed = (PINB & _BV(PB2)) == BUTTON_PRESSED;
  boolean button11Pressed = (PINB & _BV(PB3)) == BUTTON_PRESSED;

  // Turn on LED 5 if button 10 is pressed
  if (button10Pressed != button10State)
  {
    button10State = button10Pressed;
  }

  // Turn on LED 6 if button 11 is pressed
  if (button11Pressed != button11State)
  {
    button11State = button11Pressed;
  }
}