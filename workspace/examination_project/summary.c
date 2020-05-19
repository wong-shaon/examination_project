#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "libxl.h"
#include "file.h"


static int chanel = 1;
static int rank = 0;


/*
draw_test 用于完成每一行的测试次数等相关参数的制表
@ sheet 需要写入的页
@ descriptionFormat 写入的样式
@ row 需要写入的行
*/
static void draw_test(SheetHandle Sheet,FormatHandle DescriptionFormat,int row)
{
	
	char test[5]={0};
	int i = 0;
	while(i<board_max)
	{
		i=i+1;
		sprintf(test,"#%d",i);
		xlSheetWriteStr(Sheet, row-1+i, 1, test, DescriptionFormat);
		
	}
	
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

		xlSheetSetMerge(sheet, base_row+3, base_row+2+board_max, 0,0);
		xlSheetWriteStr(sheet, base_row+3, 0, "CH0_RANK0", descriptionFormat);
		draw_test(sheet,descriptionFormat,base_row+3);
		
		if(chanel==1 && rank==0)
		{
			xlSheetSetMergeA(sheet, base_row+3+board_max, base_row+2+board_max*2, 0,0);
			xlSheetWriteStr(sheet, base_row+3+board_max, 0, "CH1_RANK0", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+3+board_max);
	
		}
		if(rank == 1 && chanel==0)
		{
			xlSheetSetMergeA(sheet, base_row+3+board_max, base_row+2+board_max*2, 0,0);
			xlSheetWriteStr(sheet, base_row+3+board_max, 0, "CH0_RANK1", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+3+board_max);
			
		}
		if(rank==1 && chanel==1)
		{
			xlSheetSetMergeA(sheet, base_row+3+board_max, base_row+2+board_max*2, 0,0);
			xlSheetWriteStr(sheet, base_row+3+board_max, 0, "CH1_RANK0", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+6+board_max);
	
			
			xlSheetSetMergeA(sheet, base_row+3+board_max*2, base_row+2+board_max*3, 0,0);
			xlSheetWriteStr(sheet, base_row+3+board_max*2, 0, "CH0_RANK1", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+3+board_max*2);

			
			xlSheetSetMergeA(sheet,base_row+3+board_max*3, base_row+2+board_max*4, 0,0);
			xlSheetWriteStr(sheet, base_row+3+board_max*3, 0, "CH1_RANK1", descriptionFormat);
			draw_test(sheet,descriptionFormat,base_row+3+board_max*3);

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

static void draw_top_bottom_excel(SheetHandle sheet,FormatHandle descriptionFormat,FormatHandle summaryFormat,
					int base_row,FormatHandle headerFormat,const char *title,char buf[][14])
{
	xlSheetSetMergeA(sheet, base_row+1, base_row+1, 0,13);
	xlSheetWriteStr(sheet, base_row+1, 0, title, headerFormat);
	int i=0;
	for(i=1;i<=11;i++)
	{
		xlSheetWriteStr(sheet, base_row+2, i, buf[i-1], descriptionFormat);
	}
	xlSheetWriteStr(sheet, base_row+2, i, buf[11], summaryFormat);
	xlSheetWriteStr(sheet, base_row+2, i+1, buf[12], summaryFormat);

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
static void draw_window_excel(SheetHandle sheet,FormatHandle descriptionFormat,FormatHandle summaryFormat,
					int base_row,FormatHandle headerFormat,const char *title,char buf[][11])
{
	xlSheetSetMergeA(sheet, base_row+1, base_row+1, 0,23);
	xlSheetWriteStr(sheet, base_row+1, 0, title, headerFormat);
	
	int i=0;
	for(i=1;i<=21;i++)
	{
		xlSheetWriteStr(sheet, base_row+2, i, buf[i-1], descriptionFormat);
	}
	xlSheetWriteStr(sheet, base_row+2, i, buf[21], summaryFormat);
	xlSheetWriteStr(sheet, base_row+2, i+1, buf[22], summaryFormat);
	draw_column(sheet,descriptionFormat,base_row);

}
/*
test_board_all 主要用于画出一个汇总表
*/


void test_board_all()
{
    BookHandle book_all = xlCreateXMLBook();
	xlBookSetKey(book_all,"summary","windows-2b2c250507c6e30a66bc696fabt7u4h1");
    if(book_all) 
    {   
        FontHandle boldFont;
        FontHandle titleFont;
        FormatHandle headerFormat;
        FormatHandle descriptionFormat;
		FormatHandle summaryFormat;
		
		SheetHandle sheet_all[16];
		SheetHandle sheet_summary;
		int i=0;
		char sheet_name[5]={0};
		sheet_summary= xlBookAddSheet(book_all, "summary", 0);
		for(i=0;i<16;i++)
		{
			sprintf(sheet_name,"DQ%d",i);
			sheet_all[i]= xlBookAddSheet(book_all, sheet_name, 0);
		}
        boldFont = xlBookAddFont(book_all, 0);
        xlFontSetBold(boldFont, 1);

        titleFont = xlBookAddFont(book_all, 0);
        xlFontSetName(titleFont, "Arial Black");
        xlFontSetSize(titleFont, 16);

        headerFormat = xlBookAddFormat(book_all, 0);
        xlFormatSetAlignH(headerFormat, ALIGNH_CENTER);
        xlFormatSetBorder(headerFormat, BORDERSTYLE_THIN);
        xlFormatSetFont(headerFormat, titleFont);        
        xlFormatSetFillPattern(headerFormat, FILLPATTERN_SOLID);
        xlFormatSetPatternForegroundColor(headerFormat, COLOR_GREEN);
		
		summaryFormat = xlBookAddFormat(book_all, 0);
        xlFormatSetBorder(summaryFormat, BORDERSTYLE_THIN);
        xlFormatSetFont(summaryFormat, boldFont);
        xlFormatSetFillPattern(summaryFormat, FILLPATTERN_SOLID);
        xlFormatSetPatternForegroundColor(summaryFormat, COLOR_YELLOW);

        descriptionFormat = xlBookAddFormat(book_all, 0);
		xlFormatSetAlignH(descriptionFormat, ALIGNH_CENTER);
        xlFormatSetBorderBottom(descriptionFormat, BORDERSTYLE_THIN);
		int base_row=0;  // 用于保存行的基数
		if(rank+chanel != 0)
		{
			base_row = board_max*2*(rank+chanel)+4;
		}
		else
		{
			base_row = board_max+4;
		}
		char buf_rx_top_bottom[13][14]={"board_num","O","Q","O+Q","H","H+O","H+Q","H+Q+O","QHQ",
						"QQHQQ","QQQHQQQ","Vol_Min","Vol_Max"};
		
		char buf_tx_top[13][14]={"board_num","O","H","O+H","OHO","OOHOO","OOOHOOO","OOOOHOOOO",
						"OOOOOHOOOOO","OOOOOOHOOOOOO","QQQHQQQ","Vol_Min","Vol_Max"};
		
		char buf_rx_window[23][11]={"board_num","264.00m_LW","264.00m_RW","264.00m_TW","206.70m_LW","206.70m_RW",
					"206.70m_TW","172.90m_LW","172.90m_RW","172.90m_TW","160.60m_LW","160.60m_RW","160.60m_TW",
					"123.90m_LW","123.90m_RW","123.90m_TW","87.14m_LW","87.14m_RW","87.14m_TW",
					"H_site","V_site","Vol_Min","Vol_Max"};
		char buf_tx_window[23][11]={"board_num","28.0%_LW","28.0%_RW","28.0%_TW","24.8%_LW","24.8%_RW","24.8%_TW",
					"21.6%_LW","21.6%_RW","21.6%_TW","18.4%_LW","18.4%_RW","18.4%_TW","14.0%_LW","14.0%_RW",
					"14.0%_TW","10.0%_LW","10.0%_RW","10.0%_TW","H_site","V_site","Vol_Min","Vol_Max"};		
		
		char summary[18][6]={"NUM","judge","DQ0","DQ1","DQ2","DQ3","DQ4","DQ5","DQ6","DQ7","DQ8","DQ9","DQ10",
								"DQ11","DQ12","DQ13","DQ14","DQ15"};
		
		
		if(sheet_summary)
		{
			
			xlSheetSetMergeA(sheet_summary, 1, 1, 1,18);
			xlSheetWriteStr(sheet_summary, 1, 1, "summary of board", headerFormat);
			int i;
			
			for(i=0;i<18;i++)
			{
				xlSheetWriteStr(sheet_summary, 2, 1+i, summary[i], descriptionFormat);
			}
			draw_test(sheet_summary,descriptionFormat,3);
			
		}
		
		for(i=0;i<16;i++)
		{
			if(sheet_all[i])
			{

				xlSheetSetMergeA(sheet_all[i], 1, 1, 0,13);
				xlSheetWriteStr(sheet_all[i], 1, 0, "Summary of reliability and stability", headerFormat);
				xlSheetSetMergeA(sheet_all[i], 2, 4, 0,13);
				
				draw_top_bottom_excel(sheet_all[i],descriptionFormat,summaryFormat,
				0*base_row+5,headerFormat,"RX_TOP",buf_rx_top_bottom);
				
				draw_top_bottom_excel(sheet_all[i],descriptionFormat,summaryFormat,
				1*base_row+5,headerFormat,"RX_BOTTOM",buf_rx_top_bottom);
				
				draw_top_bottom_excel(sheet_all[i],descriptionFormat,summaryFormat,
				2*base_row+5,headerFormat,"TX_TOP",buf_tx_top);

				draw_window_excel(sheet_all[i],descriptionFormat,summaryFormat,
				3*base_row+5,headerFormat,"RX_WINDOW",buf_rx_window);

				draw_window_excel(sheet_all[i],descriptionFormat,summaryFormat,
				4*base_row+5,headerFormat,"TX_WINDOW",buf_tx_window);		

				int j;
				for(j=0;j<26;j++)
				{
					xlSheetSetCol(sheet_all[i], j, j, 16, 0, 0);
				} 
		  
			}

		}
        
        if(xlBookSave(book_all, "summary.xlsx")) 
			printf("\nFile summary.xlsx has been created.\n");
		xlBookRelease(book_all);   
    }



    return ;	
}

/*
int main()
{
	test_board_all();
	   printf("\nPress any key to exit...");
    _getch();
}
*/























