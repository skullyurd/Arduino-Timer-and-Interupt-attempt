#include <Arduino.h>

// Pin definitions
#define BUTTON_10_PIN PINB2  // D10
#define BUTTON_11_PIN PINB3  // D11
#define LED_5_PIN PIND5      // D5
#define LED_6_PIN PIND6      // D6
#define LED_8_PIN PINB0      // D8
#define LED_9_PIN PINB1      // D9

// Button states
#define BUTTON_NOT_PRESSED HIGH
#define BUTTON_PRESSED LOW

// Debounce delay (in milliseconds)
#define DEBOUNCE_DELAY 50

// Variables
volatile boolean button10State = BUTTON_NOT_PRESSED;
volatile boolean button11State = BUTTON_NOT_PRESSED;

// Counter variables
volatile int counter_8 = 0;
volatile int counter_9 = 0;

// Timer1 configuration
void configureTimer1()
{
  TCCR1A = 0;                             // Set Timer1 to normal mode
  TCCR1B = (1 << WGM12) | (1 << CS12);    // Set Timer1 to CTC mode, prescaler 256
  OCR1A = 62499;                          // Set compare match value for 1 second interrupt (16MHz / 256 / 2Hz - 1)
  TIMSK1 |= (1 << OCIE1A);                 // Enable Timer1 compare match interrupt
}

// Interrupt service routine for Timer1 compare match
ISR(TIMER1_COMPA_vect)
{
  counter_8++; // Increment counter for LED 8
  counter_9--; // Decrement counter for LED 9
}

// Interrupt service routine for button 10
void button10ISR() {
  button10State = BUTTON_PRESSED;
}

// Interrupt service routine for button 11
void button11ISR() {
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
  DDRD |= (1 << LED_5_PIN) | (1 << LED_6_PIN);
  DDRB |= (1 << LED_8_PIN) | (1 << LED_9_PIN);

  // Initialize Timer1
  configureTimer1();

  PCICR |= (1 << PCIE0);                   // Enable Pin Change Interrupt 0
  PCMSK0 |= (1 << PCINT2) | (1 << PCINT3); // Enable interrupt for BUTTON_10_PIN and BUTTON_11_PIN
}

void loop() {
  // Check if button 10 is pressed
  boolean button10Pressed = (PINB & _BV(BUTTON_10_PIN)) == BUTTON_PRESSED;
  boolean button11Pressed = (PINB & _BV(BUTTON_11_PIN)) == BUTTON_PRESSED;

  // Check if button 10 is pressed
  if (button10Pressed) {
    PORTD |= _BV(LED_5_PIN);
  } else {
    PORTD &= ~_BV(LED_5_PIN);
  }

  // Check if button 11 is pressed
  if (button11Pressed) {
    PORTD |= _BV(LED_6_PIN);
  } else {
    PORTD &= ~_BV(LED_6_PIN);
  }

  if (button10Pressed && button11Pressed) {
    PORTD |= _BV(LED_5_PIN);
    PORTD &= ~_BV(LED_6_PIN);
    delay(500);
    PORTD |= _BV(LED_6_PIN);
    PORTD &= ~_BV(LED_5_PIN);
    delay(500);
  } else if (!button10Pressed && !button11Pressed) {
    PORTD &= ~_BV(LED_5_PIN);
    PORTD &= ~_BV(LED_6_PIN);
  }

  // Display counters on LED 8 and LED 9
  PORTB = (PORTB & 0xFC) | (counter_8 & 0x03); // Use lower 2 bits of counter_8 to set LED 8 pins
  PORTB = (PORTB & 0xFC) | ((counter_9 << 2) & 0x03); // Use lower 2 bits of counter_9 to set LED 9 pins
}

// Interrupt service routine for Pin Change Interrupt
ISR(PCINT0_vect) {
  boolean button10Pressed = (PINB & _BV(PB2)) == BUTTON_PRESSED;
  boolean button11Pressed = (PINB & _BV(PB3)) == BUTTON_PRESSED;

  // Check if button 10 is pressed
  if (button10Pressed != button10State) {
    button10State = button10Pressed;
  }

  // Check if button 11 is pressed
  if (button11Pressed != button11State) {
    button11State = button11Pressed;
  }

}

