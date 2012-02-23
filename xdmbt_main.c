#include "xdmbt.h"

#include "reboot_in.xpm"
#include "reboot_out.xpm"
#include "shutdown_in.xpm"
#include "shutdown_out.xpm"

/* the colors, use X11 named color */
#define XC_BACKGROUND "darkgreen"
#define XC_FOREGROUND "white"
#define XC_SELECTED   "red"

/* Allocation and initialization of the global vars */
jmp_buf before_loop;
int quit = 0;

int main(int argc, char** argv)
{
   XC_GUI gui;
   XC_WIDGET widget[2];
   XEvent e;
   int	wn, i, fr, w, h, x, y, bw, bh;

   gui.widget = widget;
   gui.dpy = XOpenDisplay(NULL);
   gui.scr = DefaultScreen(gui.dpy);
   gui.root = XRootWindow(gui.dpy, gui.scr);
   gui.cmap = DefaultColormap(gui.dpy, gui.scr);
   gui.visual = DefaultVisual(gui.dpy, gui.scr);
   gui.gc = DefaultGC(gui.dpy, gui.scr);

   sigset(SIGINT, sigsetQuit);
   XSetErrorHandler(xLibError);
   XSetIOErrorHandler(xLibIOError);

   XftColorAllocName(gui.dpy, gui.visual, gui.cmap, XC_BACKGROUND, &gui.background);
   XftColorAllocName(gui.dpy, gui.visual, gui.cmap, XC_FOREGROUND, &gui.foreground);
   XftColorAllocName(gui.dpy, gui.visual, gui.cmap, XC_SELECTED, &gui.selected);

   wn = 0;
   XCButtonCreate("reboot", reboot_in_xpm, reboot_out_xpm, &gui,widget+wn);
   wn++;
   XCButtonCreate("shutdown",shutdown_in_xpm,shutdown_out_xpm,&gui,widget+wn);
   wn++;
   gui.nwidgets = wn;

   w = 0; h = 0; bw = 0; bh = 0; fr = 1;
   /* size of the window */
   for(i=0; i<wn; i++)
   {
      if (widget[i].h > bh)
	 bh = widget[i].h;
      bw += widget[i].w;
   }
   if (w < bw) w = bw;
   h += bh;

   gui.xid = XCreateSimpleWindow(gui.dpy, gui.root, 0, 0, w, h, 0, 0, gui.background.pixel);

   /* create and place the element on the window */
   /* align buttons to the right */
   x = w;
   y = 0;
   for(i=wn-1; i>=0; i--)
   {
      x -= widget[i].w ;
      widget[i].xid = XCreateSimpleWindow(gui.dpy, gui.xid, x, y, widget[i].w, widget[i].h, 0, 0, gui.background.pixel);
      XSelectInput(gui.dpy, widget[i].xid,  widget[i].event_mask);
      widget[i].x = x; widget[i].y = y;
   }

   XMoveWindow(gui.dpy, gui.xid, DisplayWidth(gui.dpy, gui.scr) - w - 10, DisplayHeight(gui.dpy, gui.scr) - h - 10);
   XMapWindow(gui.dpy, gui.xid);
   XMapSubwindows(gui.dpy, gui.xid);
   XMoveWindow(gui.dpy, gui.xid, DisplayWidth(gui.dpy, gui.scr) - w - 10, DisplayHeight(gui.dpy, gui.scr) - h - 10);

   /* The Window Main Loop : make an interruptible XNextEvent */
   if (setjmp(before_loop) == 0)
      while(!quit)
      {
	 fd_set fds; int ret; Window xid;
	 XFlush(gui.dpy);
	 while (XPending(gui.dpy))
	 {
	    XNextEvent(gui.dpy, &e);
	    xid = e.xany.window;
	    for(i=0; i<wn; i++)
	       if (xid == widget[i].xid)
	       {
		  if (widget[i].slot[e.type])
		     widget[i].slot[e.type](&gui, widget+i, &e);
		  break;
	       }
	 }
	 if (quit) break;
	 FD_ZERO(&fds);
	 FD_SET(ConnectionNumber(gui.dpy), &fds);
	 /* non active wait */
	 ret = select(ConnectionNumber(gui.dpy) + 1, &fds, 0, 0, 0);
      }

   for(i=0; i<wn; i++)
      widget[i].destroy(widget+i);
//    XftFontClose(gui.dpy, gui.ttf);
   XftColorFree(gui.dpy, gui.visual, gui.cmap, &gui.background);
   XftColorFree(gui.dpy, gui.visual, gui.cmap, &gui.foreground);
   XftColorFree(gui.dpy, gui.visual, gui.cmap, &gui.selected);
   XCloseDisplay(gui.dpy);
   return 0;
}
