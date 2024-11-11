/*
 * Include the 16x2 LCD library
 */
#include<LiquidCrystal.h>

/*
 * Time Intervals relevant to the project put as macro.
 */
#define SECOND_PERIOD 1000
#define MINUTE_PERIOD 60
#define HOUR_PERIOD   60
#define DAY_PERIOD    24

/*
 * Freqeuency of the key acknowledgement tone put as macro.
 */
#define NOTE_C4  262

/*
 * Key values put as macro.
 */
#define MODE_KEY    1
#define POS_KEY     2
#define EDT_KEY     3
#define OK_KEY      4


/*
 * Key read interval put as macro.
 * Experiment with duration
 */
#define KEY_READ_INTERVAL      200

/*
 * RTC modes put as macro.
 */
#define NORMAL_MODE   0
#define EDIT_MODE     1


/*
 * Intializing the LCD with arduino's pins associated  to the project.
 */
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*
 * Global Variables for creating non blocking delay 
 */
unsigned long CurrentMillis=0; 
unsigned long StartMillis=0;
unsigned long KeyMillis=0; 

/*
 * Global Variables for running time
 */
unsigned char Hour=0;
unsigned char Minute=0;
unsigned char Second=0;

/*
 * Global Variables for creating editing time
 */
unsigned char tmpHour=0;
unsigned char tmpMinute=0;
unsigned char tmpSecond=0;

/*
 * Global Variables for testing time update interval
 */
bool ToggleVal=LOW;

/*
 * Global Variables for RTC mode
 */
int Mode;
/*
 * Global Variables for LCD cursor  position
 */
int Xpos;
/*
 * Global Variables for key value
 */
int KeyVal=-1;

void setup()
{ 
 /*
  * Initialization Routines
  */
  InitializeLCD();
  InitializeKeys();   
  InitializeGlobalVars();
  
 /*
  * Start Scheduling 
  */
  StartMillis=millis(); 
}

void loop()
{  
 /*
  * Load current millis() value and compare to check if second interval has elapsed.
  * If yes update time.Running time is displayed if it is NORMAL mode.
  */
  CurrentMillis = millis();  
  if ((CurrentMillis - StartMillis) >= SECOND_PERIOD)  
  {
      StartMillis=CurrentMillis;
      
      UpdateTime();

	 /*
	  * Toggling Pin 13 to check time update interval.
	  */
      ToggleVal^=HIGH;
      digitalWrite(13,ToggleVal);


      if(Mode==NORMAL_MODE)
      {
        ShowTime();
      }
  }
 /*
  * Load current millis() value and compare to check if key read interval has elapsed.
  * If yes Read Key,
  */
  if ((CurrentMillis - KeyMillis) >= KEY_READ_INTERVAL)  
  {
    KeyMillis=CurrentMillis;
    
    ReadKey();
    
  }
}

/*
 * Initialize Global Variables of the project. 
 */
void InitializeGlobalVars(void)
{
  CurrentMillis=0; 
  StartMillis=0;
  KeyMillis=0; 

/*
 * Edit here to set initial vaues of time.
 */
  Hour=0;
  Minute=0;
  Second=0;

  tmpHour=0;
  tmpMinute=0;
  tmpSecond=0;

  Mode=NORMAL_MODE;
  Xpos=0;
  KeyVal=-1;
}

/*
 * Initialize LCD
 */
void InitializeLCD(void)
{
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("Final Project");
  lcd.setCursor(0,1);
  lcd.print("Real Time Clock");
  delay(5000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Current Time Is:");
}


/*
 * Print running time on LCD.
 */
void ShowTime(void)
{  
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(Hour);  
  lcd.setCursor(3,1);
  lcd.print(':');
  lcd.setCursor(5,1);
  lcd.print(Minute);
  lcd.setCursor(8,1);
  lcd.print(':');
  lcd.setCursor(10,1);
  lcd.print(Second);

    
}

/*
 * Print temporary for editing time on LCD.
 */
void ShowDummyTime()
{  
  lcd.noBlink();
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print(tmpHour);  
  lcd.setCursor(3,1);
  lcd.print(':');
  lcd.setCursor(5,1);
  lcd.print(tmpMinute);
  lcd.setCursor(8,1);
  lcd.print(':');
  lcd.setCursor(10,1);
  lcd.print(tmpSecond);
  lcd.blink();    
  switch(Xpos)
  {
    case  0:  lcd.setCursor(0,1);break;
    case  1:  lcd.setCursor(5,1);break;
    case  2:  lcd.setCursor(10,1);break;
  }
}

/*
 * Manage time variables
 */
void UpdateTime(void)
{
    Second++;
    if(Second>=MINUTE_PERIOD)
    {
      Second=0;
      
      Minute++;
      if(Minute>=HOUR_PERIOD)
      {
        Minute=0;
        Hour++;
        if(Hour>=DAY_PERIOD)
        {
          Hour=0;        
        }
      }
    }
}

/*
 * Buzzer routine for ackowledgement.
 * Experiment with duration
 */
void SoundBuzzer(void)
{
  tone(8, NOTE_C4, 10);  
  
}

/*
 * Set direction of pins connected to pushbuttons as input.
 * Set direction of pin 13 as output to test  Seconds Duration.
 */
void InitializeKeys(void)
{
    pinMode(6, INPUT);
    pinMode(7, INPUT);
    pinMode(9, INPUT);
    pinMode(10, INPUT);

    pinMode(13, OUTPUT);
}
/*
 * Read and process Key press. 
 */
void ReadKey(void)
{
  if(digitalRead(6)==HIGH)
  {
      KeyVal=MODE_KEY;  
  }
  else if(digitalRead(7)==HIGH)
  {
      KeyVal=POS_KEY;    
  }
  else if(digitalRead(9)==HIGH)
  {
      KeyVal=EDT_KEY;   
  }
  else if(digitalRead(10)==HIGH)
  {
      KeyVal=OK_KEY;   
  }
  else
  {
    KeyVal=-1;
  }
 
  if(KeyVal!=-1)
  {
    SoundBuzzer();
  }
  ProcessKey();
  
}

/*
 *Invoking the correct action routine related to each key press.
 */
void ProcessKey(void)
{
  switch(KeyVal)
  {
    case  MODE_KEY  : ModeKeyTask();break;  
    case  POS_KEY   : PosKeyTask();break;
    case  EDT_KEY   : EditKeyTask();break;
    case  OK_KEY    : OkKeyTask();break;
  }
  KeyVal=-1;
}

/*
 *MOD key actions
 */
void ModeKeyTask(void)
{
  Xpos=0;
  lcd.setCursor(0,1);
  
  if(Mode==NORMAL_MODE)
  {
    Mode=EDIT_MODE;
    lcd.blink();
    
    tmpHour=Hour;
    tmpMinute=Minute;
    tmpSecond=Second;
  }
  else
  {    
    Mode=NORMAL_MODE;
    lcd.noBlink();
  }  
}


/*
 *POS key actions
 */
void PosKeyTask(void)
{
  if(Mode==EDIT_MODE)
  {
    Xpos++;
    if(Xpos>2)
    {
      Xpos=0;
    }
    switch(Xpos)
    {
      case  0:  lcd.setCursor(0,1);break;
      case  1:  lcd.setCursor(5,1);break;
      case  2:  lcd.setCursor(10,1);break;
    }
  }
}
/*
 *EDT key actions
 */
void EditKeyTask(void)
{
  if(Mode==EDIT_MODE)
  {
    switch(Xpos)
    {
      case  0:  lcd.print("  ");
                lcd.setCursor(0,1);
                tmpHour++;
                if(tmpHour>23)
                {
                  tmpHour=0;
                }
                break;
      case  1:  lcd.print("  ");
                lcd.setCursor(5,1);
                tmpMinute++;
                if(tmpMinute>59)
                {
                  tmpMinute=0;
                }
                break;
      case  2:  lcd.print("  ");
                lcd.setCursor(10,1);
                tmpSecond++;
                if(tmpSecond>59)
                {
                  tmpSecond=0;
                }
                break;
    }
    ShowDummyTime();
  }
}
/*
 *OK key actions
 */
void OkKeyTask(void)
{
  if(Mode==EDIT_MODE)
  {
    Hour=tmpHour;
    Minute=tmpMinute;
    Second=tmpSecond;
    Xpos=0;
    lcd.setCursor(0,1);
    Mode=NORMAL_MODE;
    lcd.noBlink();
  }
}
