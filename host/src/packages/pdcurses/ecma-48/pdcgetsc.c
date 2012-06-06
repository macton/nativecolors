
#ifndef NO_IOCTL
#include <sys/ioctl.h>
#endif

/**********************************************************************************
  DESCRIPTION

  It's used in resize_term() to set the new value of COLS. (Some existing 
  implementations also call it internally from PDC_scr_open(), but this is not 
  required.)

  RETURN VALUE

  Returns the size of the screen in columns. 
************************************************************************************/

int PDC_get_columns(void)
{
#ifndef NO_IOCTL
  struct winsize win;

  if ( ioctl(0, TIOCGWINSZ, &win) == 0 )
  {
    return win.ws_col;
  }

  return (0);
#else
  return (FIXED_WIDTH);
#endif
}


/**********************************************************************************
  DESCRIPTION

  This function is called from initscr(), and the result is stored in 
  SP->orig_cursor, which is used by PDC_curs_set() to determine the size/shape of the 
  cursor in normal visibility mode (curs_set(1)).
 
  RETURN VALUE

  Returns the size/shape of the cursor.  The format of the result is unspecified,
  except that it must be returned as an int.
************************************************************************************/

int PDC_get_cursor_mode(void)
{
  return (0);
}


/**********************************************************************************
  DESCRIPTION
 
  It's used in resize_term() to set the new value of LINES. (Some existing
  implementations also call it internally from PDC_scr_open(), but this is not 
  required.)
   
  RETURN VALUE

  Returns the size of the screen in rows.
************************************************************************************/

int PDC_get_rows(void)
{
#ifndef NO_IOCTL
  struct winsize win;

  if ( ioctl(0, TIOCGWINSZ, &win) == 0 )
  {
    return win.ws_row;
  }

  return (0);
#else
  return (FIXED_HEIGHT);
#endif
}

