//======================================================= 
// ENGD1025 - Electronics CAE & Programme Fundamentals
// Multifuncitonal Gadget
// Victoria Iza Paez - P2655183
// 18/12/2021
//=======================================================


// Defining the file used in the program
#include "devboard.h"

// Renaming a data type to a meaningful to program so it is easier to read
typedef enum
{
  SpiritLevel_M,
  Thermometer_M,
  Timer_M,
} mode_t;

// Declaring the variables used in the program including array used
int i, x, y, z;
int L = 4;
int ref;
int AvCalc;
int Tout;
int value;
int a, b, c;
int mode;

float ratio = 0;
float C;
float F;

long total = 0;
long array_SPL[4];  

// Function created as average value of inteer values will be returned
// Long data type is used as the size of bytes is longer 
long average(long *array, int nL); 
long average(long *array, int nL)

// Average calculation 
{
  total = 0;
  for (i = 0; i <= 3; i++)
  {
    total += array_SPL[i];
  }
  return total / nL;
}

// Three function are created to switch to different modes (function)
// Spirit level coding
void SpiritLevel(); // put at the beginning of the code
void SpiritLevel()
{
  ratio = 0;
  DEVBOARD_lcdClear();
  DEVBOARD_lcdPrintString("Spirit level");
  DEVBOARD_sevenSegment(0);
  DEVBOARD_writeLedBank((byte)0);

  for (i = 0; i <= 3; i++)
  {
    DEVBOARD_readAccelerometer(&x, &y, &z);
    array_SPL[i] = z;
    delay(50);
  }

  AvCalc = average(array_SPL, L);
  ratio = AvCalc / (float)ref;

  if (ratio >= 0.9)
  {
    DEVBOARD_ledGreenOn ();
    DEVBOARD_ledYellowOff ();
    DEVBOARD_ledRedOff ();
  }
  else if ((ratio < 0.9 ) && (ratio >= 0.50))
  {
    DEVBOARD_ledYellowOn ();
    DEVBOARD_ledGreenOff ();
    DEVBOARD_ledRedOff ();
  }
  else
  {
    DEVBOARD_ledRedOn ();
    DEVBOARD_ledYellowOff ();
    DEVBOARD_ledGreenOff ();
  }

// When S2 is pressed the program will go to the function named 'Thermometer'
  if (DEVBOARD_buttonS2Status() == BUTTON_PRESSED)
  {
    mode = Thermometer_M;
  }
}

// Thermometer coding
void Thermometer(); // put at the beginning of the code
void Thermometer()
{   
    DEVBOARD_lcdClear();
    DEVBOARD_lcdSetCursor(0, 0);
    DEVBOARD_lcdPrintString("Thermometer"); 
    delay(1000);
    DEVBOARD_ledRedOff ();
    DEVBOARD_ledYellowOff ();
    DEVBOARD_ledGreenOff ();
    
  while (1)
  {
    DEVBOARD_readTemperature(&Tout);
    C = (Tout / 340.00) + 36.53;
    F = (C * 1.8) + 32;
    DEVBOARD_lcdClear();
    DEVBOARD_lcdSetCursor(0, 0);
    DEVBOARD_lcdPrintString("C = ");
    DEVBOARD_lcdSetCursor(4, 0);
    DEVBOARD_lcdPrintFloat(C);
    DEVBOARD_lcdSetCursor(0, 1);
    DEVBOARD_lcdPrintString("In Celsius");
    delay(1000);
    if (DEVBOARD_buttonS2Status() == BUTTON_PRESSED)
    {
      mode = Timer_M;
      break;
    }
    delay(1000);
    if (DEVBOARD_buttonS2Status() == BUTTON_PRESSED)
    {
      mode = Timer_M;
      break;
    }


    DEVBOARD_readTemperature(&Tout);
    DEVBOARD_lcdClear();
    DEVBOARD_lcdSetCursor(0, 0);
    DEVBOARD_lcdPrintString("F = ");
    DEVBOARD_lcdSetCursor(4, 0);
    DEVBOARD_lcdPrintFloat(F);
    DEVBOARD_lcdSetCursor(0, 1);
    DEVBOARD_lcdPrintString("In Fahrenheit ");
    delay(1000);

// Program must check if S2 is pressed twice within 2 seconds
// If S2 is pressed program will go to the function named 'Timer'
    if (DEVBOARD_buttonS2Status() == BUTTON_PRESSED)
    {
      mode = Timer_M;
      break;
    }
    delay(1000);
    if (DEVBOARD_buttonS2Status() == BUTTON_PRESSED)
    {
      mode = Timer_M;
      break;
    }
  }

}

// Timer code
void Timer(); // put at the beginning of the code
void Timer()
{
  while (1)
  {
    DEVBOARD_lcdClear();
    DEVBOARD_lcdSetCursor(0, 0);
    DEVBOARD_lcdPrintString("Timer");
    delay(1000);
    DEVBOARD_lcdClear();
    DEVBOARD_lcdPrintString("Enter number of");
    DEVBOARD_lcdSetCursor(0, 1);
    DEVBOARD_lcdPrintString("seconds- 0-999");

    value = DEVBOARD_keypadReadInt(0);
    if ((value >= 0) && (value <= 999))
    {
      for (a = value; a >= 0; a--)
      {
        DEVBOARD_lcdClear();
        DEVBOARD_lcdPrintInt(a);
        c = a / 10;
        b = a % 10;
        DEVBOARD_sevenSegment(sCharSet[b]);
        DEVBOARD_writeLedBank((byte)c);

// Code to let the user press S2 and go to Spirit level mode
        if (DEVBOARD_buttonS1Status() == BUTTON_PRESSED)
        {
          DEVBOARD_lcdClear();
          DEVBOARD_lcdSetCursor(0, 0);
          DEVBOARD_lcdPrintString("Abort");
          DEVBOARD_lcdSetCursor(0, 1);
          DEVBOARD_lcdPrintString("Press S2!");
          DEVBOARD_waitForS2Press ();
          break;
        }
        else if (a == 0)
        {
          DEVBOARD_lcdClear();
          DEVBOARD_lcdSetCursor(0, 0);
          DEVBOARD_lcdPrintString("DONE");
          DEVBOARD_lcdSetCursor(0, 1);
          DEVBOARD_lcdPrintString("Press S2!");
          DEVBOARD_soundBuzzer();
          delay(250);
          DEVBOARD_soundBuzzer();
          delay(250);
          DEVBOARD_soundBuzzer();
          delay(250);
          DEVBOARD_waitForS2Press ();
        }
        delay(1000);
      }
      mode = SpiritLevel_M;
      break;
    }
    else
    {
      DEVBOARD_lcdClear();
      DEVBOARD_lcdSetCursor(0, 0);
      DEVBOARD_lcdPrintString("Number is");
      DEVBOARD_lcdSetCursor(0, 1);
      DEVBOARD_lcdPrintString("outside range");
      delay(1000);
    }
  }
}

// Main function starts the functions starting from Spirit level mode
void setup()
{
  DEVBOARD_init();
  DEVBOARD_soundBuzzer();
  DEVBOARD_lcdClear();
  DEVBOARD_lcdSetCursor(0, 0);
  DEVBOARD_lcdPrintString("MULTIFUNCTIONAL");
  DEVBOARD_lcdSetCursor(0, 1);
  DEVBOARD_lcdPrintString("P2655183");
  DEVBOARD_readAccelerometer(&x, &y, &z);
  ref = z;
  delay(3000);
  mode = SpiritLevel_M;
}

// Function allowing to switch between modes
void loop()
{
  switch (mode)
  {
    case SpiritLevel_M:
      SpiritLevel();
      break;

    case Thermometer_M:
      Thermometer();
      break;

    case Timer_M:
      Timer();
      break;

  }
}
