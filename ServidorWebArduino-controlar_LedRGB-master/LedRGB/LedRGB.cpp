#include "LedRGB.h"

LedRGB::LedRGB(int r, int g, int b){
	pinR= r;
  pinG= g;
  pinB= b;
}

void LedRGB::begin(){
	pinMode(pinB, OUTPUT);
	pinMode(pinG, OUTPUT);
	pinMode(pinR, OUTPUT);
}

void LedRGB::red(int estado){
  digitalWrite(pinR, estado);
}

void LedRGB::green(int estado){
	digitalWrite(pinG, estado);
}

void LedRGB::blue(int estado){
	digitalWrite(pinB, estado);
}
