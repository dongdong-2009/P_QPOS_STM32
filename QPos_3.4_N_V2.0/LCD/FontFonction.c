#include "string.h"
#include "FontFonction.h"
#include "msd.h"
//#include "ccid_value.h"

#define HANZIKU_ADDR	0
void trans_hz8(unsigned char * outbuf,unsigned char * inbuf)
{
    unsigned char i=0,k=0,j=0;
	
    for(i=0;i<8;i++)
    {    
        for(k=0;k<8;k++)
        {
            j = (inbuf[k]&(0x80>>i)) ;
            if(j==0)
            {
                outbuf[i] &= ~(0x01 << k);
            }
            else
            {
                outbuf[i] |= 0x01 << k;
            } 
        }
        
    }
} 

void trans_hz4(unsigned char * outbuf,unsigned char * inbuf)
{
    unsigned char i=0,k=0,j=0;
	
    for(i=0;i<4;i++)
    {    
        for(k=0;k<8;k++)
        {
            j = (inbuf[k]&(0x80>>i)) ;
            if(j==0)
            {
                outbuf[i] &= ~(0x01 << k);
            }
            else
            {
                outbuf[i] |= 0x01 << k;
            } 
        }
        
    }
}
void get_hanzi_mu(char * hanzi, unsigned char *zimu)
{
	unsigned char c1, c2;
	unsigned long zimuaddr;
	unsigned char out_zimu[24];
	
	unsigned char buf0[8],buf1[8],buf2[8],buf3[8];
	unsigned char buf00[8],buf11[8],buf22[8],buf33[8];
	unsigned char i, a=0, b=0, c=0, d=0;
	
	memset(zimu, 0xff, 24);
	
	c1 = hanzi[0];
	c2 = hanzi[1];

	if(c1 <= 0x80)
		return ;
	
	if (c1 >= 0xb0 && c2 >= 0xa1)   //Ë«×Ö½Úºº×Ö2Çø
	{
		//zimuaddr = HANZIKU_ADDR + ((c1 - 0xb0) * 94 + (c2 - 0xa1)) * 24;
		zimuaddr = HANZIKU_ADDR + ((c1 - 0xb0) * 191 + (c2 - 0xa1)) * 24+9074*24;
	}
	
	if (c1 >= 0x81 && c1 < 0xa1 && c2 >= 0x40)
	{//Ë«×Ö½Úºº×Ö3Çø
		//zimuaddr = HANZIKU_ADDR + ((c1 - 0x81) * 190 + (c2 - 0x40)) * 2 + 6768 * 24;
		zimuaddr = HANZIKU_ADDR + ((c1 - 0x81) * 191 + (c2 - 0x40)) * 24;//* 2 + 6768 * 24;
	}
	
	if (c1 >= 0xaa && c2 >= 0x40 && c2 < 0xa1)
	{//Ë«×Ö½Úºº×Ö4Çø
		//zimuaddr = HANZIKU_ADDR + ((c1 - 0xaa) * 96 + (c2 - 0x40)) * 2 + (6768 + 6080) * 24;
		zimuaddr = HANZIKU_ADDR + ((c1 - 0xaa) * 191 + (c2 - 0x40)) * 24 + 7831 * 24;
	}
	
	if(c1 >= 0xa1 && c1 <= 0xa9 && c2 >= 0xa1 && c2 <= 0xfe)  //Ë«×Ö½Ú×Ö·û1Çø
	{
		//zimuaddr= HANZIKU_ADDR + ((c1 - 0xa1) * 94 + (c2 - 0xa1)) * 24;
		zimuaddr= HANZIKU_ADDR + ((c1 - 0xa1) * 191 + (c2 - 0xa1)) * 24+(6080+129)*24;
	}
	if(c1 >= 0xa8 && c1 <= 0xa9 && c2 >= 0x40 && c2 <= 0xfe)  //Ë«×Ö½Ú×Ö·û5Çø
	{
		zimuaddr = HANZIKU_ADDR + ((c1 - 0xa8) * 191 + (c2 - 0x40)) * 24 + 7449 * 24;
	}
	
	MSD_ReadBlock(out_zimu,zimuaddr, 24);
	
	memset(buf0, 0, 8);
	memset(buf1, 0, 8);
	memset(buf2, 0, 8);
	memset(buf3, 0, 8);
	memset(buf00, 0, 8);
	memset(buf11, 0, 8);
	memset(buf22, 0, 8);
	memset(buf33, 0, 8);
	
	for(i=0;i<24;)
	{
		if(i<16)
		{
			buf0[a++] = out_zimu[i++];
			buf1[b++] = out_zimu[i++];
		}
		else
		{
			buf2[c++] = out_zimu[i++];
			buf3[d++] = out_zimu[i++];
		}
	}
	
	
	trans_hz8(buf00, buf0);
	trans_hz4(buf11, buf1);
	trans_hz8(buf22, buf2);
	trans_hz4(buf33, buf3);
	memcpy(out_zimu,buf00,8);
	memcpy(out_zimu+8,buf11,4);
	memcpy(out_zimu+12,buf22,8);
	memcpy(out_zimu+20,buf33,4);
		
	memcpy(zimu, out_zimu, 24);
		
	return;
}

