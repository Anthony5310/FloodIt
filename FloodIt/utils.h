#ifndef _UTILS_H_
#define _UTILS_H_

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/**
 *	@brief	Element représentant les couleurs.
 *			Il s'agit d'une union de deux structures afin de pouvoir facilement exprimer les couleurs au choix selon l'espace de couleur RGB (RVB en français) ou HSV (TSV en français)
 */
typedef union uColor
{
	struct
	{
		float	r;					/**< Composante rouge de la couleur (entier entre 0 et 255).											*/
		float	g;					/**< Composante verte de la couleur (entier entre 0 et 255).											*/
		float	b;					/**< Composante bleue de la couleur (entier entre 0 et 255).											*/
	};

	struct
	{
		float	h;					/**< Composante	représentant la teinte de la couleur (angle compris dans l'interval [0° ; 360°[).		*/
		float	s;					/**< Composante	représentant la saturation de la couleur (réel variant entre 0 et 100 %).				*/
		float	v;					/**< Composante	représentant la valeur de la couleur (réel variant entre 0 et 100 %).					*/
	};
}color;

/**
 *	@brief	Fonction permettant de créer une couleur selon l'espace de couleur RGB.
 *
 *	@param	p_red		Composante rouge de la couleur à créer.
 *	@param	p_green		Composante verte de la couleur à créer.
 *	@param	p_blue		Composante bleue de la couleur à créer.
 *	
 *	@return	La couleur créée.
 */
color CreateRGBColor(unsigned char p_red, unsigned char p_green, unsigned char p_blue);

/**
*	@brief	Fonction permettant de créer une couleur selon l'espace de couleur HSV.
*
*	@param	p_h		Teinte de la couleur à créer.
*	@param	p_s		Saturation de la couleur à créer.
*	@param	p_v		Valeur de la couleur à créer.
*
*	@return	la couleur créée.
*/
color CreateHSVColor(float p_hue, float p_saturation, float p_value);


/**
*	@brief	Fonction permettant de créer une couleur selon l'espace de couleur HSV à partir d'une couleur RGB.
*
*	@param	p_colorRGB		Couleur initiale au format RGB.
*
*	@return	La couleur correspondant à p_colorRGB mais dans l'espace HSV.
*/
color RGB2HSV(color p_colorRGB);

/**
*	@brief	Fonction permettant de créer une couleur selon l'espace de couleur RGB à partir d'une couleur HSV.
*
*	@param	p_colorHSV		Couleur initiale au format HSV.
*
*	@return	La couleur correspondant à p_colorHSV mais dans l'espace RGB.
*/
color HSV2RGB(color p_colorHSV);


/**
 *	@brief	Fonction permettant de calculer le "modulo" d'un nombre réel.
 *			La fonction retourne le résulat de "p_value mod p_n".
 */
float mod(float p_value, float p_n);

/**
*	@brief	Fonction renvoyant la valeur absolue d'un nombre réel.
*/
float absolute(float p_value);

#endif