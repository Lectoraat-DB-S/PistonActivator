// define size of circular buffer
#define SIZE_OF_BUFFER 30

// variable declared timers
unsigned int delayTime = 2000;
unsigned int actualTime = 0;

// indexes to keep track of the circular buffer
int bufferLength = 0;
int readIndex = 0;
int writeIndex = 0;

// circular buffer
unsigned int circularBuffer[SIZE_OF_BUFFER] = {0};

// setup, defines the input and output signal and the interupt function.
void setup() {
  pinMode(LED_D0, OUTPUT);
  pinMode(D0, OUTPUT);
  pinMode(BTN_USER, INPUT);
  pinMode(A0, INPUT);

  attachInterrupt(digitalPinToInterrupt(BTN_USER), buttonCounterISR, RISING);
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
  digitalWrite(LED_D0, HIGH);
  delay(50);
  digitalWrite(D0, LOW);
  digitalWrite(LED_D0, LOW);
}

// if a pellet passed the sensor it registrates it in the circular buffer
void buttonCounterISR(){
  if(bufferCheck()){
    circularBuffer[writeIndex] = millis();
    bufferLength++;
    writeIndex++;
    if(writeIndex >= SIZE_OF_BUFFER){
      writeIndex = 0;
    }
  }
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