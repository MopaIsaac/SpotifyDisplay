#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <vector>

using namespace std;


#define CS 10
#define DC 9
#define RST 14

#define MOSI 11
#define SCK 12
#define MISO 13 

// button Pins

const int playButton = 35;
bool  playButtonState = HIGH ;
bool lastPlayButtonState = HIGH;
unsigned long lastPlayDebounceTime = 0;
unsigned long debouncePlayDelay = 50;


const int nextButton = 36;
bool  nextButtonState = HIGH ;
bool lastNextButtonState = HIGH;
unsigned long lastNextDebounceTime = 0;
unsigned long debounceNextDelay = 50;

const int prevButton = 37;
bool  prevButtonState = HIGH ;
bool lastPrevButtonState = HIGH;
unsigned long lastPrevDebounceTime = 0;
unsigned long debouncePrevDelay = 50;


// Display Object
Adafruit_ILI9341 tft = Adafruit_ILI9341(CS,DC,RST);

//Current Song in Queue
int currentSong = 0;

//Song Information
int currentPosition = 0;

// player state
bool playerState = false;


unsigned long previousTime = 0;

// song structure
struct songInformation{
  String songName;
  String artistName;
  int songDuration;

};

// vector object to store songs 
std::vector<songInformation> songQueue;


void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);

  SPI.begin(SCK,MISO,MOSI,CS);

  // Start the Display
  tft.begin();

  //rotate display
  tft.setRotation(1);

  // add songs to vector 
  songQueue.push_back(songInformation{"Hotline Bling", "Drake" , 200});
  songQueue.push_back(songInformation{"God's Plan", "Drake" , 120});
  songQueue.push_back(songInformation{"Talking The Hardest", "Giggs" , 320});

  // draw display UI
  drawSpotifyScreen();

  pinMode(playButton, INPUT);
  pinMode(nextButton, INPUT);
  pinMode(prevButton, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentTime = millis();

  // implemented deboucing

  // play button reading

  bool playReading = digitalRead(playButton);

  if (playReading != lastPlayButtonState){
    lastPlayDebounceTime = millis();
  }

  if (millis() - lastPlayDebounceTime > debouncePlayDelay){
    if (playReading != playButtonState){
      playButtonState = playReading;

      if (playButtonState == LOW){
        playerState = !playerState;
        updateControls();
      }
    }
  }

  lastPlayButtonState = playReading;

  // next button reading 

  bool nextReading = digitalRead(nextButton);

  if (nextReading != lastNextButtonState){
    lastNextDebounceTime = millis();
  }

  if (millis() - lastNextDebounceTime > debounceNextDelay){
    if (nextReading != nextButtonState){
      nextButtonState = nextReading;

      if (nextButtonState == LOW){
        if ((currentSong + 1) < songQueue.size()){
        currentSong += 1;
        }else{
          currentSong = 0;
        }
        updateSongInformation();
      
      }
    }
  }
  
  lastNextButtonState = nextReading;

  //prev button reading

  bool prevReading = digitalRead(prevButton);

  if (prevReading != lastPrevButtonState){
    lastPrevDebounceTime = millis();
  }

  if (millis() - lastPrevDebounceTime > debouncePrevDelay){
    if (prevReading != prevButtonState){
      prevButtonState = prevReading;

      if (prevButtonState == LOW){
        if ( currentSong > 0){
        currentSong -= 1; 
        }else{
          currentSong = (songQueue.size() - 1);
        }
        updateSongInformation();
      }
    }
  }
  
  lastPrevButtonState = prevReading;

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
      if (currentPosition < songQueue[currentSong].songDuration){
        currentPosition += 1;
        drawCurrentSongProgress(currentPosition);   
        drawCurrentTimePosition();
        
      }
    }
  } 
}

void drawSpotifyScreen(){
  drawHeader();
  drawSongInformation();
  drawProgressBar();
  drawCurrentSongProgress(currentPosition);
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

void drawSongInformation(){

  // draw current song artist Name
  tft.setTextSize(1);

  tft.setCursor(130, 90);

  tft.println(songQueue[currentSong].songName);

  tft.setCursor(130, 120);

  tft.println(songQueue[currentSong].artistName);

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

  int songMinutes = (songQueue[currentSong].songDuration)/60;
  int songSeconds = (songQueue[currentSong].songDuration)%60;

  // draw current song duration
  tft.setCursor(220,  170);


  tft.print(songMinutes);
  tft.print(":");
  tft.print(songSeconds);

}

void drawCurrentSongProgress(int currentPosition){
  
  // calculate and draw current progress bar
  int progress = ((280 * currentPosition) / songQueue[currentSong].songDuration) + 20; 
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

  // update controls state

  tft.fillRect(140,190, 25,20, ILI9341_BLACK);

  if (!playerState){     
    tft.fillTriangle(160,195,150,190,150,200,ILI9341_WHITE);
  }
  else{
    tft.fillRect(150, 190, 5, 10,ILI9341_WHITE);

    tft.fillRect(160, 190, 5, 10,ILI9341_WHITE);
  }
}

void updateSongInformation(){
  // update song information when song change 
  playerState = false;
  currentPosition = 0;

  updateControls();

  // clear previous song information and update with current song 
  tft.fillRect(130, 90, 120, 15, ILI9341_BLACK); // song name

  tft.fillRect(130, 120, 110, 15, ILI9341_BLACK); // artist name

  drawSongInformation();

  // clear previous song progress
  tft.fillRect(20,151,280,2,ILI9341_BLACK);
  drawCurrentSongProgress(currentPosition);

  // clear previous song duration and update with current song
  tft.fillRect(220, 170, 30, 10, ILI9341_BLACK); // song duration 

  drawCurrentSongDuration();
}
