#include "stdio.h"
#include "stdlib.h"

#define  OFFSET 54L		//定义头偏移量

#pragma pack(2)

/*定义UCHAR为一个字节的类型*/
typedef unsigned char UCHAR;
/*定义WORD为两个字节的类型*/
typedef unsigned short WORD;
/*定义DWORD为e四个字节的类型*/
typedef unsigned long DWORD;

/*位图文件头*/
typedef struct BMP_FILE_HEADER
{
	WORD bType;             /*  文件标识符          */
	DWORD bSize;            /*  文件的大小          */
	WORD bReserved1;        /*  保留值,必须设置为0  */       
	WORD bReserved2;        /*  保留值,必须设置为0  */
	DWORD bOffset;          /*  文件头的最后到图像数据位开始的偏移量    */
} BMPFILEHEADER;

/*位图信息头*/
typedef struct BMP_INFO
{
	DWORD bInfoSize;       /*  信息头的大小             */
	DWORD bWidth;          /*  图像的宽度               */
	DWORD bHeight;         /*  图像的高度               */
	WORD bPlanes;          /*  图像的位面数             */
	WORD bBitCount;        /*  每个像素的位数           */
	DWORD bCompression;    /*  压缩类型                 */
	DWORD bmpImageSize;    /*  图像的大小,以字节为单位  */
	DWORD bXPelsPerMeter;  /*  水平分辨率               */
	DWORD bYPelsPerMeter;  /*  垂直分辨率               */
	DWORD bClrUsed;        /*  使用的色彩数             */
	DWORD bClrImportant;   /*  重要的颜色数             */
} BMPINF;

/*彩色表*/
typedef struct RGB_QUAD 
{
	WORD rgbBlue;         /*  蓝色强度  */
	WORD rgbGreen;        /*  绿色强度  */
	WORD rgbRed;          /*  红色强度  */
	WORD rgbReversed;     /*  保留值    */
} RGBQUAD;

//定义一个字节类型为UCHAR
typedef unsigned char UCHAR;

int readBmp(UCHAR **image, int height, int width, char *filename);
void displayImg(UCHAR **image, int height, int width);
int saveImg(UCHAR **image, int height, int width, char *filename);
void test(UCHAR *image);
UCHAR BGR2GRAY(UCHAR a, UCHAR b, UCHAR c);

//主函数
int main()
{
	//基本变量定义
	char *filename = "C:\\Users\\Mirgo\\Desktop\\ac.bmp";
	char *outfname = "C:\\Users\\Mirgo\\Desktop\\a.txt";
	int height = 64;
	int width = 64;
	int i = 0;

	//给二维数组动态分配空间
	UCHAR **img;
	img = (UCHAR **)malloc( (unsigned)(height * sizeof(UCHAR *) ) );
	for (; i<width; i++)
	{
		img[i] = (UCHAR *)malloc( (unsigned)(width * sizeof(UCHAR)));
	}

	if (img == NULL)
	{
		printf("alloc memory failed!");
		exit(0);
	}
	//UCHAR img[128220] = {0};

	readBmp(img,height,width,filename);
	saveImg(img,height,width,outfname);
	displayImg(img,height,width);
	//test(img);

	system("pause");

	free(img);		//无论几维数组都是这样释放的

	return 0;
}

//读取图像
int readBmp(UCHAR **image, int height, int width, char *filename)
{
	FILE *fp;
	UCHAR *temp;
	UCHAR c, cl[3];
	BMPFILEHEADER fileHeader;
	BMPINF infoHeader;
	long offset, bmpImageSize, mwidth, mheight, bytesPerPixel, size, bitCount;
	//char m_fname[128];
	int i = 0;
	int count = 0, l = 0,r = 0,en = 0;
	//WORD c;

	temp = (UCHAR*)malloc(sizeof(UCHAR)*height*width*3);
	//sprintf(m_fname, "%s", filename);

	if ((fp = fopen(filename,"rb")) == NULL)
	{
		//出错处理
		printf("Cann't Open the BMP File!\nPlease Check the Filename!\n");
		exit(0);
	}

	fseek(fp, 0, 0);
	fread(&fileHeader, sizeof(fileHeader), 1, fp);
	fread(&infoHeader, sizeof(infoHeader), 1, fp);

	//计算并输出位图数据的偏移量，图像的大小，宽度和高度，每个像素点所占的字节
	size = fileHeader.bSize;
	offset = fileHeader.bOffset;
	bmpImageSize = infoHeader.bmpImageSize;
	mwidth = infoHeader.bWidth;
	mheight = infoHeader.bHeight;
	bitCount = infoHeader.bBitCount;
	bytesPerPixel = infoHeader.bBitCount / 8;

	printf("%c %d %d %d %d %d %d %d\n",fileHeader.bType, size, offset, bmpImageSize, mwidth, mheight, bitCount, bytesPerPixel);
	printf("%d %d\n",sizeof(fileHeader),sizeof(infoHeader));

	//偏移以跳过文件头
	//fseek(fp, OFFSET, 0);
	/*fread(&temp, sizeof(UCHAR), height*width*3, fp);
	fclose(fp);

	for (i = 0; i<height*width*3; i = i+3)
	{
		 c = (UCHAR)((temp[i]*30 + temp[i+1]*59 + temp[i+2]*11 + 50)/100);
		 image[i/3] = (c>0?1:0) + 48;
	}
	image[height*width] = '\0';*/

	//fseek(fp, -54L, 1);

	//输出每个像素点所占字节中的内容
	c = fgetc(fp);
	while (!feof(fp))
	{
		//printf("%x", c==0xff?1:c);
		c = fgetc(fp);
		cl[l++] = c;
		if (l == 3)
		{
			temp[count] = BGR2GRAY(cl[0],cl[1],cl[2]);
			l = 0;
			count++;
		}

		if (count % 64 == 0)
		{
			//printf("\n");
		}
		
		r++;
	}

	printf("\n%d %d\n",count,r);


	count = 0;
	for (i = height-1; i>=0; i--)
	{
		for (l = 0; l<width; l++)
		{
			image[i][l] = temp[count];
			count++;
		}
	}
	

	
	fclose(fp);

	system("pause");

	return 0;
}

//显示图像
void displayImg(UCHAR **image, int height, int width)
{
	int i,j;

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			if (image[i][j] == 0)
			{
				printf("%c",0);
			}
			else
			{
				printf("%c",2);
			}
		}
		printf("\n");
	}
}

//保存图像到TXT文件
int saveImg(UCHAR **image, int height, int width, char *filename)
{
	FILE *fp = NULL;
	int i, j, k = 0;
	//UCHAR c;

	fp = fopen(filename,"w");

	//fwrite(image, sizeof(UCHAR), 64*64, fp);

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			//输出到文件
			
			(image[i][j] == 0)?fputc(0, fp):fputc(2, fp);
			k++;
		}
		//一行完成，换行
		fputc('\n',fp);
	}

	printf("调用saveImg()保存完成，像素点：%d个\n",k);

	fclose(fp);

	system("pause");
	
	return 0;
}

//测试用
void test(UCHAR *image)
{
	UCHAR *p = image;
	int c = 0;

	while (*p != '\0')
	{
		printf("%d", *p>0?1:0);
		p++;
		c++;
		if (c == 64)
		{
			printf("\n");
			c = 0;
		}
	}
}

UCHAR BGR2GRAY(UCHAR a, UCHAR b, UCHAR c)
{
	return (UCHAR)((a*11 + b*59 + c*30 + 50)/100);
}