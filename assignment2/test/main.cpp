#include <stdio.h>

typedef struct File
{
	char filename[11];
	int attribute;
	int firstClus;
	struct 	File * files[20];
}File;
int  BytsPerSec;    //每扇区字节数  
int  SecPerClus;    //每簇扇区数  
int  RsvdSecCnt;    //Boot记录占用的扇区数  
int  NumFATs;   //FAT表个数  
int  RootEntCnt;    //根目录最大文件数  
int TotalSec;
int  FATSz; //FAT扇区数  

FILE * file12;
void initBPB();
File * initFile(int offset);
int main(){
	
	file12 = fopen("a.img","rb");
	initBPB();
	File * *files = new File *[12];
	int num = 0;
	for (int i = 0; i < 10; ++i)
	{
		int offset = 19 * 512 + 32 * i;
		initFile(offset);

	}
	

	printf("%d\n", num);
	fclose(file12);
};
void initBPB(){

	int * BytsPerSec_P = & BytsPerSec;
	int * SecPerClus_P = & SecPerClus;    
	int * RsvdSecCnt_P = & RsvdSecCnt;    
	int * NumFATs_P = & NumFATs;  
	int * RootEntCnt_P = & RootEntCnt; 
	int * TotalSec_P= & TotalSec;   
	int * FATSz_P = & FATSz;
	fseek(file12,11,SEEK_SET);
	fread(BytsPerSec_P,1,2,file12);
	fread(SecPerClus_P,1,1,file12);
	fread(RsvdSecCnt_P,1,2,file12);

	fread(NumFATs_P,1,1,file12);
	fread(RootEntCnt_P,1,2,file12);
	fread(TotalSec_P,1,2,file12);
	fseek(file12,22,SEEK_SET);
	fread(FATSz_P,1,2,file12);
	printf(" %d %d %d %d %d %d %d\n",BytsPerSec,SecPerClus,RsvdSecCnt,NumFATs,RootEntCnt,FATSz,TotalSec);

}
File * initFile(int offset){
	

		char filenames[11];
	int attribute;
	int firstClus;
	int * attribute_p = & attribute;
	int * firstClus_p = & firstClus;

	fseek(file12,offset,SEEK_SET);
	
	fread(filenames,1,11,file12);
	fread(attribute_p,1,1,file12);
	fseek(file12,offset+0x1A,SEEK_SET);
	fread(firstClus_p,1,2,file12);

	
	
	char realnames[12];
	
	int index = 0;
	for (int i = 0; i < 8; ++i)
	{
		if (filenames[i]!=' ')
		 	 realnames[i] = filenames[i];
		else
		{
			index = i;
			break;
		}
	}
	if (attribute == 0x20)
	{
		realnames[index]='.';
		for (int i = 0; i < 3; ++i)
		{
			realnames[index+1+i]=filenames[8+i];
		}
	}
	if (attribute == 0x20 || attribute == 0x10)
	{
		printf("%s\n",realnames);
	}
	
	return NULL;
	
	return NULL;
}