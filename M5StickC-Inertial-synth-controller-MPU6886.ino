#include <M5StickC.h>

int PIN0 = 32;
int PWMCH0 = 0;

int PIN1 = 33;
int PWMCH1 = 1;

int LED_PIN = 10;
int PWMCH2 = 2;

float accX = 0;
float accY = 0;
float accZ = 0;

float roll = 0;
float pitch = 0;
float yaw = 0;

int rotate = -1;
int threshold = 4;

void setup() {
  setCpuFrequencyMhz(10);
  
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(RED, BLACK);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(40, 0);

  M5.MPU6886.Init();

  pinMode(PIN0, OUTPUT);
  ledcSetup(PWMCH0, 7812.5, 10); // 7812.5Hz, 8Bit(1024step)
  ledcAttachPin(PIN0, PWMCH0);

  pinMode(PIN1, OUTPUT);
  ledcSetup(PWMCH1, 7812.5, 10); // 7812.5Hz, 8Bit(1024step)
  ledcAttachPin(PIN1, PWMCH1);

  pinMode(LED_PIN, OUTPUT);
  ledcSetup(PWMCH2, 7812.5, 10); // 7812.5Hz, 8Bit(1024step)
  ledcAttachPin(LED_PIN, PWMCH2);
  
  pinMode(M5_BUTTON_RST, INPUT);  

}

void loop() {

 if (digitalRead(M5_BUTTON_HOME) == LOW) {
   while (digitalRead(M5_BUTTON_HOME) == LOW);
   threshold++;
   M5.Lcd.fillScreen(BLACK);
   if (threshold > 7) {
      threshold = 1;
   }
  }

 if (digitalRead(M5_BUTTON_RST) == LOW) {
   while (digitalRead(M5_BUTTON_RST) == LOW);

   if (rotate == 1) {
      rotate = -1;
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setRotation(1);
   } else {
      rotate = 1;
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setRotation(3);
   }
  }
  
  M5.MPU6886.getAccelData(&accX,&accY,&accZ);

  roll = accX * 1000;
  if (0 > roll) roll = roll * -1;
  if ((128 * threshold) < roll) {
    ledcWrite(PWMCH0, roll);
    ledcWrite(PWMCH2, 1);
  } else {
    ledcWrite(PWMCH0, 0);
  }
  
  pitch = accY * 1000;
  if (0 > pitch) pitch = pitch * -1;
  if ((128 * threshold) < pitch) {
    ledcWrite(PWMCH1, pitch);
    ledcWrite(PWMCH2, 1);
  } else {
    ledcWrite(PWMCH1, 0);
  }

  M5.Lcd.setTextColor(BLACK, MAROON);
  M5.Lcd.setCursor(2, 2);
  M5.Lcd.println("INERTIAL SYNTH CONTROLLER ");

  M5.Lcd.setTextColor(MAROON, BLACK);

  M5.Lcd.setCursor(2, 70);
  M5.Lcd.printf("R:%.2f   P:%.2f      ",roll,pitch);
  
  M5.Lcd.setCursor(142, 70);
  M5.Lcd.println(128 * threshold);

  // cursor
  int x = accX * 1000 / 24 * rotate;
  int y = accY * 1000 / 24 * rotate;

  int tx = 80 - (128 * threshold / 24);
  int ty = 40 - (128 * threshold / 24);
  int tw = (128 * threshold / 24 * 2);
  int th = (128 * threshold / 24 * 2);

  M5.Lcd.drawRect( tx, ty,  tw,  th,  RED);
  
  M5.Lcd.drawLine(0, x + 40, 160, x + 40, RED);
  M5.Lcd.drawLine(y + 80, 0, y + 80, 80, RED);

  delay(50);

  M5.Lcd.drawLine(0, x + 40, 160, x + 40, BLACK);
  M5.Lcd.drawLine(y + 80, 0, y + 80, 80, BLACK);
  
  ledcWrite(PWMCH2, -1);

}
