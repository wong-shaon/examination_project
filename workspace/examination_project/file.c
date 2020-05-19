#include "file.h"

static int board_num_max=100;        //�Զ�������������԰���
static int test_time_max=5;			//�Զ���������Դ���
int test_max=0;     //�����⵽�Ĳ��Դ���
int board_max=0;    				 //�����⵽���ܵĿ�������
int file_num=0;    					 //�����ļ�������Ŀ 

char path_name[500][160]={0};        //���ڱ���ÿ��log�ļ���·����



/*
	search_log������һ��ָ����Ŀ¼��Ѱ�����е�.log�ļ���������log�ļ����Ƿ񳬳��޶ȣ�
	@to_search:�ò���ΪѰ��.log�ļ���ָ��·����������80�ֽڣ�����·�����ܴ����쳣��
	return Ѱ�ҳɹ�����0��ʧ�ܷ���-1��
	
*/
static int search_log(char *to_search )
{
	char path[80]={0};
	strcpy(path,to_search);
	strcat(to_search, "/*.log");
    long handle;
    struct _finddata_t fileinfo;
    handle=_findfirst(to_search,&fileinfo);
    if(-1==handle)
    {
    	return -1;
	}
	else
	{	
		if(fileinfo.name[strlen(fileinfo.name)-5]-'0' <= test_time_max)
		{
			sprintf(path_name[file_num],"%s/%s",path,fileinfo.name);
			file_num++;
			if(fileinfo.name[strlen(fileinfo.name)-5]-'0' >= test_max)
			{
				test_max = fileinfo.name[strlen(fileinfo.name)-5]-'0';
			}
		}
	    while(!( _findnext(handle,&fileinfo)))
	    {
	    	if(fileinfo.name[strlen(fileinfo.name)-5]-'0' <= test_time_max)
			{
				sprintf(path_name[file_num],"%s/%s",path,fileinfo.name);
			//	puts(path_name[file_num]);
				file_num++;
				if(fileinfo.name[strlen(fileinfo.name)-5]-'0' >= test_max)
				{
					test_max = fileinfo.name[strlen(fileinfo.name)-5]-'0';
				}
			 }

	    }
	}

    _findclose(handle);
    return 0;
}


/*
get_files�����ڸ���·����Ѱ�ҷ������������ָ���ļ��У���������ļ��е��ú���Ѱ��.log�ļ���ָ��·����
@file_addr:�ò������ڴ���һ��ָ����·������·��Ӧ�þ����̣��������ģ�
return : Ѱ�����ļ��гɹ�����0��ʧ�ܷ���-1��
*/

int get_files(const char *file_addr)
{
	
    struct _finddata_t file; //����ṹ�����
    long handle;
    char path[100]={0},cur_path[100]={0},cfile_addr[100]={0};
    sprintf(cfile_addr,"%s/*.*",file_addr);
   // puts(cur_path);
    handle = _findfirst(cfile_addr, &file);//���������ļ�

    if (handle == -1)//���handleΪ��1, ��ʾ��ǰĿ¼Ϊ��, ��������Ҷ�����
    {

	   printf("handle== -1");
	    return -1;
	}
    else
    {
		
        if (file.attrib &_A_SUBDIR) //��Ŀ¼
        {
			
            if (file.name[0] != '.'&&file.name[0] == '#')
            {	
            	if(file.name[1]-'0' <= board_num_max)
				{
					board_max++;
					//printf("board_max=%d",board_max);
				//	puts(file.name);
	               	sprintf(path,"%s/%s",file_addr,file.name);
	                if(_chdir(path) == -1)
	                {
	                	printf("80 change error");
					}
	
					search_log(path);
				 }
            

            }
        }
        while (!(_findnext(handle, &file)))
        {
            if (file.attrib &_A_SUBDIR) //��Ŀ¼
            {
                if (file.name[0] == '#'&&file.name[1] >= '0'&&file.name[1] <= '9'&&
				file.name[2] >= '0'&&file.name[1] <= '9'&&file.name[3] >= '0'&&file.name[3] <= '9')
                {
			        if((file.name[1]-'0')*100+ (file.name[2]-'0')*10+(file.name[3]-'0')*1<= board_num_max)
					{
						board_max++;
						//    puts(file.name);
		               	sprintf(path,"%s/%s",file_addr,file.name);
		                if(_chdir(path) == -1)
		                {
		                	printf("80 change error");
						}
		
						search_log(path);
					}
					else 
					{
						printf("Exceeding the maximum number of tests\n");
					}	
					
                }
            }
        }
        
    }
	_findclose(handle);
    return 0;
}

/*
search_files:ֱ�ӻ�ȡ��ǰ�Ĺ���·������ʹ�ø�·��ȥѰ�ҵ�ǰ·�������ļ����е�log�ļ�
return : Ѱ�ҳɹ�����0��ʧ�ܷ���-1��
*/
int search_files()
{	
	int i;
	char buf[100]={0};
	getcwd( buf,100);
	for(i=0;i<100;i++)
	{
		
		if(buf[i]=='\\')
		{
			buf[i]='/';
		}
	}
	if(get_files(buf)==-1)
	{
		printf("129 search files error");
		return -1;
	}

	return 0;
}


/*
get_board_num ���ڽ���һ��log�ļ��ľ���·������ȡ���еĲ��԰��źͲ��Դ�����ţ�
@buf �ò���Ϊָ���ľ���·����@ret �ṹ��ָ�����ڱ����������ֵ�ĵ�ַ��
return Ѱ�ҵ����԰�ı�ųɹ�����0��ʧ�ܷ���-1��
*/

int get_board_num(const char *buf,struct file_info *ret)
{
	int i,j;
	ret->test_time = buf[strlen(buf)-5]-'0';
	for(i=0;i<100;i++)
	{
		for(j=1;j<=100;j++)
		{

			if(buf[i-1]=='/'&&buf[i]=='#'&&buf[i+1]==(j+'0')&&buf[i+2]!='-'&&('9'<buf[i+2]||'0'>buf[i+2]))
			{
				ret->board_num = j;	
			
				return 0;
			}

			
			if(buf[i-1]=='/'&&buf[i]=='#'&&buf[i+1]==((j/10)+'0')&&buf[i+2]==((j%10)+'0')&&('9'<buf[i+3]||'0'>buf[i+3]))
			{
				ret->board_num = j;	
				return 0;
			}
			if(buf[i-1]=='/'&&buf[i]=='#'&&buf[i+1]==((j/100)+'0')&&buf[i+2]==(((j/10)%10)+'0')&&buf[i+3]==((j%10)+'0'))
			{
				ret->board_num = j;	
				return 0;
			}

		}
	
	}

	return -1;
}





/* int main()
{
	int i;
	struct file_info *ret = malloc(sizeof(*ret));
	search_files();
	for(i=0;i<file_num;i++)
	{
		get_board_num(path_name[i],ret);
	}
	printf("\nPress any key to exit...");
    getchar();
} */




