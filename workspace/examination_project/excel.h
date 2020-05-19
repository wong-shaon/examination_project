#ifndef __EXCEL_H__
#define __EXCEL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "libxl.h"
#include "analysis_info.h"
extern int base_row;

void draw_excel(FormatHandle headerFormat,FormatHandle descriptionFormat,SheetHandle *sheets);

void write_data_rx(int chanel,int rank,int test_num,struct rx_info_extract* rx,SheetHandle sheet,
					FormatHandle descriptionFormat);

void write_data_tx(int chanel,int rank,int test_num,struct tx_info_extract* tx,SheetHandle sheet,
					FormatHandle descriptionFormat);


void excel(char eye_pattern[][100],int dq_num,char mode,BookHandle book,int chanel,int rank,
					int test_num,SheetHandle sheet,FormatHandle descriptionFormat);
					
void write_max_min(int chanel,int rank,SheetHandle sheet,FormatHandle descriptionFormat,FormatHandle format);

#endif
