#include "HobbyRadioReceiver.h"
#include <Wire.h>
#include <math.h>

#define FILTRO_CONTROLE 50

int x;
int y;
int val;
int calibragemx, calibragemy;

uint8_t vetor[6] = {218, 130, 0, 1, 0, 1};


HobbyRadioReceiver rec( 2, A0, A1);

void setup()
{
  Serial.begin(9600);
  Serial.print( "Num Channels: " );
  Serial.println(rec.getNumChannels());

  Wire.begin(0x52);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event

  calibragemx = rec.check(1) * -1;  
  calibragemy = rec.check(2) * -1;
}


void controle(int vel, int dir, int limite){
  if(vel >= limite) vel = limite;
  if(dir >= limite) dir = limite;
  if(vel <= -limite) vel = -limite;
  if(dir <= -limite) dir = -limite;
  
  if(vel >= 10 || vel <= -10){
    y = 130 + vel;
    if(y < 35) y = 35;
    if(y > 230) y = 230;
    
    x = 130 + dir;
    if(x < 35) x = 35;
    if(x > 230) x = 230;
    
  }else{
    x = 130;
    y = 123;
  }

}


void converte(){
  int sinalx, sinaly;
  int direcao,velocidade;
  
  sinalx = rec.check(1) + calibragemx;  
  sinaly = rec.check(2) + calibragemy;

  if(sinalx >= -FILTRO_CONTROLE && sinalx <= FILTRO_CONTROLE) sinalx = 0;
  if(sinaly >= -FILTRO_CONTROLE && sinaly <= FILTRO_CONTROLE) sinaly = 0;

  sinalx += 255;
  sinalx /= 2;
  sinalx -= 128;

  direcao = (sinalx * 100) / 256;
  
  sinaly += 255;
  sinaly /= 2;
  sinaly -= 128;
  
  velocidade = (sinaly * 100) / 256;

  velocidade *= 2;
  direcao *= 2;

  if(velocidade < 0) direcao *= -1;
  
  controle(-velocidade, direcao,100);
}

void loop()
{
  
  converte();
//  Serial.print(x);
//  Serial.print(" ");
//  Serial.println(y);

}


void requestEvent() {
  int i;
  vetor[0] = x;
  vetor[1] = y;
  for(i = 0; i < 6; i++){
    Wire.write(vetor,6);
  }
}
