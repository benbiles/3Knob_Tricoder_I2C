// *******************
// Bens TriCoder board
// from ACE128 library


#define LCD_I2C      // use LCD on PCF8574 address 0x27
#define LCD_I2C_ADDR 0x27
// Include the encoder library and maps
#include <ACE128.h>  // Absolute Contact Encoder
 #include <ACE128map12345678.h> // mapping for pin order 12345678
//#include <ACE128map87654321.h> // mapping for pin order 87654321 (backpack module)
#include <Wire.h> // I2C bus communication library - required to support ACE128

#ifdef LCD_I2C
#include <LiquidCrystal_PCF8574.h>
LiquidCrystal_PCF8574 lcd(LCD_I2C_ADDR);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#else // use direct wire liquidcrystal library
#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2); // your pins may vary
#endif
#define ACE_ADDR 0x20
#define ACE_ADDR1 0x21
#define ACE_ADDR2 0x22


// address range 0x00 - 0x07 -> MCP23008 at address 0x20 -> 0x27
// address range 0x20 - 0x27 -> PCF8574  at address 0x20 -> 0x27
// address range 0x38 - 0x3F -> PCF8574A at address 0x38 -> 0x3F
// note that the PCF8574 datasheet lists these addresses twice these values
// they count the read/write bit in the address, which is a source of confusion
// Create an ACE128 instance called myACE at address 0 (really 0x20) using the basic encoder map
ACE128 myACE((uint8_t)ACE_ADDR, (uint8_t*)encoderMap_12345678, -1); // datasheet basics - MCP23008
ACE128 myACE1((uint8_t)ACE_ADDR1, (uint8_t*)encoderMap_12345678, -1); // datasheet basics - MCP23008
ACE128 myACE2((uint8_t)ACE_ADDR2, (uint8_t*)encoderMap_12345678, -1); // datasheet basics - MCP23008

// set-zero button on pin 13
// - button to ground e.g. MakerShield button
const int ZERO = 13;
uint8_t pinPos = 0; // pin values from MCP23008 or PCF8574
uint8_t pinPos1 = 0; // pin values from MCP23008 or PCF8574
uint8_t pinPos2 = 0; // pin values from MCP23008 or PCF8574

uint8_t rawPos = 0;
uint8_t rawPos1 = 0;
uint8_t rawPos2 = 0;

uint8_t upos = 0;
uint8_t upos1 = 0;
uint8_t upos2 = 0;

uint8_t oldPos = 255;
uint8_t oldPos1 = 255;
uint8_t oldPos2 = 255;

int8_t pos;
int8_t pos1;
int8_t pos2;

int16_t mpos;
int16_t mpos1;
int16_t mpos2;

uint8_t seen = 0;

void setup() {
  int error = 1;
  lcd.begin(16, 4); // 16 x 2 display common with Arduino kits
#ifdef LCD_I2C
  lcd.setBacklight(127);
#endif
  // check that we can talk to it
  while (error != 0) {
    lcd.setCursor(0, 0);
    lcd.print("looking for");
    lcd.setCursor(0, 1);
    lcd.print("ACE-128");
    
    Wire.beginTransmission(ACE_ADDR);
    error = Wire.endTransmission();

    Wire.beginTransmission(ACE_ADDR1);
    error = Wire.endTransmission();

    Wire.beginTransmission(ACE_ADDR2);
    error = Wire.endTransmission();

  }

  myACE.begin();    // this is required for each instance, initializes the IO expander
  // myACE.reverse(true);  // uncomment this for counter-clockwise operation
  myACE1.begin();   
  // myACE1.reverse(true);  // uncomment this for counter-clockwise operation
  myACE2.begin();   
  // myACE2.reverse(true);  // uncomment this for counter-clockwise operation
  
  pinMode(ZERO, INPUT_PULLUP);    // configure set-zero button
  pinPos = myACE.acePins();          // get IO expander pins
  pinPos1 = myACE1.acePins();          // get IO expander pins
  pinPos2 = myACE2.acePins();          // get IO expander pins
  
  oldPos = pinPos;                 // remember where we are
  oldPos1 = pinPos1;                 // remember where we are
  oldPos2 = pinPos2;                 // remember where we are

  
  
  rawPos = myACE.rawPos();           // get raw mechanical position
  rawPos1 = myACE1.rawPos();           // get raw mechanical position
  rawPos2 = myACE2.rawPos();           // get raw mechanical position
  

  
  lcd.clear();
     lcd.setCursor(0, 0);
      lcd.print("    ");
      lcd.setCursor(0, 0);
      lcd.print("Vol1 ");
      lcd.print(rawPos);
    
    lcd.setCursor(0, 1);
      lcd.print("    ");
      lcd.setCursor(0, 1);
      lcd.print("Vol2 ");
      lcd.print(rawPos1);
      
   
   lcd.setCursor(0, 2);
   lcd.print("    ");
   lcd.setCursor(0, 2);
   lcd.print("vol3 ");
   lcd.print(rawPos2);
 
}

void loop() {
  if (digitalRead(ZERO) == 0) {      // check set-zero button

    // myACE.setMpos(6723);  // set current position to 6723
    // myACE.setMpos(0);    // set logical multiturn zero to current position
    myACE.setZero();     // set logical zero to current position
    myACE1.setZero();     // set logical zero to current position
    myACE2.setZero();     // set logical zero to current position
    
    // myACE.reverse(true);  // set reverse
    oldPos = 255;                   // force display update
    oldPos1 = 255;                   // force display update
    oldPos2 = 255;                   // force display update
  }
  pinPos = myACE.acePins();          // get IO expander pins
  rawPos = myACE.rawPos();           // get raw mechanical position
 // pos = myACE.pos();                 // get logical position - signed
 // upos = myACE.upos();               // get logical position - unsigned
  mpos = myACE.mpos();               // get multiturn position - signed

  pinPos1 = myACE1.acePins();          // get IO expander pins
  rawPos1 = myACE1.rawPos();           // get raw mechanical position
 // pos1 = myACE1.pos();                 // get logical position - signed
 // upos1 = myACE1.upos();               // get logical position - unsigned
  mpos1 = myACE1.mpos();               // get multiturn position - signed

  pinPos2 = myACE2.acePins();          // get IO expander pins
  rawPos2 = myACE2.rawPos();           // get raw mechanical position
 // pos2 = myACE2.pos();                 // get logical position - signed
 // upos2 = myACE2.upos();               // get logical position - unsigned
  mpos2 = myACE2.mpos();               // get multiturn position - signed

  if (pinPos != oldPos) {            // did we move?
    seen |= pinPos ^ oldPos;         // what changed?
    oldPos = pinPos;                 // remember where we are
      lcd.setCursor(5, 0);
      lcd.print("    ");
      lcd.setCursor(0, 0);
      lcd.print("Vol1 ");
      lcd.print(rawPos);
       lcd.setCursor(16, 0);
      lcd.print("    ");
      lcd.setCursor(8, 0);
      lcd.print(" Mtrn ");
       lcd.setCursor(15, 0);
      lcd.print(mpos);
  }

    if (pinPos1 != oldPos1) {            // did we move?
    seen |= pinPos1 ^ oldPos1;         // what changed?
    oldPos1 = pinPos1;                 // remember where we are
      lcd.setCursor(5, 1);
      lcd.print("    ");
      lcd.setCursor(0, 1);
      lcd.print("Vol2 ");
      lcd.print(rawPos1);
      lcd.setCursor(16, 1);
      lcd.print("    ");
      lcd.setCursor(8, 1);
      lcd.print(" Mtrn ");
      lcd.setCursor(15, 1);
      lcd.print(mpos1);
      
  }

    if (pinPos2 != oldPos2) {            // did we move?
    seen |= pinPos2 ^ oldPos2;         // what changed?
    oldPos2 = pinPos2;                 // remember where we are
      lcd.setCursor(5, 2);
      lcd.print("    ");
      lcd.setCursor(0, 2);
      lcd.print("Vol3 ");
      lcd.print(rawPos2);
       lcd.setCursor(16, 2);
      lcd.print("    ");
      lcd.setCursor(8, 2);
      lcd.print(" Mtrn ");
      lcd.setCursor(15, 2);
      lcd.print(mpos2);

  }

}  // end loop


