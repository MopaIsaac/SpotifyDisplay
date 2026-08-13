#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>


#define CS 10
#define DC 9
#define RST 14

#define MOSI 11
#define SCK 12
#define MISO 13 


#define touch_CS 7
#define touch_IRQ 6

XPT2046_Touchscreen ts(touch_CS,touch_IRQ);
Adafruit_ILI9341 tft = Adafruit_ILI9341(CS,DC,RST);

//Song Information

int currentPosition = 0;
int songDuration = 120;

String songName = "In The Morning";    
String artistName = "J Cole";    


// player state
bool playerState = false;


unsigned long previousTime = 0;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  SPI.begin(SCK,MISO,MOSI,CS);

  // Start the Display
  tft.begin();

  // initialize touchscreen 
  ts.begin();

  //rotate display
  tft.setRotation(1);

  ts.setRotation(1);

  drawSpotifyScreen();
 


}

void loop() {
  // put your main code here, to run repeatedly:

  unsigned long currentTime = millis();

  if (ts.touched()) {
    uint16_t x, y;
    uint8_t z;

    ts.readData(&x, &y, &z);

    Serial.print("x = ");
    Serial.print(x);
    Serial.print(", y = ");
    Serial.print(y);
    Serial.print(", z = ");
    Serial.println(z);
}

  if (Serial.available() > 0){
    Serial.println(playerState);
    char incoming = Serial.read();

    if (incoming != -1 && incoming == 'p'){
      playerState = !playerState;
      updateControls();
      Serial.println(playerState);
    }
  }

  while (Serial.available() > 0){
    Serial.read();
  }

  

  // if one second has passed update song current Position and information 

  if (currentTime - previousTime >= 1000){
    previousTime = currentTime;
    if (playerState){
      if (currentPosition < songDuration){
        currentPosition += 1;
        drawCurrentSongProgress(songDuration, currentPosition);   
        drawCurrentTimePosition();
        
      }
    }
  } 


  

}

void drawSpotifyScreen(){
  drawHeader();
  drawSongInformation(songName, artistName);
  drawProgressBar();
  drawCurrentSongProgress(songDuration, currentPosition);
  drawCurrentTimePosition();
  drawCurrentSongDuration();
  drawControls();
}

void drawHeader(){
   // Clear Screen Black 
  tft.fillScreen(ILI9341_BLACK);

  // Set Text Color 
  tft.setTextColor(ILI9341_WHITE);

  //Set Text Size
  tft.setTextSize(2);

  // Set Text Cursor
  tft.setCursor(80,30);

  // Print Text to Screen
  tft.println("SPOTIFY DISPLAY");

  tft.setTextSize(1);

  tft.setCursor(130, 60);

  tft.println("NOW PLAYING");
}

void drawSongInformation(String songName, String artistName){

  // draw current song artist Name
  tft.setTextSize(1);

  tft.setCursor(130, 90);

  tft.println(songName);

  tft.setCursor(130, 120);

  tft.println(artistName);

}

void drawProgressBar(){

  // draw progress bar
  tft.drawLine(20,150,300,150,ILI9341_WHITE);
}

void drawCurrentTimePosition(){

  
  int currentMinutes = currentPosition/60;
  int currentSeconds = currentPosition%60;

  tft.fillRect(70,170, 25,20, ILI9341_BLACK);

   // draw current song current position
  tft.setCursor(70, 170);

  tft.setCursor(70, 170);
  tft.print(currentMinutes);
  tft.print(":");
  tft.print(currentSeconds);

}

void drawCurrentSongDuration(){

  int songMinutes = songDuration/60;
  int songSeconds = songDuration%60;

  // draw current song duration
  tft.setCursor(220,  170);


  tft.print(songMinutes);
  tft.print(":");
  tft.print(songSeconds);

}

void drawCurrentSongProgress(int songDuration, int currentPosition){
  
  // calculate and draw current progress bar
  int progress = ((280 * currentPosition) / songDuration) + 20; 
  tft.drawLine(20,151,progress,151,ILI9341_WHITE);

}



void drawControls(){
   // Draw Skip Backwards Button

  tft.fillTriangle(115,195,120,190, 120,200  ,ILI9341_WHITE);

  tft.fillTriangle(110,195,115,190,115,200,ILI9341_WHITE);

  tft.fillRect(107, 190, 3, 11,ILI9341_WHITE);

  // Draw Pause or Play Button
  tft.fillTriangle(160,195,150,190,150,200,ILI9341_WHITE);
  
  // Draw Skip Forwards Button
  
  tft.fillTriangle(195,195,190,190,190,200,ILI9341_WHITE);

  tft.fillTriangle(200,195,195,190,195,200,ILI9341_WHITE);

  tft.fillRect(200,190,3,11,ILI9341_WHITE);


}

void updateControls(){

  tft.fillRect(140,190, 25,20, ILI9341_BLACK);

  if (!playerState){     
    tft.fillTriangle(160,195,150,190,150,200,ILI9341_WHITE);
  }
  else{
    tft.fillRect(150, 190, 5, 10,ILI9341_WHITE);

    tft.fillRect(160, 190, 5, 10,ILI9341_WHITE);
  }
}
