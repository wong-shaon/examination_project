#include "excel.h"
#include "file.h"

//static int test_max = 5; //test_max
static int chanel = 1;
static int rank = 0;
int base_row=0;

/*
draw_test 用于完成每一行的测试次数等相关参数的制表
@ sheet 需要写入的页
@ descriptionFormat 写入的样式
@ row 需要写入的行
*/
void draw_test(SheetHandle Sheet,FormatHandle DescriptionFormat,int row)
{

	char test[3]={0};
	volatile int i = 0;
	while(i<test_max)
	{
		i=i+1;
		sprintf(test,"#%d",i);
		xlSheetWriteStr(Sheet, row-1+i, 1, test, DescriptionFormat);
		
	}
	xlSheetWriteStr(Sheet, row+i, 1, "Min", DescriptionFormat);
	xlSheetWriteStr(Sheet, row+i+1, 1, "Max", DescriptionFormat);
	xlSheetWriteStr(Sheet, row+i+2, 1, "volatility", DescriptionFormat);

}  


/*
draw_column 用于完成所有的列制表信息的写入
@ sheet 需要写入的页
@ descriptionFormat 写入的样式
@ base_row 需要写入的基准行
*/ 

static void draw_column(SheetHandle sheet,FormatHandle descriptionFormat,int base_row)
{

	if(rank>=0&&chanel>=0)
	{

		xlSheetSetMerge(sheet, base_row+3, base_row+5+test_max, 0,0);
		xlSheetWriteStr(sheet, base_row+3, 0, "CH0_RANK0", descriptionFormat);
		draw_test(sheet,descriptionFormat,base_row+3);

		
		if(chanel==1 && rank==0)
		{
			xlSheetSetMergeA(sheet, base_row+6+test_max, base_row+8+test_max*2, 0,0);
			xlSheetWriteStr(sheet, base_row+6+test_max, 0, "CH1_RANK0", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+6+test_max);

		}
		if(rank == 1 && chanel==0)
		{
			xlSheetSetMergeA(sheet, base_row+6+test_max, base_row+8+test_max*2, 0,0);
			xlSheetWriteStr(sheet, base_row+6+test_max, 0, "CH0_RANK1", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+6+test_max);
			
		}
		if(rank==1 && chanel==1)
		{
			xlSheetSetMergeA(sheet, base_row+6+test_max, base_row+8+test_max*2, 0,0);
			xlSheetWriteStr(sheet, base_row+6+test_max, 0, "CH1_RANK0", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+6+test_max);
	
			
			xlSheetSetMergeA(sheet, base_row+9+test_max*2, base_row+11+test_max*3, 0,0);
			xlSheetWriteStr(sheet, base_row+9+test_max*2, 0, "CH0_RANK1", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+9+test_max*2);

			
			xlSheetSetMergeA(sheet,base_row+ 12+test_max*3, base_row+14+test_max*4, 0,0);
			xlSheetWriteStr(sheet, base_row+12+test_max*3, 0, "CH1_RANK1", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+ 12+test_max*3);
		}
	}
}

/*
draw_top_bottom_excel 用于完成所有的top以及bottom表格的制作
@ sheet 需要写入的页
@ descriptionFormat 写入的样式
@ base_row 需要写入的基准行
@ headerFormat 写入的表格名称的样式
@ title 标题名
@ buf[][14] 需要写入的相关参数的名字
*/ 		

static void draw_top_bottom_excel(SheetHandle sheet,FormatHandle descriptionFormat,
					int base_row,FormatHandle headerFormat,const char *title,char buf[][14])
{
	xlSheetSetMergeA(sheet, base_row+1, base_row+1, 0,11);
	xlSheetWriteStr(sheet, base_row+1, 0, title, headerFormat);
	int i=0;
	for(i=1;i<=11;i++)
	{
		xlSheetWriteStr(sheet, base_row+2, i, buf[i-1], descriptionFormat);
	}

	draw_column(sheet,descriptionFormat,base_row);
}

/*
draw_window_excel 用于完成所有的window表格的制作
@ sheet 需要写入的页
@ descriptionFormat 写入的样式
@ base_row 需要写入的基准行
@ headerFormat 写入的表格名称的样式
@ title 标题名
@ buf[][11] 需要写入的相关参数的名字
*/ 	

static void draw_window_excel(SheetHandle sheet,FormatHandle descriptionFormat,
					int base_row,FormatHandle headerFormat,const char *title,char buf[][11])
{
	xlSheetSetMergeA(sheet, base_row+1, base_row+1, 0,21);
	xlSheetWriteStr(sheet, base_row+1, 0, title, headerFormat);
	
	int i=0;
	for(i=1;i<=21;i++)
	{
		xlSheetWriteStr(sheet, base_row+2, i, buf[i-1], descriptionFormat);
	}
	draw_column(sheet,descriptionFormat,base_row);

}

/*
draw_excel 用于完成所有的表格的制作
@ descriptionFormat 写入的样式
@ headerFormat 写入的表格名称的样式
@ sheets 需要制作表格的所有的页
*/ 	

void draw_excel(FormatHandle headerFormat,FormatHandle descriptionFormat,SheetHandle *sheets)
{
	int i=0;

	base_row = (test_max+3)*2*(rank+chanel)+4;

	char buf_rx_top_bottom[11][14]={"test_time","O","Q","O+Q","H","H+O","H+Q","H+Q+O","QHQ","QQHQQ","QQQHQQQ"};
	
	char buf_tx_top[11][14]={"test_time","O","H","O+H","OHO","OOHOO","OOOHOOO","OOOOHOOOO",
					"OOOOOHOOOOO","OOOOOOHOOOOOO","QQQHQQQ"};
	
	char buf_rx_window[21][11]={"test_time","264.00m_LW","264.00m_RW","264.00m_TW","206.70m_LW","206.70m_RW",
				"206.70m_TW","172.90m_LW","172.90m_RW","172.90m_TW","160.60m_LW","160.60m_RW","160.60m_TW",
				"123.90m_LW","123.90m_RW","123.90m_TW","87.14m_LW","87.14m_RW","87.14m_TW",
				"V_site","H_site"};
	char buf_tx_window[21][11]={"test_time","28.0%_LW","28.0%_RW","28.0%_TW","24.8%_LW","24.8%_RW","24.8%_TW",
				"21.6%_LW","21.6%_RW","21.6%_TW","18.4%_LW","18.4%_RW","18.4%_TW","14.0%_LW","14.0%_RW",
				"14.0%_TW","10.0%_LW","10.0%_RW","10.0%_TW","V_site","H_site"};		
	

	for(i=0;i<16;i++)
	{
		if(sheets[i])
		{
			
			draw_top_bottom_excel(sheets[i],descriptionFormat,0*base_row,headerFormat,"RX_TOP",buf_rx_top_bottom);

			draw_top_bottom_excel(sheets[i],descriptionFormat,1*base_row,headerFormat,"RX_BOTTOM",buf_rx_top_bottom);
			
			draw_top_bottom_excel(sheets[i],descriptionFormat,2*base_row,headerFormat,"TX_TOP",buf_tx_top);
			
			draw_window_excel(sheets[i],descriptionFormat,3*base_row,headerFormat,"RX_WINDOW",buf_rx_window);
			
			draw_window_excel(sheets[i],descriptionFormat,4*base_row,headerFormat,"TX_WINDOW",buf_tx_window);		
			int j;
			for(j=0;j<25;j++)
			{
				xlSheetSetCol(sheets[i], j, j, 16, 0, 0);
			} 
	  
		}
	}
        

}



/*
write_data_rx 用于完成所有的tx表格的相关数据的填写
@ chanel 通道信息，取值为0，1
@ rank rank信息，取值为0，1
@ rx 保存了rx所有参数相关信息的数据
@ sheet 需要制作表格的页
@ descriptionFormat 写入的样式
*/ 	
void write_data_rx(int chanel,int rank,int test_num,struct rx_info_extract* rx,SheetHandle sheet,
					FormatHandle descriptionFormat)
{

	int i = 0;
	if(rank==0&&chanel==0)
	{
		for(i=0;i<10;i++)  //写rx_top和rx_bottom的相关参数  char rx_top[10][8]; char rx_bottom[10][8];
		{
			xlSheetWriteNum(sheet,2+test_num, 2+i, atof(rx->rx_top[i]), descriptionFormat);
			xlSheetWriteNum(sheet,base_row+2+test_num , 2+i, atof(rx->rx_bottom[i]), descriptionFormat);	
		}
		for(i=0;i<20;i++)  //写rx_window的相关参数rx_window[20][8];
		{
			xlSheetWriteNum(sheet, base_row*3+2+test_num, 2+i,atof(rx->rx_window[i]), descriptionFormat);
		}
		
	}
	else if(rank==0&&chanel==1)
	{
		for(i=0;i<10;i++)  //写rx_top和rx_bottom的相关参数  char rx_top[10][8]; char rx_bottom[10][8];
		{
			xlSheetWriteNum(sheet,2+test_num+(test_max+3)*1, 2+i, atof(rx->rx_top[i]), descriptionFormat);
			xlSheetWriteNum(sheet,base_row*1+2+test_num+(test_max+3)*1 , 2+i, atof(rx->rx_bottom[i]), descriptionFormat);	
		}
		for(i=0;i<20;i++)  //写rx_window的相关参数rx_window[20][8];
		{
			xlSheetWriteNum(sheet, base_row*3+2+test_num+(test_max+3)*1, 2+i, atof(rx->rx_window[i]), descriptionFormat);
		}
	}
	else if(rank==1&&chanel==0)
	{
		for(i=0;i<10;i++)  //写rx_top和rx_bottom的相关参数  char rx_top[10][8]; char rx_bottom[10][8];
		{
			xlSheetWriteNum(sheet,2+test_num+(test_max+3)*2, 2+i, atof(rx->rx_top[i]), descriptionFormat);
			xlSheetWriteNum(sheet,base_row*1+2+test_num+(test_max+3)*2 , 2+i, atof(rx->rx_bottom[i]), descriptionFormat);	
		}
		for(i=0;i<20;i++)  //写rx_window的相关参数rx_window[20][8];
		{
			xlSheetWriteNum(sheet, base_row*3+2+test_num+(test_max+3)*2, 2+i, atof(rx->rx_window[i]), descriptionFormat);
		}
	}
	else if(rank==1&&chanel==1)
	{
		for(i=0;i<10;i++)  //写rx_top和rx_bottom的相关参数  char rx_top[10][8]; char rx_bottom[10][8];
		{
			xlSheetWriteNum(sheet,2+test_num+(test_max+3)*3, 2+i, atof(rx->rx_top[i]), descriptionFormat);
			xlSheetWriteNum(sheet,base_row*1+2+test_num+(test_max+3)*3 , 2+i, atof(rx->rx_bottom[i]), descriptionFormat);	
		}
		for(i=0;i<20;i++)  //写rx_window的相关参数rx_window[20][8];
		{
			xlSheetWriteNum(sheet, base_row*3+2+test_num+(test_max+3)*3, 2+i, atof(rx->rx_window[i]), descriptionFormat);
		}
	}
}


/*
write_data_tx 用于完成所有的tx表格的相关数据的填写
@ chanel 通道信息，取值为0，1
@ rank rank信息，取值为0，1
@ tx 保存了rx所有参数相关信息的数据
@ sheet 需要制作表格的页
@ descriptionFormat 写入的样式
*/ 

void write_data_tx(int chanel,int rank,int test_num,struct tx_info_extract* tx,SheetHandle sheet,
					FormatHandle descriptionFormat)
{

	int i = 0;
	if(rank==0&&chanel==0)
	{
		
		for(i=0;i<10;i++)  //写tx_top的相关参数  char tx_top[10][8];
		{
			xlSheetWriteNum(sheet,base_row*2+2+test_num, 2+i, atof(tx->tx_top[i]), descriptionFormat);
			
				
		}
		for(i=0;i<20;i++)  //写tx_window的相关参数rx_window[20][8];
		{
			xlSheetWriteNum(sheet, base_row*4+2+test_num, 2+i, atof(tx->tx_window[i]), descriptionFormat);
		}
		
	}	
	if(rank==0&&chanel==1)
	{
		for(i=0;i<10;i++)  //写tx_top的相关参数  char tx_top[10][8];
		{
			xlSheetWriteNum(sheet,base_row*2+2+test_num+(test_max+3)*1, 2+i, atof(tx->tx_top[i]), descriptionFormat);

		}	
		for(i=0;i<20;i++)  //写tx_window的相关参数rx_window[20][8];
		{
			xlSheetWriteNum(sheet, base_row*4+2+test_num+(test_max+3)*1, 2+i, atof(tx->tx_window[i]), descriptionFormat);
		}
		
	}
	if(rank==1&&chanel==0)
	{
		for(i=0;i<10;i++)  //写tx_top的相关参数  char tx_top[10][8];
		{
			xlSheetWriteNum(sheet,base_row*2+2+test_num+(test_max+3)*2, 2+i, atof(tx->tx_top[i]), descriptionFormat);	
		}
		for(i=0;i<20;i++)  //写tx_window的相关参数rx_window[20][8];
		{
			xlSheetWriteNum(sheet, base_row*4+2+test_num+(test_max+3)*2, 2+i, atof(tx->tx_window[i]), descriptionFormat);
		}
		
	}
	if(rank==1&&chanel==1)
	{
		for(i=0;i<10;i++)  //写tx_top的相关参数  char tx_top[10][8];
		{
			xlSheetWriteNum(sheet,base_row*2+2+test_num+(test_max+3)*3, 2+i, atof(tx->tx_top[i]), descriptionFormat);	
		}
		for(i=0;i<20;i++)  //写tx_window的相关参数rx_window[20][8];
		{
			xlSheetWriteNum(sheet, base_row*4+2+test_num+(test_max+3)*3, 2+i, atof(tx->tx_window[i]), descriptionFormat);
		}
		
	}
	
}

/*
write_max_min 用于完成所有的表格中最小最大值以及波动率的计算和填写
@ chanel 通道信息，取值为0，1
@ rank rank信息，取值为0，1
@ sheet 需要制作表格的页
@ descriptionFormat 写入的样式
@ format 用百分比的样式表示波动率
*/ 

void write_max_min(int chanel,int rank,SheetHandle sheet,FormatHandle descriptionFormat,FormatHandle format)
{
	char buf[15]={0},buf1[40]={0};
	char s='A';
	int i;
	if(rank==0&&chanel==0)
	{
		
		for(i=0;i<10;i++)  
		{
			//tx——top
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*2+4,s+2+i,base_row*2+3+test_max);
			xlSheetWriteFormula(sheet, base_row*2+3+test_max, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*2+4,s+2+i,base_row*2+3+test_max);
			xlSheetWriteFormula(sheet, base_row*2+4+test_max, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/32.0",s+2+i,base_row*2+4,
			s+2+i,base_row*2+3+test_max,s+2+i,base_row*2+4,s+2+i,base_row*2+3+test_max);
			xlSheetWriteFormula(sheet, base_row*2+5+test_max, 2+i, buf1, format);
			//rx_top
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,4,s+2+i,3+test_max);
			xlSheetWriteFormula(sheet, 3+test_max, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,4,s+2+i,3+test_max);
			xlSheetWriteFormula(sheet, 4+test_max, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/407.97",s+2+i,4,s+2+i,3+test_max,s+2+i,4,s+2+i,3+test_max);
			xlSheetWriteFormula(sheet, 5+test_max, 2+i, buf1, format);
			//rx_bottom
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row+4,s+2+i,base_row+3+test_max);
			xlSheetWriteFormula(sheet, base_row+3+test_max, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row+4,s+2+i,base_row+3+test_max);
			xlSheetWriteFormula(sheet, base_row+4+test_max, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/407.97",s+2+i,base_row+4,
			s+2+i,base_row+3+test_max,s+2+i,base_row+4,s+2+i,base_row+3+test_max);
			xlSheetWriteFormula(sheet, base_row+5+test_max, 2+i, buf1, format);
			
		}
		for(i=0;i<20;i++)  
		{
			//tx_window
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*4+4,s+2+i,base_row*4+3+test_max);
			xlSheetWriteFormula(sheet, base_row*4+3+test_max, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*4+4,s+2+i,base_row*4+3+test_max);
			xlSheetWriteFormula(sheet, base_row*4+4+test_max, 2+i, buf, descriptionFormat);
			if(1==18)
			{
				sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/32",s+2+i,base_row*4+4,s+2+i,
				base_row*4+3+test_max,s+2+i,base_row*4+4,s+2+i,base_row*4+3+test_max);
				xlSheetWriteFormula(sheet, base_row*4+5+test_max, 2+i, buf1, format);
			}
			else
			{
				sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/64",s+2+i,base_row*4+4,s+2+i,
				base_row*4+3+test_max,s+2+i,base_row*4+4,s+2+i,base_row*4+3+test_max);
				xlSheetWriteFormula(sheet, base_row*4+5+test_max, 2+i, buf1, format);
			}
			
			//rx_window
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*3+4,s+2+i,base_row*3+3+test_max);
			xlSheetWriteFormula(sheet, base_row*3+3+test_max, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*3+4,s+2+i,base_row*3+3+test_max);
			xlSheetWriteFormula(sheet, base_row*3+4+test_max, 2+i, buf, descriptionFormat);
			
			if(i==18)
			{
				sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/407.97",s+2+i,base_row*3+4,s+2+i,
				base_row*3+3+test_max,s+2+i,base_row*3+4,s+2+i,base_row*3+3+test_max);
				xlSheetWriteFormula(sheet,base_row*3+5+test_max, 2+i, buf1, format);
			}
			else
			{
				sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/64",s+2+i,base_row*3+4,s+2+i,
				base_row*3+3+test_max,s+2+i,base_row*3+4,s+2+i,base_row*3+3+test_max);
				xlSheetWriteFormula(sheet, base_row*3+5+test_max, 2+i, buf1, format);
				
			}

		}
		
	}
	if(rank==0&&chanel==1)
	{
		
		for(i=0;i<10;i++)  
		{
			//tx_top
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*2+4+(test_max+3)*1,s+2+i,base_row*2+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, base_row*2+3+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*2+4+(test_max+3)*1,s+2+i,base_row*2+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, base_row*2+4+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/32",s+2+i,base_row*2+4+(test_max+3)*1,s+2+i,
			base_row*2+3+test_max+(test_max+3)*1,s+2+i,base_row*2+4+(test_max+3)*1,s+2+i,base_row*2+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, base_row*2+5+test_max+(test_max+3)*1, 2+i, buf1, format);
			//RX_top
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,4+(test_max+3)*1,s+2+i,3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, 3+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,4+(test_max+3)*1,s+2+i,3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, 4+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/407.97",s+2+i,4+(test_max+3)*1,s+2+i,
			3+test_max+(test_max+3)*1,s+2+i,4+(test_max+3)*1,s+2+i,3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, 5+test_max+(test_max+3)*1, 2+i, buf1, format);
			
			
			//rx_bottom
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row+4+(test_max+3)*1,s+2+i,base_row+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, base_row+3+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row+4+(test_max+3)*1,s+2+i,base_row+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, base_row+4+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/407.97",s+2+i,base_row+4+(test_max+3)*1,s+2+i,
			base_row+3+test_max+(test_max+3)*1,s+2+i,base_row+4+(test_max+3)*1,s+2+i,base_row+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, base_row+5+test_max+(test_max+3)*1, 2+i, buf1, format);
		
		}
		for(i=0;i<20;i++)  
		{
			//tx_window
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*4+4+(test_max+3)*1,s+2+i,base_row*4+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, base_row*4+3+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*4+4+(test_max+3)*1,s+2+i,base_row*4+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet,base_row*4+4+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			
			if(i==18)
			{
				sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/407.97",s+2+i,base_row*4+4+(test_max+3)*1,s+2+i,
				base_row*4+3+test_max+(test_max+3)*1,s+2+i,base_row*4+4+(test_max+3)*1,s+2+i,base_row*4+3+test_max+(test_max+3)*1);
				xlSheetWriteFormula(sheet, base_row*4+5+test_max+(test_max+3)*1, 2+i, buf1, format);
			}
			else
			{
				sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/64",s+2+i,base_row*4+4+(test_max+3)*1,s+2+i,
				base_row*4+3+test_max+(test_max+3)*1,s+2+i,base_row*4+4+(test_max+3)*1,s+2+i,base_row*4+3+test_max+(test_max+3)*1);
				xlSheetWriteFormula(sheet, base_row*4+5+test_max+(test_max+3)*1, 2+i, buf1, format);
			}
			
			
			//rx_window
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*3+4+(test_max+3)*1,s+2+i,base_row*3+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet, base_row*3+3+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*3+4+(test_max+3)*1,s+2+i,base_row*3+3+test_max+(test_max+3)*1);
			xlSheetWriteFormula(sheet,base_row*3+4+test_max+(test_max+3)*1, 2+i, buf, descriptionFormat);
			if(i==18)
			{
				sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/407.97",s+2+i,base_row*3+4+(test_max+3)*1,s+2+i,
				base_row*3+3+test_max+(test_max+3)*1,s+2+i,base_row*3+4+(test_max+3)*1,s+2+i,base_row*3+3+test_max+(test_max+3)*1);
				xlSheetWriteFormula(sheet, base_row*3+5+test_max+(test_max+3)*1, 2+i, buf1, format);
			}
			else
			{
				sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/64",s+2+i,base_row*3+4+(test_max+3)*1,s+2+i,
				base_row*3+3+test_max+(test_max+3)*1,s+2+i,base_row*3+4+(test_max+3)*1,s+2+i,base_row*3+3+test_max+(test_max+3)*1);
				xlSheetWriteFormula(sheet, base_row*3+5+test_max+(test_max+3)*1, 2+i, buf1, format);
			}
			
	
		}
		
	}
	if(rank==1&&chanel==0)
	{
		
		for(i=0;i<10;i++)   
		{
			//tx_top
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*2+4+(test_max+3)*2,s+2+i,base_row*2+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row*2+3+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*2+4+(test_max+3)*2,s+2+i,base_row*2+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row*2+4+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/32",s+2+i,base_row*2+4+(test_max+3)*2,s+2+i,
			base_row*2+3+test_max+(test_max+3)*2,s+2+i,base_row*2+4+(test_max+3)*2,s+2+i,base_row*2+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row*2+5+test_max+(test_max+3)*2, 2+i, buf1, format);
			
			//rx_top
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,4+(test_max+3)*2,s+2+i,3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, 3+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,4+(test_max+3)*2,s+2+i,3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, 4+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/407.97",s+2+i,4+(test_max+3)*2,s+2+i,
			3+test_max+(test_max+3)*2,s+2+i,4+(test_max+3)*2,s+2+i,3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, 5+test_max+(test_max+3)*2, 2+i, buf1, format);
			
			//rx_bottom
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row+4+(test_max+3)*2,s+2+i,base_row+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row+3+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row+4+(test_max+3)*2,s+2+i,base_row+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row+4+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/407.97",s+2+i,base_row+4+(test_max+3)*2,s+2+i,
			base_row+3+test_max+(test_max+3)*2,s+2+i,base_row+4+(test_max+3)*2,s+2+i,base_row+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row+5+test_max+(test_max+3)*2, 2+i, buf1, format);
		
		}
		for(i=0;i<20;i++)  
		{
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*4+4+(test_max+3)*2,s+2+i,base_row*4+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row*4+3+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*4+4+(test_max+3)*2,s+2+i,base_row*4+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet,base_row*4+4+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/64",s+2+i,base_row*4+4+(test_max+3)*2,s+2+i,
			base_row*4+3+test_max+(test_max+3)*2,s+2+i,base_row*4+4+(test_max+3)*2,s+2+i,base_row*4+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row*4+5+test_max+(test_max+3)*2, 2+i, buf1, format);
			
			
			
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*3+4+(test_max+3)*2,s+2+i,base_row*3+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row*3+3+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*3+4+(test_max+3)*2,s+2+i,base_row*3+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet,base_row*3+4+test_max+(test_max+3)*2, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/64",s+2+i,base_row*3+4+(test_max+3)*2,s+2+i,
			base_row*3+3+test_max+(test_max+3)*2,s+2+i,base_row*3+4+(test_max+3)*2,s+2+i,base_row*3+3+test_max+(test_max+3)*2);
			xlSheetWriteFormula(sheet, base_row*3+5+test_max+(test_max+3)*2, 2+i, buf1, format);
	
		}
		
	}
	if(rank==1&&chanel==1)
	{
		
		for(i=0;i<10;i++)   
		{
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,base_row*2+4+(test_max+3)*3,s+2+i,base_row*2+3+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, base_row*2+3+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,base_row*2+4+(test_max+3)*3,s+2+i,base_row*2+3+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, base_row*2+4+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/32",s+2+i,base_row*2+4+(test_max+3)*3,s+2+i,
			base_row*2+3+test_max+(test_max+3)*3,s+2+i,base_row*2+4+(test_max+3)*3,s+2+i,base_row*2+3+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, base_row*2+5+test_max+(test_max+3)*3, 2+i, buf1, format);
			
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,4+(test_max+3)*3,s+2+i,3+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 3+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,4+(test_max+3)*3,s+2+i,3+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 4+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			//------------------------------------------------------------------
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/32",s+2+i,44+(test_max+3)*3,s+2+i,
			43+test_max+(test_max+3)*3,s+2+i,44+(test_max+3)*3,s+2+i,43+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 45+test_max+(test_max+3)*3, 2+i, buf1, format);
			
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,24+(test_max+3)*3,s+2+i,23+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 23+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,24+(test_max+3)*3,s+2+i,23+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 24+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/32",s+2+i,44+(test_max+3)*3,s+2+i,
			43+test_max+(test_max+3)*3,s+2+i,44+(test_max+3)*3,s+2+i,43+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 45+test_max+(test_max+3)*3, 2+i, buf1, format);
		
		}
		for(i=0;i<20;i++)  
		{
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,84+(test_max+3)*3,s+2+i,83+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 83+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,84+(test_max+3)*3,s+2+i,83+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet,84+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/32",s+2+i,44+(test_max+3)*3,s+2+i,
			43+test_max+(test_max+3)*3,s+2+i,44+(test_max+3)*3,s+2+i,43+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 45+test_max+(test_max+3)*3, 2+i, buf1, format);
			
			sprintf(buf,"=MIN(%c%d:%c%d)",s+2+i,64+(test_max+3)*3,s+2+i,63+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 63+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			sprintf(buf,"=MAX(%c%d:%c%d)",s+2+i,64+(test_max+3)*3,s+2+i,63+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet,64+test_max+(test_max+3)*3, 2+i, buf, descriptionFormat);
			
			sprintf(buf1,"=(MAX(%c%d:%c%d)-MIN(%c%d:%c%d))/32",s+2+i,44+(test_max+3)*3,s+2+i,
			43+test_max+(test_max+3)*3,s+2+i,44+(test_max+3)*3,s+2+i,43+test_max+(test_max+3)*3);
			xlSheetWriteFormula(sheet, 45+test_max+(test_max+3)*3, 2+i, buf1, format);
	
		}
		
	}		
}













