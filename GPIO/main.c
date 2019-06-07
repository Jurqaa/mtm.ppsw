#include <LPC21xx.H>

#define LED0_bm 0x00040000 
#define LED1_bm 0x00080000 
#define LED2_bm 0x00100000 
#define LED3_bm 0x00200000 

#define BUTT1_bm 0x10
#define BUTT2_bm 0x20
#define BUTT3_bm 0x40
#define BUTT4_bm 0x80

void Delay(unsigned int uiTimer){
  
  unsigned int uiLoopCounter;
  
  for(; uiTimer>0; --uiTimer){
    for(uiLoopCounter=0; uiLoopCounter<2418; ++uiLoopCounter){}
  }
}

enum Direction {LEFT, RIGHT};

void LedInit(void){
  IO1DIR = IO1DIR | (LED0_bm | LED1_bm | LED2_bm | LED3_bm); //Ustawia bez zmiany pozostalych rejestrów porty odpowadjace za diody 0,1,2,3 na wyjsciowe. Pozawala na kontrole kierunku poszczególnych pinów, zeby kierunek pozostalych nie zosal zmieniony
//Zero ustawia pin jako wejsciowy, jedynka wyjsciowy
  IO1SET = LED0_bm;
}

void LedOn(unsigned char ucLedIndeks){
  
  IO1CLR = (LED0_bm | LED1_bm | LED2_bm | LED3_bm);
  
  switch (ucLedIndeks){
    case 0:
      IO1SET = LED0_bm;
      break;
    case 1:
      IO1SET = LED1_bm;
      break;
    case 2:
      IO1SET = LED2_bm;
      break;
    case 3:
      IO1SET = LED3_bm;
      break;
    default:
      break;
  }
}

void LedStep(enum Direction eWhichDirection){
  
  static unsigned int uiStepPosition;

  if(eWhichDirection == LEFT){
    ++uiStepPosition;
  }
  else if(eWhichDirection == RIGHT){
    --uiStepPosition;
  }
  
  LedOn(uiStepPosition%4);
}

void LedStepLeft(void){
  LedStep(LEFT);
}

void LedStepRight(void){
  LedStep(RIGHT);
}

enum KeyboardState {RELASED, BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3};

enum KeyboardState eKeyboardRead(void){
  
  if((IO0PIN&BUTT1_bm) == 0){
    return BUTTON_0;
  }
  else if((IO0PIN&BUTT2_bm) == 0){
    return BUTTON_1;
  }
  else if((IO0PIN&BUTT3_bm) == 0){ // PUSZCZONY 0x40  WCISNIETY 0x00
    return BUTTON_2;
  }
  else if((IO0PIN&BUTT4_bm) == 0){ // PUSZCZONY 0x80  WCISNIETY 0x00
    return BUTTON_3;
  }
  else{
    return RELASED;
  }
}

void KeyboardInit(void){
  IO0DIR = IO0DIR & (~(BUTT1_bm | BUTT2_bm | BUTT3_bm | BUTT4_bm));
}

int main(void){

  LedInit();
  KeyboardInit();
  
  while(1){
    
    Delay(500);
    
    switch(eKeyboardRead()){
      case BUTTON_0:
        LedStepRight();
        break;
      case BUTTON_1:
				LedStepLeft();
        break;
      case BUTTON_2:
        break;
      case BUTTON_3:
        break;
      case RELASED:
        break;
      default:
        break;
    }
  }
}

