#include "gameSDL.h"

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
gameSDL * CreateGameSDL(unsigned char p_size, int p_res, int p_nbColor)
{
	gameSDL * result;

	// On alloue la mémoire.
	result = (gameSDL*)malloc(sizeof(gameSDL));	

	// On crée le jeu.
	result->m_game = CreateGame(p_size, p_nbColor);

	// On crée la fenêtre SDL
	result->m_pWindow = SDL_SetVideoMode(p_size*p_res, p_size*p_res, 32, SDL_DOUBLEBUF | SDL_HWSURFACE);
	SDL_WM_SetCaption("Flood-it", "Flood-it !!!");
	SDL_WM_SetIcon(NULL, NULL);
	result->m_res = p_res;
	return result;
}

/**
 *	@brief	Fonction permettant de recharger le jeu en SDL.
 *			(Attention aux fuites mémoires)
 *
 *	@param	p_gameSDL	Jeu en SDL.
 */
void ReloadGameSDL(gameSDL ** p_gameSDL)
{
	unsigned char size = 0, nbColor = 0;

	// Test pour vérifier que les données en paramètres sont valides.
	if (!p_gameSDL || !(*p_gameSDL))
		return;


	// Si un jeu existe déjà ...
	if ((*p_gameSDL)->m_game)
	{
		// ... on conserve les données du jeu précédent ...
		nbColor = (*p_gameSDL)->m_game->m_nbColor;
		size = (*p_gameSDL)->m_game->m_size;

		// ... puis on libère ce jeu.
		FreeGame((*p_gameSDL)->m_game);
	}

	// On crée un nouveau jeu.
	(*p_gameSDL)->m_game = CreateGame(size, nbColor);
}

/**
 *	@brief	Fonction permettant de "dessiner" le jeu en SDL.
 *
 *	@param	p_gameSDL	Jeu en SDL.
 */
void DrawGameSDL(gameSDL * p_gameSDL)
{
	// Element qui vas nous servir à nous déplacer dans la fenêtre pour dessiner.
	SDL_Rect rect;

	// Test pour vérifier que les données en paramètres sont valides.
	if (!p_gameSDL || !p_gameSDL->m_pWindow || !p_gameSDL->m_game || !p_gameSDL->m_game->m_board)
		return;

	// TODO ...
	// On dessine le plateau case par case.
	int i, j;
	int index = 0;
	rect.h = p_gameSDL->m_res;
	rect.w = p_gameSDL->m_res;
	rect.x = 0;
	rect.y = 0;
	for (i = 0; i < p_gameSDL->m_game->m_size; i++)
	{
		for (j = 0; j < p_gameSDL->m_game->m_size; j++)
		{
			int r = p_gameSDL->m_game->m_colors[p_gameSDL->m_game->m_board->m_graph[index]->m_colorIndex].r;
			int g = p_gameSDL->m_game->m_colors[p_gameSDL->m_game->m_board->m_graph[index]->m_colorIndex].g;
			int b = p_gameSDL->m_game->m_colors[p_gameSDL->m_game->m_board->m_graph[index]->m_colorIndex].b;
			SDL_FillRect(p_gameSDL->m_pWindow, &rect, SDL_MapRGB(p_gameSDL->m_pWindow->format, r, g, b));
			rect.x += p_gameSDL->m_res;
			index++;
		}
		rect.y += p_gameSDL->m_res;
		rect.x = 0;
	}

	// On met à jour la fenêtre complète
	SDL_UpdateRect(p_gameSDL->m_pWindow, 0, 0, 0, 0);
}

/**
 *	@brief Fonction permettant de libérer le jeu SDL.
 *
 *	@param	p_game		Jeu SDL à désallouer.
 */
void FreeGameSDL(gameSDL * p_gameSDL)
{
	// Test pour vérifier que les données en paramètres sont valides.
	if (!p_gameSDL)
		return;

	// On libère la fenêtre SDL
	if (p_gameSDL->m_pWindow)
		SDL_FreeSurface(p_gameSDL->m_pWindow);

	// On libère le jeu
	if (p_gameSDL->m_game)
		FreeGame(p_gameSDL->m_game);

	// On libère finalement le jeu SDL
	free(p_gameSDL);
}

int ClickColor(int m_posX, int m_posY, gameSDL * p_gameSDL)
{
	int i = 1;
	int indice;
	int x, y;
	x = m_posX / p_gameSDL->m_res;
	y = m_posY / p_gameSDL->m_res;
	indice = y * (p_gameSDL->m_pWindow->h/p_gameSDL->m_res) + x;
	return p_gameSDL->m_game->m_board->m_graph[indice]->m_colorIndex;
}

void PrintScore(gameSDL * p_gameSDL, int scoreIA, int scorePlayer)
{
	if (!p_gameSDL || !p_gameSDL->m_pWindow || !p_gameSDL->m_game || !p_gameSDL->m_game->m_board)
		return;
	SDL_Surface *star = NULL;
	SDL_Surface *fondStar = NULL;
	SDL_Surface *win = NULL;
	SDL_Surface *lose = NULL;
	star = IMG_Load("star.png");
	fondStar = IMG_Load("starBack.png");
	win = IMG_Load("win.png");
	lose = IMG_Load("lose.png");
	SDL_Rect rect;
	SDL_Rect text;
	rect.x = (p_gameSDL->m_pWindow->h / 2) - ((star->h) + 40);
	rect.y = (p_gameSDL->m_pWindow->h / 2) - (star->h / 2);
	text.x = rect.x - 10;
	text.y = rect.y - 60;

	SDL_BlitSurface(fondStar, NULL, p_gameSDL->m_pWindow, &rect);
	rect.x += star->h + 10;
	SDL_BlitSurface(fondStar, NULL, p_gameSDL->m_pWindow, &rect);
	rect.x += star->h + 10;
	SDL_BlitSurface(fondStar, NULL, p_gameSDL->m_pWindow, &rect);

	if (scorePlayer <= scoreIA)
	{
		rect.x = (p_gameSDL->m_pWindow->h / 2) - ((star->h) + 40);
		rect.y = (p_gameSDL->m_pWindow->h / 2) - (star->h / 2);
		SDL_BlitSurface(star, NULL, p_gameSDL->m_pWindow, &rect);
		rect.x += star->h + 10;
		SDL_BlitSurface(star, NULL, p_gameSDL->m_pWindow, &rect);
		rect.x += star->h + 10;
		SDL_BlitSurface(star, NULL, p_gameSDL->m_pWindow, &rect);
		SDL_BlitSurface(win, NULL, p_gameSDL->m_pWindow, &text);
	}
	else if(scorePlayer >= scoreIA+1 && scorePlayer <= scoreIA+5)
	{
		rect.x = (p_gameSDL->m_pWindow->h / 2) - ((star->h) + 40);
		rect.y = (p_gameSDL->m_pWindow->h / 2) - (star->h / 2);
		SDL_BlitSurface(star, NULL, p_gameSDL->m_pWindow, &rect);
		rect.x += star->h + 10;
		SDL_BlitSurface(star, NULL, p_gameSDL->m_pWindow, &rect);
		SDL_BlitSurface(win, NULL, p_gameSDL->m_pWindow, &text);
	}
	else if (scorePlayer >= scoreIA + 6 && scorePlayer <= scoreIA + 12)
	{
		rect.x = (p_gameSDL->m_pWindow->h / 2) - ((star->h) + 40);
		rect.y = (p_gameSDL->m_pWindow->h / 2) - (star->h / 2);
		SDL_BlitSurface(star, NULL, p_gameSDL->m_pWindow, &rect);
		SDL_BlitSurface(win, NULL, p_gameSDL->m_pWindow, &text);
	}
	else
	{
		SDL_BlitSurface(lose, NULL, p_gameSDL->m_pWindow, &text);
	}

	SDL_Flip(p_gameSDL->m_pWindow);
	SDL_FreeSurface(win);
	SDL_FreeSurface(lose);
	SDL_FreeSurface(star);
	SDL_FreeSurface(fondStar);
}

void PrintGrade(int p_scoreIA)
{
	if (!p_scoreIA)
		return;
	printf("----- PALIERS DE NIVEAUX POUR CE PLATEAU ------\n");
	printf("| Expert       : %2d mouvement(s) ou moins.   |\n", p_scoreIA);
	printf("| Interediaire : entre %2d et %2d mouvement(s) |\n", p_scoreIA + 1, p_scoreIA + 5);
	printf("| Debutant     : entre %2d et %2d mouvement(s) |\n", p_scoreIA + 6, p_scoreIA + 12);
	printf("-----------------------------------------------\n");
	printf("Appuyez sur \"+\" pour afficher la solution.\n");
}

