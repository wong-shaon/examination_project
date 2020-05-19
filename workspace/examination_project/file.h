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


extern int board_max;	//��⵽�Ŀ�����������
extern int test_max;	//ÿ�鿪������ԵĴ���
extern int file_num;     //�����ļ�������Ŀ 

extern char path_name[500][160];//���ڱ���ÿ��log�ļ���·������

static int search_log(char *to_search );
static int get_files(const char *file_addr); //��ȡ�ļ���������log�ļ�
int search_files();     //����ֱ�Ӵӵ�ǰ·����Ѱ��log�ļ�
int get_board_num(const char *buf,struct file_info *ret);  //���ڻ�ȡ��·������Ӧ�Ĳ��԰�źͲ��Ժ�

#endif
