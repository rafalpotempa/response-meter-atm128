#ifndef SYS_RQ_H
#define SYS_RQ_H

typedef union TSysRq {
    unsigned char msg;
    struct
    {
        unsigned char t : 1; /*time tick*/
        unsigned char r : 1;
        unsigned char l : 1;
        unsigned char pb : 1;
        unsigned char dummy : 4;
    };
} TSysRq;

#endif
