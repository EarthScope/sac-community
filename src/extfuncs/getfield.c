void getfield(fieldname, lenfield, fieldout)
char *fieldname;
int   lenfield;
char  *fieldout;
{
/* Return a NULL terminated string with leading blanks removed. */
  char *tempin, *tempout;
  int   count = 0;

  tempin = fieldname;

  while ( *tempin == ' ' ) {
    tempin++;
    count++;
  }

  tempout = fieldout;

  while((*tempin != ' ') && (count < lenfield) && (*tempout != '\0')){
    *tempout++ = *tempin++;
    count++;
  }

  *tempout = '\0';

  return;

}
