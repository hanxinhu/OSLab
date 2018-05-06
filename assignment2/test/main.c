#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void printDirname(char * dir,int size);
void printFilename(char * filename,int size);
void printStr(char * str,int size) ;


typedef struct File
{
	char filename[12];
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

int dataOffset;
int numOfFiles = 0;
FILE * file12;
void initBPB();
File * initFile(int offset);
char * intToChar(int number);
File * getfile(char * s, File ** files);
char  ope_ls[4] = {'l','s','\0'};
char  ope_cat[4] = {'c','a','t','\0'};
char  ope_count[6] = {'c','o','u','n','t','\0'};
char ope_exit[5] ={'e','x','i','t','\0'};
void ls(File ** file);
void printFile(File * f);
void lsDir(char * prefix,File * f);
File ** file_all;
int isTrue(char c){
	if( (c>='0'&&c<='9') || (c>='a'&&c<='z')
		||(c>='A'&&c<='Z')||(c==' '||c=='\0'||c=='.'))
		return 1;
	return 0;
}
int getFATValue(int num);
void countDir(File * f);
int * countNums(File * f,int * data);
void cat(File * f);
int main(){
	
	file12 = fopen("a.img","rb");
	initBPB();
	File ** files = calloc(30,sizeof(File));
//	memset(files,0,sizeof(files));
	
	for (int i = 0; i < RootEntCnt; ++i)
	{
	File * f =	initFile(19*512+32*i);
		
	if (f!=NULL)
	{
	 	files[numOfFiles] = f;
	 	// printf("%s %d %d\n",f->filename,f->attribute,f->firstClus);
		numOfFiles++;
	}
	}
	file_all = files;

	
	while(1){
	
		char ins[256]={0};
		gets(ins);
		char ope[6] = {0};
		char param[255] ={0};
		int index = 0;
		for (int i = 0; i < 6&&ins[index]!=' '&&ins[index]!=0; ++i)
		{	
			ope[index] = ins[index];
			index ++;
		}
		if(ins[index]!=0){
			index ++;
			int i = 0;
			while(ins[index]!=0)
			{
				param[i] = ins[index];
				i++;
				index++;
			}	
		}
		
		int cmp = strcasecmp(ope_exit,ope);
		if (cmp==0)
		{	
			printStr("program is end\n",16);
			break;
		}
		cmp =  strcasecmp(ope_ls,ope);
		if(0==cmp){
			if (param[0]==0)
			{
				ls(files);
				continue;
			}

			File * f_1 =  getfile(param,files);
			
			if (f_1==NULL)
			{
				printStr(param,strlen(param));
				printStr(" is not a dir\n",14);
				continue;
			}
			if(f_1->attribute==0x10)
			{lsDir(param,f_1);
			}else{
				printStr(param,strlen(param));
				printStr(" is a file\n",14);
				continue;
			}
		}
		cmp = strcasecmp(ope_count,ope);
		if (0==cmp)
		{
			File * f_2 =  getfile(param,files);
			if(f_2!=NULL)
			countDir(f_2);
		continue;
		}
		cmp = strcasecmp(ope_cat,ope);
		if (0==cmp)
		{
			File * f_2 =  getfile(param,files);
			if(f_2!=NULL)
			cat(f_2);
		continue;
		}
	
	}
	
	fclose(file12);
};

char * intToChar(int number){
		char str[5]={0};
	for (int i = 0; i < 4; ++i)
	{
		str[3-i] =(char)(( number % 10) + '0');
		number = number/ 10;
	}
	for (int i = 0; i < 3; ++i)
	{
		if (str[i]=='0')
		{
			str[i]=' ';
		}else
		break;
	}
	printStr(str,5);
	return NULL;
}
void countDir(File * f){
	int data[2]= {0,0};
	if (f->attribute!=0x10)
	{
		printStr(f->filename,12);
		printStr("is not a dir",14);
	}
	countNums(f,data);
	printStr(f->filename,strlen(f->filename));
	printStr(": ",2);
	 // char * s = intToChar(data[0]);
		// printf("%s\n",s );
	intToChar(data[0]);
	printStr(" dir(s)",7);
	// s = intToChar(data[1]);
	// printStr(s,strlen(s));
	intToChar(data[1]);
	printStr(" file(s)",8);
	printStr("\n",1);

	int index = 0;
	while(f->files[index]!=NULL){
		if (f->files[index]->attribute==0x10)
		{
			countDir(f->files[index]);
		}
		index++;
	}
};
int * countNums(File * f,int * data){
	int index = 0;
	while(f->files[index]!=NULL){
		if(f->files[index]->attribute==0x10)
			{
				data[0] += 1;
				countNums(f->files[index],data);
			}
		else
			data[1] += 1;
		index++;	
	}
};

void cat(File * f){
	printf("%s\n",f->filename );
	int clus = f->firstClus;
 	int off = dataOffset + (clus-2) * 512;
 
	int vaule =	clus;
	while(vaule < 0xff7){

 	int off = dataOffset + (vaule-2) * 512;
 	fseek(file12,off,SEEK_SET);
 	char buffer[512];
	fread(buffer,1,512,file12);
	printStr(buffer,strlen(buffer));
	vaule = getFATValue(vaule);
	
	}
	printStr("\n",1);
}
void ls(File ** files){
	printStr("/:\n",3);
	for (int i = 0; i < numOfFiles; ++i)
	{
		if (files[i]->attribute==0x10)
		{
			printDirname(files[i]->filename,11);
			printStr(" ",1);
		}else{
			printFilename(files[i]->filename,11);
			printStr(" ",1);
		}
	}
	printStr("\n",1);
	for (int i = 0; i < numOfFiles; ++i)
	{
		if (files[i]->attribute==0x10)
		{
			char prefix[15]  ={0};
			prefix[0] = '/';
			int len = strlen(files[i]->filename);
			for (int j = 0; j < len; ++j)
			{
				prefix[j+1] = files[i]->filename[j];
			}
			lsDir(prefix,files[i]);
			printStr(" ",1);
		}
	}
}
void lsDir(char * prefix,File * f){
	int isDir = f->attribute == 0x10;
	int len = strlen(prefix);
	printStr(prefix,len);
	printStr(":\n",2);
	if (!isDir)
	{
		printf("%s is not a dir\n",f->filename);
	}
	int index =0 ;
	while(f->files[index]!=NULL){
		if (f->files[index]->attribute==0x10)
		{
			printDirname(f->files[index]->filename,11);
			printStr(" ",1);
		}else{
			printFilename(f->files[index]->filename,11);
			printStr(" ",1);
		}
		index ++;
	}
	printStr("\n",1);
	for (int i = 0; i < index; ++i)
	{
		if (f->files[i]->attribute==0x10)
		{
			int filenameLen = strlen(f->files[i]->filename);
			char newPrefix[100] = {0};
			for (int j = 0; j < len; ++j)
			{
				newPrefix[j] = prefix[j];
			}
			newPrefix[len] = '/';
			for (int j = 0; j < filenameLen; ++j)
			{
				newPrefix[len+j+1] = f->files[i]->filename[j];
			}
			// printStr(newPrefix,100);
			lsDir(newPrefix,f->files[i]);
		}
	}
};

File * getfile(char * str, File ** files){
	// printf("%s\n",str );
	File ** p = files;
	char filename[12] = {0};
	File * f;
	char s = '/';
	int index = 0;

	while(str[index]!='0'){
		if (str[index]!='/')
			break;
		else 
			index += 1;
	for (int i = 0; i < 11 &&str[index]!=0; ++i)
	{
		if (str[index]=='/')
			break;
		else{
			filename[i] = str[index];
			index++;
		}

	}

	// printf("%s\n",filename);
	int fileIndex = 0;
	int find = 0;
	while(p[fileIndex]!=NULL){
		int unequal = strcasecmp(p[fileIndex]->filename,filename);
		// printf("%s %s %d\n",p[fileIndex]->filename,filename,unequal);
		if (unequal)
		{
			fileIndex ++;
			continue;
		}else{
			f = p[fileIndex];
			p = f->files;
			find = 1;
			break;
		}
	}
	 if (find == 0)
	 	return NULL;
	}
	return f;
	
}
void printFile(File * f){
	if (f->attribute==0x10)
	{	printFilename(f->filename,11);
	
		int index = 0;
		while(f->files[index]!=NULL){
			printFile(f->files[index]);
			index ++;
		}
	}
	else
		printDirname(f->filename,11);
}



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
	dataOffset = 19 * 512 + 32 * RootEntCnt;
	// printf(" %d %d %d %d %d %d %d %d\n",BytsPerSec,SecPerClus,RsvdSecCnt,NumFATs,RootEntCnt,FATSz,TotalSec,dataOffset);


};
File * initFile(int offset){
	char filenames[11]={0};
	memset(filenames,0,sizeof(filenames));
	int  attribute = 0;
	int firstClus = 0;
	int *  attribute_p = & attribute;
	int * firstClus_p = & firstClus;

	fseek(file12,offset,SEEK_SET);
	fread(filenames,1,11,file12);
	fread(attribute_p,1,1,file12);

	int off = offset + 0x1A;
	fseek(file12,off,SEEK_SET);
	fread(firstClus_p,1,2,file12);
	if(filenames[0]==0)
		return NULL;

	
	char realnames[12]={0};

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
	if (firstClus!=0)
	if (attribute == 0x20 || attribute == 0x10)
	{
		for (int i = 0; i < 12; ++i)
		{
		if(!isTrue(realnames[i]))
				return NULL;	
		}
		if (realnames[0]=='.')
		{
			return NULL;
		}
		
		 // printf("%s %d %d\n",realnames,attribute,firstClus );
		
		File * f = (File * ) malloc(sizeof(File));
		strcpy(f->filename,realnames);
		f->attribute = attribute;	
		f->firstClus = firstClus;

		if (attribute == 0x10)
		{
			int num = 0;
			for (int i = 0; i < 512 / 32; ++i)
			{
			File * ff = initFile(dataOffset + (firstClus-2) * 512 + 32 * i);
			if (ff != NULL)
				f->files[num++] = ff;
			}
			// printf("%d\n",num );
		}
		return  f;
	}
	
	return NULL;
};



int getFATValue(int num){
	    //FAT1的偏移字节  
    int fatBase = RsvdSecCnt * BytsPerSec;  
    //FAT项的偏移字节  
    int fatPos = fatBase + num*3/2;  
    //奇偶FAT项处理方式不同，分类进行处理，从0号FAT项开始  
    int type = 0;  
    if (num % 2 == 0) {  
        type = 0;  
    } else {  
        type = 1;  
    }  
    unsigned short byte = 0;
    unsigned short * byte_ptr = & byte;
    fseek(file12,fatPos,SEEK_SET);
    fread(byte_ptr,1,2,file12);
     //u16为short，结合存储的小尾顺序和FAT项结构可以得到  
    //type为0的话，取byte2的低4位和byte1构成的值，type为1的话，取byte2和byte1的高4位构成的值  
    if (type == 0) {  
        return byte<<4;  
    } else {  
        return byte>>4;  
    }   
};