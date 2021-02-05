/**
 *	@file		gameSDL.h
 *
 *	@author		TEXIER Aurélien
 *
 *	@version	1.0
 *
 *	@brief		Fichier gérant un jeu d'innondation dans un graphe en SDL.
 *
 *	@details	Ce fichier déclare les structures et fonctions utiles à la gestion de la SDL pour créer un jeu d'innondation dans un graphe.
 */

#ifndef _GAME_SDL_H_
#define _GAME_SDL_H_

#include "game.h"
#include "SDL\SDL.h"

/**
*	@brief	Structure représentant le jeu en SDL.
*/
typedef struct sGameSDL
{
	game *			m_game;			/**< Jeu. @see game.													*/
	int				m_res;			/**< Résolution des cases du jeu.										*/
	SDL_Surface	*	m_pWindow;		/**< Fenêtre SDL permettant l'affichage du jeu.							*/
}gameSDL;

/**
 *	@brief	Fonction permettant de créer le jeu en SDL.
 *			La fonction crée la fenêtre SDL.
 *
 *	@param	p_size		Taille du plateau.
 *	@param	p_res		Résolution des cases du jeu.
 *	@param	p_nbColor	Nombre de couleur utiliser pour la partie.
 *
 *	@return	Le jeu SDL.
 */
gameSDL * CreateGameSDL(unsigned char p_size, int p_res, int p_nbColor);

/**
 *	@brief	Fonction permettant de recharger le jeu en SDL.
 *			(Attention aux fuites mémoires)
 *
 *	@param	p_gameSDL	Jeu en SDL.
 */
void ReloadGameSDL(gameSDL ** p_gameSDL);

/**
*	@brief	Fonction permettant de "dessiner" le jeu en SDL.
*
*	@param	p_gameSDL	Jeu en SDL.
*/
void DrawGameSDL(gameSDL * p_gameSDL);

/**
*	@brief Fonction permettant de libérer le jeu SDL.
*
*	@param	p_game		Jeu SDL à désallouer.
*/
void FreeGameSDL(gameSDL * p_gameSDL);

int ClickColor(int m_posX, int m_posY, gameSDL * p_gameSDL);

void PrintScore(gameSDL * p_gameSDL, int scoreIA, int scorePlayer);
void PrintGrade(int p_scoreIA);

#endif
