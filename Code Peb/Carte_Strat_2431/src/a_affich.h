/*****************************************************************************
* 
******************************************************************************
* Nom Fichier 	:	a_affich.h
* Operating System:	Pas d'OS, Fichiers "maison" pour dsPIC
* Compilateur	:	MPLAB 8.10 et Microchip C30 student v3.02
* IDE		:	Notepad++
******************************************************************************
* Auteurs 		:	Benoît LINCK / David SILVA PIRES  
* Date Création 	:	2007
* Date Révision 	:	11/07/2008
******************************************************************************
* Description	:	- Fonctions d'affichage debug par liaison série.
******************************************************************************/


/** putch : affiche un caractère sur la liaison série
 * @param : unsigned char byte : un caractère
 */
void putch (unsigned char byte);

/** os_puts: affiche une chaine de caractère sur la liaison série
 * @param : const char *s : chaine
 */
void puts (const char *s);

/** i5toc : converti un nombre sur 5 digits max
 * @param : unsigned char nb : le nombre
 */
///int i5toc (unsigned int nb);

/** i5toc : converti 5 digits max en un int
 * @param : unsigned char dig0 : le digit 0
 * @param : unsigned char dig1 : le digit 1
 * @param : unsigned char dig2 : le digit 2
 * @param : unsigned char dig3 : le digit 3
 * @param : unsigned char dig4 : le digit 4
 */
///int c5toi(unsigned char dig0, unsigned char dig1, unsigned char dig2, unsigned char dig3, unsigned char dig4);

///void affichage_digits(int x);

///void affichage_float(float x);

//void affichage_nouvelle_ligne(void);

//void as_dbg_fin_init_var_glob_mst(void);


//int _i5toc(unsigned int nb);
//void _affichage_digits(int x, int nb_digit);
//void _affichage_float(float x);
void _putch(unsigned char byte) ;
void _puts(const char *s);


