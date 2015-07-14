/*********************************************
  E07 Timing - my_time.cpp

  Author:
  Susanne Kroemker, IWR - UNIVERSITAET HEIDELBERG
  Im Neuenheimer Feld 368
  D-69120 Heidelberg

  phone +49 (0)6221 54 8883
  fax   +49 (0)6221 54 8850

*********************************************/
#include <stdio.h>
#include <stdlib.h>

#ifdef WIN32

#include <windows.h>

double diff_seconds()
{
  int cur_msec;
  int diff;
  static int last_msec;
  static bool init = false;
  
  cur_msec = GetTickCount();
 
  if(!init)
  {
    init = true;
    last_msec =  cur_msec;
  }
 
  diff = cur_msec-last_msec;
  last_msec = cur_msec;
  
  return(diff/1000.0);
}

#else 

#include <time.h>
#include <sys/time.h>

double diff_seconds()
{
  struct timeval tv;
  int cur_sec;
  int cur_usec;
  double diff;
  static int last_sec;
  static int last_usec;
  static int init = 0;
 
  gettimeofday(&tv, NULL);
 
  cur_sec = tv.tv_sec;
  cur_usec = tv.tv_usec;

  if(!init) {
    init = 1;
    last_sec =  cur_sec;
    last_usec =  cur_usec;
  }
  
  diff= (cur_sec-last_sec)+(cur_usec-last_usec)/1000000.0;

  last_sec = cur_sec;
  last_usec = cur_usec;
 
  return(diff);
}

#endif
