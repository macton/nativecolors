#include <curspriv.h>
#include <string.h>

// For future reference if extending to 256 color mode:
// http://lucentbeing.com/blog/that-256-color-thing/
//
// Anatomy of a Color Code
//
//   The general structure of a color code is:
//
//   code       :: ^[[valuem
//   value      :: attributes;foreground;background
//   attributes :: attribute;attributes
//   attribute  :: 00|01|03|04|05|07|22|23|24|25|27
//   foreground :: 38;05;color
//   background :: 48;05;color
//   color      :: 000-255
// 
//   Effect         #
//   -------------- --
//   Reset          00
//   Bold           01
//   Italic         03
//   Underline      04
//   Blink          05
//   Reverse        07
//   No Bold        22
//   No Italic      23
//   No Underline   24
//   No Blink       25
//   No Reverse     27
//
// # Print text normally, in color 214, which happens to be a nice orange.
// $> echo "^[[38;05;214mHello, World"
// 
// # Make the same text bold.
// $> echo "^[[(01);(38;05;214)mHello, World"
// 
// # Print underlined and italicized text, with normal foreground, and blue
// # background.
// $> echo "^[[(03;04);(48;05;20)mHello, World"
// 
// # Bold, blinking purple text.
// $> echo "^[[(01;05);(38;05;93)mHello, World"
// 
// # Simple purple text on yellow background.
// $> echo "^[[(38;05;93);(48;05;226)mHello, World"


static SCREEN                         _sp; 
static struct { short fg; short bg; } _color_pairs[ 256 ];

/**********************************************************************************
  DESCRIPTION
 
  Returns TRUE if init_color() and color_content() give meaningful 
  results, FALSE otherwise. Called from can_change_color().
************************************************************************************/

bool PDC_can_change_color(void)
{
    return (FALSE);
}


/**********************************************************************************
  DESCRIPTION

  The core of color_content(). This does all the work of that function, 
  except checking for values out of range and null pointers.
************************************************************************************/

int PDC_color_content(short color, short *red, short *green, short *blue)
{
    return (0);
}


/**********************************************************************************
  DESCRIPTION

  The core of init_color(). This does all the work of that function, 
  except checking for values out of range.
************************************************************************************/

int PDC_init_color(short color, short red, short green, short blue)
{
    return (0);
}


/**********************************************************************************
  DESCRIPTION
 
  The core of init_pair(). This does all the work of that function, except 
  checking for values out of range. The values passed to this function 
  should be returned by a call to PDC_pair_content() with the same pair 
  number. PDC_transform_line() should use the specified colors when 
  rendering a chtype with the given pair number.
************************************************************************************/

void PDC_init_pair(short pair, short fg, short bg)
{
  _color_pairs[ pair ].fg = fg;
  _color_pairs[ pair ].bg = bg;
}


/**********************************************************************************
  DESCRIPTION
 
  The core of pair_content(). This does all the work of that function, 
  except checking for values out of range and null pointers.

************************************************************************************/

int PDC_pair_content(short pair, short *fg, short *bg)
{
  *fg = _color_pairs[ pair ].fg;
  *bg = _color_pairs[ pair ].bg;

  return (OK);
}


/**********************************************************************************
  DESCRIPTION
 
  The non-portable functionality of reset_prog_mode() is handled here -- 
  whatever's not done in _restore_mode(). In current ports: In OS/2, this 
  sets the keyboard to binary mode; in Win32, it enables or disables the 
  mouse pointer to match the saved mode; in others it does nothing.
************************************************************************************/

void PDC_reset_prog_mode(void)
{
}


/**********************************************************************************
  DESCRIPTION
 
  The same thing, for reset_shell_mode(). In OS/2 and Win32, it restores 
  the default console mode; in others it does nothing.
************************************************************************************/

void PDC_reset_shell_mode(void)
{
}


/**********************************************************************************
  DESCRIPTION
 
  This does the main work of resize_term(). It may respond to non-zero 
  parameters, by setting the screen to the specified size; to zero 
  parameters, by setting the screen to a size chosen by the user at 
  runtime, in an unspecified way (e.g., by dragging the edges of the 
  window); or both. It may also do nothing, if there's no appropriate 
  action for the platform.
************************************************************************************/

int PDC_resize_screen(int nlines, int ncols)
{
    return (ERR);
}


/**********************************************************************************
  DESCRIPTION
 
  Called from _restore_mode() in kernel.c, this function does the actual 
  mode changing, if applicable. Currently used only in DOS and OS/2.
************************************************************************************/

void PDC_restore_screen_mode(int i)
{
    (void)i;
}


/**********************************************************************************
  DESCRIPTION
 
  Called from _save_mode() in kernel.c, this function saves the actual 
  screen mode, if applicable. Currently used only in DOS and OS/2.
************************************************************************************/

void PDC_save_screen_mode(int i)
{
}


/**********************************************************************************
  DESCRIPTION
 
  The platform-specific part of endwin(). It may restore the image of the 
  original screen saved by PDC_scr_open(), if the PDC_RESTORE_SCREEN 
  environment variable is set; either way, if using an existing terminal, 
  this function should restore it to the mode it had at startup, and move 
  the cursor to the lower left corner. (The X11 port does nothing.)

************************************************************************************/

void PDC_scr_close(void)
{
}


/**********************************************************************************
  DESCRIPTION
 
  Frees the memory for SP allocated by PDC_scr_open(). Called by 
  delscreen().

************************************************************************************/

void PDC_scr_free(void)
{
    SP = NULL;
}


/**********************************************************************************
  DESCRIPTION
 
  The platform-specific part of initscr(). It's actually called from 
  Xinitscr(); the arguments, if present, correspond to those used with 
  main(), and may be used to set the title of the terminal window, or for 
  other, platform-specific purposes. (The arguments are currently used 
  only in X11.) PDC_scr_open() must allocate memory for SP, and must 
  initialize acs_map[] (unless it's preset) and several members of SP, 
  including lines, cols, mouse_wait, orig_attr (and if orig_attr is TRUE, 
  orig_fore and orig_back), mono, _restore and _preserve. (Although SP is 
  used the same way in all ports, it's allocated here in order to allow 
  the X11 port to map it to a block of shared memory.) If using an 
  existing terminal, and the environment variable PDC_RESTORE_SCREEN is 
  set, this function may also store the existing screen image for later 
  restoration by PDC_scr_close().

  SCREEN* SP;
  typedef struct
  {
      bool  alive;                   -- if initscr() called, and not endwin() 
      bool  autocr;                  -- if cr -> lf 
      bool  cbreak;                  -- if terminal unbuffered 
      bool  echo;                    -- if terminal echo 
      bool  raw_inp;                 -- raw input mode (v. cooked input) 
      bool  raw_out;                 -- raw output mode (7 v. 8 bits) 
      bool  audible;                 -- FALSE if the bell is visual 
      bool  mono;                    -- TRUE if current screen is mono 
      bool  resized;                 -- TRUE if TERM has been resized 
      bool  orig_attr;               -- TRUE if we have the original colors 
      short orig_fore;               -- original screen foreground color 
      short orig_back;               -- original screen foreground color 
      int   cursrow;                 -- position of physical cursor 
      int   curscol;                 -- position of physical cursor 
      int   visibility;              -- visibility of cursor 
      int   orig_cursor;             -- original cursor size 
      int   lines;                   -- new value for LINES 
      int   cols;                    -- new value for COLS 
      unsigned long _trap_mbe;       -- trap these mouse button events 
      unsigned long _map_mbe_to_key; -- map mouse buttons to slk 
      int   mouse_wait;              -- time to wait (in ms) for a
                                        button release after a press, in 
                                        order to count it as a click 
      int   slklines;                -- lines in use by slk_init() 
      WINDOW *slk_winptr;            -- window for slk 
      int   linesrippedoff;          -- lines ripped off via ripoffline() 
      int   linesrippedoffontop;     -- lines ripped off on 
                                        top via ripoffline() 
      int   delaytenths;             -- 1/10ths second to wait block
                                        getch() for 
      bool  _preserve;               -- TRUE if screen background
                                        to be preserved 
      int   _restore;                -- specifies if screen background
                                        to be restored, and how 
      bool  save_key_modifiers;      -- TRUE if each key modifiers saved
                                        with each key press 
      bool  return_key_modifiers;    -- TRUE if modifier keys are
                                        returned as "real" keys 
      bool  key_code;                -- TRUE if last key is a special key;
                                        used internally by get_wch() 
      short line_color;              -- color of line attributes - default -1 
  } SCREEN;

   
  RETURN VALUE
    OK or ERR

************************************************************************************/
int PDC_scr_open(int argc, char **argv)
{
    SP  = &_sp;
    memset( SP, 0, sizeof(SCREEN) );

    SP->lines      = PDC_get_rows();
    SP->cols       = PDC_get_columns();
    SP->mouse_wait = PDC_CLICK_PERIOD;
    SP->audible    = TRUE;

    PDC_reset_prog_mode();

    // setvbuf( stdout, NULL, _IONBF, 0 );  

    return OK;
}

