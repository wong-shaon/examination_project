#include "read.h"

int chanel_max=0;
int rank_max=0;

struct clf_info             //���ڱ����log�ļ��ռ�������ͼ��Ϣ 
{
	int test_num;               //���ڱ��浱ǰ�����ǵڼ���
	int chanel;                 //ͨ���ı�ţ� һ��Ϊ0��1 
	int rank;                   //rank�ı�ţ�һ��Ϊ0��1
	int dq_num;	                //DQ�ı�ţ� һ��Ϊ0-15 
	char mode;                  //mode��ֵ��һ��ΪR��T 
	char eye_pattern[90][100];  //���ڱ����ģʽ�����ŵ�ͨ����Ϣ 
};


/*
classify_info �ú������ڽ�������Ӧ��ͼ�������Ϣ����chanel,rank,dq,rx/tx
@buf[100] �ò���Ϊ������ͼ�й���Ϣ��һ���ַ���
return ����һ���ṹ��ָ�룬��ָ���Ӧ�Ľṹ�屣���˽��������ĸ�����Ϣ��
*/
struct clf_info * classify_info( char buf[100])
{
	int DQ_NUM=20;
	struct clf_info *info=(struct clf_info *)malloc(sizeof(*info));
	if(buf[14]=='R')  //�ж���rx�� 
	{
		info->chanel = buf[32]-'0';
		info->rank = buf[39]-'0';
		if(chanel_max <= buf[32]-'0') //chanel �ĸ���
		{
			chanel_max = buf[32]-'0';
		}
		if(rank_max <= buf[39]-'0') // rank �ĸ���
		{
			rank_max = buf[39]-'0';
		}
		info->mode = 'R';
		if('0'<=buf[44] && buf[44]<='9'&& buf[45]==' ') //�ж�����һ��DQ���� 
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
	else if(buf[14]=='T')  //�ж���tx�� 
	{
		info->chanel = buf[35]-'0';
		info->rank = buf[42]-'0';
		
		if(chanel_max <= buf[35]-'0')  //chanel �ĸ���
		{
			chanel_max = buf[35]-'0';
		}
		if(rank_max <= buf[42]-'0')   //rank �ĸ���
		{
			rank_max = buf[42]-'0';
		}
		
		info->mode = 'T';
		if('0'<=buf[57] && buf[57]<='9') //�ж�����һ��DQ���� 
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
get_pattern �ú������ڻ�ȡһ���ļ��е�������ͼ����ͨ��������صĺ�������ͼ��Ϣ�����������浽���
@pathname �ò������ڸ���һ���ļ��ľ���·�� 
@test_num �ò�������ָ����ǰ�Ĳ��Դ������
@descriptionFormat �ò�������ָ��д��excel���ݵ���ʽ
@sheets �ò������ڸ���д���ҳ������һҳ��һ��dq��Ӧһҳ
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










