#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>


#define CS 10
#define DC 9
#define RST 14

#define MOSI 11
#define SCK 12
#define MISO 13 

Adafruit_ILI9341 tft = Adafruit_ILI9341(CS,DC,RST);

//Song Information

int currentPosition = 30;
int songDuration = 120;

String songName = String("In The Morning");    
String artistName = String("J Cole");    

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  SPI.begin(SCK,MISO,MOSI,CS);

  // Start the Display
  tft.begin();

  //rotate display
  tft.setRotation(1);

  drawSpotifyScreen();
 


}

void loop() {
  // put your main code here, to run repeatedly:
  drawProgressBar();

}

void drawSpotifyScreen(){
  drawHeader();
  drawSongInformation(songName, artistName);
  drawProgressBar();
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


  int songMinutes = songDuration/60;
  int songSeconds = songDuration%60;

  int currentMinutes = currentPosition/60;
  int currentSeconds = currentPosition%60;

  // draw progress bar

  tft.drawLine(20,150,300,150,ILI9341_WHITE);

  drawCurrentSongProgress(songDuration, currentPosition);

  // draw current song current position
  tft.setCursor(70, 170);

  tft.print(currentMinutes);
  tft.print(":");
  tft.print(currentSeconds);
  // draw current song duration
  tft.setCursor(220,  170);


  tft.print(songMinutes);
  tft.print(":");
  tft.print(songSeconds);
}

void drawCurrentSongProgress(int songDuration, int currentPosition){
  
  // calculate and draw current progress bar
  int progress = (280 * currentPosition) / songDuration; 

  tft.drawLine(20,151,progress,151,ILI9341_WHITE);

}



void drawControls(){
   // Draw Skip Backwards Button

  tft.fillTriangle(115,195,120,190, 120,200  ,ILI9341_WHITE);

  tft.fillTriangle(110,195,115,190,115,200,ILI9341_WHITE);

  tft.fillRect(107, 190, 3, 11,ILI9341_WHITE);

  // Draw Pause Button

  tft.fillRect(150, 190, 5, 10,ILI9341_WHITE);

  tft.fillRect(160, 190, 5, 10,ILI9341_WHITE);

  // Draw Skip Forwards Button
  
  tft.fillTriangle(195,195,190,190,190,200,ILI9341_WHITE);

  tft.fillTriangle(200,195,195,190,195,200,ILI9341_WHITE);

  tft.fillRect(200,190,3,11,ILI9341_WHITE);


}
