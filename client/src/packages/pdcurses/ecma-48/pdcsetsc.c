#include <curspriv.h>
#include <stdio.h>

/**********************************************************************************
  DESCRIPTION
 
  Called from curs_set(). Changes the appearance of the cursor -- 0 turns 
  it off, 1 is normal (the terminal's default, if applicable, as 
  determined by SP->orig_cursor), and 2 is high visibility. The exact 
  appearance of these modes is not specified.
 
  The curs_set() function sets the appearance of the cursor based on the value of
  visibility:
 
     Value of visibility  Appearance of Cursor
     0                    Invisible
     1                    Terminal-specific normal mode
     2                    Terminal-specific high visibility mode
 
  The terminal does not necessarily support all the above values.
   
  RETURN VALUE
 
  If the terminal supports the cursor mode specified by visibility, then curs_set()
  returns the previous cursor state. Otherwise, the function returns ERR.
************************************************************************************/

int PDC_curs_set(int visibility)
{
  printf( "\033[%s", ( visibility ) ? "?25h" : "?25l" );
  return (1);
}


/**********************************************************************************
  DESCRIPTION

  PDC_set_blink() toggles whether the A_BLINK attribute sets an
  actual blink mode (TRUE), or sets the background color to high
  intensity (FALSE). The default is platform-dependent (FALSE in
  most cases).

  RETURN VALUE

  It returns OK if it could set the state to match 
  the given parameter, ERR otherwise. Current platforms also 
  adjust the value of COLORS according to this function -- 16 for 
  FALSE, and 8 for TRUE.
************************************************************************************/

int PDC_set_blink(bool blinkon)
{
    if (pdc_color_started)
    {
        COLORS = 16;
    }

    return blinkon ? ERR : OK;
}


/**********************************************************************************
  DESCRIPTION

  PDC_set_title() sets the title of the window in which the curses
  program is running. This function may not do anything on some
  platforms. (Currently it only works in Win32 and X11.)
************************************************************************************/

void PDC_set_title(const char *title)
{
  printf( "\033]0;%s", title );
}
