#include <Arduino.h>

// Pin definitions
#define BUTTON_10_PIN PINB2  // D10
#define BUTTON_11_PIN PINB3  // D11
#define LED_5_PIN PIND5      // D5
#define LED_6_PIN PIND6      // D6

// Button states
#define BUTTON_NOT_PRESSED HIGH
#define BUTTON_PRESSED LOW

// Variables
volatile boolean button10State = BUTTON_NOT_PRESSED;
volatile boolean button11State = BUTTON_NOT_PRESSED;

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
}

void loop() {
  // Read the state of the buttons
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
    PORTD &= ~_BV(LED_6_PIN); // Turn off LED 6
  }

  // Blink LEDs if both buttons are pressed
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
