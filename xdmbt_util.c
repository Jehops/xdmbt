#include "xdmbt.h"

void sigsetQuit(int sig) { quit = 1; }

int xLibError(Display* dpy, XErrorEvent* e)
{
   char text[1024];
   XGetErrorText(dpy, e->error_code, text, sizeof(text));
   fprintf(stderr,"XLib Error: %s\n", text);
   longjmp(before_loop, 1);
}

int xLibIOError(Display* dpy)
{
   fprintf( stderr,"Fatal XLib IO Error\n" ) ;
   longjmp(before_loop, 2);
}
