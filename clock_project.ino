const int bit_clock_pin = 11;
const int digit_clock_pin = 10;
const int data_pin = 9;

const byte digit_pattern[10] =

{

  B1000000,   // 0
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

}


void update_one_digit(int data)

{



  byte pattern;

  pattern = digit_pattern[data];

  digitalWrite(digit_clock_pin, LOW);

  shiftOut(data_pin, bit_clock_pin, MSBFIRST, ~pattern);

  digitalWrite(digit_clock_pin, HIGH);


}

void loop()

{

}

void InterruptFunction(){
  
    counter++;
    
    if (counter == 10)
    {
      counter = 0;
    }
    
    update_one_digit(counter);
  }



