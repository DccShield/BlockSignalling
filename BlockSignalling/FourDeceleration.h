#ifndef FOURD_H_
#define FOURD_H_

#include <arduino.h>

#define STOPSTIM 100   // 100*100ms
#define CAUTIONTIM 50  // 50*100ms
#define DECELEATIONTIM 50  // 50*100ms
#define SUPPRESSTIM 3
#define SUPPRESSCNT 10

  enum{
    ST_INIT = 0,
    ST_IDLE,
    ST_STOP,            // 停止
    ST_STOP2,
    ST_STOPDET,
    ST_STOPWAIT,        // 停止待ち
    ST_CAUTION,
    ST_CAUTION2,
    ST_CAUTIONWAIT,     // 注意待ち
    ST_DECELEATIONWAIT, 
    ST_SUPPRESSWAIT,
    ST_ADVANCE,
  };

class FourDeceleration
{
public:
  FourDeceleration( char L1, char L2, char L3, char L4 );
  void statechk( void ); 
  void Detection( void );
  int nowState(void);
  void ChangeState( int );
  
private:
  int state = 0;
  int cnt = 0;
  int cycle = 0;
  int detf = 0;

  char LED1 = 0;
  char LED2 = 0;
  char LED3 = 0;
  char LED4 = 0;
  
  unsigned char onoff;
  
  unsigned long PreviosTimer;


  
  void caution_stops(void);
  void advance(void);
  void Deceleration(void);
  void stops(void);
  void caution(void);
  void blank(void);
};

#endif
