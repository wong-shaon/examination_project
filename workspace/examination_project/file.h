#ifndef __FILE_H__
#define __FILE_H__


#include<stdio.h>
#include<direct.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>


struct file_info
{
	int board_num;
	int test_time;
};


extern int board_max;	//检测到的开发板总数量
extern int test_max;	//每块开发板测试的次数
extern int file_num;     //保存文件的总数目 

extern char path_name[500][160];//用于保存每个log文件的路径名；

static int search_log(char *to_search );
static int get_files(const char *file_addr); //获取文件夹中所有log文件
int search_files();     //用于直接从当前路径下寻找log文件
int get_board_num(const char *buf,struct file_info *ret);  //用于获取该路径所对应的测试板号和测试号

#endif
