#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <openssl/aes.h>
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/err.h>



unsigned int PadData (unsigned char *ibuf, unsigned int ilen, int blksize)
{
    unsigned int i; /* loop counter*/
    unsigned char pad; /* pad character (calculated)*/
    unsigned char *p; /*pointer to end of data*/

    /* calculate pad character*/
    pad = (unsigned char) (blksize - (ilen % blksize));

    /* append pad to end of string*/
    p = ibuf + ilen;
    for (i = 0; i < (int) pad; i++) {
        *p = 0x00;
        ++p;
    }

    return (ilen + pad);
}

unsigned int NoPadLen (unsigned char *ibuf, unsigned int ilen)
{
    unsigned int i; /* adjusted length*/
    unsigned char *p; /* pointer to last character*/

    p = ibuf + (ilen - 1);
    i = ilen - (unsigned int) *p;

    return (i);
}

void myfree(void *a)
{
    if(a) {
        free(a);
        a = NULL;
    }
    
    return;
}

int OpenReadInFile(char *fullname, unsigned char *OutBuf, int *rtn_size)
{
    int ifd;
    struct stat istat;
    unsigned int inSize, bit4,count;
    unsigned char *srcPtr = NULL;
    *rtn_size = 0;
    ifd = open(fullname,O_RDONLY,S_IREAD);
    if( ifd < 0 ) {
        printf("Error: Unable to open %s\n", fullname);
        return(0);
    }
    /* Get the input file properties*/
    fstat(ifd,&istat);
    /* Create the input buffer*/
    inSize = istat.st_size;
    bit4 = inSize%4;
    if(inSize+4-bit4 > 0x16000) {
        printf("File is too big.Max is 0x16000\n");
        close(ifd);
        return 0;
    }
    srcPtr = (unsigned char *) malloc(inSize+(4-bit4));
    if( srcPtr == NULL ) {
        printf("Error: Unable to create input buffer size of %d\n", inSize);
        close(ifd);
        return(0);
    }
    memset(srcPtr,0x0,inSize+(4-bit4));

    /* Read the buffer from file to local buffer*/
    count = read(ifd, (void *)srcPtr, istat.st_size);
    if( count != istat.st_size ) {
        printf("Debug: Need to handle multi-reads/error to buffer\n");
        printf("Debug: Read returned 0x%0x\n", (unsigned int) count);
        free(srcPtr);
        close(ifd);
        return(0);
    }
    memcpy(OutBuf,srcPtr,count);
    *rtn_size = count;
    close(ifd);

    return 1;
}

int OpenWriteFile(char *fullname, unsigned char *Buffer,int size)
{
    int ifd;
    struct stat istat;
    unsigned int inSize, bit4,count;
    unsigned char *srcPtr = Buffer;
    ifd = creat(fullname, S_IRUSR | S_IWUSR);
    if(ifd < 0) {
        printf("Error: Unable to create %s\n", fullname );
        return(0);
    }
    count = write(ifd, srcPtr, size);
    if( size ) {
        close(ifd);
        printf("Error: Write size error!\n", fullname );
        return(0);
    }
    close(ifd);

    return 1;
}

void hexdump(const char *title,const unsigned char *s, int len)
{
    printf("\n%s\n", title);
    for(int i = 0 ; i < len ; i++) {
        if((i % 16)==0)
            printf("\n%04x",i);
        printf(" %02x",s[i]);
    }
    printf("\n");
}

int Encrypty(const unsigned char *Key, unsigned char *InputBuf, int in_length, unsigned char *outputBuf, int *out_length)
{
    int result = 0;
    unsigned char iv[16];
    unsigned char *Output = NULL;
    unsigned char *InBuf_ptr = InputBuf;
    AES_KEY ass_key;
    int length_in_pad=0;

    /***********************
    printf("#####Key =%s\n",Key);
    printf("#####input len =%d\n",in_length);*/
    length_in_pad=PadData(InBuf_ptr,in_length,16);
    /*printf("#####length_in_pad =%d\n",length_in_pad);
    *encrypt text*/
    memset(&ass_key, 0x00, sizeof(AES_KEY));
    result = AES_set_encrypt_key(Key, 128, &ass_key);
    if(result<0) {
        printf("!!!!!!!!AES_set_encrypt_key Error!!!!!!!!!!\n ");
        return 0;
    }
    memset(iv,0,sizeof(iv));
    if((Output = (unsigned char *)malloc(length_in_pad+4)) == NULL) {
        printf("malloc error!\n");
        return 0;
    }
    memset(Output,0x00,length_in_pad+4);
    AES_cbc_encrypt(InBuf_ptr,Output,length_in_pad,&ass_key,iv,AES_ENCRYPT);
    memcpy(outputBuf,Output,length_in_pad);
    *out_length=length_in_pad;
    myfree(Output);
    /**outputBuf = Output;*/
    /*printf("#####encrypt out len =%d\n",*out_length);*/

    return 1;
}

int main(int argc, char *argv[])
{
    unsigned char Input[0x16000];
    unsigned char Output[0x16000];
    int OutputSize;
    char ifname[256];
    char fullname[256];
    char ofname[256];
    char dirname[256];
    char dst_dirname[256];
    unsigned char Key[17];/*16+1 : key length is 16 and 128 bits*/
    struct dirent *ptr = NULL;
    DIR * dir;
    int pathlen;
    int len;

    if(argc != 4) {
        printf("%s \n",argv[0]);
        return 0;
    }
    memset(Key,0,sizeof(Key));
    strcpy((char *)Key,argv[1]);
    if(strlen((char *)Key) != 16) {
        printf("Length of key must be 16 char!\n");
        return 0;
    }
    memset(Input,0x00,0x16000);
    memset(Output,0x00,0x16000);
    dir =opendir(argv[2]);
    strcpy(dirname,argv[2]);
    if(dir == NULL) {
        /*printf("open dir %s error!Not directory!\n",argv[2]);*/
        if(OpenReadInFile(argv[2],Input,&len) == 0) {
            printf("Openfile %s error!\n",argv[2]);
            return 0;
        }
        /*printf("len:%d,Inputfile:\n%s\n",len,Input);*/
        if(Encrypty(Key,Input,len,Output,&OutputSize) == 0) {
            printf("Encrypt file %s fail!\n",argv[2]);
            return 0;
        }
        /*printf("outlen:%d\n",OutputSize);*/
        /*hexdump("\nOutputfile:\n",Output,OutputSize);*/
        if(OpenWriteFile(argv[3], Output, OutputSize) == 0) {
            printf("Creat encrypt file %s fail\n",argv[3]);
            return 0;
        }
        printf("Encrypt file %s succesfull!\n",argv[2]);
        return 1;
    }

    strcpy(dst_dirname,argv[3]);
    pathlen = strlen(dst_dirname);
    if(dst_dirname[pathlen] != '/')
        strcat(dst_dirname,"/");

    while((ptr = readdir(dir))!=NULL) {
        memset(Input,0x00,0x16000);
        memset(Output,0x00,0x16000);
        strcpy(ifname, ptr->d_name);
        if(strcmp(ifname,".") == 0 || strcmp(ifname,"..") == 0 || strstr(ifname,".txt") == NULL)
            continue;
        snprintf(fullname,256,"%s%s",dirname,ifname);
        /*printf("open full path file %s !\n",fullname);*/
        snprintf(ofname, 256,"%s%s.ser",dst_dirname,ifname);
        /*printf("open file %s !\n",ifname);*/
        if(OpenReadInFile(fullname,Input,&len) == 0) {
            printf("Open file %s fail!\n",fullname);
            continue;
        }

        if(Encrypty(Key,Input,len,Output,&OutputSize) == 0) {
            printf("Encrypt file %s fail!\n",fullname);
            continue;
        }

        if(OpenWriteFile(ofname,Output,OutputSize) == 0) {
            printf("Creat encrypt file %s fail!\n",fullname);
            continue;
        }
        printf("Encrypt file %s succesfull!\n",fullname);
    }
    closedir(dir);

    return 1;
}