// 
// 閉塞信号機アクセサリ
// http://dcc.client.jp
// http://ayabu.blog.shinobi.jp

#include <arduino.h>
#include "Cds.h"
//#include "3ColorLightSignal.h"
#include "FourDeceleration.h"
//#include "FourVigillance.h"

#define DEBUG      //リリースのときはコメントアウトすること
//#define DEBUG_M    //リリースのときはコメントアウトすること 速度・ファンクションパケット表示

unsigned long gPreviousL5 = 0;
char DetectionLevel = LOW;

//各種設定、宣言
//ThirdColorLightSignal FirstSignal(9,10,11,12);
//ThirdColorLightSignal SecondSignal(5,6,7,8);
FourDeceleration FirstSignal(9,10,11,12);
FourDeceleration SecondSignal(5,6,7,8);
Cds FirstSensor(A0, 2, 200,300);
Cds SecondSensor(A1, 2, 200,300);
 
//------------------------------------------------------------------
// Arduino固有の関数 setup() :初期設定
//------------------------------------------------------------------
void setup()
{
  char sw;
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("Hello,Smile Function Decoder for Servo motor");
#endif
  
  pinMode(13, OUTPUT);  // Arduino NANO 上のLED
  digitalWrite(13, LOW);
  
  //Reset task
  gPreviousL5 = millis();
}

//---------------------------------------------------------------------
// Arduino main loop
//---------------------------------------------------------------------
void loop() {
  if((millis() - gPreviousL5) >= 100){
    gPreviousL5 = millis();
    BlockingSignalState();
    FirstSignal.statechk();
    SecondSignal.statechk();
  }
}

void BlockingSignalState( void ){
  enum{
      STS_INIT = 0,
      STS_IDLE,
  };
  static int state = STS_INIT;

  switch(state){
    case STS_INIT:
                  FirstSignal.ChangeState(ST_ADVANCE);
                  SecondSignal.ChangeState(ST_ADVANCE);
                  state = STS_IDLE;
                  break;
    case STS_IDLE:
                  if(FirstSensor.statechk(DetectionLevel) == 1){
                    FirstSensor.Reset();
                    FirstSignal.ChangeState(ST_STOPDET);
                  }
                  if(SecondSensor.statechk(DetectionLevel) == 1){
                    SecondSensor.Reset();
                    SecondSignal.ChangeState(ST_STOPDET);
                  }
                  break;
    default:
                  break;
  }
  
}
