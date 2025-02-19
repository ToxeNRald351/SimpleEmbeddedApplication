// Author: Josef Strnadel

#ifndef _IO_H
#define _IO_H

#define printf io_printf

/********************************************************************/

char
in_char(void);

void
out_char(char);

int
char_present(void);

int
io_printf(const char *, ... );

int
sprintf(char *, const char *, ... );

/********************************************************************/

#endif
