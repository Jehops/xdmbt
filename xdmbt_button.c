#include "xdmbt.h"

static void XCButtonExpose(XC_GUI*, XC_WIDGET*, XEvent*);
static void XCButtonEnter(XC_GUI*, XC_WIDGET*, XEvent*);
static void XCButtonLeave(XC_GUI*, XC_WIDGET*, XEvent*);
static void XCButtonPress(XC_GUI*, XC_WIDGET*, XEvent*);
static void XCButtonRelease(XC_GUI*, XC_WIDGET*, XEvent*);
static void XCButtonDraw(XC_GUI*, XC_WIDGET*);
static void XCButtonDestroy(XC_WIDGET* widget);

int XCButtonCreate(char* value, char** in, char** out, XC_GUI* gui, XC_WIDGET* widget)
{
   int r,w, h;
   XImage *xi_in, *xi_out;

   if( (r = XpmCreateImageFromData(gui->dpy, in, &xi_in, 0, 0)) )
      return r;
   if( (r = XpmCreateImageFromData(gui->dpy, out, &xi_out, 0, 0)) )
      return r;

   /* common attribute */
   widget->xid = 0;
   widget->type = XCTypeButton;
   widget->x = 0;
   widget->y = 0;

   widget->w = w = MAX(xi_in->width, xi_out->width);
   widget->h = h = MAX(xi_in->height, xi_out->height);

   widget->in = 0;
   for(r = 0; r<LASTEvent; r++) widget->slot[r] = 0;
   widget->slot[Expose] = (XC_Handler)XCButtonExpose;
   widget->slot[EnterNotify] = (XC_Handler)XCButtonEnter;
   widget->slot[LeaveNotify] = (XC_Handler)XCButtonLeave;
   widget->slot[ButtonPress] = (XC_Handler)XCButtonPress;
   widget->slot[ButtonRelease] = (XC_Handler)XCButtonRelease;
   widget->event_mask = ExposureMask | ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask;
   widget->destroy = XCButtonDestroy;

   /* specific attribute */
   widget->button.value = value;

   widget->button.in.xi = xi_in;
   widget->button.in.x = (w-xi_in->width)/2;
   widget->button.in.y = (w-xi_in->height)/2;

   widget->button.out.xi = xi_out;
   widget->button.out.x = (w-xi_out->width)/2;
   widget->button.out.y = (w-xi_out->height)/2;

   return 0;
}

static void XCButtonDestroy(XC_WIDGET* widget)
{
   XDestroyImage(widget->button.out.xi);
   XDestroyImage(widget->button.in.xi);
}

static void XCButtonExpose(XC_GUI* gui, XC_WIDGET* w, XEvent* e)
{
   XCButtonDraw(gui, w);
}

static void XCButtonEnter(XC_GUI* gui, XC_WIDGET* widget, XEvent* e)
{
   widget->in = 1;
   XCButtonDraw(gui, widget);
}

static void XCButtonLeave(XC_GUI* gui, XC_WIDGET* widget, XEvent* e)
{
   widget->in = 0;
   XCButtonDraw(gui, widget);
}

static void XCButtonPress(XC_GUI* gui, XC_WIDGET* widget, XEvent* e)
{
   /* Nothing done with this theme */
}

static void XCButtonRelease(XC_GUI* gui, XC_WIDGET* widget, XEvent* e)
{
   if (widget->in) puts(widget->button.value);
   quit = 1;
}

static void XCButtonDraw(XC_GUI* gui, XC_WIDGET* widget)
{
   int x, y, w, h;
   XImage* xi;
   if (widget->in)
   {
      xi = widget->button.in.xi;
      x = widget->button.in.x;
      y = widget->button.in.y;
   }
   else
   {
      xi = widget->button.out.xi;
      x = widget->button.out.x;
      y = widget->button.out.y;
   }
   h = xi->height; w = xi->width;
   XClearWindow(gui->dpy, widget->xid);
   XPutImage(gui->dpy, widget->xid, gui->gc, xi, 0, 0, x, y, w, h);
}

