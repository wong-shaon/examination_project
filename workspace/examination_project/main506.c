//I am studying git
//Language C is the best in the world.
#include "main.h"
#include <windows.h>
static char excel_name[100][10];

void create_excel(BookHandle book,int board)
{ 
    if(book) 
    {   	
        FontHandle boldFont;
        FontHandle titleFont;
        FormatHandle headerFormat;
        FormatHandle descriptionFormat;
		SheetHandle sheets[16];
		int i=0;
		char sheet_name[5]={0};
		for(i=0;i<16;i++)
		{
			sprintf(sheet_name,"DQ%d",i);
			sheets[i]= xlBookAddSheet(book, sheet_name, 0);
		}
		int  f[3];
        FormatHandle format[3];
        SheetHandle sheet;		

        f[0] = xlBookAddCustomNumFormat(book, "0.00%");
        f[1] = xlBookAddCustomNumFormat(book, "0.00");
        f[2] = xlBookAddCustomNumFormat(book, "#,###.00 $[Black][<1000];#,###.00 $[Red][>=1000]");

        for(i = 0; i < 3; ++i) {
            format[i] = xlBookAddFormat(book, 0);
            xlFormatSetNumFormat(format[i], f[i]);
			xlFormatSetAlignH(format[i], ALIGNH_CENTER);
			xlFormatSetBorderBottom(format[i], BORDERSTYLE_THIN);
        }

        boldFont = xlBookAddFont(book, 0);
        xlFontSetBold(boldFont, 1);

        titleFont = xlBookAddFont(book, 0);
        xlFontSetName(titleFont, "Arial Black");
        xlFontSetSize(titleFont, 16);

        headerFormat = xlBookAddFormat(book, 0);
        xlFormatSetAlignH(headerFormat, ALIGNH_LEFT);
        xlFormatSetBorder(headerFormat, BORDERSTYLE_THIN);
        xlFormatSetFont(headerFormat, titleFont);        
        xlFormatSetFillPattern(headerFormat, FILLPATTERN_SOLID);
        xlFormatSetPatternForegroundColor(headerFormat, COLOR_GREEN);

        descriptionFormat = xlBookAddFormat(book, 0);
		xlFormatSetAlignH(descriptionFormat, ALIGNH_CENTER);
        xlFormatSetBorderBottom(descriptionFormat, BORDERSTYLE_THIN);
		
		draw_excel(headerFormat,descriptionFormat,sheets);
		
		struct file_info *ret = malloc(sizeof(*ret));
		int flag=0;
		for(i=0;i<file_num;i++)
		{
			//puts(path_name[i]);
			get_board_num(path_name[i],ret);
			if(ret->board_num==board)
			{	
				flag=1;
				get_pattern(path_name[i],ret->test_time,descriptionFormat,sheets);
				
			}
			if(flag==0&&i==file_num-1)
			{
				printf("-----search board %d failed !---------\n",board);
			}	
		}
		
		for(i=0;i<16;i++)
		{
			write_max_min(0,0,sheets[i],descriptionFormat,format[0]);
			write_max_min(1,0,sheets[i],descriptionFormat,format[0]);
		}
		
		
	}
	
}


int main()
{
	char buf[100]={0};
	search_files();	
	if(_chdir("./..") == -1)
	{
		printf("80 change error");
	}
	getcwd( buf,100);
	puts(buf);
	int i=0;
	
	printf("board_max==================================%d\n",board_max);
	printf("file_num===================================%d\n",file_num);
	printf("test_max===================================%d\n",test_max);
	if(board_max*test_max != file_num)
	{
		printf("The number of tests may be lost");
		printf("\nPress any key to exit...");
		getchar();
		return 0;
	}
	
	for(i=0;i<=100;i++)
	{		
		sprintf(excel_name[i],"#%d.xlsx",i+1);
	}
	
	BookHandle book;
	
	for(i=0;i<board_max;i++)
	{
		
		book = xlCreateXMLBook();
		//xlBookSetKey(book,name[0],key[0]);
		xlBookSetKey(book, "hello", "windows-21202e0009c9e10465be636ca9i1kcgd");
		create_excel(book,i+1);

		if(xlBookSave(book, excel_name[i])) 
		{
			printf("\n the excel has been created.\n");
		}	
		xlBookRelease(book);  
	}	 

	printf("-----------------124------------------\n");
	test_board_all();
	printf("-----------------126------------------\n");
	get_standard();
	printf("-----------------128------------------\n");
	get_data_book(excel_name);
 
	
	printf("\nPress any key to exit...");
    getchar();
}


//gcc main506.c get_excel_data.c excel.c analysis_info.c summary.c file.c read506.c -o 519.exe -I./include_c -L./bin64 -lxl





