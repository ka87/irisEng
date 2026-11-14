/* The window messages win/wingdi.c dispatches on, with the values the
 * real SDK gives them, forced into both sides of the differential test
 * with -include.
 *
 * tests/stub/windows.h numbers these names sequentially, which is right
 * for the parse sweep and for telling two names apart, but it puts
 * WM_CREATE, WM_DESTROY, WM_ERASEBKGND and WM_MOUSEFIRST *above*
 * WM_CLOSE and WM_ACTIVATEAPP where Win32 puts them below. The two
 * message handlers in this file both open with a range test -- the
 * open/close handler with `msg > WM_CLOSE`, PC_WNDMsgHandler with
 * `msg > WM_ACTIVATEAPP` -- so under the sequential values every other
 * arm of both handlers is unreachable and no input can enter them.
 *
 * Values from the Windows SDK (winuser.h).
 */
#pragma once
#include <windows.h>

#undef  WM_CREATE
#define WM_CREATE                 0x0001
#undef  WM_DESTROY
#define WM_DESTROY                0x0002
#undef  WM_CLOSE
#define WM_CLOSE                  0x0010
#undef  WM_QUIT
#define WM_QUIT                   0x0012
#undef  WM_ERASEBKGND
#define WM_ERASEBKGND             0x0014
#undef  WM_ACTIVATEAPP
#define WM_ACTIVATEAPP            0x001C
#undef  WM_NCLBUTTONDOWN
#define WM_NCLBUTTONDOWN          0x00A1
#undef  WM_SYSCOMMAND
#define WM_SYSCOMMAND             0x0112
#undef  WM_MOUSEFIRST
#define WM_MOUSEFIRST             0x0200

/* the four the overlay's command field consumes, so PC_WNDMsgHandler's
   range test sees them where Win32 puts them */
#undef  WM_KEYDOWN
#define WM_KEYDOWN                0x0100
#undef  WM_KEYUP
#define WM_KEYUP                  0x0101
#undef  WM_CHAR
#define WM_CHAR                   0x0102
#undef  WM_SYSCHAR
#define WM_SYSCHAR                0x0106

#undef  SC_CLOSE
#define SC_CLOSE                  0xF060
