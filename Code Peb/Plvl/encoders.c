/** \file encoders.c
 *  \brief Implementation for encoders HAL
 */

#include "usr.h"

static int32_t value_codeur[2] = {0};
static int32_t value_old[2] = {0};
int32_t overflow[2] = {-1};

/* Initialisation of encoders, variables */
void encoders_invaders_init(void)
{
	POS1CNT = POS2CNT = 0;
	QEI1CONbits.QEIM = 0b111; //signal x4 + dépassement avec MAXxCNT
	MAX1CNT = 0xFFFF;
	QEI1CONbits.SWPAB = 0; //sens d'avance du codeur
	QEI1CONbits.PCDOUT = 0;//pas de sortie sur UPDNx

	DFLT1CONbits.QEOUT = 1; //Activation du filtrage en entrée
	DFLT1CONbits.QECK = 0;  //division par 1

	QEI2CON = QEI1CON; //même config pour les deux codeurs
	MAX2CNT = MAX1CNT;
	DFLT2CON = DFLT1CON;
	QEI2CONbits.SWPAB = 0; 

#if 0
	/* Encodeurs */
	RPINR14bits.QEA1R = QUAD_ENCODER_1_PIN_A;
	RPINR14bits.QEB1R = QUAD_ENCODER_1_PIN_B;
	
	RPINR16bits.QEA2R = QUAD_ENCODER_2_PIN_A;
	RPINR16bits.QEB2R = QUAD_ENCODER_2_PIN_B;
#endif

	// Init value of encoder registers
	encoders_invaders_set_value( LEFT_ENCODER_EXT, 0 );
	encoders_invaders_set_value( RIGHT_ENCODER_EXT, 0 );
}

/** Get an encoder value.
 * \param number : (void*) defining right/left encoder to get
 */
int32_t encoders_invaders_get_value(void * number)
{
	int32_t res, value;
	uint8_t flags;

	IRQ_LOCK(flags);

	// Coupure des it QEI
	IEC3bits.QEI1IE = 0;
	IEC4bits.QEI2IE = 0;

	const int16_t num = (int16_t) number;

	// Adaptation de la valeur selon le sens
	value = num ? (RIGHT_ENCODER_DIR * (int32_t) POS2CNT) : (LEFT_ENCODER_DIR * (int32_t) POS1CNT);

	res = value - value_old[num];
	if( overflow[num] != -1 )
	{
		int32_t nb_imp = num ? (RIGHT_ENCODER_DIR * IMPS) : (LEFT_ENCODER_DIR * IMPS);
		if( overflow[num] == 0 )
			res -= nb_imp;
		else if( overflow[num] == 1 )
			res += nb_imp;
		overflow[num] = -1; // PAS_OVERFLOW
	}
	value_codeur[num] += res;
	value_old[num] = value;

	res = value_codeur[num];

	IRQ_UNLOCK(flags);
	// Réactivation Interruptions QEI
	IEC3bits.QEI1IE = 1;
	IEC4bits.QEI2IE = 1;

	return res;
}

/** Set an encoder value
 * \param number : (void*) defining right/left encoder to set
 * \param v      : the value
 */
void encoders_invaders_set_value( void * number, int32_t val )
{
	uint8_t flags;
	int num = (int) number;
	
	IRQ_LOCK(flags);
	
	value_codeur[num] = value_old[num] = (int) val;
	overflow[num] = PAS_OVERFLOW;

	if (num == 0)
		POS1CNT = LEFT_ENCODER_DIR * val;
	else 
		POS2CNT = RIGHT_ENCODER_DIR * val;

	IRQ_UNLOCK(flags);
}
