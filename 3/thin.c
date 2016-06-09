void thinning(char p[64][64])
{
	char old[64][64];
	int change_flag;

	copy(p, old);

	do{

	change_flag=0;

	for(int j=1;j<63;j++)
		for(int i=1;i<63;i++)
			if(old[i][j]==1) thinImage(p,old,i,j,2,change_flag);

	for(int j=62;j>=1;j--)
		for(int i=62;i>=1;i--)
			if(old[i][j]==1) thinImage(p,old,i,j,6,change_flag);

	for(int j=1;j<63;j++)
		for(int i=62;i>=1;i--)
			if(old[i][j]==1) thinImage(p,old,i,j,0,change_flag);

	for(int j=62;j>=1;j--)
		for(int i=1;i<63;i++)
			if(old[i][j]==1) thinImage(p,old,i,j,4,change_flag);

	}while(change_flag==1);
}

void thin(char p[64][64], char old[64][64], int i, int j, int start, int change_flag)
{

	char pixel[8];
	int product,sum;

	pixel[0]=old[i-1][j-1];
	pixel[1]=old[i-1][j];
	pixel[2]=old[i-1][j+1];
	pixel[3]=old[i][j+1];
	pixel[4]=old[i+1][j+1];
	pixel[5]=old[i+1][j];
	pixel[6]=old[i+1][j-1];
	pixel[7]=old[i][j-1];

	for(int k=start;k<start+3;k++){
		product=pixel[k % 8]*pixel[(k+1) % 8]*pixel[(k+2) % 8];
		sum=pixel[(k+4) % 8]+pixel[(k+5) % 8]+pixel[(k+6) % 8];
		if(product==1 && sum==0){
			p[i][j]=0;
			change_flag=1;
			return;
		}
	}
}