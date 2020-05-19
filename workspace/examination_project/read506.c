#include "read.h"

int chanel_max=0;
int rank_max=0;

struct clf_info             //用于保存从log文件收集到的眼图信息 
{
	int test_num;               //用于保存当前测量是第几次
	int chanel;                 //通道的编号， 一般为0和1 
	int rank;                   //rank的编号，一般为0和1
	int dq_num;	                //DQ的编号， 一般为0-15 
	char mode;                  //mode的值，一般为R和T 
	char eye_pattern[90][100];  //用于保存该模式下引脚的通道信息 
};


/*
classify_info 该函数用于解析出对应眼图的相关信息，如chanel,rank,dq,rx/tx
@buf[100] 该参数为包含眼图有关信息的一串字符串
return 返回一个结构体指针，该指针对应的结构体保存了解析出来的各种信息；
*/
struct clf_info * classify_info( char buf[100])
{
	int DQ_NUM=20;
	struct clf_info *info=(struct clf_info *)malloc(sizeof(*info));
	if(buf[14]=='R')  //判断是rx的 
	{
		info->chanel = buf[32]-'0';
		info->rank = buf[39]-'0';
		if(chanel_max <= buf[32]-'0') //chanel 的个数
		{
			chanel_max = buf[32]-'0';
		}
		if(rank_max <= buf[39]-'0') // rank 的个数
		{
			rank_max = buf[39]-'0';
		}
		info->mode = 'R';
		if('0'<=buf[44] && buf[44]<='9'&& buf[45]==' ') //判断是哪一个DQ引脚 
		{
			DQ_NUM = buf[44]-'0';
			info->dq_num = DQ_NUM;
			
		}
		else if('0'<=buf[44] && buf[44]<='9' && '0'<=buf[45] && buf[45]<='9')
		{
			DQ_NUM = (buf[44]-'0')*10+buf[45]-'0';
			info->dq_num = DQ_NUM;
		}
		else
		{
			printf("rx chanel1 rank0 DQ error\n");
		}
	}
	else if(buf[14]=='T')  //判断是tx的 
	{
		info->chanel = buf[35]-'0';
		info->rank = buf[42]-'0';
		
		if(chanel_max <= buf[35]-'0')  //chanel 的个数
		{
			chanel_max = buf[35]-'0';
		}
		if(rank_max <= buf[42]-'0')   //rank 的个数
		{
			rank_max = buf[42]-'0';
		}
		
		info->mode = 'T';
		if('0'<=buf[57] && buf[57]<='9') //判断是哪一个DQ引脚 
		{
			DQ_NUM = buf[57]-'0';
			info->dq_num = DQ_NUM;

		}
		else if('0'<=buf[58] && buf[58]<='9' && '0'<=buf[59] && buf[59]<='9')
		{
			DQ_NUM = (buf[58]-'0')*10+buf[59]-'0';
			info->dq_num = DQ_NUM;
		}
	}
	else
	{
		puts(buf);
		printf("It isn't the wanted infomation \n");
		return NULL;
	}
	return info;
}





/*
get_pattern 该函数用于获取一个文件中的所有眼图，并通过调用相关的函数将眼图信息解析出来保存到表格
@pathname 该参数用于给定一个文件的绝对路径 
@test_num 该参数用于指定当前的测试次数编号
@descriptionFormat 该参数用于指定写入excel数据的样式
@sheets 该参数用于给定写入的页数是那一页，一个dq对应一页
*/
void get_pattern(char * pathname,int test_num,FormatHandle descriptionFormat,SheetHandle *sheets)
{
	struct clf_info *info_ret=(struct clf_info *)malloc(sizeof(*info_ret));
	struct rx_info_extract *rx_info_ret =(struct rx_info_extract *) malloc(sizeof(*rx_info_ret));
	struct tx_info_extract *tx_info_ret =(struct tx_info_extract *) malloc(sizeof(*tx_info_ret));
	
    int fd,r,i=0;
    FILE *fp;
    char *ret;
    char buf[200]={0};
    fp=fopen(pathname,"r");
    if(fp == NULL)
    {
        perror("open log error");
        return ;
    }
    
    while(1)
	{

		ret = fgets(buf,200,fp);
		if(ret==NULL)
		{
			printf("----------fgets error---------\n"); 
			break;
		}
	    if(buf[0]=='\n')    //1
	    {
	    	ret = fgets(buf,200,fp);  //2
			if(ret==NULL)
			{
				printf("----------fgets error---------\n"); 
				break;
			}
			if(buf[0]=='\n')
	    	{
	    		ret = fgets(buf,200,fp);  //3
				if(ret==NULL)
				{
					printf("----------fgets error---------\n"); 
					break;
				}
	    		if(buf[0]=='\n')
	    		{
	    			ret = fgets(buf,200,fp); //4
					if(ret==NULL)
					{
						printf("----------fgets error---------\n"); 
						break;
					}
	    			if(buf[0]=='\n')
	    			{
	    				ret = fgets(buf,200,fp); //5
						if(ret==NULL)
						{
							printf("----------fgets error---------\n"); 
							break;
						}
	    				info_ret=classify_info(buf);
	    				if(info_ret != NULL)
	    				{
	    					//printf("mode = %c dq_num = %d chanel = %d rank = %d \n",info_ret->mode,
							//info_ret->dq_num,info_ret->chanel,info_ret->rank);
							
							ret = fgets(buf,200,fp);
							if(ret==NULL)
							{
								printf("----------fgets error---------\n"); 
								break;
							}
							if(info_ret->mode=='T')
							{
								for(i=0;i<81;i++)
								{
									ret = fgets(buf,200,fp);
									if(buf[0]=='\n')
									{
										printf("%cx chanel%d rank%d DQ%d is not completion\n",info_ret->mode,
										info_ret->chanel,info_ret->rank,info_ret->dq_num);
										puts(pathname);
										return;
									}
									if(ret==NULL)
									{
										printf("----------fgets error---------\n"); 
										break;
									}
									strcpy(info_ret->eye_pattern[i],buf);

								} 
								//add_a_node(list,test_num,info_ret->chanel,info_ret->rank,info_ret->mode,
								//info_ret->eye_pattern,info_ret->dq_num);
								analyse_info_tx_window( info_ret->eye_pattern,tx_info_ret);
								analyse_info_tx_top(info_ret->eye_pattern,tx_info_ret); 
								write_data_tx(info_ret->chanel,info_ret->rank,test_num,tx_info_ret,
								sheets[info_ret->dq_num],descriptionFormat); 
							}	
							else if(info_ret->mode=='R')
							{
								for(i=0;i<32;i++)
								{
									ret = fgets(buf,200,fp);
									if(buf[0]=='\n')
									{
										printf("%cx chanel%d rank%d DQ%d is not completion\n",info_ret->mode,
										info_ret->chanel,info_ret->rank,info_ret->dq_num);
										puts(pathname);
										return;
									}
									if(ret==NULL)
									{
										printf("----------fgets error---------\n"); 
										break;
									}
									strcpy(info_ret->eye_pattern[i],buf);
								} 
								//add_a_node(list,test_num,info_ret->chanel,info_ret->rank,info_ret->mode,
								//info_ret->eye_pattern,info_ret->dq_num);
								analyse_info_rx_top(info_ret->eye_pattern,rx_info_ret);
								analyse_info_rx_window(info_ret->eye_pattern,rx_info_ret);
								analyse_info_rx_bottom(info_ret->eye_pattern,rx_info_ret);
								write_data_rx(info_ret->chanel,info_ret->rank,test_num,rx_info_ret,
								sheets[info_ret->dq_num],descriptionFormat);
								
							}
								
						}
		    			
					}
				}
				else
				{
					info_ret=classify_info(buf);
					if(info_ret != NULL)
					{
						//printf("mode = %c dq_num = %d chanel = %d rank = %d \n",info_ret->mode,
						//info_ret->dq_num,info_ret->chanel,info_ret->rank);
						ret = fgets(buf,200,fp);
						if(ret==NULL)
						{
							printf("----------fgets error---------\n"); 
							break;
						}
						if(info_ret->mode=='T')
						{
							for(i=0;i<81;i++)
							{
								ret = fgets(buf,200,fp);
								if(buf[0]=='\n')
								{
									printf("%cx chanel%d rank%d DQ%d is not completion\n",info_ret->mode,
									info_ret->chanel,info_ret->rank,info_ret->dq_num);
									puts(pathname);
									return;
								}
								if(ret==NULL)
								{
									printf("----------fgets error---------\n"); 
									break;
								}
								strcpy(info_ret->eye_pattern[i],buf);
								
							} 
							//add_a_node(list,test_num,info_ret->chanel,info_ret->rank,info_ret->mode,
							//	info_ret->eye_pattern,info_ret->dq_num);
							analyse_info_tx_window( info_ret->eye_pattern,tx_info_ret);
							analyse_info_tx_top(info_ret->eye_pattern,tx_info_ret); 
							write_data_tx(info_ret->chanel,info_ret->rank,test_num,tx_info_ret,
							sheets[info_ret->dq_num],descriptionFormat); 
						}	
						else if(info_ret->mode=='R')
						{
							for(i=0;i<32;i++)
							{
								ret = fgets(buf,200,fp);
								if(buf[0]=='\n')
								{
									printf("%cx chanel%d rank%d DQ%d is not completion\n",info_ret->mode,
									info_ret->chanel,info_ret->rank,info_ret->dq_num);
									puts(pathname);
									return;
								}
								if(ret==NULL)
								{
									printf("----------fgets error---------\n"); 
									break;
								}
								strcpy(info_ret->eye_pattern[i],buf);
							}

						//	add_a_node(list,test_num,info_ret->chanel,info_ret->rank,info_ret->mode,
							//	info_ret->eye_pattern,info_ret->dq_num);
								
								analyse_info_rx_top(info_ret->eye_pattern,rx_info_ret);
								analyse_info_rx_window(info_ret->eye_pattern,rx_info_ret);
								analyse_info_rx_bottom(info_ret->eye_pattern,rx_info_ret);
								write_data_rx(info_ret->chanel,info_ret->rank,test_num,rx_info_ret,
								sheets[info_ret->dq_num],descriptionFormat);
						}
					}
				}	    		
			}
		}

	}	
	free(info_ret);
	free(rx_info_ret);
	free(tx_info_ret);
}










