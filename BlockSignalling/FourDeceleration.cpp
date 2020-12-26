// 4現示抑速
#include <arduino.h>
#include "FourDeceleration.h"

FourDeceleration::FourDeceleration(char L1, char L2, char L3, char L4)
{
  LED1 = L1;
  LED2 = L2;
  LED3 = L3;
  LED4 = L4;
  
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
}

void FourDeceleration::Detection(void)
{
  detf = HIGH;
}

int FourDeceleration::nowState()
{
  return state; 
}

void FourDeceleration::ChangeState( int st)
{
  state = st; 
}

void FourDeceleration::statechk( void ) 
{
static int prestate = 0;
if(prestate != state ){
  Serial.print((int)LED1,DEC);
  Serial.print(",");
  Serial.println(state);
  prestate = state;
}
  switch(state){
      case ST_INIT:
              stops();
              state = ST_IDLE;
              break;

      case ST_IDLE:     //1
              break;

      case ST_STOP:     //2 停止
              blank();
              state = ST_STOP2;
              break;

      case ST_STOP2:     //2
              stops();
              state = ST_IDLE;
              break;

      case ST_STOPDET: //3
              stops();
              cnt = 0;
              state = ST_STOPWAIT;      
              break;
      
      case ST_STOPWAIT: //3
              cnt++;
              if( cnt > STOPSTIM ) {
                cnt = 0;
//                caution();
//                state = ST_CAUTIONWAIT;
                state = ST_CAUTION;
              }
              break;

      case ST_CAUTION:     //5 停止->注意同時　注意:制限 40km/h
              caution_stops();
              state = ST_CAUTION2;
              break;
              
      case ST_CAUTION2:
              caution();
              cnt = 0;
              state = ST_CAUTIONWAIT;
              break;
              
      case ST_CAUTIONWAIT://4
              cnt++;
              if( cnt > CAUTIONTIM) {
                cnt = 0;
                cycle = 0;
                Deceleration();
                state = ST_DECELEATIONWAIT;
              }
              break;

      case ST_DECELEATIONWAIT:  //5 減速:制限 70km/h
              cnt++;
              if( cnt > DECELEATIONTIM) {
                cnt = 0;
                cycle = 0;
                Deceleration();
                state = ST_SUPPRESSWAIT;
              }
              break;

      case ST_SUPPRESSWAIT:  //5 減速:制限 100km/h 点滅処理
              cnt++;
              if( cnt > SUPPRESSTIM) {
                cnt = 0;
                if(onoff == 0) {
                  blank();
                  onoff = 1;
                } else {
                  Deceleration();
                  onoff = 0;
                  cycle++;
                }
                if( cycle >= SUPPRESSCNT ){
                  blank();
                  state = ST_ADVANCE;
                }
              }
              break;

      case ST_ADVANCE:     //7
              advance();
              state = ST_IDLE;
              break;
              
      default:
              break;
    }
}



void FourDeceleration::advance(void) // 進行
{
   digitalWrite(LED1, LOW);
   digitalWrite(LED2, LOW);
   digitalWrite(LED3, HIGH);
   digitalWrite(LED4, LOW);
}


void FourDeceleration::Deceleration(void) // 減速
{
   digitalWrite(LED1, HIGH);
   digitalWrite(LED2, LOW);
   digitalWrite(LED3, HIGH);
   digitalWrite(LED4, LOW);
}


void FourDeceleration::caution(void) // 注意
{
   digitalWrite(LED1, HIGH);
   digitalWrite(LED2, LOW);
   digitalWrite(LED3, HIGH);
   digitalWrite(LED4, HIGH);
}

void FourDeceleration::caution_stops(void) // 注意
{
   digitalWrite(LED1, LOW);
   digitalWrite(LED2, LOW);
   digitalWrite(LED3, HIGH);
   digitalWrite(LED4, HIGH);
}


void FourDeceleration::stops(void) // 停止
{
   digitalWrite(LED1, LOW);
   digitalWrite(LED2, HIGH);
   digitalWrite(LED3, HIGH);
   digitalWrite(LED4, HIGH);
}

void FourDeceleration::blank(void) // 消灯
{
   digitalWrite(LED1, LOW);
   digitalWrite(LED2, LOW);
   digitalWrite(LED3, LOW);
   digitalWrite(LED4, LOW);
}
