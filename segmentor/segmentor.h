#ifndef __SEGMENTOR_H__
#define __SEGMENTOR_H__

#include "stdio.h"
  
class Segmentor {      
  public:   

  	virtual void test() = 0; //= 0 means "pure virtual"

    Segmentor() {
      printf("Called Segmentor constructor\n");
    }
};

#endif
