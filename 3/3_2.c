#include <stdio.h>
#include <stdlib.h>

void fileRead(FILE *fdr);
void printimg(char data[512]);
void printpattern(char pattern[64][64]);
void copy(char b[64][64], char a[64][64]);
void expand(char data[512], char pattern[64][64]);
void compress(char data[512], char pattern[64][64]);
void smooth(char p[64][64]);
int reverse(int a);

int main(int argc, char *argv[]) {
	int i,count=0;
	FILE *fdr;
	
	for (i=1; 1<argc && i<argc; i++) {
		if ((fdr = fopen(argv[i], "rb")) == NULL) {
			printf("can't open a file\n");
			exit(1);
		}
		fileRead(fdr);
		fclose(fdr);
	}
	
	while(getchar()!='\n');
}

void fileRead(FILE *fdr)
{
	char pattern[64][64];
	char data[512];
	int count=0;
	int x, y, i;
	while (count<20) {
		if (fread(data, 512, 1, fdr) != 1) {
			printf("can't open a file\n");
			exit(2);
		}
		expand(data,pattern);
		printf("元データ\n");
		printpattern(pattern);
		smooth(pattern);
		printf("スムージング後\n");
		printpattern(pattern);
		
		count++;
		
		while(getchar()!='\n');
	}
}

void printimg(char data[512])
{
	int i, j, k;
	unsigned char mask = 128;
	
	for (i=0; i<64; i++) {
		for (j=i*8; j<i*8+8; j++) {
			mask = 0x80;
			for (k=0; k<8; k++) {
				if ((data[j]&mask) == 0) {
					printf(".");
				} else {
					printf("*");
				}
				mask = mask >>1;
			}
		}
		printf("\n");
	}
}
void printpattern(char pattern[64][64])
{
	int i,j;
	for (i=0; i<64; i++) {
		for (j=0; j<64; j++) {
			printf("%s", (pattern[i][j] == 0)? "." : "*");
		}
		printf("\n");
	}
}

void copy(char b[64][64], char a[64][64])
{
	int i,j;
	for (i=0; i<64; i++) {
		for (j=0; j<64; j++) {
			a[i][j] = b[i][j];
		}
	}
}

void expand( char data[512], char pattern[64][64])
{
	int i, j, k;
	int count=0;
	unsigned char mask;

	for (i=0; i<64; i++) {
		for (j=i*8; j<i*8+8; j++) {
			mask = 0x80;
			for (k=0; k<8; k++) {
				if ((data[j]&mask) == 0) {
					pattern[i][8*j%64+k] = 0;
				} else {
					pattern[i][8*j%64+k] = 1;
				}
				mask = mask >>1;
			}
		}
	}
}

void compress(char data[512], char pattern[64][64])
{
	int i,j,k;
	int count=0;
	unsigned char mask;
	
	for (i=0; i<64; i++) {
		for (j=0; j<64; j+=8) {
			mask = 0x0;
			for (k=0; k<8; k++) {
				mask = mask <<1;
				if (pattern[i][j+k]==1) mask++;
			}
			data[count] = mask;
			count++;
		}
	}
}

void smooth(char p[64][64])
{
	int i,k,s,x,y,u,v,ur,vr,flag=1;
	static int position[3][3][2] = {
		{{-1,-1}, {-1,0}, {-1,1}},
		{ {0,-1},  {0,0},  {0,1}},
		{ {1,-1},  {1,0},  {1,1}}
	};
	static int ans[4][2] = {
		{-1,0}, {0,-1}, {1,0}, {0,1} 
	};
	static char mask[4][3][3] = {
		{
			{0,1,0},
			{1,1,1},
			{1,1,1}
		},{
			{1,0,1},
			{1,1,1},
			{1,1,1}
		},{
			{1,0,1},
			{0,0,0},
			{0,0,0}
		},{
			{0,1,0},
			{0,0,0},
			{0,0,0}
		}
	};
	for (y=1; y<63; y++) {
		for (x=1; x<63; x++) {
			for (i=0; i<4; i++) {
				k = (i==0 || i==2)? 0 : 1;
				for (s=0; s<4; s++) {
					for (v=0; v<3 && flag; v++) {
						for (u=0; u<3 && flag; u++) {
							if (i < 2) {
								if (mask[s][v][u] != p[y+position[v][u][k]][x+position[v][u][1-k]]) flag = 0;
							} else {
								vr = reverse(v);
								ur = reverse(u);
								if (mask[s][v][u] != p[y+position[vr][ur][k]][x+position[vr][ur][1-k]]) flag = 0;
							}
						}
					}

					if (flag == 1) {
						if(s == 0 || s == 3) p[y+ans[i][0]][x+ans[i][1]] = 0;
						else p[y+ans[i][0]][x+ans[i][1]] = 1;
					}
					flag = 1;
				}
			}
		}
	}
}

int reverse(int a)
{
	if (a == 2) return 0;
	if (a == 0) return 2;
	return 1;
}
