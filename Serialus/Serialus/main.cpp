#include "Serialus.h"
#include "protocole.h"

using namespace std;

#define _ALLOW_RTCc_IN_STL

/*
_frame remplissage_frame(_eser_header header, _eser_main_type main_od, _eser_data_type data_type, _user_data_type data)
{
	_frame frame;
	frame.header = header;
	frame.main_od = main_od;
	frame.data_type = data_type;
	//frame.data = data.str;
	memcpy(&frame.data, (char *) &data.str, sizeof(data.str));
	return frame;
}*/



int main()
{
	
	_user_var_id var_id;
	_user_type_id type_id;
	_user_data_type data_type;
	var_id.data_asserv = KD_DISTANCE;
	type_id.id_ax12 = 10;
	data_type.uint32 = 64869;

	Serialus serialus;
	serialus.print_logo();
	serialus.reinit_messages();
	serialus.remplissage_frame(SET_VALEUR, ASSERV, var_id, type_id, data_type);
	serialus.copie_frame_tx();
	serialus.envoi_frame();
	serialus.reception_message();
	serialus.copie_frame_rx();
	serialus.check_rx_message();
	//serialus.reception();

	printf("\nTaille du putain de header : %d", sizeof(_frame_header));
	printf("\nTaille du putain de message : %d", sizeof(_frame_data_tx));
	printf("\nTaille du putain de checksum : %d", sizeof(_fletcher_checksum));
	 
	printf("\n");
	system("pause");
	return 0;
}