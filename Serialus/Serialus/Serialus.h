#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "protocole.h"
using namespace std;

class Serialus
{
public:
	Serialus(void);
	~Serialus(void);
	void reception();
	void reception_message();
	void check_rx_message();
	void traitement_reception();
	void remplissage_frame(_eser_main_header main_header, _eser_sub_header sub_header, _user_var_id var_id, _user_type_id type_id);
	void remplissage_frame(_eser_main_header main_header, _eser_sub_header sub_header, _user_var_id var_id, _user_type_id type_id, _user_data_type data);
	void remplissage_frame(_eser_main_header main_header, _eser_sub_header sub_header, _user_var_id var_id, _user_type_id type_id, _user_data_type data1, _user_data_type data2);
	void remplissage_frame(_eser_main_header main_header, _eser_sub_header sub_header, _user_var_id var_id, _user_type_id type_id, _user_data_type data1, _user_data_type data2, _user_data_type data3);

	void copie_frame_tx();
	void copie_frame_rx();
	void envoi_frame();
	void reinit_messages();


	// fonction de print
	void print_non_reconnu(uint8_t nb);
	
//	void print_list(_eser_header list);
	void print_logo();
	_fletcher_checksum calcul_checksum(char* message, size_t size_message);

protected:
	vector<string> _buffer_reception;
	
	// Message Tx
	_frame_header tx_message_header;
	_frame_data_tx tx_message_data;
	_fletcher_checksum tx_message_checksum;
	char message_a_envoyer[SIZE_FRAME_TX];

	// Message Rx
	_frame_header rx_message_header;
	_frame_data_tx rx_message_data;
	_fletcher_checksum rx_message_checksum;
	char message_a_recevoir[SIZE_FRAME_RX];
};


