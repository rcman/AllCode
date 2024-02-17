
void fade_out_screen(void far *pal);
void bright_out_screen(void far *pal);
void fade_in_screen(void far *pal);
void bright_in_screen(void far *pal);
void fade_between_screen(void far *pal,void far *pal_dest);


void setPal(void far *pal);
void fill_pal(void far *pal,char red,char green,char blue);
void copy_pal(void far *pal,void far *pal_dest);
void rotate_pal(void far *pal,char index,char numCol, unsigned char displacement);
void sub_palette(void far *pal,void far *pal_dest);
void fade_out_once(void far *pal);
void bright_out_once(void far *pal);
void fade_in_once(void far *pal,void far *pal_dest);
void fade_between_once(void far *pal,void far *pal_dest);

