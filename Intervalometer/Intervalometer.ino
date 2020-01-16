//Code written by Sumukh Anil Purohit
//Licenced under GNU GPL v2
//
//Arduino Based DIY camera intervalometer

#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7); //LCD pins

int keypad_pin = A0;
int keypad_value = 0;
int keypad_value_old = 0;
unsigned long delayBetween = 1; //Delay between shots
char btn_push;
unsigned long picS = 0;         //No of photos to take
unsigned long delayTot = 0;     //Dummy for storing delay
byte mainMenuPage = 1;
byte mainMenuPageOld = 1;
byte mainMenuTotal = 6;
int shutterSpeedmin = 0;        //Shutter speed (Tv) in min (Set it to zero when not used)
int shutterSpeedsec = 0;        //Shutter speed (Tv) in sec (Set it to zero when not used)
unsigned long shutterMin = 0;   //Dummy for storing Tv in min
unsigned long shutterSec = 0;   //Dummy for storing Tv in sec
unsigned long shutterPin = 2;   //Optocoupler in for shutter pin
unsigned long focusPin = 3;     //Optocoupler in for focus

void setup()
{
  lcd.begin(16, 2); //Initialize a 2x16 type LCD
  pinMode(shutterPin, OUTPUT);
  pinMode(focusPin, OUTPUT);
  MainMenuDisplay();
  delay(10);
  digitalWrite(shutterPin, LOW);
  digitalWrite(focusPin, LOW);
}

void loop()
{
  btn_push = ReadKeypad();
  MainMenuBtn();
  if (btn_push == 'S') //enter selected menu
  {
    WaitBtnRelease();
    switch (mainMenuPage)
    {
      case 1:
        MenuA();
        break;
      case 2:
        MenuB();
        break;
      case 3:
        MenuC();
        break;
      case 4:
        MenuD();
        break;
      case 5:
        MenuE();
        break;
      case 6:
        MenuF();
        break;
    }

    MainMenuDisplay();
    WaitBtnRelease();
  }
  delay(10);
}

void MenuA()
{
  int shots = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No of shots");
  while (ReadKeypad() != 'L')
  {
    if (ReadKeypad() == 'U')
    {
      WaitBtnRelease();
      shots = shots + 20 ;
    }
    else if (ReadKeypad() == 'D')
    {
      WaitBtnRelease();
      shots = shots - 20;
    }
    lcd.setCursor(0, 1);
    lcd.print(shots);
    if (ReadKeypad() == 'R')
    {
      picS = shots;
      lcd.setCursor(4, 1);
      lcd.print("Stored ");
      lcd.print(picS);
    }
  }
}

void MenuB()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set delay in s");
  while (ReadKeypad() != 'L')
  {
    if (ReadKeypad() == 'U')
    {
      WaitBtnRelease();
      delayBetween++;
    }
    else if (ReadKeypad() == 'D')
    {
      WaitBtnRelease();
      delayBetween--;
    }
    lcd.setCursor(0, 1);
    lcd.print(delayBetween);
    if (ReadKeypad() == 'R')
    {
      delayTot = delayBetween;
      lcd.setCursor(3, 1);
      lcd.print("Stored");
      lcd.setCursor(10, 1);
      lcd.print(delayTot);
      lcd.setCursor(12, 1);
      lcd.print(" s");
    }
  }
}

void MenuC()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Tv in min");
  while (ReadKeypad() != 'L')
  {
    if (ReadKeypad() == 'U')
    {
      WaitBtnRelease();
      shutterSpeedmin++;
    }
    else if (ReadKeypad() == 'D')
    {
      WaitBtnRelease();
      shutterSpeedmin--;
    }
    lcd.setCursor(0, 1);
    lcd.print(shutterSpeedmin);
    if (ReadKeypad() == 'R')
    {
      shutterMin = shutterSpeedmin;
      lcd.setCursor(3, 1);
      lcd.print("Stored");
      lcd.setCursor(10, 1);
      lcd.print(shutterMin);
      lcd.setCursor(12, 1);
      lcd.print(" '");
    }
  }
}

void MenuD()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Tv in s");
  while (ReadKeypad() != 'L')
  {
    if (ReadKeypad() == 'U')
    {
      WaitBtnRelease();
      shutterSpeedsec++;
    }
    else if (ReadKeypad() == 'D')
    {
      WaitBtnRelease();
      shutterSpeedsec--;
    }
    lcd.setCursor(0, 1);
    lcd.print(shutterSpeedsec);
    if (ReadKeypad() == 'R')
    {
      shutterSec = shutterSpeedsec;
      lcd.setCursor(3, 1);
      lcd.print("Stored");
      lcd.setCursor(10, 1);
      lcd.print(shutterSec);
      lcd.setCursor(12, 1);
      lcd.print(" ''");
    }
  }
}

void MenuE()
{
  while (ReadKeypad() != 'L')
  {
    lcd.setCursor(0, 0);
    lcd.print("Shots Delay Tv");
    lcd.setCursor(0, 1);
    lcd.print(picS);
    lcd.setCursor(6, 1);
    lcd.print(delayTot);
    lcd.setCursor(8, 1);
    lcd.print("s");
    lcd.setCursor(10, 1);
    lcd.print(shutterMin);
    lcd.setCursor(12, 1);
    lcd.print("m");
    lcd.setCursor(13, 1);
    lcd.print(shutterSec);
    lcd.setCursor(15, 1);
    lcd.print("s");
  }
}

void MenuF()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PRESS RIGHT TO");
  lcd.setCursor(0, 1);
  lcd.print("START CAPTURING");
  while (ReadKeypad() != 'L')
  {
    if (ReadKeypad() == 'R')
    {
      for (int i = picS; i >= 1; i--)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Remaining");
        lcd.setCursor(10, 0);
        lcd.print(i);
        digitalWrite(focusPin, HIGH);
        delay(200);
        digitalWrite(shutterPin, HIGH);
        delay((((shutterMin * 60) + shutterSec) * 1000) + 100);
        digitalWrite(shutterPin, LOW);
        digitalWrite(focusPin, LOW);
        delay(delayTot * 1000);
            if (ReadKeypad() == 'U'){      // Emergency stop
       i = picS;
       break;
            }
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("!!!FINISHED!!!");
      lcd.setCursor(0, 1);
      lcd.print("L Back  R Again");
    }
  }
}

void MainMenuDisplay()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  switch (mainMenuPage)
  {
    case 1:
      lcd.print("Photos to take");
      break;
    case 2:
      lcd.print("Delay");
      break;
    case 3:
      lcd.print("Shutter Speed m");
      break;
    case 4:
      lcd.print("Shutter Speed s");
      break;
    case 5:
      lcd.print("Confirm!!!");
      break;
    case 6:
      lcd.print("Start");
      break;
  }
}

void MainMenuBtn()
{
  WaitBtnRelease();
  if (btn_push == 'U')
  {
    mainMenuPage++;
    if (mainMenuPage > mainMenuTotal)
      mainMenuPage = 1;
  }
  else if (btn_push == 'D')
  {
    mainMenuPage--;
    if (mainMenuPage == 0)
      mainMenuPage = mainMenuTotal;
  }

  if (mainMenuPage != mainMenuPageOld) //only update display when page change
  {
    MainMenuDisplay();
    mainMenuPageOld = mainMenuPage;
  }
}

char ReadKeypad()
{
  /* Keypad button analog Value
    no button pressed 1023
    select  741
    left    503
    down    326
    up      142
    right   0
  */
  keypad_value = analogRead(keypad_pin);
  if (keypad_value < 100)
    return 'R';
  else if (keypad_value < 200)
    return 'U';
  else if (keypad_value < 400)
    return 'D';
  else if (keypad_value < 600)
    return 'L';
  else if (keypad_value < 800)
    return 'S';
  else
    return 'N';
}

void WaitBtnRelease()
{
  while ( analogRead(keypad_pin) < 800) {}
}
