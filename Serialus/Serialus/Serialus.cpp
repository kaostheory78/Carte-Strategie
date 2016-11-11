#include "Serialus.h"
#include <sstream>
#include <algorithm>




Serialus::Serialus(void)
{
	reinit_messages();
}

Serialus::~Serialus(void)
{

}

void Serialus::reception()
{
	string buffer_temp, mot;
	
	// On récupère l'instruction puis on la convertit en lowercase
	getline(cin, buffer_temp);
	transform(buffer_temp.begin(), buffer_temp.end(), buffer_temp.begin(), ::tolower);

	// On crée un flux à partir de la ligne récupérée pour pouvoir 
	// la découper mot par mot facilement
	istringstream flux(buffer_temp);
	while (flux >> mot)
		_buffer_reception.push_back(mot);

	for (size_t i = 0; i < _buffer_reception.size(); i++)
		cout << endl << i << " : " << _buffer_reception[i];
}


void Serialus::reception_message()
{
	// Currently stub
	memcpy(&message_a_recevoir, (char *)&message_a_envoyer, sizeof(message_a_recevoir));
	memcpy(&(message_a_recevoir[sizeof(message_a_recevoir) - sizeof(_fletcher_checksum)]), (char*)&(message_a_envoyer[sizeof(message_a_envoyer) - sizeof(_fletcher_checksum)]), sizeof(rx_message_checksum));

}


void Serialus::reinit_messages()
{
	//memcpy(&message_a_envoyer, 0, sizeof(message_a_envoyer));
	tx_message_header.h1 = 0;
	tx_message_header.h2 = 0;
	tx_message_header.main_header = 0;
	tx_message_header.sub_header = 0;
	tx_message_header.type_id = 0;
	tx_message_header.var_id = 0;
	tx_message_data.data1.uint64 = 5;
	tx_message_data.data2.uint64 = 5;
	tx_message_data.data3.uint64 = 5;
	tx_message_checksum.cs1 = 0;
	tx_message_checksum.cs2 = 0;
}


/*****************************************************************************************/
/*********************************** REMPLISSAGE FRAME ***********************************/
/*****************************************************************************************/


void Serialus::remplissage_frame(_eser_main_header main_header, _eser_sub_header sub_header, _user_var_id var_id, _user_type_id type_id)
{
	tx_message_header.h1 = 0xFF;
	tx_message_header.h2 = 0xFF;
	tx_message_header.main_header = main_header;
	tx_message_header.sub_header = sub_header;
	tx_message_header.var_id =  var_id.value;
	tx_message_header.type_id = type_id.id_ax12;
	cout << endl << "h1 : " <<  (int) tx_message_header.h1 << " h2 : " << (int) tx_message_header.h2 << " h3 : " << (int) tx_message_header.main_header;
	cout << " h4 : " << (int) tx_message_header.sub_header << " h5 : " << (int) tx_message_header.var_id << " h6 : " << (int) tx_message_header.type_id;
}

void Serialus::remplissage_frame(_eser_main_header main_header, _eser_sub_header sub_header, _user_var_id var_id, _user_type_id type_id, _user_data_type data)
{
	remplissage_frame(main_header, sub_header, var_id, type_id);
	tx_message_data.data1 = data;
	tx_message_data.data2.uint64 = 0;
	tx_message_data.data2.uint64 = 0;
	cout << " data : " << (int) tx_message_data.data1.uint64;
}

void Serialus::remplissage_frame(_eser_main_header main_header, _eser_sub_header sub_header, _user_var_id var_id, _user_type_id type_id, _user_data_type data1, _user_data_type data2)
{
	remplissage_frame(main_header, sub_header, var_id, type_id);
	tx_message_data.data1 = data1;
	tx_message_data.data2 = data2;
	tx_message_data.data2.uint64 = 0;
}


void Serialus::remplissage_frame(_eser_main_header main_header, _eser_sub_header sub_header, _user_var_id var_id, _user_type_id type_id, _user_data_type data1, _user_data_type data2, _user_data_type data3)
{
	remplissage_frame(main_header, sub_header, var_id, type_id);
	tx_message_data.data1 = data1;
	tx_message_data.data2 = data2;
	tx_message_data.data2 = data3;
}

/*****************************************************************************************/
/***********************************  ***********************************/
/*****************************************************************************************/



void Serialus::copie_frame_tx()
{
	// Côpie du header
	memcpy(&message_a_envoyer, (char *)&tx_message_header, sizeof(tx_message_header));

	// Copie de la data
	memcpy(&(message_a_envoyer[sizeof(tx_message_header)]), (char *)&tx_message_data, sizeof(tx_message_data));

	// Copie du checksum
	tx_message_checksum = calcul_checksum(message_a_envoyer, sizeof(message_a_envoyer));
	memcpy(&(message_a_envoyer[sizeof(message_a_envoyer) - sizeof(tx_message_checksum)]), (char *)&tx_message_checksum, sizeof(tx_message_checksum));
}

void Serialus::copie_frame_rx()
{
	cout << endl << endl;
	for (int i = 0; i < sizeof(message_a_recevoir); i++)
	{
		printf("\t%08x", message_a_recevoir[i]);
	}
	cout << endl << endl;

	// Côpie du header
	memcpy(&rx_message_header, (char *)&message_a_recevoir, sizeof(rx_message_header));

	// Copie de la data
	memcpy(&rx_message_data, (char *)&(message_a_recevoir[sizeof(rx_message_header)]), sizeof(rx_message_data));

	// Copie du checksum
	memcpy(&rx_message_checksum, (char *)&(message_a_recevoir[sizeof(message_a_recevoir) - sizeof(rx_message_checksum)]), sizeof(rx_message_checksum));
}

void Serialus::envoi_frame()
{
	cout << endl;
	for (int i = 0; i < sizeof(message_a_envoyer); i++)
	{
		printf("\t%08x", message_a_envoyer[i]);
	}
	cout << endl;
}


void Serialus::check_rx_message()
{
	bool result = true;
	cout << endl << "h1 : " << (int)rx_message_header.h1 << " h2 : " << (int)rx_message_header.h2 << " h3 : " << (int)rx_message_header.main_header;
	cout << " h4 : " << (int)rx_message_header.sub_header << " h5 : " << (int)rx_message_header.var_id << " h6 : " << (int)rx_message_header.type_id;
	cout << " data : " << (int)rx_message_data.data1.uint64;
	cout << " CS 1 : " << (int)rx_message_checksum.cs1 << " CS 2 : " << (int)rx_message_checksum.cs2;

	if (rx_message_header.h1 != 0xFF && rx_message_header.h2 != 0xFF)
		result = false;
	//if (che)

}



void Serialus::traitement_reception()
{
	/*
	if (_buffer_reception.size() > 0)
	{
		if (_buffer_reception[0] == "list")
		else if (_buffer_reception[0] == "d")
		else if (_buffer_reception[0] == "de")
		else if (_buffer_reception[0] == "deplacement")
		else if (_buffer_reception[0] == "s")
		else if (_buffer_reception[0] == "set")
		else if (_buffer_reception[0] == "g")
		else if (_buffer_reception[0] == "get")
		else if (_buffer_reception[0] == "clc")
		else if (_buffer_reception[0] == "clear")
		else
		{
			print_non_reconnu(0);
		}
	}*/


}


/**
*  Calcul du checksum basé sur un algorithme de Fletcher 16 bits.
*/
_fletcher_checksum Serialus::calcul_checksum(char * message, size_t size_message)
{
	uint16_t sum1 = 0;
	uint16_t sum2 = 0;
	uint8_t index;
	_fletcher_checksum checksum;

	/*
	for (index = 2 ; index < sizeof(message_a_envoyer) - sizeof(tx_message_checksum) ; ++index)
	{
		sum1 = (uint16_t) ((sum1 + message_a_envoyer[index]) % 255);
		sum2 = (uint16_t) ((sum2 + sum1) % 255);
	}
	tx_message_checksum.cs1 = sum1;
	tx_message_checksum.cs2 = sum2;
	cout << " CS 1 : " << sum1 << " CS 2 : " << sum2;*/

	for (index = 2; index < size_message - sizeof(_fletcher_checksum); ++index)
	{
		sum1 = (uint16_t)((sum1 + message[index]) % 255);
		sum2 = (uint16_t)((sum2 + sum1) % 255);
	}
	checksum.cs1 = sum1;
	checksum.cs2 = sum2;
	cout << " CS 1 : " << sum1 << " CS 2 : " << sum2;
	return checksum;
}



void Serialus::print_non_reconnu(uint8_t nb)
{
	cout << " /!\\ Error : \"" << _buffer_reception[nb] << "\" non reconnu ! /!\\";
}

void Serialus::print_logo()
{
	cout << endl;
	cout << endl << "   d888888o.   8 8888888888   8 888888888o.    8 8888          .8.          8 8888      8 8888      88    d888888o.   ";
	cout << endl << " .`8888:' `88. 8 8888         8 8888    `88.   8 8888         .888.         8 8888      8 8888      88  .`8888:' `88.";
	cout << endl << " 8.`8888.   Y8 8 8888         8 8888     `88   8 8888        :88888.        8 8888      8 8888      88  8.`8888.   Y8";
	cout << endl << " `8.`8888.     8 8888         8 8888     ,88   8 8888       . `88888.       8 8888      8 8888      88  `8.`8888.    ";
	cout << endl << "  `8.`8888.    8 888888888888 8 8888.   ,88'   8 8888      .8. `88888.      8 8888      8 8888      88   `8.`8888.   ";
	cout << endl << "   `8.`8888.   8 8888         8 888888888P'    8 8888     .8`8. `88888.     8 8888      8 8888      88    `8.`8888.  ";
	cout << endl << "    `8.`8888.  8 8888         8 8888`8b        8 8888    .8' `8. `88888.    8 8888      8 8888      88     `8.`8888. ";
	cout << endl << "8b   `8.`8888. 8 8888         8 8888 `8b.      8 8888   .8'   `8. `88888.   8 8888      ` 8888     ,8P 8b   `8.`8888.";
	cout << endl << "`8b.  ;8.`8888 8 8888         8 8888   `8b.    8 8888  .888888888. `88888.  8 8888        8888   ,d8P  `8b.  ;8.`8888";
	cout << endl << " `Y8888P ,88P' 8 888888888888 8 8888     `88.  8 8888 .8'       `8. `88888. 8 888888888888 `Y88888P'    `Y8888P ,88P'";
	cout << endl << "\t          _   __                  _____  _            _____               ______ _____          ";
	cout << endl << "\t         | | / /                 |_   _|| |          |  _  |             |___  /|  _  |          ";
	cout << endl << "\t ______  | |/ /   __ _   ___   ___ | |  | |__    ___ | | | | _ __  _   _    / /  \\ \\ /   ______  ";
	cout << endl << "\t|______| |    \\  / _` | / _ \\ / __|| |  | '_ \\  / _ \\| | | || '__|| | | |  / /   / _ \\  |______|";
	cout << endl << "\t         | |\\  \\| (_| || (_) |\\__ \\| |  | | | ||  __/\\ \\_/ /| |   | |_| |./ /   | |_| |         ";
	cout << endl << "\t         \\_| \\_/ \\__,_| \\___/ |___/\\_/  |_| |_| \\___| \\___/ |_|    \\__, |\\_/    \\_____/         ";
	cout << endl << "\t                                                                    __/ |                       ";
	cout << endl << "\t                                                                   |___/                        ";
	cout << endl;
}