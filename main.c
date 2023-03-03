
#include <p18f4620.h>
#include "main.h"
// Flash Program Memory Code Protection bit (Code protection off)
// Noi khai bao hang so
#define     _XTAL_FREQ 8000000
#define     LED     PORTB
#define     ON      1
#define     OFF     0
#define     ENABLE      1
#define     DISABLE     0

#define     ADDRESS_FIRST_PROGRAM   0x20
#define     ADDRESS_HOUR_ALARM      0x10
#define     ADDRESS_MINUTE_ALARM    0x11
#define     ADDRESS_BIT_ALARM       0x12
#define     ADDRESS_FLAG_ALARM      0x13

#define     INIT_SYSTEM         0
#define     SET_HOUR_ALARM      1
#define     SET_MINUTE_ALARM    2
#define     BIT_ALARM           3
#define     COMPARE             4
#define     ALARM               5

#define     SET_HOUR            6
#define     SET_MINUTE          7
#define     SET_DAY             8
#define     SET_DATE            9
#define     SET_MONTH           10
#define     SET_YEAR            11
#define     VIE                 12
#define     GER                 13
#define     ENG                 14
#define     RUS                 15
#define     USA                 16
#define     AUS                 17
#define     FRA                 18
#define     DISDHT11            19
#define     HIGHTEMP            20
#define     LOWTEMP             21
#define     HIGHRH              22
#define     LOWRH               23
#define     MENU                24
#define     TIMERDIS            25
#define     ALARMDIS            26
#define     ALARMDIS1           39
#define     ALARMDIS2            37
#define     ALARMDIS3           38
#define     MENUALARM           40
#define     MENUALARM2          41
#define     WORLDTIME           27
#define     TEMPE               28
#define     UARTSEND            29
#define     MENU2               30
#define     MENU3               31
#define     MENU4               32
#define     MENU5               33
#define     PUTUPU              34
#define     PUTDOWNU            35
#define     MENUU2              36
#define     UART_RESET_ALL      124
#define     UART_LATE_ALL       122
#define     UART_SET_ALARM      125
#define     UART_RESET_COMPLETE 126
#define     UART_LATE_COMPLETE  127
#define     UART_ALARM_COMPLETE 128

#define     UART_ERROR          58
#define     ONUART              61
#define     OFFUART             62
#define     TEMHUM              70
#define     DAYMON              71
#define     SIM                 72

#define     ENTER_PASSWORD      75
#define     WRONG_PASS          76
#define     TRANDATA            77
#define     CHANGEPASS          78
#define     CHANGEPASS1         79
#define     CHANGEPASS2         80
#define     CHANGEPASS3         81
#define     MENUCHANGE1         82
#define     MENUCHANGE2         83
#define     MENUCHANGE3         84
//khai bao 7SEG
#define DATA_PIN PORTDbits.RD2
#define CLK_PIN PORTDbits.RD5
#define LATCH_PIN PORTDbits.RD4
//khai bao dht11
#define DHT11_Data_Pin   PORTDbits.RD3
#define DHT11_Data_Pin_Direction  TRISDbits.TRISD3
// khai bao bien dht11
unsigned char pass[3][4] = {
    {4,4,4,4},
    {5,5,5,5},
    {6,6,6,6}
};

unsigned char numberValue;


unsigned char arrayMapOfNumber [16] = {1,2,3,'A',4,5,6,'B',
                                       7,8,9,'C','*',0,'E','D'};

unsigned char arrayPassword[4];
unsigned char indexOfNumber = 0;

unsigned char Check_bit, Temp_byte_1, Temp_byte_2, RH_byte_1, RH_byte_2;

unsigned char Himudity, RH, Sumation ;
unsigned char numdh1,numdh2,numdh3,numdh4;
//khai bao ham dht11
void dht11_init();

void find_response();

unsigned char read_dht11();
void display_dht11(void);
// Noi khai bao bien toan cuc
unsigned int LED7_mask[10] = {~0x3F, ~0x06, ~0x5B, ~0x4F, ~0x66, ~0x6D, ~0x7D, ~0x07, ~0x7F, ~0x6F};
unsigned int LED7_select[4] = {0x10, 0x20,0x40,0x80};
unsigned int setSelect=0;
unsigned int statusFSM = 0;
unsigned int statusQ1  = 0;
unsigned int passwordIndex = 0;
//unsigned char LED_OF[] = 
//{// 0   1     2   3     4    5    6    7    8   9     A    B    C    D    E    F    -
//  0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90
//};
//int num1=0,num2=0,num3=0,num4=0;
 //m?ng 4 bit (4 bit led cho module 4 s?)
unsigned char second = 0,minute = 0,hour = 0;
unsigned char day = 0;
unsigned char date = 0,month = 0,year = 0;
unsigned char statusSetUpAlarm = INIT_SYSTEM;
unsigned char statusSetUpTime = INIT_SYSTEM;
unsigned char statusAlarm = INIT_SYSTEM;
unsigned char hourAlarm = 0, minuteAlarm = 0, bitAlarm = 0;
unsigned char timeBlink = 0;
unsigned int timeAlarm = 0;
unsigned char flagAlarm = 0;
unsigned char bitEnable = ENABLE;
unsigned char bitEnableQues1 = ENABLE;
unsigned char bitEnableDHT11 = ENABLE;
unsigned char bitEnableMENU1 = ENABLE;
unsigned int statusSetUpUTC = INIT_SYSTEM;
unsigned int statusSetUpDHT11 = INIT_SYSTEM;
unsigned int hourVIE,hourUSA,hourAUS,hourGER,hourRUS,hourENG,hourFRA;
unsigned int setUTC=0;
unsigned int yuart;
unsigned int remem =1000;
unsigned int hourAlarm2=14,minuteAlarm2=0,bitAlarm2=0;
unsigned int hourAlarm3=5,minuteAlarm3=30,bitAlarm3=0;
unsigned int bitUART =0;
int yMin=10,yMax=50;
int xMin =0, xMax=1023;
int Counter = 10;
unsigned int ReadValueAdc = 0;
unsigned char Menucontrol = INIT_SYSTEM;
// Khai bao cac ham co ban IO

void display_led(unsigned int index1,unsigned int index2);
void write_8bit(unsigned int num);
void select_led(unsigned int num);
//void LED4_Display(void);
//void SEVENSEG_OUT(unsigned char X);
void init_system(void);
void delay_ms(float value);
void delay_ms_7SEG(float value);
void SetupTimeForRealTime();
void SetupForFirstProgram(void);
void ReadDataFromDS1307(void);
void BaiTap_I2C();
void DisplayTime(unsigned int setUTC);
//void DisplayTimeUSA();
//void DisplayTimeRUS();
//void DisplayTimeGER();
//void DisplayTimeAUS();
//void DisplayTimeFRA();
//void DisplayTimeENG();
void SetUpAlarm();
void SetUpAlarm2();
void SetUpAlarm3();
void SetUpTime();
void ModeUTC();
void setMENU();
void setMENUQ1();
unsigned char isButtonMode();
unsigned char isButtonModeUTC();
unsigned char isButtonAlarm();
unsigned char isButtonModeHold();
unsigned char isButtonModeUTCHold();
unsigned char isButtonAlarmHold();
unsigned char isButtonIncrease();
unsigned char isButtonDecrease();
unsigned char isButtonModeTIME();
unsigned char isButtonModeTIMEHold();
void DisplayAlarmTime();
void DisplayAlarm2Time();
void DisplayAlarm3Time();
unsigned char CompareTime();
void Alarm();
void Alarm2();
void Alarm3();
//uart
unsigned char isButtonSetTimeMin();
unsigned char isButtonSetTimeMax();
unsigned char isButtonNumber();
unsigned char isButtonEnter();

unsigned char checkpass(int index);

void SendDataUART(void);
void CalibTime(void);
// upload UART
void UartDataReceiveProcess();
void FSMQUES1();
void setPass(int index);
////////////////////////////////////////////////////////////////////
//Hien thuc cac chuong trinh con, ham, module, function duoi cho nay
////////////////////////////////////////////////////////////////////
void main(void)
{
	

    unsigned int k = 0;   

	init_system();
        lcd_clear();
        LcdClearS();
        SetupForFirstProgram();      
        delay_ms(1000);
        LcdPrintString(0,5,"QUESTION 1");
        ReadDataFromDS1307();
        OSCCONbits.IRCF0=1;
        delay_ms(2000);
	while (1)
	{
            while (!flag_timer3);
            flag_timer3 = 0;
            scan_key_matrix();
//            k = (k+1)%2000;
//            LcdClearS();
//            ReadValueAdc = get_adc_value(0);
//            LcdPrintNumS(0,0,ReadValueAdc);
           
//            if(Read_DS1307(ADDRESS_HOUR)<10)
//            { num1 =0;
//              num2 = Read_DS1307(ADDRESS_HOUR);}
//            else if(Read_DS1307(ADDRESS_HOUR)==10)
//                {   num1=1;
//                    num2 =0;
//                }
//            else if(Read_DS1307(ADDRESS_HOUR)==0)
//            {   num1 =0;
//                num2 =0;
//            }
//            else { 
//              num1 = Read_DS1307(ADDRESS_HOUR)/10;
//              num2 = Read_DS1307(ADDRESS_HOUR)-(num1*10);
//            }
//            if(Read_DS1307(ADDRESS_MINUTE)<10)
//            {num3 =0;
//             num4 = minute;
//            }
//            else if(Read_DS1307(ADDRESS_MINUTE)==10)
//                {   num3=1;
//                    num4=0;
//                }
//            else if(Read_DS1307(ADDRESS_MINUTE)==0)
//            {   num3 =0;
//                num4 =0;
//            }
//            else { 
//                num3 = Read_DS1307(ADDRESS_MINUTE)/10;
//                num4 = Read_DS1307(ADDRESS_MINUTE)-(num3*10);
//            }        
//            display_led(num1,0);
//            display_led(num2,1);
//            display_led(num3,2);
//            display_led(num4,3);
        //   DisplayTime(setUTC);

            FSMQUES1();
            setMENUQ1();

//            Alarm();
//            dht11_init();
//            find_response();
           // DisplayLcdScreenOld();
////             SendDataUART();
//            if(bitUART==1)
//            {SendDataUART();
//            }
            //LED4_Display();
            DisplayLcdScreen();
           
	}
}

//hien thuc 7SEG
void write_8bit(unsigned int num) {
    int i = 0;
    int Data_bit;
    unsigned int Temp = LED7_mask[num];
    CLK_PIN = 0;
    for (i; i < 8; i++) {
        Data_bit = Temp & 0x80;
        Temp = Temp << 1;
        DATA_PIN = (Data_bit == 0x80);
        CLK_PIN = 1;
        CLK_PIN=0;
    }
}

void select_led(unsigned int num) {
    int i = 0;
    int Data_bit;
    unsigned int Temp = LED7_select[num];
    CLK_PIN = 0;
    for (i; i < 8; i++) {
        Data_bit = Temp & 0x01;
        Temp = Temp >> 1;
        DATA_PIN = Data_bit;
        CLK_PIN = 1;
        CLK_PIN = 0;
    }
}

void display_led(unsigned int index1,unsigned int index2) {
    
    // Write Led 1
    LATCH_PIN = 0;
    write_8bit(index1);
    select_led(index2);
    LATCH_PIN=1; 
//    LATCH_PIN = 0;
}
void delay_ms(float value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<40;j++);
}

void delay_ms_7SEG(float value)
{
	int i,j;
	for(i=0;i<value;i++)
		for(j=0;j<1;j++);
}

void init_system(void)
{
        TRISB = 0x00;		//setup PORTB is output
        TRISD = 0x00;
        PORTD = 0x00;
        TRISA=0x00;
        init_lcd();
//        LED = 0x00;
	init_interrupt();
        delay_ms(1000);
        init_timer0(4695);//dinh thoi 1ms sai so 1%
        init_timer1(9390);//dinh thoi 2ms
	init_timer3(46950);//dinh thoi 10ms
	SetTimer0_ms(2);
        SetTimer1_ms(10);
	SetTimer3_ms(50); //Chu ky thuc hien viec xu ly input,proccess,output
        init_key_matrix();
        init_i2c();
        init_key_matrix_with_uart_i2c();
        init_uart();
        init_adc();
}
///////I2C
void SetupTimeForRealTime()
{
    second = 10;
    minute = 54;
    hour = 21;
    day = 6;
    date = 16;
    month = 12;
    year = 22;
    Write_DS1307(ADDRESS_SECOND, second);
    Write_DS1307(ADDRESS_MINUTE, minute);
    Write_DS1307(ADDRESS_HOUR, hour);
    Write_DS1307(ADDRESS_DAY, day);
    Write_DS1307(ADDRESS_DATE, date);
    Write_DS1307(ADDRESS_MONTH, month);
    Write_DS1307(ADDRESS_YEAR, year);
}

void SetupForFirstProgram(void)
{
    if(Read_DS1307(ADDRESS_FIRST_PROGRAM) != 0x22)
    {
        SetupTimeForRealTime();
        Write_DS1307(ADDRESS_HOUR_ALARM, 0);
        Write_DS1307(ADDRESS_MINUTE_ALARM, 0);
        Write_DS1307(ADDRESS_BIT_ALARM, 0);
        Write_DS1307(ADDRESS_FLAG_ALARM, 0);

        Write_DS1307(ADDRESS_FIRST_PROGRAM, 0x22);
    }
}

void ReadDataFromDS1307(void)
{
    
    hourAlarm = Read_DS1307(ADDRESS_HOUR_ALARM);
    minuteAlarm = Read_DS1307(ADDRESS_MINUTE_ALARM);
    bitAlarm = Read_DS1307(ADDRESS_BIT_ALARM);
    flagAlarm = Read_DS1307(ADDRESS_FLAG_ALARM);
}
void BaiTap_I2C()
{
    second = Read_DS1307(ADDRESS_SECOND);
    minute = Read_DS1307(ADDRESS_MINUTE);
    hour = Read_DS1307(ADDRESS_HOUR);
    day = Read_DS1307(ADDRESS_DAY);
    date = Read_DS1307(ADDRESS_DATE);
    month = Read_DS1307(ADDRESS_MONTH);
    year = Read_DS1307(ADDRESS_YEAR);
    
    LcdPrintNumS(0,0,year);
    LcdPrintNumS(0,3,month);
    LcdPrintNumS(0,6,date);
    LcdPrintNumS(0,9,day);
    LcdPrintNumS(1,0,hour);
    LcdPrintNumS(1,3,minute);
    LcdPrintNumS(1,6,second);
}

void DisplayTime(unsigned int setUTC)
{
    LcdClearS();
    if(day>28 && month ==2)
    { day = 1;
      Write_DS1307(ADDRESS_DAY,day);
      month = 3;
      Write_DS1307(ADDRESS_MONTH,month);
    }
    if(day>30 && month == 4)
    { day = 1;
      Write_DS1307(ADDRESS_DAY,day);
      month = 5;
      Write_DS1307(ADDRESS_MONTH,month);
    }
    if(day>30 && month == 6)
    { day = 1;
      Write_DS1307(ADDRESS_DAY,day);
      month = 7;
      Write_DS1307(ADDRESS_MONTH,month);
    }
    if(day>30 && month == 9)
    { day = 1;
      Write_DS1307(ADDRESS_DAY,day);
      month = 10;
      Write_DS1307(ADDRESS_MONTH,month);
    }
    if(day>30 && month == 11)
    { day = 1;
      Write_DS1307(ADDRESS_DAY,day);
      month = 12;
      Write_DS1307(ADDRESS_MONTH,month);
    }
    second = Read_DS1307(ADDRESS_SECOND);
    minute = Read_DS1307(ADDRESS_MINUTE);
    hour = Read_DS1307(ADDRESS_HOUR);
    day = Read_DS1307(ADDRESS_DAY);
    date = Read_DS1307(ADDRESS_DATE);
    month = Read_DS1307(ADDRESS_MONTH);
    year = Read_DS1307(ADDRESS_YEAR);
    //////day
    if(setUTC==0)
    {
    switch(day)
    {
        case 1:
            LcdPrintStringS(0,0,"SUN");
            break;
        case 2:
            LcdPrintStringS(0,0,"MON");
            break;
        case 3:
            LcdPrintStringS(0,0,"TUE");
            break;
        case 4:
            LcdPrintStringS(0,0,"WED");
            break;
        case 5:
            LcdPrintStringS(0,0,"THU");
            break;
        case 6:
            LcdPrintStringS(0,0,"FRI");
            break;
        case 7:
            LcdPrintStringS(0,0,"SAT");
            break;
    }
    switch(month)
    {
        case 1:
            LcdPrintStringS(1,2,"JAN");
            break;
        case 2:
            LcdPrintStringS(1,2,"FEB");
            break;
        case 3:
            LcdPrintStringS(1,2,"MAR");
            break;
        case 4:
            LcdPrintStringS(1,2,"APR");
            break;
        case 5:
            LcdPrintStringS(1,2,"MAY");
            break;
        case 6:
            LcdPrintStringS(1,2,"JUN");
            break;
        case 7:
            LcdPrintStringS(1,2,"JUL");
            break;
        case 8:
            LcdPrintStringS(1,2,"AUG");
            break;
        case 9:
            LcdPrintStringS(1,2,"SEP");
            break;
        case 10:
            LcdPrintStringS(1,2,"OCT");
            break;
        case 11:
            LcdPrintStringS(1,2,"NOV");
            break;
        case 12:
            LcdPrintStringS(1,2,"DEC");
            break;
    }
    }
    if(setUTC==1)
    {switch(day)
    {
        case 1:
            LcdPrintStringS(0,0,"SON");
            break;
        case 2:
            LcdPrintStringS(0,0,"MON");
            break;
        case 3:
            LcdPrintStringS(0,0,"DIE");
            break;
        case 4:
            LcdPrintStringS(0,0,"MIT");
            break;
        case 5:
            LcdPrintStringS(0,0,"DON");
            break;
        case 6:
            LcdPrintStringS(0,0,"FRE");
            break;
        case 7:
            LcdPrintStringS(0,0,"SAM");
            break;
    }
    switch(month)
    {
        case 1:
            LcdPrintStringS(1,2,"JAN");
            break;
        case 2:
            LcdPrintStringS(1,2,"FEB");
            break;
        case 3:
            LcdPrintStringS(1,2,"MAR");
            break;
        case 4:
            LcdPrintStringS(1,2,"APR");
            break;
        case 5:
            LcdPrintStringS(1,2,"KAN");
            break;
        case 6:
            LcdPrintStringS(1,2,"JUN");
            break;
        case 7:
            LcdPrintStringS(1,2,"JUL");
            break;
        case 8:
            LcdPrintStringS(1,2,"AUG");
            break;
        case 9:
            LcdPrintStringS(1,2,"SEP");
            break;
        case 10:
            LcdPrintStringS(1,2,"OKT");
            break;
        case 11:
            LcdPrintStringS(1,2,"NOV");
            break;
        case 12:
            LcdPrintStringS(1,2,"DEZ");
            break;
    }
    }
    if(setUTC==2)
    {
        switch(day)
    {
        case 1:
            LcdPrintStringS(0,0,"SUN");
            break;
        case 2:
            LcdPrintStringS(0,0,"MON");
            break;
        case 3:
            LcdPrintStringS(0,0,"TUE");
            break;
        case 4:
            LcdPrintStringS(0,0,"WED");
            break;
        case 5:
            LcdPrintStringS(0,0,"THU");
            break;
        case 6:
            LcdPrintStringS(0,0,"FRI");
            break;
        case 7:
            LcdPrintStringS(0,0,"SAT");
            break;
    }
    switch(month)
    {
        case 1:
            LcdPrintStringS(1,2,"JAN");
            break;
        case 2:
            LcdPrintStringS(1,2,"FEB");
            break;
        case 3:
            LcdPrintStringS(1,2,"MAR");
            break;
        case 4:
            LcdPrintStringS(1,2,"APR");
            break;
        case 5:
            LcdPrintStringS(1,2,"MAY");
            break;
        case 6:
            LcdPrintStringS(1,2,"JUN");
            break;
        case 7:
            LcdPrintStringS(1,2,"JUL");
            break;
        case 8:
            LcdPrintStringS(1,2,"AUG");
            break;
        case 9:
            LcdPrintStringS(1,2,"SEP");
            break;
        case 10:
            LcdPrintStringS(1,2,"OCT");
            break;
        case 11:
            LcdPrintStringS(1,2,"NOV");
            break;
        case 12:
            LcdPrintStringS(1,2,"DEC");
            break;
    }
    }
    if(setUTC==3)
    {
         switch(day)
    {
        case 1:
            LcdPrintStringS(0,0,"BOC");
            break;
        case 2:
            LcdPrintStringS(0,0,"NOH");
            break;
        case 3:
            LcdPrintStringS(0,0,"BTO");
            break;
        case 4:
            LcdPrintStringS(0,0,"CPE");
            break;
        case 5:
            LcdPrintStringS(0,0,"YET");
            break;
        case 6:
            LcdPrintStringS(0,0,"NTH");
            break;
        case 7:
            LcdPrintStringS(0,0,"CY6");
            break;
    }
         switch(month)
    {
        case 1:
            LcdPrintStringS(1,2,"HBA");
            break;
        case 2:
            LcdPrintStringS(1,2,"EPB");
            break;
        case 3:
            LcdPrintStringS(1,2,"MAP");
            break;
        case 4:
            LcdPrintStringS(1,2,"ANP");
            break;
        case 5:
            LcdPrintStringS(1,2,"MAN");
            break;
        case 6:
            LcdPrintStringS(1,2,"NHO");
            break;
        case 7:
            LcdPrintStringS(1,2,"NHO");
            break;
        case 8:
            LcdPrintStringS(1,2,"ABR");
            break;
        case 9:
            LcdPrintStringS(1,2,"CEH");
            break;
        case 10:
            LcdPrintStringS(1,2,"OKT");
            break;
        case 11:
            LcdPrintStringS(1,2,"HOR");
            break;
        case 12:
            LcdPrintStringS(1,2,"EKA");
            break;
    }
    }
    if(setUTC==4)
    {
        switch(day)
    {
        case 1:
            LcdPrintStringS(0,0,"SUN");
            break;
        case 2:
            LcdPrintStringS(0,0,"MON");
            break;
        case 3:
            LcdPrintStringS(0,0,"TUE");
            break;
        case 4:
            LcdPrintStringS(0,0,"WED");
            break;
        case 5:
            LcdPrintStringS(0,0,"THU");
            break;
        case 6:
            LcdPrintStringS(0,0,"FRI");
            break;
        case 7:
            LcdPrintStringS(0,0,"SAT");
            break;
    }
    switch(month)
    {
        case 1:
            LcdPrintStringS(1,2,"JAN");
            break;
        case 2:
            LcdPrintStringS(1,2,"FEB");
            break;
        case 3:
            LcdPrintStringS(1,2,"MAR");
            break;
        case 4:
            LcdPrintStringS(1,2,"APR");
            break;
        case 5:
            LcdPrintStringS(1,2,"MAY");
            break;
        case 6:
            LcdPrintStringS(1,2,"JUN");
            break;
        case 7:
            LcdPrintStringS(1,2,"JUL");
            break;
        case 8:
            LcdPrintStringS(1,2,"AUG");
            break;
        case 9:
            LcdPrintStringS(1,2,"SEP");
            break;
        case 10:
            LcdPrintStringS(1,2,"OCT");
            break;
        case 11:
            LcdPrintStringS(1,2,"NOV");
            break;
        case 12:
            LcdPrintStringS(1,2,"DEC");
            break;
    }
    }
    if(setUTC==5)
    {
        switch(day)
    {
        case 1:
            LcdPrintStringS(0,0,"SUN");
            break;
        case 2:
            LcdPrintStringS(0,0,"MON");
            break;
        case 3:
            LcdPrintStringS(0,0,"TUE");
            break;
        case 4:
            LcdPrintStringS(0,0,"WED");
            break;
        case 5:
            LcdPrintStringS(0,0,"THU");
            break;
        case 6:
            LcdPrintStringS(0,0,"FRI");
            break;
        case 7:
            LcdPrintStringS(0,0,"SAT");
            break;
    }
    switch(month)
    {
        case 1:
            LcdPrintStringS(1,2,"JAN");
            break;
        case 2:
            LcdPrintStringS(1,2,"FEB");
            break;
        case 3:
            LcdPrintStringS(1,2,"MAR");
            break;
        case 4:
            LcdPrintStringS(1,2,"APR");
            break;
        case 5:
            LcdPrintStringS(1,2,"MAY");
            break;
        case 6:
            LcdPrintStringS(1,2,"JUN");
            break;
        case 7:
            LcdPrintStringS(1,2,"JUL");
            break;
        case 8:
            LcdPrintStringS(1,2,"AUG");
            break;
        case 9:
            LcdPrintStringS(1,2,"SEP");
            break;
        case 10:
            LcdPrintStringS(1,2,"OCT");
            break;
        case 11:
            LcdPrintStringS(1,2,"NOV");
            break;
        case 12:
            LcdPrintStringS(1,2,"DEC");
            break;
    }
    }
    if(setUTC==6)
    {
        switch(day)
    {
        case 1:
            LcdPrintStringS(0,0,"DIM");
            break;
        case 2:
            LcdPrintStringS(0,0,"LUN");
            break;
        case 3:
            LcdPrintStringS(0,0,"MAR");
            break;
        case 4:
            LcdPrintStringS(0,0,"MEC");
            break;
        case 5:
            LcdPrintStringS(0,0,"JEU");
            break;
        case 6:
            LcdPrintStringS(0,0,"VEN");
            break;
        case 7:
            LcdPrintStringS(0,0,"SAM");
            break;
    }
        switch(month)
    {
        case 1:
            LcdPrintStringS(1,2,"JAN");
            break;
        case 2:
            LcdPrintStringS(1,2,"FEV");
            break;
        case 3:
            LcdPrintStringS(1,2,"MAR");
            break;
        case 4:
            LcdPrintStringS(1,2,"AVR");
            break;
        case 5:
            LcdPrintStringS(1,2,"PEU");
            break;
        case 6:
            LcdPrintStringS(1,2,"JUI");
            break;
        case 7:
            LcdPrintStringS(1,2,"JUI");
            break;
        case 8:
            LcdPrintStringS(1,2,"AOU");
            break;
        case 9:
            LcdPrintStringS(1,2,"SEP");
            break;
        case 10:
            LcdPrintStringS(1,2,"OCT");
            break;
        case 11:
            LcdPrintStringS(1,2,"NOV");
            break;
        case 12:
            LcdPrintStringS(1,2,"DEC");
            break;
    }
    }
    if(hour>23)
    { hour =0;
    }
    if(hour < 10)
    {
        LcdPrintStringS(0,4,"0");
        LcdPrintNumS(0,5,hour);
    }
    else
        LcdPrintNumS(0,4,hour);
    
    LcdPrintStringS(0,6,":");
    if(minute < 10)
    {
        LcdPrintStringS(0,7,"0");
        LcdPrintNumS(0,8,minute);
    }
    else
        LcdPrintNumS(0,7,minute);

    LcdPrintStringS(0,9,":");
    if(second < 10)
    {
        LcdPrintStringS(0,10,"0");
        LcdPrintNumS(0,11,second);
    }
    else
        LcdPrintNumS(0,10,second);

    switch(bitAlarm)
    {
        case 0:
            LcdPrintStringS(0,13," ");
            break;
        case 1:
            LcdPrintStringS(0,13,"O");
            break;
    }
    switch(bitAlarm2)
    {
        case 0:
            LcdPrintStringS(0,14," ");
            break;
        case 1:
            LcdPrintStringS(0,14,"O");
            break;
    }
    switch(bitAlarm3)
    {
        case 0:
            LcdPrintStringS(0,15," ");
            break;
        case 1:
            LcdPrintStringS(0,15,"O");
            break;
    }
    switch(bitUART)
    {
       case 0:
        LcdPrintStringS(1,14,"OF");
        break;
        case 1:
        LcdPrintStringS(1,14,"ON");
        break;  
       }

    LcdPrintStringS(1,5," ");
    if(date < 10)
    {
        LcdPrintStringS(1,6," ");
        LcdPrintNumS(1,7,date);
    }
    else
        LcdPrintNumS(1,6,date);
    LcdPrintStringS(1,8," ");
    LcdPrintNumS(1,9,20);
    LcdPrintNumS(1,11,year);

}

void DisplayAlarmTime()
{
    LcdPrintStringS(0,0,"      ALARM     ");
    LcdPrintStringS(1,0,"  ");
    if(hourAlarm < 10)
    {
        LcdPrintStringS(1,2,"0");
        LcdPrintNumS(1,3,hourAlarm);
    }
    else
        LcdPrintNumS(1,2,hourAlarm);
    LcdPrintStringS(1,4,":");
    if(minuteAlarm < 10)
    {
        LcdPrintStringS(1,5,"0");
        LcdPrintNumS(1,6,minuteAlarm);
    }
    else
        LcdPrintNumS(1,5,minuteAlarm);
    LcdPrintStringS(1,7,"   ");
    switch(bitAlarm)
    {
        case 0:
            LcdPrintStringS(1,10,"OFF");
            break;
        case 1:
            LcdPrintStringS(1,10,"ON ");
            break;
    }
    LcdPrintStringS(1,13,"    ");
    
}
void DisplayAlarm2Time()
{
    LcdPrintStringS(0,0,"      ALARM 2   ");
    LcdPrintStringS(1,0,"  ");
    if(hourAlarm2 < 10)
    {
        LcdPrintStringS(1,2,"0");
        LcdPrintNumS(1,3,hourAlarm2);
    }
    else
        LcdPrintNumS(1,2,hourAlarm2);
    LcdPrintStringS(1,4,":");
    if(minuteAlarm2 < 10)
    {
        LcdPrintStringS(1,5,"0");
        LcdPrintNumS(1,6,minuteAlarm2);
    }
    else
        LcdPrintNumS(1,5,minuteAlarm2);
    LcdPrintStringS(1,7,"   ");
    switch(bitAlarm2)
    {
        case 0:
            LcdPrintStringS(1,10,"OFF");
            break;
        case 1:
            LcdPrintStringS(1,10,"ON ");
            break;
    }
    LcdPrintStringS(1,13,"    ");
    
}
void DisplayAlarm3Time()
{
    LcdPrintStringS(0,0,"      ALARM 3   ");
    LcdPrintStringS(1,0,"  ");
    if(hourAlarm3 < 10)
    {
        LcdPrintStringS(1,2,"0");
        LcdPrintNumS(1,3,hourAlarm3);
    }
    else
        LcdPrintNumS(1,2,hourAlarm3);
    LcdPrintStringS(1,4,":");
    if(minuteAlarm2 < 10)
    {
        LcdPrintStringS(1,5,"0");
        LcdPrintNumS(1,6,minuteAlarm3);
    }
    else
        LcdPrintNumS(1,5,minuteAlarm3);
    LcdPrintStringS(1,7,"   ");
    switch(bitAlarm3)
    {
        case 0:
            LcdPrintStringS(1,10,"OFF");
            break;
        case 1:
            LcdPrintStringS(1,10,"ON ");
            break;
    }
    LcdPrintStringS(1,13,"    ");
    
}
void SetUpTime()
{   DisplayTime(setUTC);
    switch(statusSetUpTime)
    {
        case INIT_SYSTEM:
            if(isButtonModeTIME() && (bitEnable == ENABLE))
                statusSetUpTime = SET_HOUR;
            break;
        case SET_HOUR:
            bitEnable = DISABLE;
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 30)
                LcdPrintStringS(0,4,"  ");
            if(isButtonIncrease())
            {
                hour = (hour + 1)%24;
                Write_DS1307(ADDRESS_HOUR,hour);
            }
            if(isButtonDecrease())
            {
                hour = (hour - 1);
                if(hour > 23)
                    hour = 23;
                Write_DS1307(ADDRESS_HOUR,hour);
            }
            if(isButtonModeTIME())
                statusSetUpTime = SET_MINUTE;
            if(isButtonModeTIMEHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpTime = INIT_SYSTEM;
            }
            break;
        case SET_MINUTE:
            bitEnable = DISABLE;
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(0,7,"  ");
            if(isButtonIncrease())
            {
                minute = (minute + 1)%60;
                Write_DS1307(ADDRESS_MINUTE,minute);
            }
            if(isButtonDecrease())
            {
                minute = (minute - 1);
                if(minute > 59)
                    minute = 59;
                Write_DS1307(ADDRESS_MINUTE,minute);
            }
            if(isButtonModeTIME())
                statusSetUpTime = SET_DAY;
            if(isButtonModeTIMEHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpTime = INIT_SYSTEM;
            }
            break;
        case SET_DAY:
            bitEnable = DISABLE;
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(0,0,"   ");
            if(isButtonIncrease())
            {
                day = day + 1;
                if(day > 7)
                    day = 1;
                Write_DS1307(ADDRESS_DAY,day);
            }
            if(isButtonDecrease())
            {
                day = day - 1;
                if(day > 7)
                    day = 7;
                Write_DS1307(ADDRESS_DAY,day);
            }
            if(isButtonModeTIME())
                statusSetUpTime = SET_DATE;
            if(isButtonModeTIMEHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpTime = INIT_SYSTEM;
            }
            break;
        case SET_DATE:
            bitEnable = DISABLE;
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(1,6,"  ");
            if(isButtonIncrease())
            {
                date = date + 1;
                if(date > 31)
                    date = 1;
                Write_DS1307(ADDRESS_DATE,date);
            }
            if(isButtonDecrease())
            {
                date = date - 1;
                if(date < 1)
                    date = 31;
                Write_DS1307(ADDRESS_DATE,date);
            }
            if(isButtonModeTIME())
                statusSetUpTime = SET_MONTH;
            if(isButtonModeTIMEHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpTime = INIT_SYSTEM;
            }
            break;
        case SET_MONTH:
            bitEnable = DISABLE;
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(1,2,"   ");
            if(isButtonIncrease())
            {
                month = month + 1;
                if(month > 12)
                    month = 1;
                Write_DS1307(ADDRESS_MONTH,month);
            }
            if(isButtonDecrease())
            {
                month = month - 1;
                if(month < 1)
                    month = 12;
                Write_DS1307(ADDRESS_MONTH,month);
            }
            if(isButtonModeTIME())
                statusSetUpTime = SET_YEAR;
            if(isButtonModeTIMEHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpTime = INIT_SYSTEM;
            }
            break;
        case SET_YEAR:
            bitEnable = DISABLE;
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(1,9,"    ");
            if(isButtonIncrease())
            {
                year = year + 1;
                if(year > 99)
                    year = 0;
                Write_DS1307(ADDRESS_YEAR,year);
            }
            if(isButtonDecrease())
            {
                year = year - 1;
                if(year > 99)
                    month = 99;
                Write_DS1307(ADDRESS_YEAR,year);
            }
            if(isButtonModeTIME())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpTime = INIT_SYSTEM;
            }
            if(isButtonModeTIMEHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpTime = INIT_SYSTEM;
            }
            break;
        default:
            bitEnable = ENABLE;
            statusSetUpAlarm = INIT_SYSTEM;
            break;

    }
}

void SetUpAlarm()
{
    switch(statusSetUpAlarm)
    {
        case INIT_SYSTEM:
            if(isButtonAlarm() && (bitEnable == ENABLE))
                statusSetUpAlarm = SET_HOUR_ALARM;
            break;
        case SET_HOUR_ALARM:
            bitEnable = DISABLE;
            DisplayAlarmTime();
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(1,2,"  ");
            if(isButtonIncrease())
            {
                hourAlarm = (hourAlarm + 1)%24;
                Write_DS1307(ADDRESS_HOUR_ALARM,hourAlarm);
            }
            if(isButtonDecrease())
            {
                hourAlarm = (hourAlarm - 1);
                if(hourAlarm > 23)
                    hourAlarm = 23;
                Write_DS1307(ADDRESS_HOUR_ALARM,hourAlarm);
            }
            if(isButtonAlarm())
                statusSetUpAlarm = SET_MINUTE_ALARM;
            if(isButtonAlarmHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpAlarm = INIT_SYSTEM;
            }
            break;
        case SET_MINUTE_ALARM:
            bitEnable = DISABLE;
            DisplayAlarmTime();
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(1,5,"  ");
            if(isButtonIncrease())
            {
                minuteAlarm = (minuteAlarm + 1)%60;
                Write_DS1307(ADDRESS_MINUTE_ALARM,minuteAlarm);
            }
            if(isButtonDecrease())
            {
                minuteAlarm = (minuteAlarm - 1);
                if(minuteAlarm > 59)
                    minuteAlarm = 59;
                Write_DS1307(ADDRESS_MINUTE_ALARM,minuteAlarm);
            }
            if(isButtonAlarm())
                statusSetUpAlarm = BIT_ALARM;
            if(isButtonAlarmHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpAlarm = INIT_SYSTEM;
            }
            break;
        case BIT_ALARM:
            bitEnable = DISABLE;
            DisplayAlarmTime();
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 30)
                LcdPrintStringS(1,10,"   ");
            if(isButtonIncrease())
            {
                bitAlarm = (bitAlarm + 1)%2;
                Write_DS1307(ADDRESS_BIT_ALARM,bitAlarm);
            }
            if(isButtonDecrease())
            {
                bitAlarm = (bitAlarm - 1);
                if(bitAlarm > 1)
                    bitAlarm = 1;
                Write_DS1307(ADDRESS_BIT_ALARM,bitAlarm);
            }
            if(isButtonAlarm())
            {
                LcdClearS();
                statusSetUpAlarm = INIT_SYSTEM;
                bitEnable = ENABLE;
            }
            if(isButtonAlarmHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpAlarm = INIT_SYSTEM;
            }
            break;
        default:
            break;
    }
}
void SetUpAlarm2()
{
    switch(statusSetUpAlarm)
    {
        case INIT_SYSTEM:
            if(isButtonAlarm() && (bitEnable == ENABLE))
                statusSetUpAlarm = SET_HOUR_ALARM;
            break;
        case SET_HOUR_ALARM:
            bitEnable = DISABLE;
            DisplayAlarm2Time();
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(1,2,"  ");
            if(isButtonIncrease())
            {
                hourAlarm2 = (hourAlarm2 + 1)%24;
            }
            if(isButtonDecrease())
            {
                hourAlarm2 = (hourAlarm2 - 1);
                if(hourAlarm2 > 23)
                    hourAlarm2 = 23;
            }
            if(isButtonAlarm())
                statusSetUpAlarm = SET_MINUTE_ALARM;
            if(isButtonAlarmHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpAlarm = INIT_SYSTEM;
            }
            break;
        case SET_MINUTE_ALARM:
            bitEnable = DISABLE;
            DisplayAlarm2Time();
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(1,5,"  ");
            if(isButtonIncrease())
            {
                minuteAlarm2 = (minuteAlarm2 + 1)%60;
            }
            if(isButtonDecrease())
            {
                minuteAlarm2 = (minuteAlarm2 - 1);
                if(minuteAlarm2 > 59)
                    minuteAlarm2 = 59;
            }
            if(isButtonAlarm())
                statusSetUpAlarm = BIT_ALARM;
            if(isButtonAlarmHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpAlarm = INIT_SYSTEM;
            }
            break;
        case BIT_ALARM:
            bitEnable = DISABLE;
            DisplayAlarm2Time();
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 30)
                LcdPrintStringS(1,10,"   ");
            if(isButtonIncrease())
            {
                bitAlarm2 = (bitAlarm2 + 1)%2;
            }
            if(isButtonDecrease())
            {
                bitAlarm2 = (bitAlarm2 - 1);
                if(bitAlarm2 > 1)
                    bitAlarm2 = 1;
            }
            if(isButtonAlarm())
            {
                LcdClearS();
                statusSetUpAlarm = INIT_SYSTEM;
                bitEnable = ENABLE;
            }
            if(isButtonAlarmHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpAlarm = INIT_SYSTEM;
            }
            break;
        default:
            break;
    }
}
void SetUpAlarm3()
{
    switch(statusSetUpAlarm)
    {
        case INIT_SYSTEM:
            if(isButtonAlarm() && (bitEnable == ENABLE))
                statusSetUpAlarm = SET_HOUR_ALARM;
            break;
        case SET_HOUR_ALARM:
            bitEnable = DISABLE;
            DisplayAlarm3Time();
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(1,2,"  ");
            if(isButtonIncrease())
            {
                hourAlarm3 = (hourAlarm3 + 1)%24;
            }
            if(isButtonDecrease())
            {
                hourAlarm3 = (hourAlarm3 - 1);
                if(hourAlarm3 > 23)
                    hourAlarm3 = 23;
            }
            if(isButtonAlarm())
                statusSetUpAlarm = SET_MINUTE_ALARM;
            if(isButtonAlarmHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpAlarm = INIT_SYSTEM;
            }
            break;
        case SET_MINUTE_ALARM:
            bitEnable = DISABLE;
            DisplayAlarm3Time();
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 15)
                LcdPrintStringS(1,5,"  ");
            if(isButtonIncrease())
            {
                minuteAlarm3 = (minuteAlarm3 + 1)%60;
            }
            if(isButtonDecrease())
            {
                minuteAlarm3 = (minuteAlarm3 - 1);
                if(minuteAlarm3 > 59)
                    minuteAlarm3 = 59;
            }
            if(isButtonAlarm())
                statusSetUpAlarm = BIT_ALARM;
            if(isButtonAlarmHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpAlarm = INIT_SYSTEM;
            }
            break;
        case BIT_ALARM:
            bitEnable = DISABLE;
            DisplayAlarm3Time();
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink > 30)
                LcdPrintStringS(1,10,"   ");
            if(isButtonIncrease())
            {
                bitAlarm3 = (bitAlarm3 + 1)%2;
            }
            if(isButtonDecrease())
            {
                bitAlarm3 = (bitAlarm3 - 1);
                if(bitAlarm3 > 1)
                    bitAlarm3 = 1;
            }
            if(isButtonAlarm())
            {
                LcdClearS();
                statusSetUpAlarm = INIT_SYSTEM;
                bitEnable = ENABLE;
            }
            if(isButtonAlarmHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpAlarm = INIT_SYSTEM;
            }
            break;
        default:
            break;
    }
}
unsigned char isButtonMode()
{
    if (key_code[4] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonModeHold()
{
    if (key_code[4] == 10)
        return 1;
    else
        return 0;
}
unsigned char isButtonModeTIME()
{
    if (key_code[4] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonModeTIMEHold()
{
    if (key_code[4] == 10)
        return 1;
    else
        return 0;
}
unsigned char isButtonAlarm()
{
    if (key_code[4] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonAlarmHold()
{
    if (key_code[4] == 10)
        return 1;
    else
        return 0;
}

unsigned char isButtonIncrease()
{
    if (key_code[5] == 1 || (key_code[5] > 10 && key_code[5]%2 == 1))
        return 1;
    else
        return 0;
}

unsigned char isButtonDecrease()
{
    if (key_code[6] == 1 || (key_code[6] > 10 && key_code[6]%2 == 1))
        return 1;
    else
        return 0;
}
unsigned char isButtonModeUTC()
{
    if (key_code[4] == 1)
        return 1;
    else
        return 0;
}
unsigned char isButtonModeUTCHold()
{
    if (key_code[4] == 10)
        return 1;
    else
        return 0;
}
unsigned char CompareTime()
{
    if((hour == hourAlarm) && (minute == minuteAlarm) && (second == 0) && (bitAlarm == ON) && (bitEnable == ENABLE))
        return 1;
    else
        return 0;
}

void setMENU()
{
    switch(Menucontrol)
    {   case INIT_SYSTEM:
        if(isButtonMode()==1 && bitEnable==ENABLE)
            Menucontrol = MENU;
            break;
        case MENU:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,3,"MENU CLOCK");
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(1,4,"1.SET TIMER");
            if(isButtonMode())
            { Menucontrol = TIMERDIS;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = MENU5;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = MENU2;
                break;
            }      
            break;
        case MENU2:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,0,">");
            LcdPrintStringS(0,4,"2.ALARM");
            LcdPrintStringS(1,4,"3.WORLD TIME");
            if(isButtonMode())
            { Menucontrol = ALARMDIS;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = MENU;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = MENU3;
                break;
            }      
            break;
        case MENU3:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,4,"2.ALARM");
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(1,4,"3.WORLD TIME");
            if(isButtonMode())
            { Menucontrol = WORLDTIME;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = MENU2;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = MENU4;
                break;
            }      
            break;
        case MENU4:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,0,">");
            LcdPrintStringS(0,4,"4.TEMP&HUMID");           
            LcdPrintStringS(1,4,"5.UART");
            if(isButtonMode())
            { Menucontrol = TEMPE;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = MENU3;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = MENU5;
                break;
            }      
            break;
        case MENU5:
            LcdClearS();
            bitEnable = DISABLE;
            
            LcdPrintStringS(0,4,"4.TEMP&HUMID");  
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(1,4,"5.UART");
            if(isButtonMode())
            { Menucontrol = UARTSEND;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = MENU4;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = MENU;
                break;
            }      
            break;
            
        case TIMERDIS:
            LcdClearS();
            bitEnable = ENABLE;
            SetUpTime();
            if(isButtonModeHold())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            break;
            }
            break;
        case ALARMDIS:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,5,"SET ALARM");
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(1,4,"1.ALARM 1");
            
            if(isButtonMode())
            { Menucontrol = ALARMDIS1;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = MENUALARM2;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = MENUALARM;
                break;
            }      
            break;
            case MENUALARM:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,4,"2.ALARM 2");
            LcdPrintStringS(0,0,">");
            LcdPrintStringS(1,4,"3.ALARM 3");
            
            if(isButtonMode())
            { Menucontrol = ALARMDIS2;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = ALARMDIS;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = MENUALARM2;
                break;
            }      
            break;
            case MENUALARM2:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,4,"2.ALARM 2");
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(1,4,"3.ALARM 3");
            
            if(isButtonMode())
            { Menucontrol = ALARMDIS3;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = MENUALARM;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = ALARMDIS;
                break;
            }      
            break;
        case ALARMDIS1:
            LcdClearS();
            bitEnable = ENABLE;
            SetUpAlarm();
            if(isButtonModeHold())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            break;
            }
            break;
        case ALARMDIS2:
            LcdClearS();
            bitEnable = ENABLE;
            SetUpAlarm2();
            if(isButtonModeHold())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            break;
            }
            break;
        case ALARMDIS3:
            LcdClearS();
            bitEnable = ENABLE;
            SetUpAlarm3();
            if(isButtonModeHold())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            break;
            }
            break;
        case WORLDTIME:
            LcdClearS();
            bitEnable = ENABLE;
            ModeUTC();
            if(isButtonModeHold())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            break;
            }
            break;
        case TEMPE:
            LcdClearS();
            bitEnable = ENABLE;
            display_dht11();
            if(isButtonModeHold())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            break;
            }
            break;
        case UARTSEND:
            
            LcdClearS();
            bitEnable = DISABLE;
            
            LcdPrintStringS(0,4,"1.PUT UP ");  
            LcdPrintStringS(0,0,">");
            LcdPrintStringS(1,4,"2.PUT DOWN ");
            if(isButtonMode())
            { Menucontrol = PUTUPU;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = MENUU2;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = MENUU2;
                break;
            }      
            break;
        case MENUU2:
            LcdClearS();
            bitEnable = DISABLE;
            
            LcdPrintStringS(0,4,"1.PUT UP ");  
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(1,4,"2.PUT DOWN ");
            if(isButtonMode())
            { Menucontrol = PUTDOWNU;
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            Menucontrol = INIT_SYSTEM;
            bitEnable = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   Menucontrol = UARTSEND;
                break;
            }
            if(isButtonDecrease())
            {   Menucontrol = UARTSEND;
                break;
            }      
            break;
        case PUTUPU:
             LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,4,"1.ON");
            LcdPrintStringS(0,1,">");
            LcdPrintStringS(1,4,"2.OFF");
            if(isButtonModeHold())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            break;
            }
            if(isButtonMode())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            bitUART=1;
            break;
            }
            if(isButtonIncrease())
            { Menucontrol= OFFUART;
                    break;
            }
            if(isButtonDecrease())
            { Menucontrol= OFFUART;
                    break;
            }
            break;
            case OFFUART:
             LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,4,"1.ON");
            LcdPrintStringS(1,1,">");
            LcdPrintStringS(1,4,"2.OFF");
            if(isButtonModeHold())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            break;
            }
            if(isButtonMode())
            {   LcdClearS();
            bitEnable = ENABLE;
            Menucontrol=INIT_SYSTEM;
            bitUART=0;
            break;
            }
            if(isButtonIncrease())
            { Menucontrol= PUTUPU;
                    break;
            }
            if(isButtonDecrease())
            { Menucontrol= PUTUPU;
                    break;
            }
            break;
        case PUTDOWNU:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,5,"WAITING");
             uart_isr_simulate_machine();
            UartDataReceiveProcess();
            break;
        default:
            LcdClearS();
            Menucontrol=INIT_SYSTEM;
            bitEnable=ENABLE;
            break;
    }
}
void Alarm()
{
    static unsigned char timeBlink = 0;
    switch(statusAlarm)
    {
        case INIT_SYSTEM:
            statusAlarm = COMPARE;
            break;
        case COMPARE:
            if(CompareTime())
            {
                flagAlarm = 1;
                Write_DS1307(ADDRESS_FLAG_ALARM,flagAlarm);
            }
            if(flagAlarm)
                statusAlarm = ALARM;
            break;
        case ALARM:
            /// bao chuong
            bitEnable = DISABLE;
            timeBlink = (timeBlink + 1)%20;
            if(timeBlink < 10)
            {
                LcdClearS();
            }
            timeAlarm ++;
            if(timeAlarm > 200)
            {
                bitEnable = ENABLE;
                flagAlarm = 0;
                Write_DS1307(ADDRESS_FLAG_ALARM,flagAlarm);
                timeAlarm = 0;
                LcdClearS();
                statusAlarm = INIT_SYSTEM;
            }
            if(isButtonModeHold())
            {LcdClearS();
            statusAlarm = INIT_SYSTEM;
            bitEnable = ENABLE;
            }
            break;
    }
}
void ModeUTC()
{ 
    switch(statusSetUpUTC)
    {   
        case INIT_SYSTEM:
            if(isButtonModeUTC() && (bitEnable == ENABLE))
                statusSetUpUTC = VIE;
            break;
        case VIE:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,0,"VIE");
            LcdPrintStringS(0,4," ");
            LcdPrintStringS(0,6,"HANOI");
            LcdPrintStringS(1,4,"TIME=UTC+7");
            if(isButtonIncrease())
            {
                statusSetUpUTC = GER;
                break;
            }
            if(isButtonDecrease())
            {
                statusSetUpUTC = FRA;
                break;
            }
            if(isButtonModeUTC())
            {   LcdClearS();
                hourVIE = hour;
                Write_DS1307(ADDRESS_HOUR,hour);
                setUTC =0;
                    break;
            }
            if(isButtonModeUTCHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
                statusSetUpUTC = INIT_SYSTEM;
                
            }
            break;
        case GER:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,0,"GER");
            LcdPrintStringS(0,4," ");
            LcdPrintStringS(0,6,"BERLIN");
            LcdPrintStringS(1,4,"TIME=UTC+1");
            if(isButtonIncrease())
            {
                statusSetUpUTC = ENG;
                break;
            }
            if(isButtonDecrease())
            {
                statusSetUpUTC = VIE;
                break;
            }
            if(isButtonModeUTC())
            {   
                LcdClearS();
                hourGER = hour - 6;
                Write_DS1307(ADDRESS_HOUR,hourGER);
                setUTC=1;
                    break;
            }
            if(isButtonModeUTCHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
            statusSetUpUTC = INIT_SYSTEM;
              
            }
            break;
        case ENG:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,0,"ENG");
            LcdPrintStringS(0,4," ");
            LcdPrintStringS(0,6,"LONDON");
            LcdPrintStringS(1,4,"TIME=UTC");
            if(isButtonIncrease())
            {
                statusSetUpUTC = RUS;
                break;
            }
            if(isButtonDecrease())
            {
                statusSetUpUTC = GER;
                break;
            }
            if(isButtonModeUTC())
            {   
                LcdClearS();
                hourENG = hour-7;
                Write_DS1307(ADDRESS_HOUR,hourENG);
                setUTC=2;               
                    break;
            }
            if(isButtonModeUTCHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
            statusSetUpUTC = INIT_SYSTEM;
            
            }
            break;
        case RUS:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,0,"RUS");
            LcdPrintStringS(0,4," ");
            LcdPrintStringS(0,6,"MOSCOW");
            LcdPrintStringS(1,4,"TIME=UTC+3");
            if(isButtonIncrease())
            {
                statusSetUpUTC = USA;
                break;
            }
            if(isButtonDecrease())
            {
                statusSetUpUTC = ENG;
                break;
            }
            if(isButtonModeUTC())
            {   
                LcdClearS();
                hourRUS = hour - 4;
                Write_DS1307(ADDRESS_HOUR,hourRUS);
                setUTC=3;
                    break;
            }
            if(isButtonModeUTCHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
            statusSetUpUTC = INIT_SYSTEM;
               
            }
            break;
        case USA:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,0,"USA");
            LcdPrintStringS(0,4," ");
            LcdPrintStringS(0,6,"NEWYORK");
            LcdPrintStringS(1,4,"TIME=UTC-4");
            if(isButtonIncrease())
            {
                statusSetUpUTC = AUS;
                break;
            }
            if(isButtonDecrease())
            {
                statusSetUpUTC = RUS;
                break;
            }
            if(isButtonModeUTC())
            {   
                LcdClearS();
                hourUSA = hour - 11;
                Write_DS1307(ADDRESS_HOUR,hourUSA);
                setUTC=4;
                    break;
            }
            if(isButtonModeUTCHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
            statusSetUpUTC = INIT_SYSTEM;
                
                
            }
            break;
        case AUS:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,0,"AUS");
            LcdPrintStringS(0,4," ");
            LcdPrintStringS(0,6,"CANBERA");
            LcdPrintStringS(1,4,"TIME=UTC+11");
            if(isButtonIncrease())
            {
                statusSetUpUTC = FRA;
                break;
            }
            if(isButtonDecrease())
            {
                statusSetUpUTC = USA;
                break;
            }
            if(isButtonModeUTC())
            {   
                LcdClearS();
                hourAUS = hour + 4;
                Write_DS1307(ADDRESS_HOUR,hourAUS);
                setUTC=5;
                    break;
            }
            if(isButtonModeUTCHold())
            {
                LcdClearS();
                bitEnable = ENABLE;
            statusSetUpUTC = INIT_SYSTEM;
              
              
            }
           break;
       case FRA:
            LcdClearS();
            bitEnable = DISABLE;
            LcdPrintStringS(0,0,"FRA");
            LcdPrintStringS(0,4," ");
            LcdPrintStringS(0,6,"PARIS");
            LcdPrintStringS(1,4,"TIME=UTC+3");
            if(isButtonIncrease())
            {
                statusSetUpUTC = VIE;
                break;
            }
            if(isButtonDecrease())
            {
                statusSetUpUTC = AUS;
                break;
            }
            if(isButtonModeUTC())
            {   
                LcdClearS();
                hourFRA = hour -4;
                Write_DS1307(ADDRESS_HOUR,hourFRA);
                setUTC=6;
                    break;
            }
            if(isButtonModeUTCHold())
            {
                LcdClearS();
                
                bitEnable = ENABLE;
            statusSetUpUTC = INIT_SYSTEM;                
            }
            break;
         
        default:
            LcdClearS();
            bitEnable = ENABLE;
            statusSetUpUTC = INIT_SYSTEM;
            break;

    }
}
void dht11_init(){

 DHT11_Data_Pin_Direction= 0; //Configure RD0 as output

 DHT11_Data_Pin = 0; //RD0 sends 0 to the sensor

 delay_ms(25);

 DHT11_Data_Pin = 1; //RD0 sends 1 to the sensor


 DHT11_Data_Pin_Direction = 1; //Configure RD0 as input

 }
void find_response(){

 Check_bit = 0;

 if (DHT11_Data_Pin == 0){
 delay_ms_7SEG(1);
 if (DHT11_Data_Pin == 1){
    Check_bit = 1;
 }
 }  
}
unsigned char read_dht11(){

        unsigned char data, for_count;

 for(for_count = 0; for_count < 8; for_count++){

     while(!DHT11_Data_Pin); 

    if(DHT11_Data_Pin == 0){

        data&= ~(1<<(7 - for_count)); //Clear bit (7-b)
        
    }

    else{

        data|= (1 << (7 - for_count)); //Set bit (7-b)

        while(DHT11_Data_Pin);

    } //Wait until PORTD.F0 goes LOW

    }

 return data;
 }
unsigned char isButtonSetTimeMin()
{
    if (key_code[1] == 1)
        return 1;
    else
        return 0;
}

unsigned char isButtonSetTimeMax()
{
    if (key_code[0] == 1)
        return 1;
    else
        return 0;
}
void CalibTime(void)
{
    if (isButtonSetTimeMin())
    {
        LcdClearS();
        UartSendString(" SET TAT CA VE NGAY DAU NAM ");
        Write_DS1307(ADDRESS_HOUR,1);
        Write_DS1307(ADDRESS_MINUTE,0);
        Write_DS1307(ADDRESS_SECOND,0);
        Write_DS1307(ADDRESS_DAY,1);
        Write_DS1307(ADDRESS_DATE,1);
        Write_DS1307(ADDRESS_MONTH,1);
        //TMP_value_min = 2500; 
    }
    
    if (isButtonSetTimeMax())
    {
        LcdClearS();
        UartSendString(" SET TAT CA VE NGAY CUOI NAM ");
        Write_DS1307(ADDRESS_HOUR,23);
        Write_DS1307(ADDRESS_MINUTE,59);
        Write_DS1307(ADDRESS_SECOND,0);
        Write_DS1307(ADDRESS_DAY,31);
        Write_DS1307(ADDRESS_DATE,12);
        Write_DS1307(ADDRESS_MONTH,12);
        //TMP_value_max = 5000; 
    }
}
void SendDataUART(void)
{
        UartSendString("Country: ");
        switch(setUTC)
        {   case 0:
            UartSendString("VIE ,");
            break;
            case 1:
                UartSendString("GER ,");
                break;
            case 2:
                UartSendString("ENG ,");
                break;
            case 3:
                UartSendString("RUS ,");
                break;
            case 4:
                UartSendString("USA ,");
                break;
            case 5:
                UartSendString("AUS ,");
                break;
            case 6:
                UartSendString("FRA ,");
                break;
        }
        UartSendString("Time: ");
        UartSendNum(Read_DS1307(ADDRESS_HOUR));
        UartSendString(":");
        UartSendNum(Read_DS1307(ADDRESS_MINUTE));
        UartSendString(":");
        UartSendNum(Read_DS1307(ADDRESS_SECOND));
        UartSendString(" ,");	
        UartSendString("ALARM: ");
        UartSendNum(Read_DS1307(ADDRESS_HOUR_ALARM));
        UartSendString(":");
        UartSendNum(Read_DS1307(ADDRESS_MINUTE_ALARM));
        UartSendString(", ALARM MODE: ");
        switch(bitAlarm)
    {
        case 0:
            UartSendString("OFF ");
            break;
        case 1:
            UartSendString("ON ");
            break;
    }
        UartSendString(", DATE: ");
        UartSendNum(Read_DS1307(ADDRESS_DATE));
        UartSendString(", DAY: ");
        if(setUTC==0)
    {
    switch(day)
    {
        case 1:
            UartSendString("SUN");
            break;
        case 2:
            UartSendString("MON");
            break;
        case 3:
            UartSendString("TUE");
            break;
        case 4:
            UartSendString("WED");
            break;
        case 5:
            UartSendString("THU");
            break;
        case 6:
            UartSendString("FRI");
            break;
        case 7:
            UartSendString("SAT");
            break;
    }
    switch(month)
    {   
        case 1:
            UartSendString(", MONTH:JAN");
            break;
        case 2:
            UartSendString(", MONTH:FEB");
            break;
        case 3:
            UartSendString(", MONTH:MAR");
            break;
        case 4:
            UartSendString(", MONTH:APR");
            break;
        case 5:
            UartSendString(", MONTH:MAY");
            break;
        case 6:
            UartSendString(", MONTH:JUN");
            break;
        case 7:
            UartSendString(", MONTH:JUL");
            break;
        case 8:
            UartSendString(", MONTH:AUG");
            break;
        case 9:
            UartSendString(", MONTH:SEP");
            break;
        case 10:
            UartSendString(", MONTH:OCT");
            break;
        case 11:
            UartSendString(", MONTH:NOV");
            break;
        case 12:
            UartSendString(", MONTH:DEC");
            break;
    }
    }
    if(setUTC==1)
    {switch(day)
    {
        case 1:
            UartSendString("SON");
            break;
        case 2:
            UartSendString("MON");
            break;
        case 3:
            UartSendString("DIE");
            break;
        case 4:
            UartSendString("MIT");
            break;
        case 5:
            UartSendString("DON");
            break;
        case 6:
            UartSendString("FRE");
            break;
        case 7:
            UartSendString("SAM");
            break;
    }
    switch(month)
    {   
        case 1:
            UartSendString(", MONTH:JAN");
            break;
        case 2:
            UartSendString(", MONTH:FEB");
            break;
        case 3:
            UartSendString(", MONTH:MAR");
            break;
        case 4:
            UartSendString(", MONTH:APR");
            break;
        case 5:
            UartSendString(", MONTH:KAN");
            break;
        case 6:
           UartSendString(", MONTH:JUN");
            break;
        case 7:
           UartSendString(", MONTH:JUL");
            break;
        case 8:
            UartSendString(", MONTH:AUG");
            break;
        case 9:
            UartSendString(", MONTH:SEP");
            break;
        case 10:
            UartSendString(", MONTH:OKT");
            break;
        case 11:
            UartSendString(", MONTH:NOV");
            break;
        case 12:
            UartSendString(", MONTH:DEZ");
            break;
    }
    }
    if(setUTC==2)
    {
        switch(day)
    {
        case 1:
            UartSendString("SUN");
            break;
        case 2:
            UartSendString("MON");
            break;
        case 3:
            UartSendString("TUE");
            break;
        case 4:
            UartSendString("WED");
            break;
        case 5:
            UartSendString("THU");
            break;
        case 6:
            UartSendString("FRI");
            break;
        case 7:
            UartSendString("SAT");
            break;
    }
    switch(month)
    {   
        case 1:
            UartSendString(", MONTH:JAN");
            break;
        case 2:
            UartSendString(", MONTH:FEB");
            break;
        case 3:
            UartSendString(", MONTH:MAR");
            break;
        case 4:
            UartSendString(", MONTH:APR");
            break;
        case 5:
            UartSendString(", MONTH:MAY");
            break;
        case 6:
            UartSendString(", MONTH:JUN");
            break;
        case 7:
            UartSendString(", MONTH:JUL");
            break;
        case 8:
            UartSendString(", MONTH:AUG");
            break;
        case 9:
            UartSendString(", MONTH:SEP");
            break;
        case 10:
            UartSendString(", MONTH:OCT");
            break;
        case 11:
            UartSendString(", MONTH:NOV");
            break;
        case 12:
            UartSendString(", MONTH:DEC");
            break;
    }
    }
    if(setUTC==3)
    {
         switch(day)
    {
        case 1:
            UartSendString("BOC");
            break;
        case 2:
            UartSendString("NOH");
            break;
        case 3:
            UartSendString("BTO");
            break;
        case 4:
            UartSendString("CPE");
            break;
        case 5:
            UartSendString("YET");
            break;
        case 6:
            UartSendString("NTH");
            break;
        case 7:
            UartSendString("CY6");
            break;
    }
         switch(month)
    {   
        case 1:
            UartSendString(", MONTH:HBA");
            break;
        case 2:
            UartSendString(", MONTH:EPB");
            break;
        case 3:
            UartSendString(", MONTH:MAP");
            break;
        case 4:
            UartSendString(", MONTH:ANP");
            break;
        case 5:
            UartSendString(", MONTH:MAN");
            break;
        case 6:
            UartSendString(", MONTH:NHO");
            break;
        case 7:
            UartSendString(", MONTH:NHO");
            break;
        case 8:
            UartSendString(", MONTH:ABR");
            break;
        case 9:
            UartSendString(", MONTH:CEH");
            break;
        case 10:
            UartSendString(", MONTH:OKT");
            break;
        case 11:
            UartSendString(", MONTH:HOR");
            break;
        case 12:
            UartSendString(", MONTH:EKA");
            break;
    }
    }
    if(setUTC==4)
    {
        switch(day)
    {
        case 1:
            UartSendString("SUN");
            break;
        case 2:
            UartSendString("MON");
            break;
        case 3:
            UartSendString("TUE");
            break;
        case 4:
            UartSendString("WED");
            break;
        case 5:
            UartSendString("THU");
            break;
        case 6:
            UartSendString("FRI");
            break;
        case 7:
            UartSendString("SAT");
            break;
    }
    switch(month)
    {   
        case 1:
            UartSendString(", MONTH:JAN");
            break;
        case 2:
            UartSendString(", MONTH:FEB");
            break;
        case 3:
            UartSendString(", MONTH:MAR");
            break;
        case 4:
            UartSendString(", MONTH:APR");
            break;
        case 5:
            UartSendString(", MONTH:MAY");
            break;
        case 6:
            UartSendString(", MONTH:JUN");
            break;
        case 7:
            UartSendString(", MONTH:JUL");
            break;
        case 8:
            UartSendString(", MONTH:AUG");
            break;
        case 9:
            UartSendString(", MONTH:SEP");
            break;
        case 10:
            UartSendString(", MONTH:OCT");
            break;
        case 11:
            UartSendString(", MONTH:NOV");
            break;
        case 12:
            UartSendString(", MONTH:DEC");
            break;
    }
    }
    if(setUTC==5)
    {
        switch(day)
    {
        case 1:
            UartSendString("SUN");
            break;
        case 2:
            UartSendString("MON");
            break;
        case 3:
            UartSendString("TUE");
            break;
        case 4:
            UartSendString("WED");
            break;
        case 5:
            UartSendString("THU");
            break;
        case 6:
            UartSendString("FRI");
            break;
        case 7:
            UartSendString("SAT");
            break;
    }
    switch(month)
    {   
        case 1:
            UartSendString(", MONTH:JAN");
            break;
        case 2:
            UartSendString(", MONTH:FEB");
            break;
        case 3:
            UartSendString(", MONTH:MAR");
            break;
        case 4:
            UartSendString(", MONTH:APR");
            break;
        case 5:
            UartSendString(", MONTH:MAY");
            break;
        case 6:
            UartSendString(", MONTH:JUN");
            break;
        case 7:
            UartSendString(", MONTH:JUL");
            break;
        case 8:
            UartSendString(", MONTH:AUG");
            break;
        case 9:
            UartSendString(", MONTH:SEP");
            break;
        case 10:
            UartSendString(", MONTH:OCT");
            break;
        case 11:
            UartSendString(", MONTH:NOV");
            break;
        case 12:
            UartSendString(", MONTH:DEC");
            break;
    }
    }
    if(setUTC==6)
    {
        switch(day)
    {
        case 1:
            UartSendString("DIM");
            break;
        case 2:
            UartSendString("LUN");
            break;
        case 3:
            UartSendString("MAR");
            break;
        case 4:
            UartSendString("MEC");
            break;
        case 5:
            UartSendString("JEU");
            break;
        case 6:
            UartSendString("VEN");
            break;
        case 7:
            UartSendString("SAM");
            break;
    }
        switch(month)
    {   
        case 1:
            UartSendString(", MONTH:JAN");
            break;
        case 2:
            UartSendString(", MONTH:FEV");
            break;
        case 3:
            UartSendString(", MONTH:MAR");
            break;
        case 4:
            UartSendString(", MONTH:AVR");
            break;
        case 5:
            UartSendString(", MONTH:PEU");
            break;
        case 6:
            UartSendString(", MONTH:JUI");
            break;
        case 7:
            UartSendString(", MONTH:JUI");
            break;
        case 8:
            UartSendString(", MONTH:AOU");
            break;
        case 9:
            UartSendString(", MONTH:SEP");
            break;
        case 10:
            UartSendString(", MONTH:OCT");
            break;
        case 11:
            UartSendString(", MONTH:NOV");
            break;
        case 12:
            UartSendString(", MONTH:DEC");
            break;
    }
    }	
        UartSendString(", YEAR: ");
         yuart = 2000+year;
        UartSendNum(yuart);
        UartSendString("\r\n");
}
void UartDataReceiveProcess() {
    switch(statusOfUart) {
        case UART_RESET_ALL:
            if (flagOfDataReceiveComplete == 1) {
                flagOfDataReceiveComplete = 0;
                if (dataReceive[0] == 'R') {
                    UartSendString("Data received successfully! \r\n");
                    UartSendString("Set Time is: 00:00:00 01/01/2023");
                    UartSendString("\r\n");
                    LcdClearS();
                    statusOfUart = UART_RESET_COMPLETE;
                    break;
                } else {
                    LcdClearS();
                    statusOfUart = UART_ERROR;
                    break;

                }
            }
            break;
        case UART_LATE_ALL:
            if (flagOfDataReceiveComplete == 1) {
                flagOfDataReceiveComplete = 0;
                if (dataReceive[0] == 'L') {
                    
                    UartSendString("Data received successfully! \r\n");
                    UartSendString("Set Time is: 23:59:00 31/12/2022");
                    UartSendString("\r\n");
     
                    LcdClearS();
                    statusOfUart = UART_LATE_COMPLETE;
                } else {
                    LcdClearS();
                    statusOfUart = UART_ERROR;
			break;
                }
            }
            break;
        case UART_SET_ALARM:
            if (flagOfDataReceiveComplete == 1) {
                flagOfDataReceiveComplete = 0;
                if (dataReceive[0] == 'A')
			{
                    hourAlarm=(dataReceive[1]*10)+ (dataReceive[2] );
			minuteAlarm=(dataReceive[3]*10) + (dataReceive[4]);
                    UartSendString("Data received successfully! \r\n");
                    UartSendString("New Alarm is: ");
                    UartSendNum(hourAlarm);
			UartSendString(":");
			UartSendNum(minuteAlarm);
                    UartSendString("\r\n");
                    LcdClearS();
                    statusOfUart = UART_ALARM_COMPLETE;
                } else {
                    LcdClearS();
                    statusOfUart = UART_ERROR;
                }
            }
            break;
        case UART_RESET_COMPLETE:
           	LcdClearS();
            LcdPrintStringS(0, 0, "RESET TIMER");
            LcdPrintStringS(1, 5, "COMPLETE");
            if (isButtonModeHold())
		{
                LcdClearS();
			Menucontrol = INIT_SYSTEM;
			break;
		}
            break;

        case UART_LATE_COMPLETE:
            LcdClearS();
            LcdPrintStringS(0, 0, "SET TIMER");
            LcdPrintStringS(1, 5, "COMPLETE:");
            if (isButtonModeHold()) {
                LcdClearS();
                Menucontrol=INIT_SYSTEM;
			break;
            } 
            break;
        case UART_ALARM_COMPLETE:
           
            LcdPrintStringS(0, 0, "SET ALARM TIME");
            LcdPrintStringS(1, 5, "COMPLETE");
             if (isButtonModeHold()) {
                LcdClearS();
                Menucontrol=INIT_SYSTEM;
			break;
            } 
            break;
        case UART_ERROR:
            LcdClearS();
            UartSendString("Syntax error! Please try again. \r\n");
            break;
        default:
            break;
    }
}
void display_dht11(void)
{
   if(Check_bit == 1){
        RH_byte_1 = read_dht11();
        RH_byte_2 = read_dht11();
        Temp_byte_1 = read_dht11();
        Temp_byte_2 = read_dht11();
        Sumation = read_dht11();
        if(Sumation == ((RH_byte_1+RH_byte_2+Temp_byte_1+Temp_byte_2) & 0XFF)){
            Himudity = Temp_byte_1;
            RH = RH_byte_1;    
            numdh1 =48+((Himudity/10)%10);
            numdh2 =48+(Himudity % 10);
            numdh3=48 + ((RH / 10) % 10);
            numdh4=48 + (RH % 10);
            LcdPrintStringS(0,0,"TEMP: ");
            LcdPrintCharS(0,7,numdh1);
            LcdPrintCharS(0,8,numdh2);
            LcdPrintCharS(0,9,0xDF);
            LcdPrintStringS(0,10,"C   ");           
            LcdPrintStringS(1,0,"HUMID: ");
             //lcd_puts("                ");
           LcdPrintCharS(1,8,numdh3);
            LcdPrintCharS(1,9,numdh4);
             LcdPrintStringS(1,10,"%");
            }
        else{
            LcdPrintStringS(0,0,"Check sum error");
        }
    }
   else
   { LcdPrintStringS(0,0,"ERROR READ DHT");
   }
}
void FSMQUES1()
{
    if(bitEnableQues1 == 0) return;
    switch(statusFSM)
    {
        case INIT_SYSTEM:
            LcdClearS();
            statusFSM = TEMHUM;
            bitEnableQues1 = ENABLE;
            SetTimer0_ms(7000);
            break;
        case TEMHUM:
            LcdClearS();
//            bitEnableQues1 = DISABLE;
            ReadValueAdc = get_adc_value(0);
            LcdPrintStringS(0,0,"TEMP: ");
            LcdPrintNumS(0,7,ReadValueAdc);
            LcdPrintCharS(0,12,0xDF);
            LcdPrintStringS(0,13,"C   ");           
            LcdPrintStringS(1,0,"HUMID: ");
             //lcd_puts("                ");
           LcdPrintNumS(1,7,60);
             LcdPrintStringS(1,10,"%");
             if(flag_timer0){
                 statusFSM = DAYMON;
                 flag_timer0 = 0;
                 break;
             }
             
             break;
        case DAYMON:
            LcdClearS();
//            bitEnableQues1 = DISABLE;
            DisplayTime(setUTC);

//            delay_ms(5000);
             if(flag_timer0){
                 statusFSM = SIM;
                 flag_timer0 = 0;
                 break;
             }
            break;
        case SIM:
            LcdClearS();
//            bitEnableQues1 = DISABLE;
            LcdPrintStringS(0,4,"CONECTED");
            LcdPrintStringS(1,2,"0336xxxxxx");
            if(flag_timer0){
                 statusFSM = TEMHUM;
                 flag_timer0 = 0;
                 break;
             }
            break;
        default:
            LcdClearS();
//            bitEnableQues1 = ENABLE;
            break;
    }
}
void setMENUQ1()
{
//    if(bitEnableMENU1 == 0) return;
    switch(statusQ1){        
        case INIT_SYSTEM:
        if(isButtonMode()==1 && bitEnableMENU1==ENABLE){
            statusQ1 = MENU;
            bitEnableQues1 = DISABLE;
        }
            break;
        case MENU:
            LcdClearS();
//            bitEnableMENU1 = DISABLE;
            LcdPrintStringS(0,2,"MENU QUESTION");
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(1,2,"1.SET TIMER");
            if(isButtonMode())
            {   passwordIndex=1;
                statusQ1 = ENTER_PASSWORD;
                indexOfNumber= 0;
            LcdClearS();
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            statusQ1 = INIT_SYSTEM;
            statusFSM = INIT_SYSTEM;
            bitEnableMENU1 = ENABLE;
            bitEnableQues1 = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   statusQ1 = MENU3;
                break;
            }
            if(isButtonDecrease())
            {   statusQ1 = MENU2;
                break;
            }      
            break;
        case MENU2:
            LcdClearS();
//            bitEnableMENU1=DISABLE;
            LcdPrintStringS(0,0,">");
            LcdPrintStringS(0,2,"2.TRANFER DATA");
            LcdPrintStringS(1,2,"3.CHANGE PASS");
            if(isButtonMode())
            {   passwordIndex=2;
                statusQ1 = ENTER_PASSWORD;
                indexOfNumber= 0;
            LcdClearS();
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            statusQ1 = INIT_SYSTEM;
            bitEnableMENU1 = ENABLE;
            statusFSM = INIT_SYSTEM;
            bitEnableQues1 = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   statusQ1 = MENU;
                break;
            }
            if(isButtonDecrease())
            {   statusQ1 = MENU3;
                break;
            }      
            break;
        case MENU3:
            LcdClearS();
//            bitEnableMENU1=DISABLE;
            LcdPrintStringS(0,2,"2.TRANFER DATA");
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(1,2,"3.CHANGE PASS");
            if(isButtonMode())
            {   passwordIndex=3;
                statusQ1 = ENTER_PASSWORD;
                indexOfNumber= 0;
                       LcdClearS();
            break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            statusFSM = INIT_SYSTEM;
            statusQ1 = INIT_SYSTEM;
            bitEnableMENU1 = ENABLE;
            bitEnableQues1 = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   statusQ1 = MENU2;
                break;
            }
            if(isButtonDecrease())
            {   statusQ1 = MENU;
                break;
            }      
            break;
        case ENTER_PASSWORD:
//            LcdClearS();
//            bitEnableMENU1=DISABLE;
            LcdPrintStringS(0,0,"ENTER PASSWORD  ");
//            timeDelay ++;

            if (indexOfNumber >= 4){
                            if (isButtonEnter()){
                                indexOfNumber = 0;
                                if(checkpass(0)==1 && passwordIndex==1){
                                    statusQ1 = TIMERDIS;
                                    break;
                                } 
                                if(checkpass(1)==1 && passwordIndex==2){
                                    Counter=10;
                                    SetTimer0_ms(1000);
                                    statusQ1 = TRANDATA;
                                    break;
                                } 
                                if(checkpass(2)==1 && passwordIndex==3){
                                    statusQ1 = MENUCHANGE1;
                                    break;
                                } 
                                else{
                                    statusQ1 = WRONG_PASS;
                                    SetTimer0_ms(1000);
                                    LcdClearS();
                                    break;
                                }
                            }
                
            } else {
                            if (isButtonNumber())
            {
                LcdPrintStringS(1,indexOfNumber,"*");
                arrayPassword [indexOfNumber] = numberValue;
                indexOfNumber ++;
//                timeDelay = 0;
            }
            }
                
//            if (timeDelay >= 100)
//                statusPassword = INIT_SYSTEM;

            break;
        case WRONG_PASS:
            LcdPrintStringS(0,1,"WRONG PASSWORD");
            if(flag_timer0){
                LcdClearS();
                flag_timer0 = 0;
                statusFSM = INIT_SYSTEM;
                statusQ1 = INIT_SYSTEM;
                bitEnableQues1=ENABLE;
            }
            break;
        case TIMERDIS:
            LcdClearS();
            bitEnableMENU1 = ENABLE;
            SetUpTime();
            if(isButtonModeHold())
            {   LcdClearS();
            statusFSM = INIT_SYSTEM;
            bitEnableMENU1 = ENABLE;
            statusQ1=INIT_SYSTEM;
            bitEnableQues1 = ENABLE;
            break;
            }
            break;
        case TRANDATA:

            bitEnableMENU1 = ENABLE;           
            if(Counter>0)
            {
            LcdClearS();
            LcdPrintStringS(0,4,"WAITTING");
            LcdPrintNumS(1,8,Counter);   
            }
            if(flag_timer0)              
            {   
                flag_timer0 = 0;
                if(Counter<=0)
                {
                LcdClearS();
                LcdPrintStringS(0,4,"COMPLETE");     
                }
                if(Counter<0)
                {
                     LcdClearS();
                     statusFSM = INIT_SYSTEM;
                     bitEnableMENU1 = ENABLE;
                     statusQ1=INIT_SYSTEM;
                     bitEnableQues1 = ENABLE;
                }    
                Counter--;
            }
            
            if(isButtonModeHold())
            {   LcdClearS();
            statusFSM = INIT_SYSTEM;
            bitEnableMENU1 = ENABLE;
            statusQ1=INIT_SYSTEM;
            bitEnableQues1 = ENABLE;
            break;
            }
            break;
        case MENUCHANGE1:
            LcdClearS();
//            bitEnableMENU1 = ENABLE;
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(0,2,"CHANGE PASS");
            LcdPrintStringS(1,2,"1.PASS1");
           if(isButtonMode())
            {   
                statusQ1 = CHANGEPASS1;
                   LcdClearS();
                   indexOfNumber = 0;
                   passwordIndex = 1;
                        break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            statusFSM = INIT_SYSTEM;
            statusQ1 = INIT_SYSTEM;
            bitEnableMENU1 = ENABLE;
            bitEnableQues1 = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   statusQ1 = MENUCHANGE3;
                break;
            }
            if(isButtonDecrease())
            {   statusQ1 = MENUCHANGE2;
                break;
            }      
            break;
        case MENUCHANGE2:
            LcdClearS();
//            bitEnableMENU1 = ENABLE;
            LcdPrintStringS(0,0,">");
            LcdPrintStringS(0,2,"2.PASS2");
            LcdPrintStringS(1,2,"3.PASS3");
           if(isButtonMode())
            {   
                statusQ1 = CHANGEPASS1;
                indexOfNumber = 0;
                LcdClearS();
                passwordIndex = 2;
                break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            statusFSM = INIT_SYSTEM;
            statusQ1 = INIT_SYSTEM;
            bitEnableMENU1 = ENABLE;
            bitEnableQues1 = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   statusQ1 = MENUCHANGE1;
                break;
            }
            if(isButtonDecrease())
            {   statusQ1 = MENUCHANGE3;
                break;
            }      
            break;
        case MENUCHANGE3:
            LcdClearS();
            bitEnableMENU1 = ENABLE;
            LcdPrintStringS(1,0,">");
            LcdPrintStringS(0,2,"2.PASS2");
            LcdPrintStringS(1,2,"3.PASS3");
           if(isButtonMode())
            {   
                statusQ1 = CHANGEPASS1;
                indexOfNumber = 0;
                LcdClearS();
                passwordIndex = 3;
                break;
            }
            if(isButtonModeHold())
            {  LcdClearS();
            statusFSM = INIT_SYSTEM;
            statusQ1 = INIT_SYSTEM;
            bitEnableMENU1 = ENABLE;
            bitEnableQues1 = ENABLE;
            break;
            }
            if(isButtonIncrease())
            {   statusQ1 = MENUCHANGE2;
                break;
            }
            if(isButtonDecrease())
            {   statusQ1 = MENUCHANGE1;
                break;
            }      
            break;
        case CHANGEPASS1:

            LcdPrintStringS(0,0,"ENTER NEW PASS ");
            if (indexOfNumber >= 4){
                            if (isButtonEnter()){
                                setPass(passwordIndex-1);
                                LcdClearS();
                                statusFSM = INIT_SYSTEM;
                                statusQ1 = INIT_SYSTEM;
                                bitEnableMENU1 = ENABLE;
                                bitEnableQues1 = ENABLE;
                            }
                
            } else {
                            if (isButtonNumber())
            {
                LcdPrintNumS(1,indexOfNumber,numberValue);
                arrayPassword [indexOfNumber] = numberValue;
                indexOfNumber ++;
//                timeDelay = 0;
            }
            }
                
//            if (timeDelay >= 100)
//                statusPassword = INIT_SYSTEM;

            break;
    }
}

unsigned char isButtonNumber()
{
    unsigned char i;
    for (i = 0; i<=15; i++)
        if (key_code[i] == 1)
        {
            numberValue = arrayMapOfNumber[i];
            return 1;
        }
    return 0;
}

unsigned char isButtonEnter()
{
    if (key_code[4] == 1)
        return 1;
    else
        return 0;
}

unsigned char checkpass(int index){
    int i;
    for(i = 0; i <4; i++){
        if(arrayPassword[i] != pass[index][i]) return 0;
    }
    return 1;
}

void setPass(int index){
    int i;
    for(i = 0; i < 4; i++){
        pass[index][i] = arrayPassword[i];
    }
}