#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void extractFeatures(FILE *fdr, FILE *ofdr);
void saveFeatures(int features[64], FILE *ofdr);

void printimg(char data[512]);
void printnum(char pattern[64][64]);
void printpattern(char pattern[64][64]);

void copy(char b[64][64], char a[64][64]);
void expand(char data[512], char pattern[64][64]);
void compress(char data[512], char pattern[64][64]);

void callPreprocessor(char pattern[64][64], int features[64]);
// ノイズ除去	
int label(char p[64][64]);
void appendnum(char p[64][64], int y, int x, int num);
void noise(char p[64][64], int size);
void removenum(char p[64][64], char a);
// スムージング
void smooth(char p[64][64]);
int reverse(int a);
// 正規化
void normalize(char p[64][64]);
void searchend(double ends[4], char p[64][64]);
// 輪郭抽出
void outline(char p[64][64]);

// 特徴計算
void calcFeatures(char p[64][64], int features[64]);
int isEnd(int i);

int main(int argc, char *argv[]) {
	int i,count=0;
	FILE *fdr, *ofdr;
	char fn[100], *find;
	
	for (i=1; 1<argc && i<argc; i++) {
		if ((fdr = fopen(argv[i], "rb")) == NULL) {
			printf("can't open a file\n");
			exit(1);
		}

		find = strstr(argv[i], ".img");
		strcpy(find, ".ftr");
		if ((ofdr = fopen(argv[i], "w")) == NULL) {
			printf("can't open a file\n");
			exit(1);
		}

		extractFeatures(fdr, ofdr);

		fclose(fdr);
		fclose(ofdr);
	}
}

void extractFeatures(FILE *fdr, FILE *ofdr)
{
	char pattern[64][64];
	char data[512];
	int features[64];
	int count=0, num;
	int x, y, i;

	fseek(fdr, 0, SEEK_END);
	num = ftell(fdr) / 512;
	fseek(fdr, 0, SEEK_SET);

	while (count<num) {
		if (fread(data, 512, 1, fdr) != 1) {
			printf("can't open a file\n");
			exit(2);
		}
		expand(data,pattern);

		callPreprocessor(pattern, features);
		calcFeatures(pattern, features);
		saveFeatures(features, ofdr);
		
		count++;
		// while(getchar()!='\n');
	}
}

void saveFeatures(int features[64], FILE *ofdr)
{
	for (int i=0; i<64; i++) {
		fprintf(ofdr, "%d ", features[i]);
	}
	fprintf(ofdr, "\n");
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


void callPreprocessor(char pattern[64][64], int features[64])
{
	noise(pattern, 15);
	smooth(pattern);
	normalize(pattern);
	outline(pattern);
}

int label(char p[64][64])
{
	int y, x, num = 2;
	for (y=0; y<64; y++) {
		for (x=0; x<64; x++) {
			if (p[y][x]==1) {
				appendnum(p, y, x, num);
				num++;
				if (num > 255) return 1;
			}
		}
	}
	return 0;
}

void appendnum(char p[64][64], int y, int x, int num)
{
	int v, u;
	p[y][x] = num;

	if (x!=0  && p[y][x-1]==1) appendnum(p, y, x-1, num);
	if (x!=63 && p[y][x+1]==1) appendnum(p, y, x+1, num);

	if (y!=0) {
		if (x!=0  && p[y-1][x-1]==1) appendnum(p, y-1, x-1, num);
		if (x!=63 && p[y-1][x+1]==1) appendnum(p, y-1, x+1, num);
		if (p[y-1][x]==1) appendnum(p, y-1, x, num);
	}
	if (y!=63) {
		if (x!=0  && p[y+1][x-1]==1) appendnum(p, y+1, x-1, num);
		if (x!=63 && p[y+1][x+1]==1) appendnum(p, y+1, x+1, num);
		if (p[y+1][x]==1) appendnum(p, y+1, x, num);
	}
}

void noise(char p[64][64], int size)
{
	int num[255], a, max=0;
	int x, y, i;
	label(p);

	for (y=0; y<64; y++) {
		for (x=0; x<64; x++) {
			if (1 < p[y][x]) {
				a = p[y][x];
				if (a > max) {
					max = a;
					num[a] = 0;
				}
				num[a]++;
			}
		}
	}

	for (i=2; i<=max; i++) {
		if (num[i]<=size) {
			removenum(p, (char)i);
		}
	}

	for (y=0; y<64; y++) {
		for (x=0; x<64; x++) {
			if (p[y][x]>1) p[y][x] = 1;
		}
	}
}

void removenum(char p[64][64], char a)
{
	int x, y;
	for (y=0; y<64; y++) {
		for (x=0; x<64; x++) {
			if (p[y][x] == a) p[y][x] = 0;
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

void normalize(char p[64][64])
{
	int x, y;
	int u, v;
	double hratio, wratio;
	double ends[4];
	char o[64][64];
	
	copy(p, o);
	searchend(ends, p);
	wratio = ends[2] / 64;
	hratio = ends[3] / 64;
	for (v=0; v<64; v++) {
		for (u=0; u<64; u++) {
			x = (int)(u*wratio + ends[0] + 0.5);
			y = (int)(v*hratio + ends[1] + 0.5);
			p[v][u] = o[y][x];
		}
	}
}

void searchend(double ends[4], char p[64][64])
{
	int x, y;
	int flag = 0;
	for (x=0; x<64 && flag == 0; x++) {
		for (y=0; y<64 && flag == 0; y++) {
			if (p[y][x]==1) {
				ends[0] = x;
				flag = 1;
			}
		}
	}
	flag = 0;
	
	for (y=0; y<64 && flag == 0; y++) {
		for (x=0; x<64 && flag == 0; x++) {
			if (p[y][x]==1){
				ends[1] = y;
				flag = 1;
			}
		}
	}
	flag = 0;

	for (x=63; x>=0 && flag == 0; x--) {
		for (y=63; y>=0 && flag == 0; y--) {
			if (p[y][x]==1) {
				ends[2] = x-ends[0];
				flag = 1;
			}
		}
	}
	flag = 0;

	for (y=63; y>=0 && flag == 0; y--) {
		for (x=63; x>=0 && flag == 0; x--) {
			if (p[y][x]==1){
				ends[3] = y-ends[1];
				flag = 1;
			}
		}
	}
}

void outline(char p [64][64])
{
	int x,y;
	char o[64][64];
	copy(p, o);
	for (y=1; y<63; y++) {
		for (x=1; x<63; x++) {
			if (o[y-1][x] && o[y+1][x] && o[y][x-1] && o[y][x+1]) p[y][x] = 0;
		}
	}
}

void calcFeatures(char p[64][64], int features[64])
{
	int x, y, i;

	for (i=0; i<64; i++) {
		features[i] = 0;
	}

	for (i=0; i<16; i++) {
		for (y=16*(i/4); y<16*(i/4+1); y++) {
			for (x=16*(i%4); x<16*(i%4+1); x++) {
				if      (isEnd(y) && !isEnd(x)) features[4*i]   += p[y][x-1] & p[y][x+1];
				else if (isEnd(x))              features[4*i+1] += p[y-1][x] & p[y+1][x];
				else {
					features[4*i]   += p[y][x-1] & p[y][x+1];
					features[4*i+1] += p[y-1][x] & p[y+1][x];
					features[4*i+2] += p[y-1][x+1] & p[y+1][x-1];
					features[4*i+3] += p[y-1][x-1] & p[y+1][x+1];
				}
			}
		}
	}
}

int isEnd(int i) {
	if (i==0 || i==63) return 1;
	return 0;
}