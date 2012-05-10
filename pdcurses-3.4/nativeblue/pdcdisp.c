#include <curses.h>
#include <curspriv.h>
#include <alloca.h>

/**********************************************************************************
  DESCRIPTION

  Move the physical cursor (as opposed to the logical cursor affected by 
  wmove()) to the given location. This is called mainly from doupdate(). 
  In general, this function need not compare the old location with the new 
  one, and should just move the cursor unconditionally.
************************************************************************************/

void PDC_gotoyx(int y, int x)
{
  PDC_LOG(("PDC_gotoyx %d,%d\n",y,x));
  printf("[%d;%dH",y,x);
  // hterm.Terminal.prototype.setAbsoluteCursorPosition
}

/**********************************************************************************
  DESCRIPTION

  The core output routine. It takes len chtype entities from srcp (a 
  pointer into curscr) and renders them to the physical screen at line 
  lineno, column x. It must also translate characters 0-127 via acs_map[], 
  if they're flagged with A_ALTCHARSET in the attribute portion of the 
  chtype.
************************************************************************************/

void PDC_transform_line(int lineno, int x, int len, const chtype* srcp)
{
  int j;

  // hterm.Terminal.prototype.setAbsoluteCursorPosition
  // hterm.Terminal.prototype.interpret

  char* out = (char*)alloca( len+1 );


  for (j = 0; j < len; j++)
  {
      chtype ch = srcp[j];
      out[j]    = ch & A_CHARTEXT; 
  }
  out[len] = 0;

  PDC_LOG(("PDC_transform_line %d,%d,%d,%s\n",lineno,x,len,out));
  printf("[%d;%dH%s",lineno,x,out);
  fflush(stdout);
}

