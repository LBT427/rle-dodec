#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 65536000
//�����ļ� 
//����ʹ�õ���char���洢���ݣ�char�����ֵҲ��ֻ��255��
//����char����λ������Ҫ������¼�����������ݣ������ܹ���¼����󳤶�Ҳ��ֻ��127
unsigned char file1[MAX];
unsigned char file2[MAX];

//�ж��Ƿ񳬹�3λ��ͬ 
int Judge(unsigned char* src, int left) {
    if (left < 2)
        return 0;
    if (*src == *(src + 1) && *src == *(src + 2))
        return 1;
    return 0;
}

//���ظ�ʱ�ĳ��� 
int GetCnt(unsigned char* src, int left) {
    if (left <= 2)
        return left + 1;
    else {
        int cnt = 0,
            leftcnt = left;
        unsigned char* g = src;
        //�����ڳ�3λ��ͬ 
        while (Judge(g, leftcnt) == 0) {
            g++;
            leftcnt--;
            cnt++;
            //�ﵽ��󳤶ȣ�ֱ�ӷ��� 
            if (cnt >= 128)
                return cnt;
            if (leftcnt == 0)
                return cnt + 1;
        }
        return cnt;
    }
}
//�ظ�ʱ�ĳ��� 
int GetYnt(unsigned char* src, int left) {
	if (left <= 2)
		return left + 1;
	else {
		int cnt = 0;
			//leftcnt = left;
		//unsigned char* g = src;
		//�����ڳ�3λ��ͬ 
		while (Judge(src, left) == 1) {
			src++;
			left--;
			cnt++;
			//�ﵽ��󳤶ȣ�ֱ�ӷ��� 
			if (cnt >= 128)
				return cnt;
			if (left == 0)
				return cnt + 1;
		}
		return cnt;
	}
}
int Rle_Encode(unsigned char *inbuf, int inSize, unsigned char *outbuf, int onuBufSize)
{
    unsigned char *src = inbuf;
    int i;
    int encSize = 0;
    int srcLeft = inSize;
    while(srcLeft > 0)
    {
        int count = 0;
        if(Judge(src, srcLeft)) /*�Ƿ����������ֽ�������ͬ�� */
        {
            if ((encSize + 2) > onuBufSize) /* ����������ռ䲻���� */
            {
                return -1;
            }
            count = GetYnt(src, srcLeft);
            outbuf[encSize++] = count | 0x80;
            outbuf[encSize++] = *src;
            src += count;
            srcLeft -= count;
        }
        else
        {
            count = GetCnt(src, srcLeft);//��ȡ���ظ��ĳ���
            if ((encSize + count + 1) > onuBufSize) /* ����������ռ䲻���� */
            {
                return -1;
            }
            outbuf[encSize++] = count;
            for(i = 0; i < count; i++) /*���������Щ����*/
            {
                outbuf[encSize++] = *src++;;
            }
            srcLeft -= count;
        }
    }
    return encSize;
}
int Rle_Decode(unsigned char *inbuf, int inSize, unsigned char *outbuf, int onuBufSize) {
    unsigned char *src = inbuf;
    int i;
    int decSize = 0;
    int count = 0;
    while(src < (inbuf + inSize))
    {
        unsigned char sign = *src++;
        int count = sign & 0x3F;
        if ((decSize + count) > onuBufSize) /* ����������ռ䲻���� */
        {
            return -1;
        }
        if ((sign & 0x80) == 0x80) /* �����ظ����ݱ�־ */
        {
            for(i = 0; i < count; i++)
            {
                outbuf[decSize++] = *src;
            }
            src++;
        }
        else
        {
            for(i = 0; i < count; i++)
            {
                outbuf[decSize++] = *src++;
            }
        }
    }
    return decSize;
}
int main(int argc, char** argv) {
    char buff1[100]={1,2,2,2,2,3,3,3,3,3,4,4,4,4,6,5,4,3,2};//{'a','s','d','g','h','h','j','y','t','r','e','e','r','r','n','g','j','j','i','y','j','t','f'};
    char buff2[100]={0};
    char buff3[100]={0};

    Rle_Encode(buff1, 100, buff2, 100);
    Rle_Decode(buff2, 100, buff3, 100);
   // Encode(buff2, 100, buff3, 100);

    for (int i = 0; i < 19; i++)
    {
        printf("%d\n", buff3[i]);
    }
    return 0;
}
