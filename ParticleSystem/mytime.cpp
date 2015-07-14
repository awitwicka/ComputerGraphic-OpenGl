#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <sys/time.h>


#define false 0
#define true 1

double diff_seconds()
{
  struct timeval tv;
  int diese_sec;
  int diese_usec;
  double diff;
  static int letzte_sec;
  static int letzte_usec;
  static int initialisiert=false;
 
  gettimeofday(&tv, NULL);
 
  diese_sec=tv.tv_sec;
  diese_usec=tv.tv_usec;

  if(!initialisiert) {
    initialisiert=true;
    letzte_sec =  diese_sec;
    letzte_usec =  diese_usec;
  }
 
  diff=(diese_sec - letzte_sec) + (diese_usec - letzte_usec)/1000000.0;

  letzte_sec =  diese_sec;
  letzte_usec =  diese_usec;
 
  return diff;
}

