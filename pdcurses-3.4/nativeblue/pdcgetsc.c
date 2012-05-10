
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
  // this.screen_.getWidth()
  return (80);
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
  // no equivalent in hterm that I can see. 
  return (1);
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
  // this.screen_.getHeight()
  return (80);
}

