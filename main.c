#include <stdio.h>
#include "mainloop.h"

int main()
{
    t_mainloop *mainloop;
//    mainloop = construct_mainloop((t_ivec2){{1366, 768}}, "Realtime raytrasing");
    mainloop = construct_mainloop((t_ivec2){{800, 600}}, "Realtime raytrasing");

    mainloop->run(mainloop);
    return 0;
}
