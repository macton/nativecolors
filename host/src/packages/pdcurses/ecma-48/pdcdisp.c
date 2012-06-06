#include <curspriv.h>
#include <stdio.h>

/* ACS definitions originally by jshumate@wrdis01.robins.af.mil -- these
   match code page 437 and compatible pages (CP850, CP852, etc.) */

#ifdef CHTYPE_LONG

# define A(x) ((chtype)x | A_ALTCHARSET)

chtype acs_map[128] =
{
    A(0), A(1), A(2), A(3), A(4), A(5), A(6), A(7), A(8), A(9), A(10), 
    A(11), A(12), A(13), A(14), A(15), A(16), A(17), A(18), A(19), 
    A(20), A(21), A(22), A(23), A(24), A(25), A(26), A(27), A(28), 
    A(29), A(30), A(31), ' ', '!', '"', '#', '$', '%', '&', '\'', '(', 
    ')', '*',

    A(0x1a), A(0x1b), A(0x18), A(0x19),

    '/',

    0xdb,

    '1', '2', '3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', 
    '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 
    'X', 'Y', 'Z', '[', '\\', ']', '^', '_',

    A(0x04), 0xb1,

    'b', 'c', 'd', 'e',

    0xf8, 0xf1, 0xb0, A(0x0f), 0xd9, 0xbf, 0xda, 0xc0, 0xc5, 0x2d, 0x2d, 
    0xc4, 0x2d, 0x5f, 0xc3, 0xb4, 0xc1, 0xc2, 0xb3, 0xf3, 0xf2, 0xe3, 
    0xd8, 0x9c, 0xf9,

    A(127)
};

# undef A

#endif

static void _set_attr( chtype attr, bool rev )
{
    short fg;
    short bg;

    PDC_pair_content( PAIR_NUMBER(attr), &fg, &bg );

    /* Reverse flag = highlighted selection XOR A_REVERSE set */
    rev ^= !!(attr & A_REVERSE);

    // hterm does not support inverse correctly (it resets the color)
    // printf( "\033[%d;%d;%d;%d;%dm", fg+30, bg+40, (attr&A_BOLD)?1:22, (attr&A_BLINK)?5:25, (rev)?7:27 );

    printf( "\033[%d;%d;%d;%d;%dm", (rev)?7:27, (attr&A_BLINK)?5:25, (attr&A_BOLD)?1:22, fg+30, bg+40 );
}

/**********************************************************************************
  DESCRIPTION

  Move the physical cursor (as opposed to the logical cursor affected by 
  wmove()) to the given location. This is called mainly from doupdate(). 
  In general, this function need not compare the old location with the new 
  one, and should just move the cursor unconditionally.
************************************************************************************/

void PDC_gotoyx(int y, int x)
{
    printf("\033[%d;%dH",y,x);
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
    int    j;
    char   out[len+1];
    chtype prev_attr;
    chtype attr;

    prev_attr = srcp[0] & A_ATTRIBUTES;
    attr      = prev_attr;

    _set_attr( attr, FALSE );

    for (j = 0; j < len; j++)
    {
      chtype ch = srcp[j];
      attr      = ch & A_ATTRIBUTES;

#ifdef CHTYPE_LONG
        if (ch & A_ALTCHARSET && !(ch & 0xff80))
        {
            ch = acs_map[ch & 0x7f];
        }
#endif
        if ( attr != prev_attr )
        {
            _set_attr( attr, FALSE );
            prev_attr = attr;
        }

        out[j] = ch & A_CHARTEXT; 
    }
    out[len] = 0;

    printf("\033[%d;%dH%s",lineno,x,out);
}
