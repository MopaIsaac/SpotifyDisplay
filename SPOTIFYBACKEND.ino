#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <esp_random.h>
#include "mbedtls/md.h"
#include "mbedtls/base64.h"
#include "stdlib.h"
#include <secrets.h>


#include <vector>
#include <string>


// Spotify Server 
const char* serverName = "https://accounts.spotify.com/api/token";

bool isConnected(); // returns if connected to wifi
void postRequest(); // 
void getArtistInformation(String accessToken); // get spotify artist information
void getUsersPlaylist(String accessToken); // get Users playlist
void getCurrentUserProfile(String accessToken); // get Users current profile information
 
String generateRandomString(int length); // generate random string

void printBlockAsHex(std::vector<uint8_t>& data);
void printBlockAsUtf8(std::vector<uint8_t> data);

std::vector<uint8_t> generateSHA256(const std::string& text);
std::string encodeBase64(const std::vector<uint8_t>& data);
std::string base64ToBase64Url(std::string base64);
std::string generateCodeChallenge(const std::string& codeVerifier);



// checks if esp32 is connected to the internet
bool isConnected(){
  if (WiFi.status() == WL_CONNECTED){
    return true;
  }else{
    return false;
  }
}



// connect to wifi network

void connectToWiFi(){
  WiFi.mode(WIFI_STA);

  WiFi.disconnect();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED){
    Serial.print(". ");
    Serial.println(" ");
  }

  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());
  Serial.println(" ");
}


// request spotify access Token
void requestAccessToken(){
  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure();

  http.begin(client, serverName);

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String httpRequestData = String("grant_type=client_credentials&client_id=") + CLIENT_ID + "&client_secret=" + CLIENT_SECRET;

  int httpCode = http.POST(httpRequestData);

  String payload;

  if (httpCode > 0){
    payload = http.getString();
  }else{
    Serial.print("Esp32 Error: ");
    Serial.println(http.errorToString(httpCode));
  }

  JsonDocument doc;

  DeserializationError err = deserializeJson(doc, payload);

  if (err){
    Serial.print("Json Parsing Failed: ");
    Serial.println(err.c_str());
  }else{
    String accessToken = doc["access_token"].as<String>(); 
    String tokenType = doc["token_type"].as<String>();
    int expiresIn = doc["expires_in"];

    Serial.println(accessToken);
    Serial.println(tokenType);
    Serial.println(expiresIn);
  }

  http.end();
}




// CODE VERIFIER AND CODE CHALLENGE


// Generate PKCE code verifier

String generateRandomString(int length){

  const char allowedChars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789" "-._~";
  String randomString = "";

  int charCount = sizeof(allowedChars) - 1;

  for (int i = 0; i < length; i++){
      int randomIndex = esp_random() % charCount;

      char randomChar = allowedChars[randomIndex];

      randomString += randomChar;
  }

  return randomString;
}

// Print raw bytes as hexadecimal

void printBlockAsHex(std::vector<uint8_t>& data)
{
	for(int i = 0; i < data.size(); i++)
	{
		Serial.printf("%02X ", data[i]);
	}
  Serial.println();
}

// SHA-256
std::vector<uint8_t> generateSHA256(const std::string& text)
{
  std::vector<uint8_t> hash(32);

  mbedtls_md_context_t ctx;

  const mbedtls_md_info_t* mdInfo =
    mbedtls_md_info_from_type(MBEDTLS_MD_SHA256);

  mbedtls_md_init(&ctx);

  mbedtls_md_setup(
    &ctx,
    mdInfo,
    0
  );

  mbedtls_md_starts(&ctx);

  mbedtls_md_update(
    &ctx,
    (const unsigned char*)text.c_str(),
    text.length()
  );

  mbedtls_md_finish(
    &ctx,
    hash.data()
  );

  mbedtls_md_free(&ctx);

  return hash;
}

void printBlockAsUtf8(std::vector<uint8_t> data)
{
	for(int i = 0; i < data.size(); i++)
	{
		Serial.print((char)data[i]);
	}
}

// Normal Base64 encoding 

std::string encodeBase64(const std::vector<uint8_t>& data)
{
  size_t encodedLength =
    4 * ((data.size() + 2) / 3);

  std::vector<unsigned char> encoded(
    encodedLength + 1
  );

  size_t outputLength = 0;

  int result = mbedtls_base64_encode(
    encoded.data(),
    encoded.size(),
    &outputLength,
    data.data(),
    data.size()
  );

  if (result != 0)
  {
    return "";
  }

  return std::string(
    (char*)encoded.data(),
    outputLength
  );
}


// Convert Base64 into Base64URL

std::string base64ToBase64Url(std::string base64)
{
    // Base64URL replacements
    for (char& c : base64)
    {
        if (c == '+')
            c = '-';
        else if (c == '/')
            c = '_';
    }

    // Remove = padding from the end
    while (!base64.empty() && base64.back() == '=')
    {
        base64.pop_back();
    }

    return base64;
}

// Generate PKCE code challenge

std::string generateCodeChallenge(
  const std::string& codeVerifier
)
{
  std::vector<uint8_t> hash =
    generateSHA256(codeVerifier);

  std::string base64 =
    encodeBase64(hash);

  return base64ToBase64Url(base64);
}




