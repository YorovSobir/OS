#include "serial_port.h"

void init_serial()
{
	//отключаем прерывание
	out8(0x3f9,0);
	//определяем коэффициент деления
	out8(0x3fb,128);
	
	uint16_t div = 0x11;
	uint8_t lbyte, hbyte;
	lbyte = div & 0x00FF;
	hbyte = ((div & 0xFF00) >> 8);;

	out8(0x3f8,lbyte);
	out8(0x3f9,hbyte);
	//формат кадра: 00000011
	out8(0x3fb,3);
	
}

void print_serial(const char * s)
{
	const char * ptr = s;
	int i = 0, m = 0;
	while(*ptr){++i; ++ptr;};
	while(m < i)
	{
		if(in8(0x3fd) & 32)
		{
			out8(0x3f8,*(s+m)); ++m;
		}

	}

}

void print_num(uint64_t num) 
{
    char str_num[20];
    int strlen = 0;
    
    for (; num > 0 && strlen < 20; num /= 10, ++strlen)
    {
        str_num[strlen] = '0' + (num % 10);
    }

    str_num[strlen] = '0';
    ++strlen;
    serial_write(str_num, strlen);
}

void serial_write(const char *buf, uint64_t size)
{
	uint64_t m = 0;
	
	while(m < size)
	{
		if(in8(0x3fd) & 32)
		{
			out8(0x3f8,*(buf+m)); ++m;
		}

	}
}


void print_inter(uint64_t t)
{
	switch(t){
		case 0: {print_serial("0, Деление на ноль\n"); break;}
		case 1: {print_serial("1, прерывание отладки, пошаговое исполнение\n"); break;}
		case 2: {print_serial("2, Немаскируемое прерывание\n"); break;}
		case 3: {print_serial("3, Прерывание точки останова\n"); break;}
		case 4: {print_serial("4, Переполнение при выполнении команды Into\n"); break;}
		case 5: {print_serial("5, Прерывание выхода за границы данных\n"); break;}
		case 6: {print_serial("6, Прерывание неправильного кода операции\n"); break;}
		case 7: {print_serial("7, Прерывание отсутствие сопроцессора\n"); break;}
		case 8: {print_serial("8, Прерывание двойной ошибки\n"); break;}
		case 9: {print_serial("9, Нарушение сегментации памяти сопроцессором\n"); break;}
		case 10: {print_serial("10, Неправильный TSS\n"); break;}
		case 11: {print_serial("11, Отсутствие сегмента\n"); break;}
		case 12: {print_serial("12, Ошибка стека\n"); break;}
		case 13: {print_serial("13, Ошибка общей защиты\n"); break;}
		case 14: {print_serial("14, Ошибка системы страничной организации памяти\n"); break;}
		case 15: {print_serial("15, Неизвестное прерывание\n"); break;}
		case 16: {print_serial("16, Ошибка сопроцессора\n"); break;}
		case 17: {print_serial("17, Прерывание контроля выравнивания\n"); break;}
		case 18: {print_serial("18, Прерывание, связанное с общей работой процессора\n"); break;}
		case 32: {print_serial("32, PIT\n"); break;}
		default: {print_serial("I don't know this type of interrupt"); break;}
	}
}