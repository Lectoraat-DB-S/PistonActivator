// define size of circular buffer
#define SIZE_OF_BUFFER 30

// variable declared timers
unsigned int delayTime = 2000;
unsigned int actualTime = 0;

// indexes to keep track of the circular buffer
int bufferLength = 0;
int readIndex = 0;
int writeIndex = 0;

// used for blinking the LED
unsigned int blinkyTime = 0;
int ledStatus = 0;

// circular buffer
unsigned int circularBuffer[SIZE_OF_BUFFER] = {0};

// setup, defines the input and output signal and the interupt function.
void setup() {
  pinMode(LED_D0, OUTPUT);        // used to show that the Arduino is running
  pinMode(LED_D1, OUTPUT);        // used to show that the relay was activated
  pinMode(D0, OUTPUT);            // activation of the relay
  pinMode(A0, INPUT);             // input of the sensor
  
  attachInterrupt(digitalPinToInterrupt(A0), pelletCounterISR, RISING);
}

// General loop, activates the piston if enough time has passed
void loop() {
  // put your main code here, to run repeatedly:
  actualTime = millis();
  if(bufferLength!=0){
    if((actualTime - circularBuffer[readIndex])>=delayTime){
      pistonPush();
      char buffer [40];
      sprintf(buffer, "sensor time: %u, actual time: %u\n", circularBuffer[readIndex], actualTime);
      Serial.print(buffer);
      bufferLength--;
      readIndex++;
      if(readIndex >= SIZE_OF_BUFFER){
        readIndex = 0;
      }
    }
  }
  if(actualTime - blinkyTime >= 500){
    if(ledStatus == 1){
      digitalWrite(LED_D0, LOW);
      ledStatus = 0;
    }else{
      digitalWrite(LED_D0, HIGH);
      ledStatus = 1;
    }
    blinkyTime = actualTime;
  }
}

// checks if the buffer is full, no == true, yes == false
bool bufferCheck() {
  if (bufferLength == SIZE_OF_BUFFER)
  {
      Serial.print("Buffer is full!");
      return false;
  }
  return true;
}

// activates the piston
void pistonPush() {
  digitalWrite(D0, HIGH);
  digitalWrite(LED_D1, HIGH);
  delay(50);
  digitalWrite(D0, LOW);
  digitalWrite(LED_D1, LOW);
}

// if a pellet passed the sensor it registrates it in the circular buffer
void pelletCounterISR(){
  if(bufferCheck()){
    circularBuffer[writeIndex] = millis();
    bufferLength++;
    writeIndex++;
    if(writeIndex >= SIZE_OF_BUFFER){
      writeIndex = 0;
    }
  }
}
