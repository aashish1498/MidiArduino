#include <MIDI.h>  // Add Midi Library
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LedControl.h>
#include <FastLED.h>
#define LED 13    // Arduino Board LED is on Pin 13

//Create an instance of the library with default name, serial port and settings
MIDI_CREATE_DEFAULT_INSTANCE();
#define LED_PIN     5
#define NUM_LEDS    60
#define BRIGHTNESS  58
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int maxNote = 84;
int minNote = 36;

void setup() {
  Wire.begin();
  TWBR = 10;
  pinMode (LED, OUTPUT); // Set Arduino board pin 13 to output
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize the Midi Library.
  // OMNI sets it to listen to all channels.. MIDI.begin(2) would set it
  // to respond to notes on channel 2 only.
  MIDI.setHandleNoteOn(MyHandleNoteOn); // This is important!! This command
  // tells the Midi Library which function you want to call when a NOTE ON command
  // is received. In this case it's "MyHandleNoteOn".
  MIDI.setHandleNoteOff(MyHandleNoteOff); // This command tells the Midi Library
  // to call "MyHandleNoteOff" when a NOTE OFF command is received.
  delay(3000); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

void loop() { // Main loop
  MIDI.read(); // Continuously check if Midi data has been received.
}

void MyHandleNoteOn(byte channel, byte pitch, byte velocity) {
  if ((int) pitch >= minNote && (int) pitch <= maxNote) {
    Wire.beginTransmission(8);
    Wire.write(1);
    Wire.write(pitch);
    Wire.write(velocity);
    Wire.endTransmission();

    int pitchint = pitch;
    int velocityint = velocity;
    int letter = pitchint % 12;

    setLEDstrip(pitchint, letter, velocityint);
  }

}

void MyHandleNoteOff(byte channel, byte pitch, byte velocity) {
  Wire.beginTransmission(8);
  Wire.write(0);
  Wire.write(pitch);
  Wire.write(velocity);
  Wire.endTransmission();

  int pitchint = pitch;
  int ledPos = pitchint - 36 + 4;
  leds[ledPos] = CRGB::Black;
  FastLED.show();
}

void setLEDstrip(int pitchint, int letter, int velocityint) {
  // Bring position to 1, then move to 5
  int ledPos = (pitchint - 36 + 4);
  int ledColour = (letter + 1) * 21;
  //leds[ledPos] = CHSV(ledColour, 255, (velocityint*2));
  leds[ledPos] = CHSV(ledColour, 255, 200);
  FastLED.show();
}
