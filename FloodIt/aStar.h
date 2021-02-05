/**
 *	@file		gameSDL.h
 *
 *	@author		TEXIER Aurélien
 *
 *	@version	1.0
 *
 *	@brief		Fichier gérant l'application de l'algorithme A* dans un jeu d'innondation dans un graphe.
 *
 *	@details	Ce fichier déclare les structures et fonctions utiles au développement de l'algorithme A* pour la résolution du jeu d'innondation dans un graphe.
 */

#ifndef _ASTAR_H_
#define _ASTAR_H_

#include "game.h"

/**
*	@brief	Structure représentant les listes utilisées par l'algorithme A*.
*/
typedef struct sAStar
{
	int					m_g;			/**< Distance déjà parcourue entre l'état initial et l'état courant.							*/
	int					m_f;			/**< Evaluation de la distance à parcourir entre l'état initial et l'état final (g + h).		*/
	board *				m_board;		/**< Plateau du jeu réduit dans le noeud courant.												*/
	unsigned int		m_color;		/**< Dernière couleur appliquée pour arriver à ce plateau.										*/
	struct sAStar *		m_prev;			/**< Pointeur vers le noeud correspondant au plateau précédent dans la solution.				*/
	struct sAStar *		m_next;			/**< Pointeur vers le noeud suivant (liste simplement chainée)									*/
}aStar;

/**
*	@brief	Fonction permettant de créer (allouer) un noeud de liste utilisée pour A*.
*
*	@param	p_board		Plateau réduit du jeu.
*	@param	p_g			Distance déjà parcourue entre l'état initial et l'état courant
*	@param	p_f			Evaluation de la distance à parcourir entre l'état initial et l'état final (g + h).
*	@param	p_color		Dernière couleur appliquée pour arriver à ce plateau.
*	@param	p_prev		Pointeur vers le noeud correspondant au plateau précédent dans la solution.
*
*	@return	Le noeud de liste alloué et initialisé.
*/
aStar * CreateNodeAStar(board * p_board, int p_g, int p_f, unsigned int p_color, aStar * p_prev);

/**
*	@brief	Fonction permettant d'insérer un noeud dans un liste de manière triée ou non.
*
*	@param	p_list		Liste utilisée pour A*.
*	@param	p_node		Noeud à insérer dans la liste.
*	@param	p_tri		1 si l'insertion doit se ffaire de manière trié, 0 sinon.
*/
void InsertNodeAStar(aStar ** p_list, aStar * p_node, int p_tri);

/**
*	@brief	Fonction permettant de récupérer la tête d'une liste utilisée pour A*.
*
*	@param	p_list		Pointeur sur la liste pour A*.
*
*	@return Pointeur sur le noeud de liste extrait.
*/
aStar * PopListAStar(aStar ** p_list);

/**
 *	@brief	Fonction permettant de savoir si un plateau se trouve dans une liste A*.
 *
 *	@param	p_list		Liste A*.
 *	@param	p_board		Plateau à chercher.
 *
 *	@return Noeud dans lequel on trouve le plateau passé en paramètre (pointeur sur le pointeur dans la liste).
 */
aStar ** IsInList(aStar ** p_list, board * p_board);

/**
 *	@brief	Fonction de résolution du jeu en utilisant l'algorithme A*.
 *
 *	@param	p_board				Plateau à résoudre (plateau réduit).
 *	@param	p_colors[OUT]		Tableau des couleurs de la solution.
 *	@param	p_nbColors[OUT]		Nombre de couleurs dans la solution.
 *
 *	@return	 1	si le plateau a été résolu,
 *			-1	si une erreur est sruvenue,
 *			 0	sinon.
 */
int SolveGame(board * p_board, unsigned int ** p_colors, unsigned int * p_nbColors);

/**
*	@brief	Fonction de calcul de l'heuristique pour l'algorithme A*.
*
*	@param	p_board			Plateau du jeu.
*
*	@return	Valeur de l'heuristique.
*/
int H(board * p_board);
void FreeAStarList(aStar **p_list);
#endif
