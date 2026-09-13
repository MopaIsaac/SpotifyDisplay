#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <esp_random.h>
#include "mbedtls/md.h"
#include "mbedtls/base64.h"
#include "stdlib.h"
#include <secrets.h>
#include "SPOTIFYBACKEND.h"
#include "SPOTIFYPLAYER.h"

Track currentTrack;

// GET IF SONG IS CURRENTLY PLAYING 
void getPlayBackState(){

  String url = "https://api.spotify.com/v1/me/player";

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  http.begin(client , url);

  http.addHeader("Authorization", "Bearer " + accessToken);

  int httpRequestCode = http.GET();

  if (httpRequestCode >= 200 && httpRequestCode <= 300){
    Serial.print("Playback State HTTP code: ");
    Serial.println(httpRequestCode);

    String payload = http.getString();
    
    JsonDocument doc;

    DeserializationError err = deserializeJson(doc,payload);

    if (err){
      Serial.print("Json Parson Failed: ");
      Serial.println(err.c_str());
    }else{
      if (httpRequestCode == 204){
        currentTrack.isPlaying = false;
      }else{
      currentTrack.isPlaying = doc["is_playing"];
      Serial.println(currentTrack.isPlaying);
      }
    }

  }else{
    Serial.println(http.errorToString(httpRequestCode));
  }

  http.end();
}

// CHANGE PLAYER STATE (PAUSE)
void pausePlayState(){

  String url = "https://api.spotify.com/v1/me/player/pause";

  WiFiClientSecure client;
  HTTPClient http;


  client.setInsecure();

  http.begin(client, url);

  http.addHeader("Authorization", "Bearer " + accessToken);

  http.addHeader("Content-Length", "0");

  int httpRequestCode = http.PUT("");

  Serial.print("Playback HTTP code: ");
  Serial.println(httpRequestCode);

  if (httpRequestCode >= 200 && httpRequestCode <= 300){
    // String payload = http.getString();
    // Serial.println(payload);
  }else{
    Serial.println(http.errorToString(httpRequestCode));
  }

  http.end();

}

// CHANGE PLAYER STATE (PLAY)
void playPlayState(){

  String url = "https://api.spotify.com/v1/me/player/play";

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  http.begin(client,url);


  http.addHeader("Authorization", "Bearer " + accessToken);
  http.addHeader("Content-Length", "0");

  int httpRequestCode = http.PUT("");

  Serial.print("Playback HTTP code: ");
  Serial.println(httpRequestCode);

  if (httpRequestCode >= 200 && httpRequestCode <= 300){
    // String payload = http.getString();
    // Serial.println(payload);
  }else{
    Serial.println(http.errorToString(httpRequestCode));
  }

  http.end();

}


// GET CURRENT USERS SONG 
void getCurrentPlayingSong(){
  String url = "https://api.spotify.com/v1/me/player/currently-playing";

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  http.begin(client, url);

  http.addHeader("Authorization", "Bearer " + accessToken);

  int httpRequestCode = http.GET();

   if (httpRequestCode >= 200 && httpRequestCode <= 300){
    Serial.print("Currently Playing HTTP code: ");
    Serial.println(httpRequestCode);

    String payload = http.getString();
    
    JsonDocument doc;

    DeserializationError err = deserializeJson(doc,payload);

    if (err){
      Serial.print("Json Parson Failed: ");
      Serial.println(err.c_str());
    }else{
      currentTrack.songName = doc["item"]["name"].as<String>();
      currentTrack.artistName = doc["item"]["artists"][0]["name"].as<String>();
      currentTrack.durationMs = doc["item"]["duration_ms"];
      currentTrack.progressMs = doc["progress_ms"];


      Serial.println(currentTrack.artistName);
      Serial.println(currentTrack.songName);
      Serial.println(currentTrack.durationMs);
      Serial.println(currentTrack.progressMs);
    }



  }else{
    Serial.println(http.errorToString(httpRequestCode));
  }

  http.end();
}


void skipToNext(){

  String url = "https://api.spotify.com/v1/me/player/next";

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  http.begin(client, url);

  http.addHeader("Authorization", "Bearer " + accessToken);
  http.addHeader("Content-Length", "0");

  int httpRequestCode = http.POST("");

  Serial.println("Skip HTTP Code: ");
  Serial.println(httpRequestCode);

  if (httpRequestCode != 204|| httpRequestCode != 200){
    Serial.println(http.errorToString(httpRequestCode));
  }

  http.end();
}


void skipToPrevious(){

  String url = "https://api.spotify.com/v1/me/player/previous";

  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  http.begin(client, url);

  http.addHeader("Authorization", "Bearer " + accessToken);
  http.addHeader("Content-Length", "0");

  int httpRequestCode = http.POST("");

  Serial.println("Skip HTTP Code: ");
  Serial.println(httpRequestCode);

  if (httpRequestCode != 204 || httpRequestCode != 200){
    Serial.println(http.errorToString(httpRequestCode));
  }

  http.end();
}
















































void requestCurrentUserProfile(String accessToken){
  String url = "https://api.spotify.com/v1/me";
  
  WiFiClientSecure client;
  HTTPClient http;
  
  client.setInsecure();

  http.begin(client, url);

  http.addHeader("Authorization", "Bearer " + accessToken);

  int httpCode = http.GET();

 if (httpCode >= 200 && httpCode < 300) {

    String payload = http.getString();

    JsonDocument doc;

    DeserializationError err =
      deserializeJson(doc, payload);

    if (err) {
      Serial.print("JSON parsing failed: ");
      Serial.println(err.c_str());
    }
    else {

      String displayName =
        doc["display_name"].as<String>();

      String id =
        doc["id"].as<String>();

      String country =
        doc["country"].as<String>();

      String email =
        doc["email"].as<String>();

      String product =
        doc["product"].as<String>();

      int followers =
        doc["followers"]["total"];

      Serial.println("----- PROFILE -----");

      Serial.print("Name: ");
      Serial.println(displayName);

      Serial.print("ID: ");
      Serial.println(id);

      Serial.print("Country: ");
      Serial.println(country);

      Serial.print("Email: ");
      Serial.println(email);

      Serial.print("Product: ");
      Serial.println(product);

      Serial.print("Followers: ");
      Serial.println(followers);
    }
  }
  else {

    Serial.print("Spotify response: ");

    String payload = http.getString();
    Serial.println(payload);
  }

  http.end();
}




