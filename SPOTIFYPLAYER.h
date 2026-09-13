#ifndef SPOTIFYPLAYER_H
#define SPOTIFYPLAYER_H

#include <Arduino.h>

struct Track {
  String songName;
  String artistName;

  int durationMs;
  int progressMs;

  bool isPlaying;
};

extern Track currentTrack;

#endif