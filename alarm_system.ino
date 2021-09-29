const int sensorPin = A0;
const int dummySensorPin = A1;
//const int led = 12;
const int speaker = 3;

void setup() {
  Serial.begin(9600);
//  pinMode(led, OUTPUT);
//  digitalWrite(led, 1);
  beep(1);
}

int CURRENT_PROGRAM_STAGE = 0;

#define SensorType int

const int collectionIterations = 400;
int lightCollectionIterationCount = 0;
SensorType lightMinValue = 9999;
SensorType lightMaxValue = 0;
SensorType lastSensorValue;
SensorType sensorTolerance;

void printValues() {
    Serial.print("sensor: ");
    Serial.print(lastSensorValue);
    Serial.print(", min: ");
    Serial.print(lightMinValue);
    Serial.print(", max: ");
    Serial.print(lightMaxValue);
    Serial.print(", tolerance: ");
    Serial.print(sensorTolerance);
    Serial.println();
}

SensorType getSensorValue() {
  const SensorType sensorValue1 = analogRead(sensorPin);
//  const double sensorValue2 = analogRead(dummySensorPin);
  lastSensorValue = sensorValue1;
  return sensorValue1;// / sensorValue2;
}

void beep(int dir) {
  const int start = dir ? 0 : 3;
  const int ending = dir ? 3 : 0;
  const int d = dir ? 1 : -1;
  for (int i = start; i != ending; i += d) 
  {
    tone(speaker, 220 * (2 << i));
    delay(100);
    noTone(speaker);
    delay(10); 
  }
}

void collectLightValues() {
  const SensorType sensorValue = getSensorValue();
  if (sensorValue < lightMinValue)
  {
    lightMinValue = sensorValue;
    lightCollectionIterationCount = 0;
  }
  else if (sensorValue > lightMaxValue)
  {
    lightMaxValue = sensorValue;
    lightCollectionIterationCount = 0;
  }

  if (lightCollectionIterationCount++ >= collectionIterations)
  {
    CURRENT_PROGRAM_STAGE++;
    beep(0);
    const int sensorGap = lightMaxValue - lightMinValue;
    sensorTolerance = sensorGap * 2;
//    digitalWrite(led, 0);
    printValues();
  }
  delay(5);
}

void standby() {
  const SensorType sensorValue = getSensorValue();
  const int lower = lightMinValue;
  if (sensorValue < lightMinValue - sensorTolerance || sensorValue > lightMaxValue + sensorTolerance)
  {
    printValues();
    CURRENT_PROGRAM_STAGE++;
  }
  delay(100);
}

const int blare_constant_A = 2000;
const int blare_constant_B = blare_constant_A / 3.0;
void blare() {
  const int t = millis();
  const int threeFactor = floor((t % blare_constant_A) / blare_constant_B);
  const int C =  1 + threeFactor;
//  const int lightFactor = t % (40 * C) > 10 * C ? 0 : 1;
  const double frequencyFactor1 = t % 8000 > 400 ? 20 : 16;
  const double frequencyFactor = frequencyFactor1 * threeFactor;
  const int frequency = sin(t / frequencyFactor) * 500 + 3100;
  
//  digitalWrite(light, lightFactor);
  tone(speaker, frequency);
  delay(10);
}

void (*STAGES[])(void) = {
  collectLightValues,
  standby,
  blare
};

void loop() {
  STAGES[CURRENT_PROGRAM_STAGE]();
}
