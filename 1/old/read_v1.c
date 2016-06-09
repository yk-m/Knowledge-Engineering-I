#include <stdio.h>

void printImg(unsigned char data[]);
void fprintImg(unsigned char data[]);
void fileRead(FILE *fdr);

int main(int argc, char *argv[]) {
	int i;
	FILE *fdr;
	
	unsigned char data[20][512];
	int count=0;
	
	if (argc>1) {
		if ((fdr = fopen(argv[1], "rb")) == NULL) {
			printf("can't open a file\n");
			exit(1);
		}
	} else exit(0);
	
	while (count<20) {
		if (fread(data[count], 512, 1, fdr) != 1) {
			printf("can't open a file\n");
			exit(2);
		}
		printImg(data[count]);
		while(getchar()!='\n');
		count++;
	}
	
	fclose(fdr);
	
	printf("Pushing Enter key leads the exit.");
	while(getchar()!='\n');
}

void printImg(unsigned char data[])
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

void fprintImg(unsigned char data[])
{
	int i, j, k;
	unsigned char mask = 128;
	FILE *fp;
	
	fp=fopen("result.txt","a+");
	if (fp==NULL) {
		printf("can't open a file\n");
		exit(1);
	}
	
	for (i=0; i<64; i++) {
		for (j=i*8; j<i*8+8; j++) {
			mask = 0x80;
			for (k=0; k<8; k++) {
				if ((data[j]&mask) == 0) {
					fprintf(fp, ".");
				} else {
					fprintf(fp, "*");
				}
				mask = mask >>1;
			}
		}
		fprintf(fp, "\n");
	}
	fclose(fp);
}

