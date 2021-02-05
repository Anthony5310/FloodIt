#include "gameSDL.h"
#include "aStar.h"

#define	BOARD_SIZE	20
#define	RESOLUTION	20

int main(int argc, char ** argv)
{
	int end = 0;
	int solutionLoad = 0;
	gameSDL * g;
	board * b = NULL;
	unsigned int *colors = NULL, nbColor = 0;
	int nbMoves = 0;
	long clockTime;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER))
	{
		system("pause");
		return EXIT_FAILURE;
	}

	srand((unsigned int)time(NULL));

	g = CreateGameSDL(BOARD_SIZE, RESOLUTION, NB_MAX_COLOR);
	DrawGameSDL(g);
	printf("----- INFORMATION PARTIE -----\n\n");
	printf("Confrontez vous a un ordinateur en appuyant sur ENTER.\n");

	while (!end)
	{
		SDL_Event e;		

		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_KEYUP:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					end = 1;
					break;

				case SDLK_SPACE:
					ReloadGameSDL(&g);
					DrawGameSDL(g);
					nbMoves = 0;
					break;
				case SDLK_RETURN:
				case SDLK_KP_ENTER:
					if (g->m_game)
					{
						b = (board *)calloc(1, sizeof(board));
						colors = NULL;
						nbColor = 0;
						
						if (colors)
						{
							free(colors);
						}
							
						printf("Recherche en cours ...\n");

						ReduceBoard(g->m_game->m_board, &b);
						clockTime = clock();
						SolveGame(b, &colors, &nbColor);

						printf("Solved in %d moves in %.3fs.\n", nbColor, (float)(clock() - clockTime) / 1000.f);
						PrintGrade(nbColor);
						solutionLoad = 1;
						
					}
					break;
				case SDLK_KP_PLUS:
					if (colors)
					{
						for (unsigned int i = 0; i < nbColor; i++)
							printf("\t - %d\n", colors[i]);

						free(colors);
						//colors = NULL;
					}
					else
					{
						printf("Appuyez sur ENTER pour charger une solution.\n");
					}
					
					break;
				}
				break;

			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_KP0:
				case SDLK_KP1:
				case SDLK_KP2:
				case SDLK_KP3:
				case SDLK_KP4:
				case SDLK_KP5:
				case SDLK_KP6:
				case SDLK_KP7:
				case SDLK_KP8:
				case SDLK_KP9:
					if (g->m_game)
					{
						int choice = e.key.keysym.sym - SDLK_KP0;

						if (choice >= 0 && choice < g->m_game->m_nbColor)
						{
							ApplyColor(&(g->m_game->m_board), choice);
							FreeGraph(g->m_game->m_reducedBoard->m_graph, g->m_game->m_reducedBoard->m_size);
							ReduceBoard(g->m_game->m_board, &g->m_game->m_reducedBoard);
							nbMoves++;
						}

						if (IsFinished(g->m_game->m_reducedBoard))
						{
							printf("Victoire en %d mouvements.\n", nbMoves);
							DrawGameSDL(g);
							if (nbColor)
								PrintScore(g, nbColor, nbMoves);
						}
						else
							DrawGameSDL(g);
					}
					break;

				case SDLK_0:
				case SDLK_1:
				case SDLK_2:
				case SDLK_3:
				case SDLK_4:
				case SDLK_5:
				case SDLK_6:
				case SDLK_7:
				case SDLK_8:
				case SDLK_9:
					if (g->m_game)
					{
						int choice = e.key.keysym.sym - SDLK_0;

						if (choice >= 0 && choice < g->m_game->m_nbColor)
						{
							ApplyColor(&(g->m_game->m_board), choice);
							FreeGraph(g->m_game->m_reducedBoard->m_graph, g->m_game->m_reducedBoard->m_size);
							ReduceBoard(g->m_game->m_board, &g->m_game->m_reducedBoard);
							nbMoves++;
						}

						if (IsFinished(g->m_game->m_reducedBoard))
						{
							printf("Victoire en %d mouvements.\n", nbMoves);
							DrawGameSDL(g);
							if (nbColor)
								PrintScore(g, nbColor, nbMoves);
						}
						else
							DrawGameSDL(g);
					}
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP://permet de jouer en appliquant la couleur cliquée
				if (g->m_game)
				{
					int choiceClick = ClickColor(e.button.x, e.button.y, g);

					if (choiceClick >= 0 && choiceClick < g->m_game->m_nbColor)
					{
						ApplyColor(&(g->m_game->m_board), choiceClick);
						FreeGraph(g->m_game->m_reducedBoard->m_graph, g->m_game->m_reducedBoard->m_size);
						ReduceBoard(g->m_game->m_board, &g->m_game->m_reducedBoard);
						nbMoves++;
					}

					if (IsFinished(g->m_game->m_reducedBoard))
					{
						printf("Victoire en %d mouvements.\n", nbMoves);
						DrawGameSDL(g);
						if (nbColor)
							PrintScore(g, nbColor, nbMoves);
					}
					else
						DrawGameSDL(g);
				}
				break;
			case SDL_QUIT:
				end = 1;
				break;

			default:
				break;
			}
		}
		
		SDL_Delay(1);
	}
	if (colors)
		free(colors);
	FreeGameSDL(g);

	return EXIT_SUCCESS;
}