#include <stdio.h>

#include <stdlib.h>

void fileRead(FILE *fdr);
void printimg(char data[512]);
void printpattern(char pattern[64][64]);
void copy(char b[64][64], char a[64][64]);
void expand(char data[512], char pattern[64][64]);
void compress(char data[512], char pattern[64][64]);
void smooth(char p[64][64]);
void normalize(char p[64][64]);
void searchend(double ends[4], char p[64][64]);
void outline(char p[64][64]);
void comp(int a, int b, char p[64][64]);
int reverse(int a);

int main(int argc, char *argv[]) {
	int i,count=0;
	FILE *fdr;
	char fn[100];
	
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
		printpattern(pattern);
		printf("smooth\n");
		smooth(pattern);
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

void smooth(char p[64][64])
{
	int i,s,x,y,u,v,flag=1;
	static int position[3][3][2] = {
		{{-1,-1}, {-1,0}, {-1,1}},
		{ {0,-1},  {0,0},  {0,1}},
		{ {1,-1},  {1,0},  {1,1}}
	};
	static int ans[4][2] = {
		{-1,0}, {0,-1}, {1,0}, {0,1} 
	};
	static char mask[4][3][3] = {
		{{0,1,0}, {1,1,1}, {1,1,1}},
		{{1,0,1}, {1,1,1}, {1,1,1}},
		{{1,0,1}, {0,0,0}, {0,0,0}},
		{{0,1,0}, {0,0,0}, {0,0,0}}
	};
	for (y=1; y<63; y++) {
		for (x=1; x<63; x++) {
			for (i=0; i<4; i++) {
				for (s=0; s<4; s++) {
					for (v=0; v<3 && flag; v++) {
						for (u=0; u<3 && flag; u++) {
							switch (i) {
								case 0:
									if (mask[s][v][u] != p[y+position[v][u][0]][x+position[v][u][1]]) flag = 0;
									break;
								case 1:
									if (mask[s][v][u] != p[y+position[v][u][1]][x+position[v][u][0]]) flag = 0;
									break;
								case 2:
									if (mask[s][v][u] != p[y+position[reverse(v)][reverse(u)][0]][x+position[reverse(v)][reverse(u)][1]]) flag = 0;
									break;
								case 3:
									if (mask[s][v][u] != p[y+position[reverse(v)][reverse(u)][1]][x+position[reverse(v)][reverse(u)][0]]) flag = 0;
									// printf("%d %d vu %d %d\n",a+position[v][u][0], b+position[v][u][1], v, u);
									break;
							}
						}
					}

					if (flag == 1) {
						if(s == 0 || s == 3) p[ans[i][0]][ans[i][1]] = 0;
						else p[ans[i][0]][ans[i][1]] = 1;
					}
					flag = 1;
				}
			}
		}
	}
}

void comp(int a, int b, char p[64][64])
{
	int i,s,x,y,u,v,flag=1;
	static int position[3][3][2] = {
		{{-1,-1}, {-1,0}, {-1,1}},
		{ {0,-1},  {0,0},  {0,1}},
		{ {1,-1},  {1,0},  {1,1}}
	};
	static char mask[4][3][3] = {
		{{0,1,0}, {1,1,1}, {1,1,1}},
		{{1,0,1}, {1,1,1}, {1,1,1}},
		{{1,0,1}, {0,0,0}, {0,0,0}},
		{{0,1,0}, {0,0,0}, {0,0,0}}
	};
	for (i=0; i<4; i++) {
		for (s=0; s<4; s++) {
			for (y=0; y<3 && flag; y++) {
				for (x=0; x<3 && flag; x++) {
					switch (i) {
						case 0:
							if (mask[s][y][x] != p[a+position[y][x][0]][b+position[y][x][1]]) flag = 0;
							break;
						case 1:
							if (mask[s][y][x] != p[a+position[y][x][1]][b+position[y][x][0]]) flag = 0;
							break;
						case 2:
							if (mask[s][y][x] != p[a+position[reverse(y)][reverse(x)][0]][b+position[reverse(y)][reverse(x)][1]]) flag = 0;
							break;
						case 3:
							if (mask[s][y][x] != p[a+position[reverse(y)][reverse(x)][1]][b+position[reverse(y)][reverse(x)][0]]) flag = 0;
							// printf("%d %d yx %d %d\n",a+position[y][x][0], b+position[y][x][1], y, x);
							break;
					}
				}
			}

			if (flag == 1) {
				switch (i) {
					case 0:
						if(s == 0 || s == 3) p[a-1][b] = 0;
						else p[a-1][b] = 1;
						break;
					case 1:
						if(s == 0 || s == 3) p[a][b-1] = 0;
						else p[a][b-1] = 1;
						break;
					case 2:
						if(s == 0 || s == 3) p[a+1][b] = 0;
						else p[a+1][b] = 1;
						break;
					case 3:
						if(s == 0 || s == 3) p[a][b+1] = 0;
						else p[a][b+1] = 1;
						break;
				}
			}
			flag = 1;
		}
	}
}

int reverse(int a)
{
	if (a == 2) return 0;
	if (a == 0) return 2;
	return 1;
}

// 00 01 02
// 10 11 12
// 20 21 22

// 00 01 02 10 11 12 20 21 22
// 00 10 20 01 11 12 02 12 22 // xy逆転
// 22 21 20 12 11 10 02 01 00 // 逆順
// 22 12 02 21 11 01 20 10 00 // xy逆転の逆順

// 0 1 2
// 2 1 0

