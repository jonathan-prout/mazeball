//types
typedef struct ballholder {
  float x;
  float y;
  float xSpeed;
  float ySpeed;
} BALLHOLDER;
typedef struct goal {
  int x;
  int y;
  int x2;
  int y2;
} GOAL;
//Both

#include <Wire.h>

//Accelerometer
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

// Screen

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Screen


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_DC     6
#define OLED_CS     7
#define OLED_RESET  8
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         &SPI, OLED_DC, OLED_RESET, OLED_CS);


// Detection

#define COL_NO        0b000000
#define COL_TOP       0b000001
#define COL_RIGHT     0b000010
#define COL_BOTTOM    0b000100
#define COL_LEFT      0b001000
#define COL_CENTRE    0b010000
#define COL_VICTORY   0b100000
//#include "dollar.h"

const unsigned char dollar [] = {
0xFE, 0x32, 0xED, 0xA9, 0x00, 0xD9, 0xDD, 0x33, 0xFF, 0xFF, 0x03, 0x03, 
};
const PROGMEM uint8_t dollar2 [] = {
  0b00010000,
  0b01111110,
  0b10010001,
  0b10010001,
  0b01110000,
  0b00011110,
  0b10010001,
  0b10010001,
  0b01111110,
  0b00010000
  };
const int debug_pin = 12;



//Accelerometer setup
/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print  ("Data Rate:    ");

  switch (accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 ");
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 ");
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 ");
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 ");
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 ");
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 ");
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 ");
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 ");
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 ");
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 ");
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 ");
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 ");
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 ");
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 ");
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 ");
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 ");
      break;
    default:
      Serial.print  ("???? ");
      break;
  }
  Serial.println(" Hz");
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- ");

  switch (accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 ");
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 ");
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 ");
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 ");
      break;
    default:
      Serial.print  ("?? ");
      break;
  }
  Serial.println(" g");
}

BALLHOLDER ball;
GOAL goal;
void setup() {
  pinMode(debug_pin, INPUT);
  Serial.begin(9600);


  // accel

#ifndef ESP8266
  //while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.println("Accelerometer Test"); Serial.println("");

  /* Initialise the sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }

  /* Set the range to whatever is appropriate for your project */
  //accel.setRange(ADXL345_RANGE_16_G);
  // accel.setRange(ADXL345_RANGE_8_G);
  accel.setRange(ADXL345_RANGE_4_G);
  // accel.setRange(ADXL345_RANGE_2_G);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("start display");

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }


  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();


  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.write("done setup");
  delay(100); // Pause for 0.1 seconds
  display.display();
  ball.x = (float)display.width() / 2;
  ball.y = (float)display.height() / 2;
  ball.xSpeed = 0;
  ball.ySpeed = 0;
  Serial.println("done setup");
}
char x_txt[16];
char y_txt[16];
char z_txt[32];


int level = 0;
float score = 0;

const int fps = 60;
const int weight = 3;
const int frameDelay = 1000 / fps;
const float friction = 1.0 - 1.0 / (2000 / fps);
const float scale = 15.0; //m to pixels

int applySpeed(BALLHOLDER *ball, sensors_event_t *event) {
  float dAccx = 0 - event->acceleration.x / weight; //*(1000 /(float)fps);
  float dAccy =   event->acceleration.y / weight; //*(1000 /(float)fps);
  ball->ySpeed += dAccy;
  ball->xSpeed += dAccx;
  ball->ySpeed *= friction;
  ball->xSpeed *= friction;
  ball->x += (ball->xSpeed / (float)fps) * scale;
  ball->y += (ball->ySpeed / (float)fps) * scale;
  return 0;
}


int detectCollision(BALLHOLDER *ball) {
  // Draw a bounding box around the ball check each all pixels move and change speed if needed
  // Let's start with checking pixels either side of the ball, and go fo

  int collision = COL_NO;
  

  // detect if we have hit something

  float startPositionX = ball->x - (ball->xSpeed / (float)fps) * scale;
  float startPositionY = ball->y - (ball->ySpeed / (float)fps) * scale;

  int iStartPositionX = (int)startPositionX;
  int iStartPositionY = (int)startPositionY;
  int iBallX = (int)ball->x;
  int iBallY = (int)ball->y;

  if(iBallX > goal.x){
    if(iBallX < goal.x2){
      if(iBallY > goal.y){
      if(iBallY < goal.y2){
          return COL_VICTORY;
        }
       }
     }
    }
  // detect border hits
  if (ball->x < 0)
  {
    ball->x = 1;
    ball->xSpeed = 1;
    return COL_LEFT;
  }
  if (ball->y < 0)
  {
    ball->ySpeed = 1;
    ball->y = 1;
    return COL_TOP;
  }
  if (ball->x > display.width())
  {
    ball->xSpeed = -1;
    ball->x = display.width() - 1;
    return COL_RIGHT;
  }
  if (ball->y > display.height())
  {
    ball->ySpeed = -1;
    ball->y = display.height() - 1;
    return COL_BOTTOM;
  }

  if (iStartPositionY > iBallY) {
    // moving UP
    for (int yd = iStartPositionY; yd >= iBallY - 3; yd--) {

      if (display.getPixel(ball->x, yd)) {
        collision = collision | COL_TOP;
      }
    }
  } else if (iStartPositionY < iBallY) {
    // moving Down
    for (int yd = iStartPositionY; yd <= iBallY + 3; yd++) {
      if (display.getPixel(ball->x, yd)) {

        collision = collision | COL_BOTTOM;
      }
    }
  }
  if (iStartPositionX > iBallX) {
    // moving left
    for (int xd = iStartPositionX; xd >= iBallX - 3; xd--) {
      if (display.getPixel( xd, ball->y)) {

        collision = collision | COL_LEFT;
      }
    }
  } else if (iStartPositionX < iBallX) {
    // moving right
    for (int xd = iStartPositionX; xd <= iBallX + 3; xd++) {
      if (display.getPixel(xd, ball->y)) {

        collision = collision | COL_RIGHT;
      }
    }
  }

  if (collision == COL_NO) {
    for (int yd = -2; yd < 3; yd++) {

      for (int xd = -2; xd < 3; xd++) {
        if ((abs(xd) == 2) ^ (abs(yd) == 2)) {
          if (display.getPixel(ball->x + xd, ball->y + yd)) {

            if (xd == 2) {
              collision = collision | COL_RIGHT;
            }
            if (xd == -2) {
              collision = collision | COL_LEFT;
            }
            if (yd == -2) {
              collision = collision | COL_TOP;
            }
            if (yd == +2) {
              collision = collision | COL_BOTTOM;
            }
            display.drawPixel(ball->x + xd, ball->y + yd, BLACK);

          } else if (yd == 0 && xd == 0) {

            if (display.getPixel(ball->x + xd, ball->y + yd)) {

              collision = collision | COL_CENTRE;
              display.drawPixel(ball->x + xd, ball->y + yd, BLACK);
            }
          } else {
            display.drawPixel(ball->x + xd, ball->y + yd, WHITE);
          }
        }
      }
    }
  }

  if ((collision & COL_TOP) == COL_TOP) {
    ball->ySpeed = 0 - (ball->ySpeed) / 2; //Bounce!
    ball->y = iStartPositionY + 1;
  }
  if ((collision & COL_BOTTOM) == COL_BOTTOM) {
    ball->ySpeed = 0 - (ball->ySpeed) / 2; //Bounce!
    ball->y = iStartPositionY - 1;
  }
  if ((collision & COL_LEFT) == COL_LEFT) {
    ball->xSpeed = 0 - (ball->xSpeed) / 2; //Bounce!
    ball->x = iStartPositionX + 1;
  }
  if ((collision & COL_RIGHT) == COL_RIGHT) {

    ball->xSpeed = 0 - (ball->xSpeed) / 2; //Bounce!
    ball->x = iStartPositionX - 1;
  }
  if ((collision & COL_CENTRE) == COL_CENTRE) {
    ball->xSpeed = 0;
    ball->ySpeed = 0; //Bounce!
    ball->x = iStartPositionX ^ 9; //glitch 9 pixels
    ball->y = iStartPositionY ^ 9; //glitch 9 pixels
  }
  bool debug = digitalRead(debug_pin) == HIGH;
  if (debug) {
    sprintf(y_txt, "X: %d", iStartPositionX);
    sprintf(x_txt, "->%d", iBallX);
    display.setCursor(70, 10);     // Start at top-left corner
    strcat(y_txt, x_txt);
    display.println(y_txt);
    sprintf(y_txt, "Y: %d", iStartPositionY);
    sprintf(x_txt, "->%d", iBallY);

    strcat(y_txt, x_txt);
    display.setCursor(70, 20);     // Start at top-left corner
    display.println(y_txt);
  }
  return collision;
}


void drawLevel1() {

  display.fillRect(0, 0, display.width(), display.height(), WHITE);
  display.fillRect(2, 2, display.width() - 4, display.height() - 4, BLACK);
  display.drawFastVLine(10, 10, 30, WHITE);
  display.drawFastHLine(10, 40, 50, WHITE);
  
  display.drawFastVLine(30, 50, 14, WHITE);
  display.drawFastVLine(20, 50, 14, WHITE);
  
  
  display.drawFastVLine(20, 10, 10, WHITE);
  display.drawFastHLine(20, 10, 40, WHITE);

  display.drawFastHLine(30, 30, 10, WHITE);
  display.drawFastVLine(40, 20, 10, WHITE);
  //display.drawFastHLine(10, 118, 20, WHITE);
  //display.drawFastHLine(32, 50, 20, WHITE);
   display.drawFastVLine(40, 40, 10, WHITE);
   display.drawFastHLine(40, 50, 30, WHITE);
   display.drawFastVLine(50, 50, 14, WHITE);
   display.drawFastVLine(80, 50, 14, WHITE);
   display.drawFastVLine(80, 0, 40, WHITE);
   display.drawFastHLine(40, 80, 30, WHITE);
   display.drawFastHLine(80, 40, 48, WHITE);
    display.drawFastVLine(100, 40, 64, WHITE);
    //Serial.println(z_txt);
    display.setCursor(90, 10);     // Start at top-left corner
    display.println("LEVEL");
    display.setCursor(100, 20); 
    display.println("1");
    goal.x = 20;
    goal.x2 =30;
    goal.y = 50;
    goal.y2 = 60;
}
void loop() {
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);
  display.clearDisplay();
  bool debug = digitalRead(debug_pin) == HIGH;
  if( level ==1){
    display.setTextSize(2);
    strcpy(x_txt, "Time:");
    dtostrf(score, 2, 2, &x_txt[strlen(x_txt)]);
    //Serial.println(x_txt);
    display.setCursor(10, 10);     // Start at top-left corner
    display.println("Victory");
    display.println(x_txt);
  }else{
  drawLevel1();
  
  applySpeed(&ball, &event);



  /* Display the results (acceleration is measured in m/s^2) */

  float fx = event.acceleration.x * 4;
  float fy = event.acceleration.y * 4;
  //display.drawCircle(display.width()/2, display.height()/2, 30, WHITE);
  //display.drawCircle(display.width()/2, display.height()/2, 31, WHITE);

  int collision = detectCollision(&ball);
  if (!debug) {
    display.fillCircle((int)ball.x, (int)ball.y, 3, WHITE);
  }
  if (collision & COL_VICTORY == COL_VICTORY) {

  }
  display.drawBitmap(goal.x,goal.y, dollar2, 8,10,WHITE);
  if (debug) {
    strcpy(x_txt, "X: ");
    dtostrf(ball.x, 2, 2, &x_txt[strlen(x_txt)]);
    //Serial.println(x_txt);
    display.setCursor(10, 10);     // Start at top-left corner
    display.println(x_txt);

    strcpy(y_txt, "Y: ");
    dtostrf(ball.y, 2, 2, &y_txt[strlen(y_txt)]);
    //Serial.println(y_txt);
    display.setCursor(10, 20);     // Start at top-left corner
    display.println(y_txt);

    //sprintf(z_txt, "Z: %d  ", event.acceleration.z);
    strcpy(z_txt, "XA: ");
    dtostrf(ball.xSpeed, 2, 2, &z_txt[strlen(z_txt)]);
    //Serial.println(z_txt);
    display.setCursor(10, 30);     // Start at top-left corner
    display.println(z_txt);
    strcpy(z_txt, "YA: ");
    dtostrf(ball.ySpeed, 2, 2, &z_txt[strlen(z_txt)]);
    //Serial.println(z_txt);
    display.setCursor(10, 40);     // Start at top-left corner
    display.println(z_txt);

    if ((collision & 0b1111) != 0) {
      strcpy(z_txt, "BOUNCE: ");
      if ( collision & COL_TOP    ) {
        strcat(z_txt, "TOP ");
      }
      if ( collision & COL_RIGHT  ) {
        strcat(z_txt, "RIGHT ");
      }
      if ( collision & COL_BOTTOM ) {
        strcat(z_txt, "BOTTOM ");
      }
      if ( collision & COL_LEFT   ) {
        strcat(z_txt, "LEFT ");
      }
      if ( collision & COL_CENTRE   ) {
        strcat(z_txt, "CENTRE ");
      }
      display.println(z_txt);
    }
  }
  if ((collision & COL_VICTORY) == COL_VICTORY) {

    level++;
    score = (float)millis()/1000;
  }

  }
  display.display();
  delay(frameDelay);
  

}
