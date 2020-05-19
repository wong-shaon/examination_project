#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__


#include<stdio.h>
#include<stdlib.h>
#include<string.h>



/*
rx_top[0]-rx_top[10]对应'O' 'Q' 'O+Q' 'H' 'H+O' 'H+Q' 'H+Q+O' "QHQ" "QQHQQ" "QQQHQQQ" 
rx_bottom[0]-rx_bottom[10]对应'O' 'Q' 'O+Q' 'H' 'H+O' 'H+Q' 'H+Q+O' "QHQ" "QQHQQ" "QQQHQQQ"

*/
struct rx_info_extract
{
	char rx_top[10][8];   
	char rx_bottom[10][8];
	char rx_window[20][8];
		
};

struct tx_info_extract
{
	char tx_top[10][7];
	char tx_window[20][7];	
};


/* struct rx_info_f
{
	float rx_top[10];   
	float rx_bottom[10];
	float rx_window[20];
		
};

struct tx_info_f
{
	float tx_top[10];
	float tx_window[20];	
};




struct rx_info_f *rx_to_float(struct rx_info_extract * rx,struct rx_info_f * ret);
struct tx_info_f *tx_to_float(struct tx_info_extract * tx,struct tx_info_f * ret); */



void analyse_info_rx_bottom(char eye_pattern[][100],struct rx_info_extract *rx_info_bottom);

void analyse_info_rx_top(char eye_pattern[][100],struct rx_info_extract *rx_info_top);

void analyse_info_tx_top(char eye_pattern_tx[][100],struct tx_info_extract *tx_info);

void analyse_info_rx_window(char eye_pattern[][100],struct rx_info_extract *rx_info_ret);

void analyse_info_tx_window(char eye_pattern_tx[][100],struct tx_info_extract *tx_info_ret);


#endif
