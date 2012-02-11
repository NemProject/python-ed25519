
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "crypto_sign.h"

char *msg = "Hello World";

int main(int argc, char *argv[]) {
    unsigned char sk[SECRETKEYBYTES], vk[PUBLICKEYBYTES];
    unsigned char *sigmsg, *newmsg;
    unsigned long long sigmsglen, newmsglen;
    int ret;
    unsigned char seed[32];
    int i;
    for (i=0; i<32; i++) seed[i] = i; // not so random
    crypto_sign_publickey(vk, sk, seed);
    printf("got keypair\n");
    sigmsg = malloc(strlen(msg)+1+SIGNATUREBYTES);
    if (!sigmsg)
        return 1;
    crypto_sign(sigmsg, &sigmsglen, (unsigned char *)msg, strlen(msg)+1, sk);
    printf("got signature\n");
    if (sigmsglen != strlen(msg)+1+SIGNATUREBYTES)
        return 2;
    newmsg = malloc(sigmsglen);
    if (!newmsg)
        return 3;
    ret = crypto_sign_open(newmsg, &newmsglen, sigmsg, sigmsglen, vk);
    printf("verified signature\n");
    if (ret == 0)
        printf("good!\n");
    else
        printf("bad\n");
    sigmsg[0] ^= 0x01;
    ret = crypto_sign_open(newmsg, &newmsglen, sigmsg, sigmsglen, vk);
    if (ret == 0) 
        printf("bad: failed to detect simple corruption\n");
    else
        printf("good: detected simple corruption\n");
    return 0;
}
