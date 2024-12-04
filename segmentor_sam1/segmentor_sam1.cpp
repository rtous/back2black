#include "stdio.h"
#include "segmentor_sam1.h"

Segmentor& get_sam1_segmentor() 
{
    static SAM1Segmentor s1;
    return s1;
}

void SAM1Segmentor::test() 
{
    printf("called SAM1Segmentor::test()\n");
};
