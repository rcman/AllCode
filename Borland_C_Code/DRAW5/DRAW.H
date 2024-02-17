extern void ptext(void);
extern void Init_Mode(void);
extern void Close_Mode(void);
extern void helpscreen(void);
extern void Calc_Coords(void);
extern void Init_Screen(void);
extern void initarray(void);
extern void PutColor();
extern void BoxImage();
extern void BoxMask();
extern int Get_Mouse(int command);
extern void CutImage(void);
extern void PutImage(void);
extern void RestoreBack(void);
extern int filewrite(void);
extern int fileread(void);

extern struct fshape
	{

		int w,h;
		int n,c;
		int flag;
		int rowflag;
		char far shp[260];
					// TOTALSHAPE  =  20
	};



extern struct animshape z{
		int active;
		int animwidth;
		int animheight;
		int animox;
		int animoy;
		int animx;
		int animy;
		int prox;
		int animspeed;
		int currentshape;
		int oldshape;
		int max;
		int row;

		struct fshape *fshp[TOTALSHAPE];        // 10 Pointers to data


	 }  animobjects[TOTALANIMS];



#define 	TOTALSHAPE 	10
#define 	TOTALANIMS 	10

