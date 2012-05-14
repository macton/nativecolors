#include <curses.h>

unsigned long pdc_key_modifiers = 0L;

/**********************************************************************************
  DESCRIPTION
 
  Keyboard/mouse event check, called from wgetch(). 
  This function must be non-blocking.
   
  RETURN VALUE

  Returns TRUE if there's an event ready to process. 
************************************************************************************/

bool PDC_check_key(void)
{
  // pull from queue
  // request more?
  // override hterm.Terminal.prototype.onVTKeystroke
  return (FALSE);
}


/**********************************************************************************
  DESCRIPTION

  This is the core of flushinp(). It discards any pending key or mouse
  events, removing them from any internal queue and from the OS queue, if
  applicable.
************************************************************************************/

void PDC_flushinp(void)
{
}


/**********************************************************************************
  DESCRIPTION

  Get the next available key, or mouse event (indicated by a return of
  KEY_MOUSE), and remove it from the OS' input queue, if applicable. This
  function is called from wgetch(). This function may be blocking, and
  traditionally is; but it need not be. If a valid key or mouse event
  cannot be returned, for any reason, this function returns -1. Valid keys
  are those that fall within the appropriate character set, or are in the
  list of special keys found in curses.h (KEY_MIN through KEY_MAX). When
  returning a special key code, this routine must also set SP->key_code to
  TRUE; otherwise it must set it to FALSE. If SP->return_key_modifiers is
  TRUE, this function may return modifier keys (shift, control, alt),
  pressed alone, as special key codes; if SP->return_key_modifiers is
  FALSE, it must not. If modifier keys are returned, it should only happen
  if no other keys were pressed in the meantime; i.e., the return should
  happen on key up. But if this is not possible, it may return the
  modifier keys on key down (if and only if SP->return_key_modifiers is
  TRUE).
************************************************************************************/
int PDC_get_key(void)
{
  return (-1);
}


/**********************************************************************************
  DESCRIPTION
 
  Called from PDC_return_key_modifiers(). If your platform needs to do 
  anything in response to a change in SP->return_key_modifiers, do it 
  here. 
   
  RETURN VALUE

  Returns OK or ERR, which is passed on by the caller.
************************************************************************************/
int PDC_modifiers_set(void)
{
  return (OK);
}


/**********************************************************************************
  DESCRIPTION
 
  Called by mouse_set(), mouse_on(), and mouse_off() -- all the functions 
  that modify SP->_trap_mbe. If your platform needs to do anything in 
  response to a change in SP->_trap_mbe (for example, turning the mouse 
  cursor on or off), do it here. 
   
  RETURN VALUE

  Returns OK or ERR, which is passed on by the caller.
************************************************************************************/
int PDC_mouse_set(void)
{
  return (OK);
}


/**********************************************************************************
  DESCRIPTION
 
  Set keyboard input to "binary" mode. If you need to do something to keep 
  the OS from processing ^C, etc. on your platform, do it here. TRUE turns 
  the mode on; FALSE reverts it. This function is called from raw() and 
  noraw().

************************************************************************************/
void PDC_set_keyboard_binary(bool on)
{
}


/**********************************************************************************
  DESCRIPTION
 
  PDC_get_input_fd() returns the file descriptor that PDCurses 
  reads its input from. It can be used for select().
   
************************************************************************************/
unsigned long PDC_get_input_fd(void)
{
  return (0UL);
}

