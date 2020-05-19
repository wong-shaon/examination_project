#include "file.h"

static int board_num_max=100;        //自定义的最大测量测试板数
static int test_time_max=5;			//自定义的最大测试次数
int test_max=0;     //保存检测到的测试次数
int board_max=0;    				 //保存检测到的总的开发板数
int file_num=0;    					 //保存文件的总数目 

char path_name[500][160]={0};        //用于保存每个log文件的路径名



/*
	search_log用于在一个指定的目录下寻找所有的.log文件，并解析log文件名是否超出限度；
	@to_search:该参数为寻找.log文件的指定路径，不超过80字节，中文路径可能存在异常；
	return 寻找成功返回0，失败返回-1；
	
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
get_files用于在给定路径下寻找符合命名规则的指定文件夹，并进入该文件夹调用函数寻找.log文件的指定路径；
@file_addr:该参数用于传递一个指定的路径，该路径应该尽量短，不含中文；
return : 寻找子文件夹成功返回0，失败返回-1；
*/

int get_files(const char *file_addr)
{
	
    struct _finddata_t file; //定义结构体变量
    long handle;
    char path[100]={0},cur_path[100]={0},cfile_addr[100]={0};
    sprintf(cfile_addr,"%s/*.*",file_addr);
   // puts(cur_path);
    handle = _findfirst(cfile_addr, &file);//查找所有文件

    if (handle == -1)//如果handle为－1, 表示当前目录为空, 则结束查找而返回
    {

	   printf("handle== -1");
	    return -1;
	}
    else
    {
		
        if (file.attrib &_A_SUBDIR) //是目录
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
            if (file.attrib &_A_SUBDIR) //是目录
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
search_files:直接获取当前的工作路径，并使用该路径去寻找当前路径下子文件夹中的log文件
return : 寻找成功返回0，失败返回-1；
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
get_board_num 用于解析一个log文件的绝对路径，获取其中的测试板编号和测试次数编号；
@buf 该参数为指定的绝对路径，@ret 结构体指针用于保存解析到的值的地址；
return 寻找到测试板的编号成功返回0，失败返回-1；
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




