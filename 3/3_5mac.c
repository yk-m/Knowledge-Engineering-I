#include <stdio.h>
#include <stdlib.h>

void fileRead(FILE *fdr);
void printimg(char data[512]);
void printnum(char pattern[64][64]);
void printpattern(char pattern[64][64]);
void copy(char b[64][64], char a[64][64]);
void expand(char data[512], char pattern[64][64]);
void compress(char data[512], char pattern[64][64]);
void thinning(char p[64][64]);
void thinImage(char p[64][64], int i, int j, int start, int change_flag);

int main(int argc, char *argv[]) {
	int i,count=0;
	FILE *fdr;
	char fn[100];
	
	scanf("%s", fn);
	if ((fdr = fopen(fn, "rb")) == NULL) {
		printf("can't open a file\n");
		exit(1);
	}
	fileRead(fdr);
	fclose(fdr);
	
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
		thinning(pattern);
		printf("細線化後\n");
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
void printnum(char pattern[64][64])
{
	int i,j;
	for (i=0; i<64; i++) {
		for (j=0; j<64; j++) {
			printf("%d", pattern[i][j]);
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

void thinning(char p[64][64])
{
	char old[64][64];
	int i, j;
	int change_flag;

	copy(p, old);

	do {
		change_flag=0;

		for (j=0;j<64;j++)
			for(i=0;i<64;i++)
				if(p[i][j]==1) thinImage(p,i,j,2,change_flag);

		for (j=63;j>=0;j--)
			for(i=63;i>=0;i--)
				if(p[i][j]==1) thinImage(p,i,j,6,change_flag);

		for (j=0;j<64;j++)
			for(i=63;i>=0;i--)
				if(p[i][j]==1) thinImage(p,i,j,0,change_flag);

		for (j=63;j>=0;j--)
			for(i=0;i<64;i++)
				if(p[i][j]==1) thinImage(p,i,j,4,change_flag);

	} while(change_flag==1);
}

void thinImage(char p[64][64], int i, int j, int start, int change_flag)
{

	char pixel[8];
	int product,sum;

	pixel[0]= (i!= 0 && j!= 0)? p[i-1][j-1] : 0;
	pixel[1]= (i!= 0 )?         p[i-1][j]   : 0;
	pixel[2]= (i!= 0 && j!=63)? p[i-1][j+1] : 0;
	pixel[3]= (j!=63 )?         p[i][j+1]   : 0;
	pixel[4]= (i!=63 && j!=63)? p[i+1][j+1] : 0;
	pixel[5]= (i!=63 )?         p[i+1][j]   : 0;
	pixel[6]= (i!=63 && j!= 0)? p[i+1][j-1] : 0;
	pixel[7]= (j!= 0 )?         p[i][j-1]   : 0;

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