#include <arduino.h>
#include "FourVigillance.h"

FourVigillance::FourVigillance(void)
{
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
}

void FourVigillance::Detection(void)
{
  detf = HIGH;
//  state = ST_STOP;
}

int FourVigillance::nowState(void)
{
  return state; 
}

void FourVigillance::statechk( void ) 
{
static int prestate = 0;
if(prestate != state ){
  Serial.println(state);
  prestate = state;
}  
  switch(state){
    case ST_INIT:
              advance();
              state = ST_IDLE;
              break;
      case ST_IDLE:     //1
              if(detf == HIGH){
                detf = LOW;
                state = ST_STOP;
              }
              break; 
    case ST_STOP:
              stops();
              cnt = 0;
              state = ST_STOPWAIT;
              break;
              
      case ST_STOPWAIT:
              cnt++;
              if( cnt > STOPSTIM ) {
                cnt = 0;
                vigilance();
                state = ST_VIGILANCEWAIT;
              }
              break;   
    case   ST_VIGILANCEWAIT:
              cnt++;
              if( cnt > VIGILANCETIM ) {
                cnt = 0;
                caution();
                state = ST_CAUTIONWAIT;
              }
              break;
    case ST_CAUTIONWAIT:
              cnt++;
              if( cnt > CAUTIONTIM ) {
                cnt = 0;
                advance();
               state = ST_IDLE;
              }
              break;

    default:
              break;
    }
  }

    
void FourVigillance::advance(void) // 進行
{
   digitalWrite(9, LOW);
   digitalWrite(10, LOW);
   digitalWrite(11, HIGH);
   digitalWrite(12, LOW);
}

void FourVigillance::vigilance(void) // 警戒
{
   digitalWrite(9, HIGH);
   digitalWrite(10, LOW);
   digitalWrite(11, LOW);
   digitalWrite(12, HIGH);
}

void FourVigillance::caution(void) // 注意
{
   digitalWrite(9, HIGH);
   digitalWrite(10, LOW);
   digitalWrite(11, HIGH);
   digitalWrite(12, HIGH);
}

void FourVigillance::stops(void) // 停止
{
   digitalWrite(9, LOW);
   digitalWrite(10, HIGH);
   digitalWrite(11, HIGH);
   digitalWrite(12, HIGH);
}
