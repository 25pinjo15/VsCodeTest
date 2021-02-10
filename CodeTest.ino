//	Test de code
//	Johny Pineault johny.pineault@gmail.com
//
//  -Code now hosted on github
//	-Switched over VS Code
//
// 
// Configure LCD_COLS, LCD_ROWS and BAUDRATE if desired/needed
// Expected behavior of the sketch:
// - characters received from serial port are displayed on LCD
// - CR and LF are ignored/dropped
//
// Synthax used: NOTE: ... its a note, DEBUGER: related, ERROR: there is a error here ... it don't work yet ect ,
// ==== : Start of ...
// ---- : end of ...
//
//
// ---------------------------------------------------------------------------


// ==== LIBRARY AND ECT ====

	// include the needed headers and library
#include <Wire.h>
#include <hd44780.h>						// main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>	// i2c expander i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

	// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const int BAUDRATE = 9600; // Baudrate for serial

// ---- LIBRARY AND ECT END ----


// ==== PIN NUMBER ====

	// Input
#define buttonUp 7
#define buttonDown 5
#define buttonLeft 6
#define buttonRight 4
#define buttonSelect 3
#define buttonBack 8
int selectMenuPin = A0;		// Potentiometer connected to Analog in 0

	// Output

//---- PIN NUMBER END ----

// ==== VARIABLE DECLARATION START ====

	// Input Related
byte buttonStateUp = HIGH;			// Button 0 = Pressed 1 = Not Pressed
byte lastButtonStateUp = HIGH;		// Last reading

byte buttonStateDown = HIGH;			// Button 0 = Pressed 1 = Not Pressed
byte lastButtonStateDown = HIGH; 	// Last reading

byte buttonStateLeft = HIGH;			// Button 0 = Pressed 1 = Not Pressed
byte lastButtonStateLeft = HIGH; 	// Last reading

byte buttonStateRight = HIGH;		// Button 0 = Pressed 1 = Not Pressed
byte lastButtonStateRight = HIGH;	// Last reading

byte buttonStateSelect = HIGH;		// Button 0 = Pressed 1 = Not Pressed
byte lastButtonStateSelect = HIGH;	// Last reading

byte buttonStateBack = HIGH;			// Button 0 = Pressed 1 = Not Pressed
byte lastButtonStateBack = HIGH;		// Last reading

int selectMenuRead = 0;				// Brute reading of potentiometer
int selectStateMenu = 0;			// Will be the analog map to the numeber of menu
int lastSelectStateMenu = 0;		// Last reading

	
	// Timming and counting related
int count = 0; 							// To use with FOR

int menu1ButtonTestPage = 1;			// Page at wich the menu is 

int serialOutput = 20; 				// At wich speed the serial will output to console
unsigned long lastSerialOutput = 0; 	//

unsigned long lastDebounceDelay = 0;		// The last time the output pin was toggled
int debounceDelay = 70; 		// The debounce time , how long a button need to be pressed

int menuSelect = 0;
int lastMenuSelect = 0;

// ---- VARIABLE DECLARATION END ----



// ==== SETUP START ====
void setup()
{

	// ===BUTTON INIT===
	pinMode(buttonUp, INPUT);
	pinMode(buttonDown, INPUT);
	pinMode(buttonLeft, INPUT);
	pinMode(buttonRight, INPUT);
	pinMode(buttonSelect, INPUT);
	pinMode(buttonBack, INPUT);







	//	=== LCD INIT ===

	int status;

	// initalize Serial port
	Serial.begin(BAUDRATE);

	/* initialize LCD with number of columns and rows:
	* hd44780 returns a status from begin() that can be used
	* to determine if initalization failed.
	* the actual status codes are defined in <hd44780.h>
	*/
	status = lcd.begin(LCD_COLS, LCD_ROWS);
	if(status) // non zero status means it was unsuccesful
	{
		// begin() failed

		Serial.print("LCD initalization failed: ");
		Serial.println(status);

		// blink error code using the onboard LED if possible
		hd44780::fatalError(status); // does not return
	}

	// Serial.end();



	/* turn on automatic line wrapping
	* which automatically wraps lines to the next lower line and wraps back
	* to the top when at the bottom line
	* NOTE:
	* noLineWrap() can be used to disable automatic line wrapping.
	* _write() can be called instead of write() to send data bytes
	* to the display bypassing any special character or line wrap processing.
	*/
	lcd.lineWrap();

	lcd.print("Test_Button_I2C");
	if(LCD_ROWS > 1)
	{
		lcd.setCursor(0,1);
		lcd.print("Baud:");
		lcd.print(BAUDRATE);
	}

	// === LCD INIT END ===


	delay(3000);

	
	// DEBUGER: Don't end serial for debug
	//Serial.end()

}
// ---- SETUP END ----




// ==== LOOP START ====
void loop()
{
	unsigned long currentTime = millis();			// Use for common timming thing
	// Call the button routine
	buttonRoutine();

	// Menu selection related
	selectMenuRead = analogRead(selectMenuPin);				// Read the potentiometer
	selectStateMenu = map(selectMenuRead, 0, 1024, 1, 5);
	
	if (selectStateMenu == 1) {
		menu1ButtonTest();
	} else if (selectStateMenu == 2) {
			menu2Message();
		} else if (selectStateMenu == 3) {
			menu3();
			} else if (selectStateMenu == 4) {
				menu4();
				}

	
// NOTE: 	
//DEBUGER:

	if (currentTime - lastSerialOutput >= serialOutput) {
		Serial.print(buttonStateUp);
		Serial.print(" and ");
		Serial.print(lastButtonStateUp);
		Serial.print(lastButtonStateDown);
		Serial.println();
		Serial.print(count);
		Serial.println();

		lastSerialOutput = currentTime;
	}

}
// ---- LOOP END ----


// ==== BUTTON MANAGEMENT START ====
void buttonRoutine() {
	// Routine to state all button

	buttonStateUp = digitalRead(buttonUp);
	buttonStateDown = digitalRead(buttonDown);
	buttonStateLeft = digitalRead(buttonLeft);
	buttonStateRight = digitalRead(buttonRight);
	buttonStateSelect = digitalRead(buttonSelect);
	buttonStateBack = digitalRead(buttonBack);

	if (buttonStateUp = LOW)
	{
	count++	;
	}
	

	
	
	// Routine to detect button press with software debounce 
	int debounceTimer;
	int lastButtonStateArray[6] = {lastButtonStateUp, lastButtonStateDown, lastButtonStateLeft, lastButtonStateRight, lastButtonStateSelect, lastButtonStateBack};
	int buttonStateArray[6] = {buttonStateUp, buttonStateDown, buttonStateLeft, buttonStateRight, buttonStateSelect, buttonStateBack};
	for (int i = 0; i < 6; ++i) {
		
	//	if (lastButtonStateArray[i] != lastButtonStateArray[i]) {
	//		lastDebounceDelay = millis();
	//	}

 

		if (buttonStateArray[i] == LOW) {
			count++;
			
			
			}
			
		if (count > debounceDelay) {
			lastButtonStateArray[i] = LOW;
			Serial.print("coliss");
			count = 0;
			}

	

		
	}

}
// ---- BUTTON MANAGEMENT END ----

// ==== SCREEN UPDATE ======== 

void menu1ButtonTest()
{
	if (lastSelectStateMenu != 1)
	{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("we are in test 1");
	lastSelectStateMenu = 1;
	}

	lcd.setCursor(0,1);
	lcd.print(buttonStateUp);
	lcd.print(buttonStateDown);
	lcd.print(buttonStateLeft);
	//delay(2000);
	

  

}

void 	menu2Message()
{
	
	// Routine to show a message first.	
	if (lastSelectStateMenu != 2)
	{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("we are in message");
	lastSelectStateMenu = 2;
	delay(1500);
	}

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(count);


	
	//do something else

}

void menu3()
{
	if (lastSelectStateMenu != 3)
	{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("we are in 3");
	lastSelectStateMenu = 3;
	}

// write something

}

void menu4()
{
	if (lastSelectStateMenu != 4)
	{
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("we are in 4");
	lastSelectStateMenu = 4;
	}

// write something 
	//delay(2000);

}
// ---- SCREEN UPDATE END ----
