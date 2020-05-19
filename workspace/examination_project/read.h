#ifndef __READ_H__
#define __READ_H__


#include<stdio.h>
#include<error.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdlib.h>
#include "linklist.h"
#include "excel.h"
#include "analysis_info.h"

extern int DQ_NUM;
extern int chanel_max;
extern int rank_max;

void get_pattern(char * pathname,int test_num,FormatHandle descriptionFormat,SheetHandle *sheets);

struct clf_info * classify_info( char buf[100]);





#endif
