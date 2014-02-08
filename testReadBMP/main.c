#include "stdio.h"
#include "stdlib.h"

#define  OFFSET 54L		//����ͷƫ����

#pragma pack(2)

/*����UCHARΪһ���ֽڵ�����*/
typedef unsigned char UCHAR;
/*����WORDΪ�����ֽڵ�����*/
typedef unsigned short WORD;
/*����DWORDΪe�ĸ��ֽڵ�����*/
typedef unsigned long DWORD;

/*λͼ�ļ�ͷ*/
typedef struct BMP_FILE_HEADER
{
	WORD bType;             /*  �ļ���ʶ��          */
	DWORD bSize;            /*  �ļ��Ĵ�С          */
	WORD bReserved1;        /*  ����ֵ,��������Ϊ0  */       
	WORD bReserved2;        /*  ����ֵ,��������Ϊ0  */
	DWORD bOffset;          /*  �ļ�ͷ�����ͼ������λ��ʼ��ƫ����    */
} BMPFILEHEADER;

/*λͼ��Ϣͷ*/
typedef struct BMP_INFO
{
	DWORD bInfoSize;       /*  ��Ϣͷ�Ĵ�С             */
	DWORD bWidth;          /*  ͼ��Ŀ��               */
	DWORD bHeight;         /*  ͼ��ĸ߶�               */
	WORD bPlanes;          /*  ͼ���λ����             */
	WORD bBitCount;        /*  ÿ�����ص�λ��           */
	DWORD bCompression;    /*  ѹ������                 */
	DWORD bmpImageSize;    /*  ͼ��Ĵ�С,���ֽ�Ϊ��λ  */
	DWORD bXPelsPerMeter;  /*  ˮƽ�ֱ���               */
	DWORD bYPelsPerMeter;  /*  ��ֱ�ֱ���               */
	DWORD bClrUsed;        /*  ʹ�õ�ɫ����             */
	DWORD bClrImportant;   /*  ��Ҫ����ɫ��             */
} BMPINF;

/*��ɫ��*/
typedef struct RGB_QUAD 
{
	WORD rgbBlue;         /*  ��ɫǿ��  */
	WORD rgbGreen;        /*  ��ɫǿ��  */
	WORD rgbRed;          /*  ��ɫǿ��  */
	WORD rgbReversed;     /*  ����ֵ    */
} RGBQUAD;

//����һ���ֽ�����ΪUCHAR
typedef unsigned char UCHAR;

int readBmp(UCHAR **image, int height, int width, char *filename);
void displayImg(UCHAR **image, int height, int width);
int saveImg(UCHAR **image, int height, int width, char *filename);
void test(UCHAR *image);
UCHAR BGR2GRAY(UCHAR a, UCHAR b, UCHAR c);

//������
int main()
{
	//������������
	char *filename = "C:\\Users\\Mirgo\\Desktop\\ac.bmp";
	char *outfname = "C:\\Users\\Mirgo\\Desktop\\a.txt";
	int height = 64;
	int width = 64;
	int i = 0;

	//����ά���鶯̬����ռ�
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

	free(img);		//���ۼ�ά���鶼�������ͷŵ�

	return 0;
}

//��ȡͼ��
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
		//������
		printf("Cann't Open the BMP File!\nPlease Check the Filename!\n");
		exit(0);
	}

	fseek(fp, 0, 0);
	fread(&fileHeader, sizeof(fileHeader), 1, fp);
	fread(&infoHeader, sizeof(infoHeader), 1, fp);

	//���㲢���λͼ���ݵ�ƫ������ͼ��Ĵ�С����Ⱥ͸߶ȣ�ÿ�����ص���ռ���ֽ�
	size = fileHeader.bSize;
	offset = fileHeader.bOffset;
	bmpImageSize = infoHeader.bmpImageSize;
	mwidth = infoHeader.bWidth;
	mheight = infoHeader.bHeight;
	bitCount = infoHeader.bBitCount;
	bytesPerPixel = infoHeader.bBitCount / 8;

	printf("%c %d %d %d %d %d %d %d\n",fileHeader.bType, size, offset, bmpImageSize, mwidth, mheight, bitCount, bytesPerPixel);
	printf("%d %d\n",sizeof(fileHeader),sizeof(infoHeader));

	//ƫ���������ļ�ͷ
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

	//���ÿ�����ص���ռ�ֽ��е�����
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

//��ʾͼ��
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

//����ͼ��TXT�ļ�
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
			//������ļ�
			
			(image[i][j] == 0)?fputc(0, fp):fputc(2, fp);
			k++;
		}
		//һ����ɣ�����
		fputc('\n',fp);
	}

	printf("����saveImg()������ɣ����ص㣺%d��\n",k);

	fclose(fp);

	system("pause");
	
	return 0;
}

//������
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