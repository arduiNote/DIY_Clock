// Rotary Encoder Inputs
#define CLK 7
#define DT 5
#define SW 3
int helpvar=0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";

int sensorPin = A0;    // select the input pin for the potentiometer

int sensorValue = 0;  // variable to store the value coming from the sensor

int brightness;

const int bit_clock_pin = 11;
const int digit_clock_pin = 10;
const int data_pin = 9;
int Second = 0;
int Minute = 0;
int Hour = 0;

bool Time_Change_in_Progress = false;

int digits[4]={0};

const byte digit_pattern[10] =

{

  B10000000,   // 0
  B11111001,  // 1
  B00100100,  // 2 
  B01000110,  // 3
  B01010011,  // 4
  B01001010,  // 5
  B01001000,  // 6
  B11000111,  // 7
  B00000000,  // 8
  B01000010,  // 9

};

unsigned int counter = 0;

void setup()

{
  pinMode(2, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(2), InterruptFunction, RISING);

  pinMode(data_pin, OUTPUT);

  pinMode(bit_clock_pin, OUTPUT);

  pinMode(digit_clock_pin, OUTPUT); 

  pinMode(6, OUTPUT);

  pinMode(CLK,INPUT);
  
  pinMode(DT,INPUT);
  
  pinMode(SW, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(SW), Button_interrupt, RISING);

  lastStateCLK = digitalRead(CLK);

  Serial.begin(9600);

}


void display_new_time()

{



  byte pattern;

  digitalWrite(digit_clock_pin, LOW);
  
  for (int i = 0; i<4; i++)
  {
    pattern = digit_pattern[digits[i]];
    shiftOut(data_pin, bit_clock_pin, MSBFIRST, ~pattern);
  }

  digitalWrite(digit_clock_pin, HIGH);


}


void loop()
{
    sensorValue = analogRead(sensorPin);
    brightness=map(sensorValue, 50 ,300, 0, 200);
    analogWrite(6, brightness);

    if(Time_Change_in_Progress)
    
      {
          // Read the current state of CLK
          currentStateCLK = digitalRead(CLK);

          // If last and current state of CLK are different, then pulse occurred
          // React to only 1 state change to avoid double count
         if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

             // If the DT state is different than the CLK state then
            // the encoder is rotating CCW so decrement
            if (digitalRead(DT) != currentStateCLK) {
                counter --;
                currentDir ="CCW";
            } else {
                // Encoder is rotating CW so increment
                   Minute++;
                   if (Minute > 60)
                        {
                           Minute=1;
                           Hour++;
                   if (Hour > 12)
                         {
                             Hour=0;
                          }
                        }
                  calculate_new_Digits();     
                  currentDir ="CW";
                  }

          }

        lastStateCLK = currentStateCLK;
        delay(1);
      
      }
}


void InterruptFunction(){
  
    Second=61;  // needs to be:  Second++;
    
    if (Second > 60)
    {
      Second = 0;
      Minute++;
      if (Minute > 60)
      {
        Minute=1;
        Hour++;
        if (Hour > 12)
        {
         Hour=0;
        }
      }
      calculate_new_Digits();
      }

     
    
}

void calculate_new_Digits(){
  
    
    if(Hour >10)
    {
      digits[0]=1;
      digits[1]= Hour-10;      
    }
    else
    {
      digits[0]=0;
      digits[1]=Hour;
    }

    if(Minute >= 10 && Minute <20)
    {
      digits[2]=1;
      digits[3]= Minute-10;      
    }
    else if(Minute >= 20 && Minute <30)
    {
      digits[2]=2;
      digits[3]= Minute-20;      
    }
    else if(Minute >= 30 && Minute <40)
    {
      digits[2]=3;
      digits[3]= Minute-30;      
    } 
    else if(Minute >= 40 && Minute <50)
    {
      digits[2]=4;
      digits[3]= Minute-40;      
    }  
    else if(Minute >= 50 && Minute <60)
    {
      digits[2]=5;
      digits[3]= Minute-50;      
    } 
    else
    {
      digits[2]=0;
      digits[3]=Minute;
    }   
        
      
    display_new_time();

    
  }

void Button_interrupt(){
  

  if(helpvar==1)Time_Change_in_Progress=!Time_Change_in_Progress; //Debouncing
  helpvar=!helpvar;
  
  Serial.println(Time_Change_in_Progress);
  
  delay(5000);
  }



  
