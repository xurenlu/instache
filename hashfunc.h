//
//  hashfunc.h
//  hlite
//
//  Created by xu renlu on 13-11-7.
//  Copyright (c) 2013年 xu renlu. All rights reserved.
//
unsigned int SDBMHash(char *str);
unsigned int RSHash(char *str);
unsigned int JSHash(char *str);
// P. J. Weinberger Hash Function
unsigned int PJWHash(char *str);
// ELF Hash Function
unsigned int ELFHash(char *str);
unsigned int BKDRHash(char *str);
// DJB Hash Function
unsigned int DJBHash(char *str);
// AP Hash Function
unsigned int APHash(char *str);
