#include "segmentor_sam1.h"
#include "stdio.h"

int main()
{
    Segmentor& s = get_sam1_segmentor(); 
    s.test();
}
