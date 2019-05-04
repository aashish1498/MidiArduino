// Wire Slave Receiver
// by Nicholas Zambetti <http://www.zambetti.com>

#include <MIDI.h>  // Add Midi Library
#include <Wire.h>

struct HairlessMidiSettings : public midi::DefaultSettings
{
  static const bool UseRunningStatus = false;
  static const long BaudRate = 115200;
};
MIDI_CREATE_CUSTOM_INSTANCE(HardwareSerial, Serial, MIDI, HairlessMidiSettings);
//bool

byte noteIsOn = 1;
byte state;
void setup() {
  MIDI.begin(MIDI_CHANNEL_OMNI);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  //Serial.begin(115200);           // start serial for output
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  state = Wire.read(); // receive byte as a character
  //Serial.print(c);         // print the character
  byte pitch = Wire.read();
  byte velocity = Wire.read();
  playNote(pitch, velocity);
}

void playNote (byte pitch, byte velocity) {
  if (state == noteIsOn) {
    MIDI.sendNoteOn(pitch, velocity, 1);
  }
  else {
    MIDI.sendNoteOff(pitch, velocity, 1);
  }
}
