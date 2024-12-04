#ifndef __SAM1FACTORY_H__
#define __SAM1FACTORY_H__

#include "segmentor.h"

class SAM1Segmentor : public Segmentor {      
  public:  

    void test();           

    SAM1Segmentor() {
      printf("Called SAM1Segmentor constructor\n");
    }
};
  
Segmentor& get_sam1_segmentor();

#endif
