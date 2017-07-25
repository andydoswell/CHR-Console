/* Console controller V1.0 
 *  (c) A.G.Doswell 17th January 2017
 License: The MIT License (See full license at the bottom of this file)
 */

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

byte oneBar[8] = {                //these define the special characters required
  16, 16, 16, 16, 16, 16, 16, 16
};

byte oneBarPlusThree[8] = {
  20, 20, 20, 20, 20, 20, 20, 20
};

byte oneBarPlusFour[8] = {
  18, 18, 18, 18, 18, 18, 18, 18
};

byte oneBarPlusFive[8] = {
  17, 17, 17, 17, 17, 17, 17, 17
};

byte twoBar[8] = {
  24, 24, 24, 24, 24, 24, 24, 24

};

byte twoBarPlusFour[8] = {
  26, 26, 26, 26, 26, 26, 26, 26
};

byte twoBarPlusFive[8] = {
  27, 27, 27, 27, 27, 27, 27, 27
};

byte threeBar[8] = {
  28, 28, 28, 28, 28, 28, 28, 28
};

byte threeBarPlusFive[8] = {
  29, 29, 29, 29, 29, 29, 29, 29
};

byte fourBar[8] = {
  30, 30, 30, 30, 30, 30, 30, 30
};

byte barTwo[8] = {
  8, 8, 8, 8, 8, 8, 8, 8
};

byte barThree[8] = {
  4, 4, 4, 4, 4, 4, 4, 4
};

byte barFour[8] = {
  2, 2, 2, 2, 2, 2, 2, 2
};

byte barFive[8] = {
  1, 1, 1, 1, 1, 1, 1, 1
};

float dBLevel;
boolean rehearse;
float zero = 512;
float dBPeak;
int  count;
int barsPlotted;
int wholeCharsPlotted;
int remainingBarsPlotted;
int cursorPos;
int peakWholeCharsPlotted;
int peakBarsPlotted;
int peakRemainingBarsPlotted;
int rehPin = 9;
int micLiveInputPin = 8;
int micLiveOutputPin = 6;
int rehRelayPin = 10;
int rehLEDPin = 7;
int overLEDPin = 13;
int rehCounter = 0;
boolean rehFlag = false;

void setup() {
  // set up IO's
  pinMode (rehPin, INPUT);
  digitalWrite(rehPin, HIGH);
  pinMode (micLiveInputPin, INPUT);
  pinMode (micLiveOutputPin, OUTPUT);
  digitalWrite (micLiveOutputPin, LOW);
  pinMode (rehRelayPin, OUTPUT);
  digitalWrite (rehRelayPin, LOW);
  pinMode (rehLEDPin, OUTPUT);
  digitalWrite (rehLEDPin, LOW);
  pinMode (overLEDPin, OUTPUT);
  digitalWrite (overLEDPin, LOW);
  // set up LCD
  lcd.begin (20, 4);
  lcd.clear ();
  lcd.print ("Cotswold Hospital");
  lcd.setCursor (0, 1);
  lcd.print ("Radio 'Dolls house'");
  lcd.setCursor (0, 2);
  lcd.print ("(C) 2017 A.G.Doswell");
  delay (1000);
  lcd.createChar (2, barFive);
  lcd.clear ();
  lcd.print ("Desk LIVE   Mic off");
  lcd.setCursor (15, 2);
  lcd.write (byte(2));
  lcd.setCursor (7, 2);
  lcd.print ("Set PFL>");
}

void loop() {
  getSample();
  updateStatus ();
 
  barsPlotted = (1.845 * dBLevel) + 100;  // convert dB to bars plotted (there are 100 bars on the display)
  peakBarsPlotted = (1.845 * dBPeak) + 100; // as above but for the peak level
  wholeCharsPlotted = barsPlotted / 5;
  peakWholeCharsPlotted = peakBarsPlotted / 5;
  remainingBarsPlotted = barsPlotted - (wholeCharsPlotted * 5);
  peakRemainingBarsPlotted = peakBarsPlotted - (peakWholeCharsPlotted * 5);


  for (cursorPos = 0 ; cursorPos < wholeCharsPlotted; cursorPos++) { //plot whole characters
    lcd.setCursor (cursorPos, 3);
    lcd.print (char(255));
  }
  switch (remainingBarsPlotted) { //plot partial charachers, by defining chars on the fly
    case 4:

      lcd.createChar(0, fourBar);
      lcd.setCursor (cursorPos, 3);
      lcd.write(byte(0));
      cursorPos++;
      break;

    case 3:

      lcd.createChar(0, threeBar);
      lcd.setCursor (cursorPos, 3);
      lcd.write(byte(0));
      cursorPos++;
      break;

    case 2:

      lcd.createChar(0, twoBar);
      lcd.setCursor (cursorPos, 3);
      lcd.write(byte(0));
      cursorPos++;
      break;

    case 1:

      lcd.createChar(0, oneBar);
      lcd.setCursor (cursorPos, 3);
      lcd.write(byte(0));
      cursorPos++;
      break;
  }

  for (cursorPos; cursorPos < 20; cursorPos++) {
    lcd.setCursor (cursorPos, 3);
    lcd.print(" ");
  }

  //plot peak level
  cursorPos = peakWholeCharsPlotted ;
  if (peakBarsPlotted != barsPlotted) {
    if (peakWholeCharsPlotted > wholeCharsPlotted) {
      lcd.setCursor (peakWholeCharsPlotted, 3);
      switch (peakRemainingBarsPlotted) {
        case 4:

          lcd.createChar(1, barFour);
          lcd.setCursor (cursorPos, 3);
          lcd.write(byte(1));

          break;

        case 3:

          lcd.createChar(1, barThree);
          lcd.setCursor (cursorPos, 3);
          lcd.write(byte(1));

          break;

        case 2:

          lcd.createChar(1, barTwo);
          lcd.setCursor (cursorPos, 3);
          lcd.write(byte(1));

          break;


        case 1:

          lcd.createChar(1, oneBar);
          lcd.setCursor (cursorPos, 3);
          lcd.write(byte(1));

          break;
      }
    }
  }



  if (wholeCharsPlotted > 15) {
    lcd.setCursor (16, 2);
    lcd.print("OVER");
    digitalWrite (13, HIGH);
  }
  else {
    lcd.setCursor (16, 2);
    lcd.print("    ");
    digitalWrite (13, LOW);
  }

  dBPeak = dBPeak - 0.1; //decay the peak value, but limit it to -54.19
  if (dBPeak <= -54.19) {
    dBPeak = -54.19;
  }
}

void getSample () {
  float maxRaw = 0;
  float raw;
  for (int sample = 0; sample < 48; sample++) { //get 48 samples from the A to D, use the biggest. ~5mS delay to the meter
    raw = analogRead(A0);
    if (raw > maxRaw) {
      maxRaw = raw;
    }
  }
  if (maxRaw == 512) {  // This fudges the result slightly, so we don't return an inf (divide by zero)
    maxRaw = 511;
  }
  dBLevel =  (20 * log10 (abs(maxRaw - zero) / zero)); // convert to dB scale
  if (dBLevel > dBPeak) { //sets "peak" value
    dBPeak = dBLevel;
  }
}

void updateStatus () { //check all digital inputs and update rehearse staus/ mic live as required
  int val = digitalRead(micLiveInputPin);
  digitalWrite (micLiveOutputPin, val);
  if (val == HIGH) {
    lcd.setCursor (16, 0);
    lcd.print ("LIVE");
  }
  else {
    lcd.setCursor (16, 0);
    lcd.print ("Off ");
  }
  if (digitalRead (rehPin) == LOW) { // Read reh push button. Counter prevents switch bounce, and also gives a press-and-hold function to mode select to prevent accidental switching.
    rehCounter ++;
    lcd.setCursor (rehCounter / 5, 1);
    lcd.print (char(255));
  }
  else {
    rehCounter=0;
  }


  if (rehCounter == 100) {
    rehFlag = !rehFlag;
    rehCounter = 0;
  }
  if (rehCounter == 0 ) {
    lcd.setCursor (0, 1);
    lcd.print ("                    ");
  }

  if (rehFlag == true) {
    digitalWrite (rehRelayPin, HIGH);
    digitalWrite (rehLEDPin, HIGH);
    lcd.setCursor (5, 0);
    lcd.print ("REH ");
  }
  else {
    digitalWrite (rehRelayPin, LOW);
    digitalWrite (rehLEDPin, LOW);
    lcd.setCursor (5, 0);
    lcd.print ("LIVE");
  }
}

/*
   Copyright (c) 2016 Andrew Doswell

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permiSerialion notice shall be included in
   all copies or substantial portions of the Software.

   Any commercial use is prohibited without prior arrangement.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESerial FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHOR(S) OR COPYRIGHT HOLDER(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/
