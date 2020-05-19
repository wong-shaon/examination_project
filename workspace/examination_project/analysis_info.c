#include "analysis_info.h"


char voltage[7]={0};    //保存一个电压值，为了避免返回局部变量而出错
int location_rx_H=0;   //为windows记录H的位置
float location_rx_V=0;   //为windows记录V的位置 

int location_tx_H=0;   //为windows记录H的位置
float location_tx_V=0;   //为windows记录V的位置
char percent[7]={0};   //保存一个百分比值。为了避免返回局部变量出错

struct lrt     //该结构体用于保存windows相关参数的指标值
{
	char l[7];   //左
	char r[7];   //右
	char t[7];   //总
};

void analyse_info_bottom(char eye_pattern[][100],int flag[11],int i,int j,struct rx_info_extract * rx_info);
void analyse_info_top(char eye_pattern[][100],int flag[11],int i,int j,struct rx_info_extract * rx_info);

/*
ctof用于将一个指定的字符串转化为float类型
@ eye_pattern[][100] 一个眼图信息数组 
@i 需要转化的字符串对应的二维数组的第一维的值
*/
float ctof(char eye_pattern[][100],int i)
{
	char a[8]={0};
	int k,temp=0;
	float num = 0,temp0;
	for(k=0;k<6;k++)
	{
		a[k] = eye_pattern[i][5+k];
	}
	num=strtod(a,NULL);

	return num;
}

/*
 get_lrt 用于获取眼图中有关windows参数的值
 @eye_pattern 眼图信息数组
 @row  定位需要进行解析的行坐标
 @lrt_all 保存解析出来的相关信息的结构体的地址
*/

void get_lrt(char eye_pattern[][100],int row,struct lrt *lrt_all)
{

	int i = 0,flag_h=0,l=0,r=0,t=0;
	
	for(i=0;i<100;i++)
	{
		if((eye_pattern[row][13+i]=='O'||eye_pattern[row][13+i]=='Q'||eye_pattern[row][13+i]=='V')&&flag_h==0)
		{
			l++;
		}
		if(eye_pattern[row][13+i]=='H')
		{
			flag_h = 1;
			while(eye_pattern[row][13+i+1] != '.'&&eye_pattern[row][13+i+1] != ' ')
			{
				r++;
				i++;
			}
		}
	}
	if(flag_h==0)
	{
		r = l-1;
		l = 0;
		
	}
	t = l+r+1;
	itoa(l,lrt_all->l,10);
	itoa(r,lrt_all->r,10);
	itoa(t,lrt_all->t,10);	

}

/*
get_voltage获取rx眼图指标的对应电压 
 @eye_pattern 眼图信息数组
 @row  定位需要进行解析的行坐标
 return 返回解析到的字符串的地址
*/
char *get_voltage(char eye_pattern[][100],int row)
{
	int i = 0;
	for(i=0;i<7;i++)
	{
		if(eye_pattern[row][5+i] != 'm')
		{
			voltage[i] = eye_pattern[row][5+i];
		}
		else
		{
			voltage[i] = '\0';
		}
	}
	return voltage;
}

/*
get_percent获取tx眼图指标的对应的百分比
@eye_pattern 眼图信息数组
@row  定位需要进行解析的行坐标
return 返回解析到的字符串的地址
*/ 
char *get_percent(char eye_pattern_tx[][100],int row)
{
	int i = 0;
	for(i=0;i<6;i++)
	{
		percent[i] = eye_pattern_tx[row][5+i];
	}
	percent[6]='\0';
	return percent;
}

/*
analyse_info_rx_bottom 从眼图中解析出rx_bottom相关指标的参数
@eye_pattern 眼图信息数组
@rx_info_bottom 用于保存解析出的相关信息的结构体的指针
*/
void analyse_info_rx_bottom(char eye_pattern[][100],struct rx_info_extract *rx_info_bottom)
{	
	int i,j,k,flag_all=0;
	int flag[11] = {0};//flag[0]-flag[10]对应'O' 'Q' 'O+Q' 'H' 'H+O' 'H+Q' 'H+Q+O' "QHQ" 
						//"QQHQQ" "QQQHQQQ" 
	
	for(i=32;i>0;i--)
	{
		for(j=0;j<100;j++)
		{
			analyse_info_bottom(eye_pattern,flag,i,j,rx_info_bottom);
		}
		flag_all=0; 
		for(k=0;k<11;k++)
		{
			flag_all+=flag[k];
		//	printf("flag[%d]=%d",k,flag[k]);
		}
		if(flag_all == 11)
		{
		//	printf("search rx all_bottom success\n");
			break;
		}
	}	
}

/*
analyse_info_rx_top 从眼图中解析出rx_top相关指标的参数
@eye_pattern 眼图信息数组
@rx_info_top 用于保存解析出的相关信息的结构体的指针
*/

void analyse_info_rx_top(char eye_pattern[][100],struct rx_info_extract *rx_info_top)
{
	int i,j,k,flag_all=0;
	int flag[11] = {0};//flag[0]-flag[10]对应'O' 'Q' 'O+Q' 'H' 'H+O' 'H+Q' 'H+Q+O' "QHQ" 
						//"QQHQQ" "QQQHQQQ" 
	
	for(i=0;i<32;i++)
	{
		for(j=0;j<100;j++)
		{
			analyse_info_top(eye_pattern,flag,i,j,rx_info_top);
		}
		flag_all=0; 
		for(k=0;k<11;k++)
		{
			flag_all+=flag[k];
		}
		if(flag_all == 11)
		{
		//	printf("search rx all_top success\n");
			break;
		}
	}	
} 

/*
analyse_info_tx_top 从眼图中解析出tx_top相关指标的参数
@eye_pattern 眼图信息数组
@tx_info 用于保存解析出的相关信息的结构体的指针
*/
void analyse_info_tx_top(char eye_pattern_tx[][100],struct tx_info_extract *tx_info)
{
	int i,j,k,flag_all;
	int flag[11] = {0};
	for(i=0;i<81;i++)
	{
		for(j=0;j<100;j++)
		{
			if(eye_pattern_tx[i][j]=='V' && eye_pattern_tx[i][j+1]=='V' && flag[0] == 0)//'V'
			{
				flag[0] = 1;
				strcpy(tx_info->tx_window[18],get_percent(eye_pattern_tx,i)); 
		//	printf("=====368=====\n");
		//	printf("=====location_V=%s====\n",tx_info->tx_window[18]);
				
			}
			else if(eye_pattern_tx[i][j]=='O'&&(flag[1]==0||flag[3]==0||flag[4]==0
			||flag[5]==0||flag[6]==0||flag[7]==0||flag[8]==0||flag[10]==0)) //'O'
			{
				if(flag[1]==0)
				{
					flag[1]=1;
			//		puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[0],get_percent(eye_pattern_tx,i)); 
			//		printf("=====O=%s====\n",tx_info->tx_top[0]);	
				}
				if(flag[3]==0)
				{
					flag[3]=1;
			//		puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[2],get_percent(eye_pattern_tx,i)); 
			//		printf("=====O+H=%s====\n",tx_info->tx_top[2]);	
				}
				if(eye_pattern_tx[i][j+1]=='H'&&eye_pattern_tx[i][j+2]=='O'&&flag[4]==0)  //"OHO"
				{
		
					flag[4]=1;
			//		puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[3],get_percent(eye_pattern_tx,i)); 
			//		printf("=====OHO=%s====\n",tx_info->tx_top[3]);	
				}
				if(eye_pattern_tx[i][j+1]=='O'&&eye_pattern_tx[i][j+2]=='H'&&eye_pattern_tx[i][j+3]=='O'
					&&eye_pattern_tx[i][j+4]=='O'&&flag[5]==0) // "OOHOO"
				{
				
					flag[5]=1;
			//		puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[4],get_percent(eye_pattern_tx,i)); 
			//		printf("=====OOHOO=%s====\n",tx_info->tx_top[4]);	
				}
				if(eye_pattern_tx[i][j+1]=='O'&&eye_pattern_tx[i][j+2]=='O'&&eye_pattern_tx[i][j+3]=='H'
					&&eye_pattern_tx[i][j+4]=='O'&&eye_pattern_tx[i][j+5]=='O'&&eye_pattern_tx[i][j+6]=='O'&&flag[6]==0) // "OOOHOOO"
				{
					flag[6]=1;
			//		puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[5],get_percent(eye_pattern_tx,i)); 
			//		printf("=====OOOHOOO=%s====\n",tx_info->tx_top[5]);	
				}
				if(eye_pattern_tx[i][j+1]=='O'&&eye_pattern_tx[i][j+2]=='O'&&eye_pattern_tx[i][j+3]=='O'
					&&eye_pattern_tx[i][j+4]=='H'&&eye_pattern_tx[i][j+5]=='O'&&eye_pattern_tx[i][j+6]=='O'
					&&eye_pattern_tx[i][j+7]=='O'&&eye_pattern_tx[i][j+8]=='O'&&flag[7]==0) // "OOOOHOOOO"
				{
					flag[7]=1;
				//	puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[6],get_percent(eye_pattern_tx,i)); 
				//	printf("=====OOOOHOOOO=%s====\n",tx_info->tx_top[6]);	
				}
				if(eye_pattern_tx[i][j+1]=='O'&&eye_pattern_tx[i][j+2]=='O'&&eye_pattern_tx[i][j+3]=='O'
					&&eye_pattern_tx[i][j+4]=='O'&&eye_pattern_tx[i][j+5]=='H'&&eye_pattern_tx[i][j+6]=='O'
					&&eye_pattern_tx[i][j+7]=='O'&&eye_pattern_tx[i][j+8]=='O'&&eye_pattern_tx[i][j+9]=='O'
					&&eye_pattern_tx[i][j+10]=='O'&&flag[8]==0) // "OOOOOHOOOOO"
				{
					flag[8]=1;
			//		puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[7],get_percent(eye_pattern_tx,i)); 
			//		printf("=====OOOOOHOOOOO=%s====\n",tx_info->tx_top[7]);	
				}
				if(eye_pattern_tx[i][j+1]=='O'&&eye_pattern_tx[i][j+2]=='O'&&eye_pattern_tx[i][j+3]=='O'
					&&eye_pattern_tx[i][j+4]=='O'&&eye_pattern_tx[i][j+5]=='O'&&eye_pattern_tx[i][j+6]=='H'
					&&eye_pattern_tx[i][j+7]=='O'&&eye_pattern_tx[i][j+8]=='O'&&eye_pattern_tx[i][j+9]=='O'
					&&eye_pattern_tx[i][j+10]=='O'&&eye_pattern_tx[i][j+11]=='O'&&eye_pattern_tx[i][j+12]=='O'
					&&flag[10]==0) // "OOOOOOHOOOOOO"
				{
					//printf("===359==OOOOOOHOOOOOO=====\n");
					flag[10]=1;
					//puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[8],get_percent(eye_pattern_tx,i)); 
				//	printf("=====OOOOOOHOOOOOO=%s====\n",tx_info->tx_top[8]);	
				}
				
				
			}
			
			else if(eye_pattern_tx[i][j]=='H'&&(flag[2]==0||flag[3]==0)) //'H'
			{
				location_tx_H=j-11;
				itoa(j-11,tx_info->tx_window[19],10);
			//	printf("=====location_H=%s====\n",tx_info->tx_window[19]);
				if(flag[2]==0)
				{
					flag[2]=1;	
				//	puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[1],get_percent(eye_pattern_tx,i)); 
				//	printf("=====H=%s====\n",tx_info->tx_top[1]);	
				}
				if(flag[3]==0)
				{
					flag[3]=1;
				//	puts(eye_pattern_tx[i]);
					strcpy(tx_info->tx_top[2],get_percent(eye_pattern_tx,i)); 
				//	printf("=====O+H=%s====\n",tx_info->tx_top[2]);	
				}
			}
			else if(eye_pattern_tx[i][j]=='Q'&&eye_pattern_tx[i][j+1]=='Q'&&eye_pattern_tx[i][j+2]=='Q'&&eye_pattern_tx[i][j+3]=='H'
			&&eye_pattern_tx[i][j+4]=='Q'&&eye_pattern_tx[i][j+5]=='Q'&&eye_pattern_tx[i][j+6]=='Q'&&flag[9] ==0)  ///QQQHQQQ
			{
				flag[9] = 1;
			//	puts(eye_pattern_tx[i]);
				strcpy(tx_info->tx_top[9],get_percent(eye_pattern_tx,i)); 
			//	printf("=====QQQHQQQ=%s====\n",tx_info->tx_top[9]);	
			}
			
		}
		flag_all=0;
		for(k=0;k<11;k++)
		{
			flag_all+=flag[k];
		}
		if(flag_all==11)
		{
		//	printf("search tx all_top success\n" );
			break;
		}
	}
}

/*
analyse_info_rx_top 从眼图中解析出rx_bottom相关指标的参数的具体操作流程
@eye_pattern 眼图信息数组
@flag[11] 用于保存各个参数的对应数值是否被找到的情况
@i,j  用于确定眼图的一个字符
@rx_info 用于保存解析出的相关信息的结构体的指针
*/

void analyse_info_top(char eye_pattern[][100],int flag[11],int i,int j,struct rx_info_extract * rx_info)
{
	
	if(eye_pattern[i][j]=='V' && eye_pattern[i][j+1]=='V' && flag[10] == 0)
	{
		
		flag[10] = 1;
		strcpy(rx_info->rx_window[18],get_voltage(eye_pattern,i)); 
	//	printf("=====368=====\n");
	//	printf("=====location_V=%s====\n",rx_info->rx_window[18]);
		
	}
	else if(eye_pattern[i][j]=='O'&&(flag[0]==0||flag[2]==0||flag[4]==0||flag[6]==0))//遇到'O' 
	{
		
		if(flag[0] ==0)
		{
			flag[0] =1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[0],get_voltage(eye_pattern,i)); 
	//		printf("=====O=%s====\n",rx_info->rx_top[0]);
		}
		if(flag[2] == 0)
		{
			flag[2] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[2],get_voltage(eye_pattern,i)); 
	//		printf("=====O+Q=%s====\n",rx_info->rx_top[2]);
		}
		if(flag[4] == 0)
		{
			flag[4] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[4],get_voltage(eye_pattern,i)); 
	//		printf("=====O+H=%s====\n",rx_info->rx_top[4]);
		}
		
		if(flag[6] == 0)
		{
	//		printf("=====398=====\n");
			flag[6] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[6],get_voltage(eye_pattern,i)); 
	//		printf("=====H+Q+O=%s====\n",rx_info->rx_top[6]);
		}
		
		
	}
	
	else if(eye_pattern[i][j]=='H'&&(flag[3]==0||flag[4]==0||flag[5]==0||flag[6]==0))//遇到'H'
	{
		
		
		location_rx_H = j-12;
		//sprintf(rx_info->rx_window[19],"%d",j-12);
		itoa(j-12,rx_info->rx_window[19],10);
	//	printf("=====location_H=%s====\n",rx_info->rx_window[19]);
		if(flag[3] == 0)
		{
			flag[3] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[3],get_voltage(eye_pattern,i)); 
	//		printf("=====H=%s====\n",rx_info->rx_top[3]);
			
		}
		if(flag[4] == 0)
		{
			flag[4] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[4],get_voltage(eye_pattern,i)); 
	//		printf("=====H+O=%s====\n",rx_info->rx_top[4]);
		}
		if(flag[5] == 0)
		{
			flag[5] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[5],get_voltage(eye_pattern,i)); 
	//		printf("=====H+Q=%s====\n",rx_info->rx_top[5]);
		}
		if(flag[6] == 0)
		{
			flag[6] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[6],get_voltage(eye_pattern,i)); 
	//		printf("=====H+Q+O=%s====\n",rx_info->rx_top[6]);
		}
	}

	else if(eye_pattern[i][j]=='Q'&&(flag[1]==0||flag[2]==0||flag[5]==0||flag[6]==0
	||flag[7]==0||flag[8]==0||flag[9]==0))//遇到'Q' 
	{
		if(flag[1] == 0)
		{
			flag[1] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[1],get_voltage(eye_pattern,i)); 
	//		printf("=====Q=%s====\n",rx_info->rx_top[1]);
			
		}
		if(flag[2] == 0)
		{
			flag[2] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[2],get_voltage(eye_pattern,i)); 
	//		printf("=====Q+O=%s====\n",rx_info->rx_top[2]);
		}
		if(flag[5] == 0)
		{
			flag[5] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[5],get_voltage(eye_pattern,i)); 
	//		printf("=====H+Q=%s====\n",rx_info->rx_top[5]);
		}
		if(flag[6] == 0)
		{
			flag[6] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[6],get_voltage(eye_pattern,i)); 
	//		printf("=====H+Q+O=%s====\n",rx_info->rx_top[6]);
		}
		if(eye_pattern[i][j+1]=='H'&&eye_pattern[i][j+2]=='Q'&&flag[7] == 0)  //QHQ
		{
			flag[7] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[7],get_voltage(eye_pattern,i)); 
	//		printf("=====QHQ=%s====\n",rx_info->rx_top[7]);
		}
		if(eye_pattern[i][j+1]=='Q'&&eye_pattern[i][j+2]=='H'&&eye_pattern[i][j+3]=='Q'
		&&eye_pattern[i][j+4]=='Q'&&flag[8] == 0)  //QQHQQ
		{
			flag[8] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[8],get_voltage(eye_pattern,i)); 
	//		printf("=====QQHQQ=%s====\n",rx_info->rx_top[8]);
		}
		if(eye_pattern[i][j+1]=='Q'&&eye_pattern[i][j+2]=='Q'&&eye_pattern[i][j+3]=='H'&&eye_pattern[i][j+4]=='Q'
		&&eye_pattern[i][j+5]=='Q'&&eye_pattern[i][j+6]=='Q'&&flag[9] ==0)  //QQQHQQQ
		{
			flag[9] = 1;
	//		puts(eye_pattern[i]);
			strcpy(rx_info->rx_top[9],get_voltage(eye_pattern,i)); 
	//		printf("=====QQQHQQQ=%s====\n",rx_info->rx_top[9]);
		}
	}
}



/*
analyse_info_rx_bottom 从眼图中解析出rx_bottom相关指标的参数的具体操作流程
@eye_pattern 眼图信息数组
@flag[11] 用于保存各个参数的对应数值是否被找到的情况
@i,j  用于确定眼图的一个字符
@rx_info 用于保存解析出的相关信息的结构体的指针
*/


void analyse_info_bottom(char eye_pattern[][100],int flag[11],int i,int j,struct rx_info_extract * rx_info)
{
	
	if(eye_pattern[i][j]=='V' && eye_pattern[i][j+1]=='V' && flag[10] == 0)
	{
		
		flag[10] = 1;
		strcpy(rx_info->rx_window[18],get_voltage(eye_pattern,i)); 
	//	printf("=====368=====\n");
	//	printf("=====location_V=%s====\n",rx_info->rx_window[18]);
		
	}
	else if(eye_pattern[i][j]=='O'&&(flag[0]==0||flag[2]==0||flag[4]==0||flag[6]==0))//遇到'O' 
	{
		
		if(flag[0] ==0)
		{
			flag[0] =1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[0],get_voltage(eye_pattern,i)); 
		//	printf("=====O=%s====\n",rx_info->rx_bottom[0]);
		}
		if(flag[2] == 0)
		{
			flag[2] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[2],get_voltage(eye_pattern,i)); 
		//	printf("=====O+Q=%s====\n",rx_info->rx_bottom[2]);
		}
		if(flag[4] == 0)
		{
			flag[4] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[4],get_voltage(eye_pattern,i)); 
		//	printf("=====O+H=%s====\n",rx_info->rx_bottom[4]);
		}
		
		if(flag[6] == 0)
		{
			flag[6] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[6],get_voltage(eye_pattern,i)); 
		//	printf("=====H+Q+O=%s====\n",rx_info->rx_bottom[6]);
		}
		
		
	}
	
	else if(eye_pattern[i][j]=='H'&&(flag[3]==0||flag[4]==0||flag[5]==0||flag[6]==0))//遇到'H'
	{
		
		
		location_rx_H = j-12;
		//sprintf(rx_info->rx_window[19],"%d",j-12);
		itoa(j-12,rx_info->rx_window[19],10);
	//	printf("=====location_H=%s====\n",rx_info->rx_window[19]);
		if(flag[3] == 0)
		{
			flag[3] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[3],get_voltage(eye_pattern,i)); 
		//	printf("=====H=%s====\n",rx_info->rx_bottom[3]);
			
		}
		if(flag[4] == 0)
		{
			flag[4] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[4],get_voltage(eye_pattern,i)); 
		//	printf("=====H+O=%s====\n",rx_info->rx_bottom[4]);
		}
		if(flag[5] == 0)
		{
			flag[5] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[5],get_voltage(eye_pattern,i)); 
		//	printf("=====H+Q=%s====\n",rx_info->rx_bottom[5]);
		}
		if(flag[6] == 0)
		{
			flag[6] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[6],get_voltage(eye_pattern,i)); 
		//	printf("=====H+Q+O=%s====\n",rx_info->rx_bottom[6]);
		}
	}

	else if(eye_pattern[i][j]=='Q'&&(flag[1]==0||flag[2]==0||flag[5]==0||flag[6]==0
	||flag[7]==0||flag[8]==0||flag[9]==0))//遇到'Q' 
	{
		if(flag[1] == 0)
		{
			flag[1] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[1],get_voltage(eye_pattern,i)); 
		//	printf("=====Q=%s====\n",rx_info->rx_bottom[1]);
			
		}
		if(flag[2] == 0)
		{
			flag[2] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[2],get_voltage(eye_pattern,i)); 
		//	printf("=====Q+O=%s====\n",rx_info->rx_bottom[2]);
		}
		if(flag[5] == 0)
		{
			flag[5] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[5],get_voltage(eye_pattern,i)); 
		//	printf("=====H+Q=%s====\n",rx_info->rx_bottom[5]);
		}
		if(flag[6] == 0)
		{
			flag[6] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[6],get_voltage(eye_pattern,i)); 
		//	printf("=====H+Q+O=%s====\n",rx_info->rx_bottom[6]);
		}
		if(eye_pattern[i][j+1]=='H'&&eye_pattern[i][j+2]=='Q'&&flag[7] == 0)  //QHQ
		{
			flag[7] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[7],get_voltage(eye_pattern,i)); 
		//	printf("=====QHQ=%s====\n",rx_info->rx_bottom[7]);
		}
		if(eye_pattern[i][j+1]=='Q'&&eye_pattern[i][j+2]=='H'&&eye_pattern[i][j+3]=='Q'
		&&eye_pattern[i][j+4]=='Q'&&flag[8] == 0)  //QQHQQ
		{
			flag[8] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[8],get_voltage(eye_pattern,i)); 
		//	printf("=====QQHQQ=%s====\n",rx_info->rx_bottom[8]);
		}
		if(eye_pattern[i][j+1]=='Q'&&eye_pattern[i][j+2]=='Q'&&eye_pattern[i][j+3]=='H'&&eye_pattern[i][j+4]=='Q'
		&&eye_pattern[i][j+5]=='Q'&&eye_pattern[i][j+6]=='Q'&&flag[9] ==0)  //QQQHQQQ
		{
			flag[9] = 1;
		//	puts(eye_pattern[i]);
			strcpy(rx_info->rx_bottom[9],get_voltage(eye_pattern,i)); 
		//	printf("=====QQQHQQQ=%s====\n",rx_info->rx_bottom[9]);
		}
	}
}

/*
analyse_info_rx_window 从眼图中解析出rx_window相关指标的参数
@eye_pattern 眼图信息数组
@rx_info_ret 用于保存解析出的相关信息的结构体的指针
*/

void analyse_info_rx_window(char eye_pattern[][100],struct rx_info_extract *rx_info_ret)  //'-'在79的位置 
{
	struct lrt *lrt_rx = (struct lrt *)malloc(sizeof(*lrt_rx));
	int i=0,k,flag_window_all=0;
	int flag_window[6] = {0};
	for(i=0;i<32;i++)
	{
			strcpy(lrt_rx->l," ");
			strcpy(lrt_rx->r," ");
			strcpy(lrt_rx->t," ");
		
		if((int)ctof(eye_pattern,i)==264) //264.00 
		{
			flag_window[0] = 1;
	//		puts(eye_pattern[i]);
			get_lrt(eye_pattern,i,lrt_rx);
			strcpy(rx_info_ret->rx_window[0],lrt_rx->l);
			strcpy(rx_info_ret->rx_window[1],lrt_rx->r);
			strcpy(rx_info_ret->rx_window[2],lrt_rx->t);
	//		printf("l=%s  r=%s  t=%s\n",rx_info_ret->rx_window[0],rx_info_ret->rx_window[1],rx_info_ret->rx_window[2]);
		}
		else if((int)ctof(eye_pattern,i)==206) //206.70 
		{
			flag_window[1] = 1;
	//		puts(eye_pattern[i]);
			get_lrt(eye_pattern,i,lrt_rx);
			strcpy(rx_info_ret->rx_window[3],lrt_rx->l);
			strcpy(rx_info_ret->rx_window[4],lrt_rx->r);
			strcpy(rx_info_ret->rx_window[5],lrt_rx->t);
	//		printf("l=%s  r=%s  t=%s\n",rx_info_ret->rx_window[3],rx_info_ret->rx_window[4],rx_info_ret->rx_window[5]);
		}
		else if((int)ctof(eye_pattern,i)==172) //172.90 
		{
			flag_window[2] = 1;
	//		puts(eye_pattern[i]);
			get_lrt(eye_pattern,i,lrt_rx);
			strcpy(rx_info_ret->rx_window[6],lrt_rx->l);
			strcpy(rx_info_ret->rx_window[7],lrt_rx->r);
			strcpy(rx_info_ret->rx_window[8],lrt_rx->t);
	//		printf("l=%s  r=%s  t=%s\n",rx_info_ret->rx_window[6],rx_info_ret->rx_window[7],rx_info_ret->rx_window[8]);
		}
		else if((int)ctof(eye_pattern,i)==160) //160.60 
		{
			flag_window[3] = 1;
	//		puts(eye_pattern[i]);
			get_lrt(eye_pattern,i,lrt_rx);
			strcpy(rx_info_ret->rx_window[9],lrt_rx->l);
			strcpy(rx_info_ret->rx_window[10],lrt_rx->r);
			strcpy(rx_info_ret->rx_window[11],lrt_rx->t);
	//		printf("l=%s  r=%s  t=%s\n",rx_info_ret->rx_window[9],rx_info_ret->rx_window[10],rx_info_ret->rx_window[11]);
		}
		else if((int)ctof(eye_pattern,i)==123) //123.90 
		{
			flag_window[4] = 1;
	//		puts(eye_pattern[i]);
			get_lrt(eye_pattern,i,lrt_rx);
			strcpy(rx_info_ret->rx_window[12],lrt_rx->l);
			strcpy(rx_info_ret->rx_window[13],lrt_rx->r);
			strcpy(rx_info_ret->rx_window[14],lrt_rx->t);
	//		printf("l=%s  r=%s  t=%s\n",rx_info_ret->rx_window[12],rx_info_ret->rx_window[13],rx_info_ret->rx_window[14]);
		}
		else if((int)ctof(eye_pattern,i)==87) //87.14
		{
			flag_window[5] = 1;
	//		puts(eye_pattern[i]);
			get_lrt(eye_pattern,i,lrt_rx);
			strcpy(rx_info_ret->rx_window[15],lrt_rx->l);
			strcpy(rx_info_ret->rx_window[16],lrt_rx->r);
			strcpy(rx_info_ret->rx_window[17],lrt_rx->t);
	//		printf("l=%s  r=%s  t=%s\n",rx_info_ret->rx_window[15],rx_info_ret->rx_window[16],rx_info_ret->rx_window[17]);
		}
		flag_window_all = 0;
		for(k=0;k<6;k++)
		{
			flag_window_all += flag_window[k];
			if(flag_window_all==6)
			{
				
	//			printf("search all voltage success\n");
				return;
			}
		}
	}
	free(lrt_rx);
	
	
}


/*
analyse_info_tx_window 从眼图中解析出tx_window相关指标的参数
@eye_pattern 眼图信息数组
@tx_info_ret 用于保存解析出的相关信息的结构体的指针
*/

void analyse_info_tx_window(char eye_pattern_tx[][100],struct tx_info_extract *tx_info_ret)  
{
	struct lrt *lrt_tx = (struct lrt *)malloc(sizeof(*lrt_tx));
	int i=0,k,flag_window_all=0;
	int flag_window[6] = {0};
	for(i=0;i<81;i++)
	{
		strcpy(lrt_tx->l," ");
		strcpy(lrt_tx->r," ");
		strcpy(lrt_tx->t," ");
		if(ctof(eye_pattern_tx,i)<=28.1 && ctof(eye_pattern_tx,i)>=27.9) //28.0
		{
			flag_window[0] = 1;
	//		puts(eye_pattern_tx[i]);
			get_lrt(eye_pattern_tx,i,lrt_tx);
			strcpy(tx_info_ret->tx_window[0],lrt_tx->l);
			strcpy(tx_info_ret->tx_window[1],lrt_tx->r);
			strcpy(tx_info_ret->tx_window[2],lrt_tx->t);
	//		printf("l=%s  r=%s  t=%s\n",tx_info_ret->tx_window[0],tx_info_ret->tx_window[1],tx_info_ret->tx_window[2]);
		}
		else if(ctof(eye_pattern_tx,i)<=24.9 && ctof(eye_pattern_tx,i)>=24.7) //24.8
		{
			flag_window[1] = 1;
	//		puts(eye_pattern_tx[i]);
			get_lrt(eye_pattern_tx,i,lrt_tx);
			strcpy(tx_info_ret->tx_window[3],lrt_tx->l);
			strcpy(tx_info_ret->tx_window[4],lrt_tx->r);
			strcpy(tx_info_ret->tx_window[5],lrt_tx->t);
	//		printf("l=%s  r=%s  t=%s\n",tx_info_ret->tx_window[3],tx_info_ret->tx_window[4],tx_info_ret->tx_window[5]);
		}
		else if(ctof(eye_pattern_tx,i)<=21.7 && ctof(eye_pattern_tx,i)>=21.5) //21.6
		{
			flag_window[2] = 1;
	//		puts(eye_pattern_tx[i]);
			get_lrt(eye_pattern_tx,i,lrt_tx);
			strcpy(tx_info_ret->tx_window[6],lrt_tx->l);
			strcpy(tx_info_ret->tx_window[7],lrt_tx->r);
			strcpy(tx_info_ret->tx_window[8],lrt_tx->t);
	//		printf("l=%s  r=%s  t=%s\n",tx_info_ret->tx_window[6],tx_info_ret->tx_window[7],tx_info_ret->tx_window[8]);
		}
		else if(ctof(eye_pattern_tx,i)<=18.5 && ctof(eye_pattern_tx,i)>=18.3) //18.4
		{
			flag_window[3] = 1;
	//		puts(eye_pattern_tx[i]);
			get_lrt(eye_pattern_tx,i,lrt_tx);
			strcpy(tx_info_ret->tx_window[9],lrt_tx->l);
			strcpy(tx_info_ret->tx_window[10],lrt_tx->r);
			strcpy(tx_info_ret->tx_window[11],lrt_tx->t);
	//		printf("l=%s  r=%s  t=%s\n",tx_info_ret->tx_window[9],tx_info_ret->tx_window[10],tx_info_ret->tx_window[11]);
		}
		else if(ctof(eye_pattern_tx,i)<=14.1 && ctof(eye_pattern_tx,i)>=13.9) //14.0
		{
			flag_window[4] = 1;
	//		puts(eye_pattern_tx[i]);
			get_lrt(eye_pattern_tx,i,lrt_tx);
			strcpy(tx_info_ret->tx_window[12],lrt_tx->l);
			strcpy(tx_info_ret->tx_window[13],lrt_tx->r);
			strcpy(tx_info_ret->tx_window[14],lrt_tx->t);
	//		printf("l=%s  r=%s  t=%s\n",tx_info_ret->tx_window[12],tx_info_ret->tx_window[13],tx_info_ret->tx_window[14]);
		}
		else if(ctof(eye_pattern_tx,i)<=10.1 && ctof(eye_pattern_tx,i)>=9.9) //10.0
		{
			flag_window[5] = 1;
	//		puts(eye_pattern_tx[i]);
			get_lrt(eye_pattern_tx,i,lrt_tx);
			strcpy(tx_info_ret->tx_window[15],lrt_tx->l);
			strcpy(tx_info_ret->tx_window[16],lrt_tx->r);
			strcpy(tx_info_ret->tx_window[17],lrt_tx->t);
	//		printf("l=%s  r=%s  t=%s\n",tx_info_ret->tx_window[15],tx_info_ret->tx_window[16],tx_info_ret->tx_window[17]);
		}
		flag_window_all = 0;
		for(k=0;k<6;k++)
		{
			flag_window_all += flag_window[k];
			if(flag_window_all==6)
			{
	//			printf("search all % success\n");
				return;
			}
		}		
	}
	free(lrt_tx);
}

/* int main()
{
	struct rx_info_extract *rx_info_ret =(struct rx_info_extract *) malloc(sizeof(*rx_info_ret));
	struct tx_info_extract *tx_info_ret =(struct tx_info_extract *) malloc(sizeof(*tx_info_ret));
	const char *a="Vref-249.70m|................................OOOQQQHQQQOOOOOO................. -6 9 16";
	int i=0;
	analyse_info_rx_top(eye_pattern,rx_info_ret);
	printf("-----------------------------------\n");
	analyse_info_rx_window(eye_pattern,rx_info_ret);
	
	analyse_info_tx_window( eye_pattern_tx,tx_info_ret);

	analyse_info_tx_top(eye_pattern_tx,tx_info_ret);

	printf("=================\n");
	analyse_info_rx_bottom(eye_pattern,rx_info_ret);
	for(i=0;i<100;i++)
	{
		if(a[i]=='6')
		{
			printf("6=====%d",i);
		}
	}
	printf("\nPress any key to exit...");
    getchar();
} */

