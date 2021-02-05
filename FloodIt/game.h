/**
 *	@file		game.h
 *
 *	@author		TEXIER Aurélien
 *
 *	@version	1.0
 *
 *	@brief		Fichier gérant le jeu d'innondation dans un graphe.
 *
 *	@details	Ce fichier déclare les structures et fonctions utiles pour créer un jeu d'innondation dans un graphe.
 */

#ifndef _GAME_H_
#define _GAME_H_

#include "utils.h"

#define NB_MAX_COLOR		6

						////////////////////////////////////
						///     STRUCTURES UTILISEES     ///
						////////////////////////////////////

typedef struct sNode node;

/**
 *	@brief	Structure représentant les listes d'adjacences.
 */
typedef struct sList
{
	node *			m_node;				/**< Pointeur vers le noeud voisin.										*/
	struct sList *	m_next;				/**< Pointeur vers le noeud de liste suivant.							*/
}list;

/**
 *	@brief	Structure représentant un noeud de graphe.
 */
struct sNode
{
	char			m_colorIndex;		/**< Le numéro de la couleur appliquer sur le noeud.					*/

	char			m_nbNeighbours;		/**< Le nombre de voisin du noeud.										*/
	list *			m_neighbours;		/**< La liste des voisins (Liste d'adjacence).							*/

	int				m_visited;			/**< Flag pouvant servir à savoir si un noeud a été parcouru.			*/
};

/**
 *	@brief	Structure représentant un plateau de jeu.
 */
typedef struct sBoard
{
	node **			m_graph;			/**< Tableau contenant les noeuds du graphe (tableau à 1 dimension)		*/
	unsigned int	m_size;				/**< Nombre de noeud composant le graphe.								*/
}board;

/**
 *	@brief	Structure représentant le jeu.
 */
typedef struct sGame
{
	board *			m_board;			/**< Plateau du jeu.													*/
	board *			m_reducedBoard;		/**< Plateau du jeu après réduction.									*/
	
	unsigned char	m_size;			    /**< Taille du jeu.														*/

	unsigned char	m_nbColor;			/**< Nombre de couleurs utilisées dans la partie.						*/
	color *			m_colors;			/**< Liste des couleurs utilisées.										*/

	unsigned char	m_nbMoves;			/**< Nombre de mouvements effectués par le joueur.						*/
} game;



				////////////////////////////////////////////////////
				///         FONCTIONS CONCERNANT LE JEU          ///
				////////////////////////////////////////////////////

/**
 *	@brief	Fonction permettant d'allouer et d'initialiser le jeu.
 *	
 *	@param	p_size	Taille du plateau.
 *	@param	p_nbColor	Nombre de couleur utiliser pour la partie.
 *
 *	@return	Le jeu alloué et initialisé.
 */
game * CreateGame(unsigned char p_size, unsigned char p_nbColor);

/**
 *	@brief Fonction permettant de libérer le jeu.
 *	
 *	@param	p_game		Jeu à désallouer. 
 */
void FreeGame(game * p_game);



				////////////////////////////////////////////////////
				///        FONCTIONS CONCERNANT LE PLATEAU       ///
				////////////////////////////////////////////////////

/**
 *	@brief	Fonction permettant de réduire un plateau.
 *			Le résultat est envoyé via un paramètre sortant passé par adresse qui est alloué dans la fonction (Attention aux fuites mémoires).
 *
 *	@param	p_boardIn[IN]		Plateau à réduire.
 *	@param	p_boardOut[OUT]		Plateau réduit.
 */
int ReduceBoard(board * p_boardIn, board ** p_boardOut);

/**
 *	@brief	Fonction permettant de copier un plateau.
 *
 *	@param	p_board			Plateau à copier.
 *
 *	@return	Le plateau copié
 */
board * CopyBoard(board * p_board);

/**
 *	@brief	Fonction permettant de comparer deux plateaux.
 *
 *	@return	1 si les plateau sont identiques,
 *			0 sinon.
 */
int SameBoards(board * p_board1, board * p_board2);

/**
 *	@brief	Fonction permettant d'appliquer une couleur au plateau.
 *
 *	@param	p_board			Pointeur sur le plateau.
 *	@param	p_colorIndex	Couleur à appliquer sur le plateau.
 */
void ApplyColor(board ** p_board, unsigned char p_colorIndex);

/**
 *	@brief	Fonction permettant de savoir si le plateau est fini.
 *			La fonction se basera sur un plateau réduit.
 *
 *	@param	p_board			Plateau réduit.
 *
 *	@return	1 si le plateau est fini
 *			0 sinon
 */
int IsFinished(board * p_board);

/**
 *	@brief	Fonction permettant de libérer un plateau.
 *	
 *	@param	p_board		Plateau à désallouer. 
 */
void FreeBoard(board * p_board);



				////////////////////////////////////////////////////
				///        FONCTIONS CONCERNANT LE GRAPHE        ///
				////////////////////////////////////////////////////

/**
 *	@brief	Fonction permettant de vérifier si deux noeuds de graphe sont voisins.
 *	
 *	@return	1 si les noeuds sont voisins,
 *			0 sinon.
 */
int IsNeighbours(node * p_node1, node *p_node2);

/**
 *	@brief	Fonction permettant de libérer un graphe.
 *	
 *	@param	p_graph		Graphe à désallouer. 
 */
void FreeGraph(node ** p_graph, unsigned int p_size);



				////////////////////////////////////////////////////
				/// FONCTIONS CONCERNANT LES LISTES D'ADJACENCES ///
				////////////////////////////////////////////////////

/**
 *	@brief	Fonction permettant d'ajouter un noeud à une liste d'adjacence (Fonction récursive).
 *
 *	@param	p_list		Liste d'adjacence.
 *	@param	p_node		Noeud de liste à ajouter.
 *
 *	@return	liste d'adjacence modifier.
 */
list * AddNodeList(list * p_list, node * p_node);

/**
 *	@brief	Fonction permettant de récupérer la tête de la liste d'adjacence.
 *
 *	@param	p_list		Pointeur sur la liste d'adjacence.
 *
 *	@return Pointeur sur le noeud de liste extrait.
 */
list * PopList(list ** p_list);

/**
 *	@brief	Fonction permettant de libérer une liste d'adjacence.
 *
 *	@param	p_list		Liste d'adjacence à désallouer.
 */
void FreeList(list * p_list);

#endif
