struct fshape
	{
	int w,h;
	int flag;
	char shp[260];
	};


struct animobject
	{
	int x,y;
	int oldx,oldy;
	struct fshape *fshp[TOTALSHAPE];
	} anims[TOTALANIMS];

main()
{

	char far *p,*st1;
	p=farmalloc(sizeof(struct fshape)*TOTALSHAPE*TOTALANIMS);

	st1 = p;

	for (i=0;i<TOTALANIMS;i++)
	{
		for (j=0;j<TOTALSHAPES;j++)
		{
		anims[i].fshp[j]=p;

		anims[i].fshp[j]->w=15;

		p+=sizeof(struct fshape));


}


