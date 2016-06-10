#include <iostream>
#include <string>
#include "protocole.h"

using namespace std;

typedef struct
{
	_eser_header header;
	_eser_main_type main_od;
	_eser_data_type data_type;
	char data[sizeof(double)];
}_frame;


_frame remplissage_frame(_eser_header header, _eser_main_type main_od, _eser_data_type data_type, _user_data_type data)
{
	_frame frame;
	frame.header = header;
	frame.main_od = main_od;
	frame.data_type = data_type;
	//frame.data = data.str;
	memcpy(&frame.data, (char *) &data.str, sizeof(data.str));
	return frame;
}



int main()
{
	string buffer_reception;
	_frame frame;
	_user_data_type data;
	_user_data_type data2;
	double nb;

	data.reel64 = 3.145169985544522;

	printf("\nReel : %.20f", data.reel64);
	frame = remplissage_frame(SET_VALEUR, ODOMETRIE, DOUBLE, data);
	printf("\nstr : %x", frame.data[0]);
	memcpy(&data2.str, (char*) frame.data, sizeof(frame.data));
	memcpy(&nb, (char*) frame.data, sizeof(frame.data));
	printf("\nstr : %x, value : %.20f", data2.str[0], data2.reel64);
	printf("\ndouble : %.20f", nb);

	data.uint8 = 3;
	printf("\n\nint : %d", data.uint8);
	frame = remplissage_frame(SET_VALEUR, ODOMETRIE, UINT8, data);
	printf("\nstr : %x", frame.data[0]);
	memcpy(&data2.str, (char*) frame.data, sizeof(frame.data));
	printf("\nstr : %x, value : %.20f, int : %d", data2.str[0], data2.reel64, data.int8);

	 
	printf("\n");
	system("pause");
	return 0;
}