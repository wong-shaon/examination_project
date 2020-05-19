#include "get_excel_data.h"
#include "file.h"
#include "excel.h"
#include "read.h"

//用于破解的键值对
static char name[10][7]={"READ1","READ2","READ3","READ4","READ5","READ6","READ7","READ8","READ9","READ10"};
static char key[10][60]={"windows-25212d0a04c6e60761bc6c61a3kblfh4","windows-2d2d2c0009cbee0d65b7616ca2u3uaib",
					"windows-2525260500c4e30c62bb6f6da8v4v9ia","windows-292728050bc8ef0a63b26f64a6l8mfh2",
					"windows-2829250005c6ed0061ba6f66a4f7g2hd","windows-2b272b0207c5ea076fb46f68afuavbha",
					"windows-272c290902c5e10862bc6e6faetat5id","windows-2e26280e04cde60e61b56c6cabq1rehc",
					"windows-2722230107c6ed046eb46662aco7p3hb","windows-222a230207cee9086cb66a6da3a9c6g6",};


//保存读取出来的表格的信息
struct excel_info
{
	int chanel;
	int rank;
	float rx_top[12];   
	float rx_bottom[12];
	float rx_window[22];
	float tx_top[12];
	float tx_window[22];
};

//保存标准的相关参数
struct standard
{
	int chanel;
	int rank;
	float rx_top[10];   
	float rx_bottom[10];
	float rx_window[20];
	float tx_top[10];
	float tx_window[20];	
};

struct standard std[4];
//int test_max=5;
//int chanel_max = 1;
//int rank_max = 0;

int dq[16] ;
int dq_now ;
int flag_pass;//0为通过,1为fail


/*用于获取标准参考表中的各个参数的参考指标*/
void get_standard()  
{
	
	int i,j;
	BookHandle book_standard = xlCreateXMLBook();
	xlBookSetKey(book_standard,"book_standard","windows-2b2d2d0b0bcbef0569bb6764a4n4odhd");
	if(book_standard) 
    {    
        if(xlBookLoad(book_standard, "standard.xlsx")) 
        {
			SheetHandle sheet = xlBookGetSheet(book_standard, 0);
			if(sheet)
			{
				for(i=0;i<4;i++)
				{
					for(j=0;j<10;j++)
					{
						std[i].rx_top[j] = xlSheetReadNum(sheet,2+i,1+j, 0);
						std[i].rx_bottom[j] = xlSheetReadNum(sheet,10+i,1+j, 0);
						std[i].tx_top[j] = xlSheetReadNum(sheet,18+i,1+j, 0);
					}
	
				}
				for(i=0;i<4;i++)
				{
					for(j=0;j<20;j++)
					{
						std[i].rx_window[j] = xlSheetReadNum(sheet,26+i,1+j, 0);
						std[i].tx_window[j] = xlSheetReadNum(sheet,34+i,1+j, 0);
					}
				}
				
			}
			if(xlBookSave(book_standard, "standard.xlsx"))
			{
				printf("\nFile standard.xlsx has been read.\n");
			}
        } 
		xlBookRelease(book_standard);

		
	}
}

/*
get_data_all 用于从表格中的一页中读取所有的相关信息，并根据这些信息计算出波动率
@ sheet 要读物的表格的一页的页句柄
@ info 用于保存从表格中获取到的信息的结构体指针
@ rank 当前获取的数据的rank是哪一个，一般取0，1
@ chanel 当前获取的数据的chanel是哪一个，一般取0，1
@ a 读取数据的基准行
*/

void get_data_all(SheetHandle sheet,struct excel_info * info,int rank,int chanel,int a)
{
	
	int i,j;
	info->rank = rank;
	info->chanel = chanel;
	info->rx_top[10]=100;
	info->rx_bottom[10]=100;
	info->tx_top[10]=100;
	info->tx_window[20]=100;
	info->rx_window[20]=100;
	
	info->rx_top[11]=0;
	info->rx_bottom[11]=0;
	info->tx_top[11]=0;
	info->rx_window[21]=0;
	info->tx_window[21]=0;
	
	float min=1000,max=0,vol=0,temp=0;
	if(sheet)
	{
		 for(i=0;i<10;i++)  //rx_top
		{
			min=1000;
			max=0;
			for(j=1;j<=test_max;j++)
			{
				temp = xlSheetReadNum(sheet, a+2+j, 2+i, 0);
				if(temp > max)
				{
					max = temp;
				}
				if(temp < min)
				{
					min = temp;
				}
			}
			vol = (max-min)/407.97;
			info->rx_top[i] = vol;
			if(vol > info->rx_top[11])
			{
				info->rx_top[11] = vol;
			}
			if(vol < info->rx_top[10])
			{
				info->rx_top[10] = vol;
			}
		}
		for(i=0;i<10;i++)  //rx_bottom
		{
			min=1000;
			max=0;
			for(j=1;j<=test_max;j++)
			{
				temp = xlSheetReadNum(sheet,a+base_row+2+j, 2+i, 0);
				if(temp > max)
				{
					max = temp;
				}
				if(temp < min)
				{
					min = temp;
				}
			}
			vol = (max-min)/407.97;
			info->rx_bottom[i] = vol;
			if(vol > info->rx_bottom[11])
			{
				info->rx_bottom[11] = vol;
			}
			if(vol < info->rx_bottom[10])
			{
				info->rx_bottom[10] = vol;
			}
		}
		for(i=0;i<10;i++)  //tx_top
		{
			min=1000;
			max=0;
			for(j=1;j<=test_max;j++)
			{
				temp = xlSheetReadNum(sheet, a+base_row*2+2+j, 2+i, 0);
				if(temp > max)
				{
					max = temp;
				}
				if(temp < min)
				{
					min = temp;
				}
			}

			vol = (max-min)/32;
			info->tx_top[i] = vol;
			if(vol > info->tx_top[11])
			{
				info->tx_top[11] = vol;
			}
			if(vol < info->tx_top[10])
			{
				info->tx_top[10] = vol;
			}
		} 
		for(i=0;i<20;i++)  //rx_window
		{
			min=1000;
			max=0;
			for(j=1;j<=test_max;j++)
			{
				temp = xlSheetReadNum(sheet, a+base_row*3+2+j, 2+i, 0);
				//printf("--------------V[%d][%d]----------------\n",a+62+j,2+i);
				//printf("--------------temp=%lf----------------\n",temp);
				if(temp > max)
				{
					max = temp;
				}
				if(temp < min)
				{
					min = temp;
				}
			}
			if(i==18)
			{
				vol = (max-min)/407.97;
			}
			else
			{
				vol = (max-min)/64;
			}
		//	printf("------------152-min=%lf------------\n",min);
		//	printf("------------153-max=%lf------------\n",max);
			info->rx_window[i] = vol;
			if(vol > info->rx_window[21])
			{
				info->rx_window[21] = vol;
			}
			if(vol < info->rx_window[20])
			{
				info->rx_window[20] = vol;
			}
		}
		for(i=0;i<20;i++)  //tx_window
		{
			min=1000;
			max=0;
			for(j=1;j<=test_max;j++)
			{
				temp = xlSheetReadNum(sheet, a+base_row*4+2+j, 2+i, 0);
				if(temp > max)
				{
					max = temp;
				}
				if(temp < min)
				{
					min = temp;
				}
			}
			if(i==18)
			{
				vol = (max-min)/32;
			}
			else
			{
				vol = (max-min)/64;
			}
			info->tx_window[i] = vol;
			if(vol > info->tx_window[21])
			{
				info->tx_window[21] = vol;
			}
			if(vol < info->tx_window[20])
			{
				info->tx_window[20] = vol;
			}
		}
	}

}

/*
write_data_sheet 该函数用于将读取到的波动率写到汇总表的适当位置，并根据各个参数标准来判断测试版是否达标
@sheet 要写入的汇总表的句柄页
@ format 写入数据正常的一种格式
@ info 需要被写入的数据结构体的指针
@ base_row_s 写入的基准行
@ add_row 写入的增加行
@ format_judge 写入的数据异常的格式
@ chanel 当前获取的数据的chanel是哪一个，一般取0，1
@ rank 当前获取的数据的rank是哪一个，一般取0，1
*/

void write_data_sheet(SheetHandle sheet,FormatHandle format,struct excel_info * info,int base_row_s,
					int add_row,FormatHandle format_judge,int chanel,int rank)
{
	int i;
	int index = chanel+rank*2;
	for(i=0;i<12;i++)
	{
		if(info->rx_top[i]>std[index].rx_top[i] && i<10)
		{
			xlSheetWriteNum(sheet,7+add_row, 2+i,info->rx_top[i], format_judge);	//rx_top
			flag_pass = 1;
			dq[dq_now] = 1;
		}
		else
		{
			xlSheetWriteNum(sheet,7+add_row, 2+i,info->rx_top[i], format);	//rx_top
		}
		
		if(info->rx_bottom[i]>std[index].rx_bottom[i] && i<10)
		{
			xlSheetWriteNum(sheet,7+base_row_s*1+add_row, 2+i,info->rx_bottom[i], format_judge);  //rx_bottom
			flag_pass = 1;
			dq[dq_now] = 1;
		}
		else
		{
			xlSheetWriteNum(sheet,7+base_row_s*1+add_row, 2+i,info->rx_bottom[i], format);  //rx_bottom
		}
		
		if(info->tx_top[i]>std[index].tx_top[i] && i<10)
		{
			xlSheetWriteNum(sheet,7+base_row_s*2+add_row, 2+i,info->tx_top[i], format_judge);	//tx_top
			flag_pass = 1;
			dq[dq_now] = 1;
		}
		else
		{
			xlSheetWriteNum(sheet,7+base_row_s*2+add_row, 2+i,info->tx_top[i], format);	//tx_top
		}
		
	}
	
	for(i=0;i<22;i++)
	{
		if(info->rx_window[i]>std[index].rx_window[i] && i<20)
		{
			xlSheetWriteNum(sheet, 7+base_row_s*3+add_row, 2+i,info->rx_window[i], format_judge);	//rx_window
			flag_pass = 1;
			dq[dq_now] = 1;
		}
		else
		{
			xlSheetWriteNum(sheet, 7+base_row_s*3+add_row, 2+i,info->rx_window[i], format);	//rx_window
		}
		
		if(info->tx_window[i]>std[index].tx_window[i] && i<20)
		{
			xlSheetWriteNum(sheet, 7+base_row_s*4+add_row, 2+i,info->tx_window[i], format_judge);	//tx_window
			flag_pass = 1;
			dq[dq_now] = 1;
		}
		else
		{
			xlSheetWriteNum(sheet, 7+base_row_s*4+add_row, 2+i,info->tx_window[i], format);	//tx_window
		}
	}
	
}


/*
get_data_sheet 用于从指定表格的指定页中获取相关数据，并将获取到的数据写入到另一个指定的表格
@sheet 需要从中读取数据的页句柄
@info 用于保存从表格中获取到的信息的结构体指针
@sheet_summary 需要向其写入数据的页句柄
@ format 写入数据正常的一种格式
@ board_num 当前的测试板编号
@ format_judge 写入的数据异常的格式
*/
void get_data_sheet(SheetHandle sheet,struct excel_info * info,SheetHandle sheet_summary,FormatHandle format,
				int board_num,FormatHandle format_judge)
{
	int i,j,k,a,base_row_s=board_max*(chanel_max+rank_max)*2+4,add_row;
	
	for(i=0;i<=chanel_max;i++)
	{
		for(j=0;j<=rank_max;j++)
		{
			if(i==0 && j==0)//chanel0 rank0
			{
				a = 0*(test_max+3);
				get_data_all(sheet,info,j,i,a);
				
				add_row = board_num;
				write_data_sheet(sheet_summary,format,info,base_row_s,add_row,format_judge,i,j);
			}
			if(i==1 && j==0)//chanel1 rank0
			{
				a = 1*(test_max+3);
				get_data_all(sheet,info,j,i,a);
				
				add_row = board_num+board_max;
				write_data_sheet(sheet_summary,format,info,base_row_s,add_row,format_judge,i,j);
			}
			if(i==0 && j==1)//chanel0 rank1
			{
				a = 2*(test_max+3);
				get_data_all(sheet,info,j,i,a);
				
				add_row = board_num+board_max*2;
				write_data_sheet(sheet_summary,format,info,base_row_s,add_row,format_judge,i,j);
			}
			if(i==1 && j==1)//chanel1 rank1
			{
				a = 3*(test_max+3);
				get_data_all(sheet,info,j,i,a);
				
				add_row = board_num+board_max*3;
				write_data_sheet(sheet_summary,format,info,base_row_s,add_row,format_judge,i,j);
			}
		}

	}
		
}



void get_data_book(char  excel_name[][10])
{
	int board_num;
	struct excel_info * info = malloc(sizeof(*info));
	BookHandle book_summary = xlCreateXMLBook();
	xlBookSetKey(book_summary,"book_summary","windows-20242e0206cfee086bbb6e66a0o5p6h3");
	int i,f[3];

	if(book_summary) 
    {    

        if(xlBookLoad(book_summary, "summary.xlsx")) 
        {
			FormatHandle format[3];
			f[0] = xlBookAddCustomNumFormat(book_summary, "0.00%");
			f[1] = xlBookAddCustomNumFormat(book_summary, "0.0 [Black][=<0.0];0.0 [Red][>0.0]");
			f[2] = xlBookAddCustomNumFormat(book_summary, "0.00% [Black][=<0.00];0.00% [Red][>0.00]");

			for(i = 0; i < 3; ++i) 
			{
				format[i] = xlBookAddFormat(book_summary, 0);
				xlFormatSetNumFormat(format[i], f[i]);
				xlFormatSetAlignH(format[i], ALIGNH_CENTER);
				xlFormatSetBorderBottom(format[i], BORDERSTYLE_THIN);
			}
			
            SheetHandle sheet_summary[16],sheet_all;
			sheet_all = xlBookGetSheet(book_summary, 0);
			for(board_num=0;board_num<board_max;board_num++)
			{	
				flag_pass = 0;  //每一块板子都初始化为0
				
				for(i=0;i<16;i++)
				{
					dq[i] =0;
				}
				BookHandle book;
				book = xlCreateXMLBook();
				xlBookSetKey(book,name[0],key[0]);
				if(book) 
				{       
					if(xlBookLoad(book, excel_name[board_num])) 
					{
						SheetHandle sheets[16];
						int i=0;
						for(i=0;i<16;i++)
						{
							dq_now = i;
							sheet_summary[i] = xlBookGetSheet(book_summary, i+1);
							sheets[i]= xlBookGetSheet(book, i);
							if(sheet_summary[i]&&sheets[i])
							{	
								get_data_sheet(sheets[i],info,sheet_summary[i],format[0],board_num+1,format[2]);
							}
							
						}

					}
				}
				
				if(xlBookSave(book, excel_name[board_num])) 
					printf("\nFile has been modified.\n");
				xlBookRelease(book);
				if(flag_pass==0)
				{
					xlSheetWriteStr(sheet_all, 3+board_num, 2, "pass", 0);
				}
				else
				{
					xlSheetWriteStr(sheet_all, 3+board_num, 2, "fail", 0);
				}
				for(i=0;i<16;i++)
				{
					xlSheetWriteNum(sheet_all, 3+board_num, 3+i, dq[i], format[1]);
				}
			}
        } 
		
		if(xlBookSave(book_summary, "summary.xlsx")) 
				printf("\nFile has been modified.\n");
	}
	xlBookRelease(book_summary);


}

//char excel_name[3][10]={"#1.xlsx","#2.xlsx","#3.xlsx"};

/* int main()
{
	
	get_standard();
	get_data_book(excel_name);
	
	
	printf("\nPress any key to exit...");
    _getch();

} */










