#include <nds.h>

int main(void) 
{
    irqInit();
    irqEnable(IRQ_VBLANK);

    while (true)
    {
        swiWaitForVBlank();
    }    

	return 0;
}
