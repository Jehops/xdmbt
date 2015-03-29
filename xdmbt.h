#ifndef __XDMBT__H__
#define __XDMBT__H__

/* to create you own artwork, modify xc_main.c */

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <setjmp.h>

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xft/Xft.h>
#include <X11/xpm.h>
#include <signal.h>
#include <stdio.h>

#define XCTypeButton 2

typedef struct _xc_button
{
   struct
   {
      XImage*	xi;
      int x, y;
   } in, out;
   char* value;
} XC_BUTTON;

typedef void (*XC_Handler)(void*, void*, XEvent*);

typedef struct _xc_widget
{
   Window xid;
   int	type;
   int	x, y, w, h, in;
   int	event_mask;
   XC_Handler slot[LASTEvent]; /* for event response */
   void (*destroy)(struct _xc_widget*);
   XC_BUTTON button;
} XC_WIDGET;

typedef struct _xc_gui
{
   Display* dpy;
   Window root, xid;
   int scr;
   Colormap cmap;
   GC gc;
   Visual* visual;
   XftFont* ttf;
   XftColor background, foreground, selected;
   XC_WIDGET* widget;
   int nwidgets;
} XC_GUI;

/* some MACRO */
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* some global variables */
extern jmp_buf before_loop;
extern int quit;

/* prototype of the functions */

/* file : xc_util.c */
void sigsetQuit(int sig);
int xLibError(Display* dpy, XErrorEvent* e);
int xLibIOError(Display* dpy);

/* file : xc_button.c */
int XCButtonCreate(char* value, char** in, char** out, XC_GUI*, XC_WIDGET*);

#endif
