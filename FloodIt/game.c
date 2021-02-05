#include "game.h"

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
game * CreateGame(unsigned char p_size, unsigned char p_nbColor)
{
	game * result = NULL;
	/*
	* Si proba = 200, plateau unicolor
	* 
	*/
	int proba = 80;
	int i;
	int boardSize = p_size * p_size;

	// TODO ...
	// 1. Allocation de la mémoire
	result = (game*)malloc(sizeof(game));
	result->m_board = (board*)malloc(sizeof(board));
	result->m_reducedBoard = (board*)malloc(sizeof(board));
	result->m_colors = (color*)malloc(sizeof(color) * p_nbColor);
	result->m_board->m_graph = (node**)malloc(sizeof(node*) * boardSize);
	for (i = 0; i < boardSize; i++)
	{
		result->m_board->m_graph[i] = (node*)malloc(sizeof(node));
		result->m_board->m_graph[i]->m_neighbours = NULL;
		result->m_board->m_graph[i]->m_nbNeighbours = 0;
		result->m_board->m_graph[i]->m_visited = -1;
	}
	
	// 2. Initialisation des données et des voisins
	color colorHSV;
	int color = 0;
	int pasColorGeneration = 255 / p_nbColor;
	for (i = 0; i < p_nbColor; i++)
	{	 
		result->m_colors[i] = HSV2RGB(CreateHSVColor(color, 0.8, 0.8));
		color += pasColorGeneration;
	}

	for (i = 0; i < boardSize; i++)
	{
		int alea = rand() % 100;
		if (alea < proba)
		{
			if (alea < proba / 2)
			{
				if (i - 1 >= 0)
				{
					result->m_board->m_graph[i]->m_colorIndex = result->m_board->m_graph[i - 1]->m_colorIndex;
				}
				else
				{
					result->m_board->m_graph[i]->m_colorIndex = rand() % p_nbColor;
				}
			}
			else
			{
				if (i - p_size >= 0)
					result->m_board->m_graph[i]->m_colorIndex = result->m_board->m_graph[i - p_size]->m_colorIndex;
				else
					result->m_board->m_graph[i]->m_colorIndex = rand() % p_nbColor;
			}
				
		}
		else
		{
			alea = rand() % p_nbColor;
			result->m_board->m_graph[i]->m_colorIndex = alea;
		}			
	}

	result->m_size = p_size;
	result->m_nbColor = p_nbColor;
	result->m_nbMoves = 0;
	result->m_board->m_size = p_size * p_size;

	for (i = 0; i < boardSize; i++)
	{
		if (i - 1 >= 0 && (i - 1) % p_size < i%p_size && i - 1 >= 0)
		{
			result->m_board->m_graph[i]->m_neighbours = AddNodeList(result->m_board->m_graph[i]->m_neighbours, result->m_board->m_graph[i - 1]);
			result->m_board->m_graph[i]->m_nbNeighbours++;
		}
		if (i >= p_size)
		{
			result->m_board->m_graph[i]->m_neighbours = AddNodeList(result->m_board->m_graph[i]->m_neighbours, result->m_board->m_graph[i - p_size]);
			result->m_board->m_graph[i]->m_nbNeighbours++;
		}
		if (i + 1 < boardSize && (i + 1) % p_size > i%p_size)
		{
			result->m_board->m_graph[i]->m_neighbours = AddNodeList(result->m_board->m_graph[i]->m_neighbours, result->m_board->m_graph[i + 1]);
			result->m_board->m_graph[i]->m_nbNeighbours++;
		}
		if (i + p_size < boardSize)
		{
			result->m_board->m_graph[i]->m_neighbours = AddNodeList(result->m_board->m_graph[i]->m_neighbours, result->m_board->m_graph[i + p_size]);
			result->m_board->m_graph[i]->m_nbNeighbours++;
		}
	}
	result->m_reducedBoard->m_graph = NULL;

	return result;
}

/**
 *	@brief Fonction permettant de libérer le jeu.
 *
 *	@param	p_game		Jeu à désallouer.
 */
void FreeGame(game * p_game)
{
	if (!p_game)
		return;
	FreeBoard(p_game->m_board);
	FreeBoard(p_game->m_reducedBoard);
	free(p_game->m_colors);
	free(p_game);
	p_game = NULL;
}



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
int ReduceBoard(board * p_boardIn, board ** p_boardOut)
{
	// TODO ...
	// 1. Recherche du nombre de noeud du graphe réduit
	int i = 0;
	unsigned int nbSommetReducedGraph = 0;
	list * pile = NULL;
	list * currentNode = NULL;
	list * neighbours = NULL;
	for (i = 0; i < p_boardIn->m_size; i++)
	{
		if (p_boardIn->m_graph[i]->m_visited == -1)
		{
			p_boardIn->m_graph[i]->m_visited = nbSommetReducedGraph;
			pile = AddNodeList(pile, p_boardIn->m_graph[i]);
			while (pile)
			{
				currentNode = PopList(&pile);
				neighbours = currentNode->m_node->m_neighbours;
				while (neighbours)
				{
					if (neighbours->m_node->m_colorIndex == currentNode->m_node->m_colorIndex && neighbours->m_node->m_visited == -1)
					{
						neighbours->m_node->m_visited = nbSommetReducedGraph;
						pile = AddNodeList(pile, neighbours->m_node);
					}
					neighbours = neighbours->m_next;
				}
				FreeList(currentNode);
				currentNode = NULL;
			}
			nbSommetReducedGraph++;
		}
	}
	
	//printf("Nombre de zone: %d\n", nbSommetReducedGraph);
	
	// 2. Création du graphe réduit
	//Allocation et initialisation du graph Réduit
	unsigned char indexZoneColor;
	if (!(*p_boardOut))
		(*p_boardOut) = (board*)malloc(sizeof(board));
	(*p_boardOut)->m_graph = (node**)malloc(sizeof(node*) * nbSommetReducedGraph);
	(*p_boardOut)->m_size = nbSommetReducedGraph;
	for (i = 0; i < nbSommetReducedGraph; i++)
	{
		(*p_boardOut)->m_graph[i] = (node*)malloc(sizeof(node));
		(*p_boardOut)->m_graph[i]->m_nbNeighbours = 0;
		(*p_boardOut)->m_graph[i]->m_neighbours = NULL;
		(*p_boardOut)->m_graph[i]->m_visited = -1;
	}

	//Initialisation des couleurs du graph réduit
	for (i = 0; i < p_boardIn->m_size; i++)
	{
		(*p_boardOut)->m_graph[p_boardIn->m_graph[i]->m_visited]->m_colorIndex = p_boardIn->m_graph[i]->m_colorIndex;
	}
	for (i = 0; i < p_boardIn->m_size; i++)
	{
		neighbours = p_boardIn->m_graph[i]->m_neighbours;
		while (neighbours)
		{
			if (neighbours->m_node->m_colorIndex != p_boardIn->m_graph[i]->m_colorIndex)
			{
				if (!IsNeighbours((*p_boardOut)->m_graph[p_boardIn->m_graph[i]->m_visited], (*p_boardOut)->m_graph[neighbours->m_node->m_visited]))
				{				
					(*p_boardOut)->m_graph[p_boardIn->m_graph[i]->m_visited]->m_neighbours = AddNodeList((*p_boardOut)->m_graph[p_boardIn->m_graph[i]->m_visited]->m_neighbours, (*p_boardOut)->m_graph[neighbours->m_node->m_visited]);
					(*p_boardOut)->m_graph[p_boardIn->m_graph[i]->m_visited]->m_nbNeighbours++;
				}				
			}
			neighbours = neighbours->m_next;
		}
	}
	
	for (i = 0; i < p_boardIn->m_size; i++)
	{
		p_boardIn->m_graph[i]->m_visited = -1;
	}
	
	return 1;
}
/**
 *	@brief	Fonction permettant de copier un plateau.
 *
 *	@param	p_board			Plateau à copier.
 *
 *	@return	Le plateau copié
 */

board * CopyBoard(board * p_board)
{
	board * result = NULL;
	list * neighbours = NULL;

	result = (board*)malloc(sizeof(board));
	result->m_size = p_board->m_size;
	result->m_graph = (node**)malloc(sizeof(node*) * p_board->m_size);
	int i;
	for (i = 0; i < p_board->m_size; i++)
	{
		result->m_graph[i] = (node*)malloc(sizeof(node));
		result->m_graph[i]->m_neighbours = NULL;
		result->m_graph[i]->m_nbNeighbours = p_board->m_graph[i]->m_nbNeighbours;
		result->m_graph[i]->m_visited = -1;
		result->m_graph[i]->m_colorIndex = p_board->m_graph[i]->m_colorIndex;
	}
	for (i = 0; i < p_board->m_size; i++)
	{
		p_board->m_graph[i]->m_visited = i;
	}
	for (i = 0; i < p_board->m_size; i++)
	{
		neighbours = p_board->m_graph[i]->m_neighbours;
		while (neighbours)
		{
			result->m_graph[i]->m_neighbours = AddNodeList(result->m_graph[i]->m_neighbours, result->m_graph[neighbours->m_node->m_visited]);
			neighbours = neighbours->m_next;
		}
	}
	for (i = 0; i < p_board->m_size; i++)
	{
		p_board->m_graph[i]->m_visited = -1;
	}
	return result;
}

/**
 *	@brief	Fonction permettant de comparer deux plateaux.
 *
 *	@return	1 si les plateau sont identiques,
 *			0 sinon.
 */
int SameBoards(board * p_board1, board * p_board2)
{
	list * neighboursBoard1 = NULL;
	list * neighboursBoard2 = NULL;
	int wasFind = 0;
	if (p_board1->m_size != p_board2->m_size)
		return 0;
	int i;
	for (i = 0; i < p_board1->m_size; i++)
	{
		neighboursBoard1 = p_board1->m_graph[i]->m_neighbours;
		neighboursBoard2 = p_board2->m_graph[i]->m_neighbours;
		while (neighboursBoard2)
		{
			neighboursBoard2->m_node->m_visited = 0;
			neighboursBoard2 = neighboursBoard2->m_next;
		}
		while (neighboursBoard1)
		{
			neighboursBoard2 = p_board2->m_graph[i]->m_neighbours;
			while (neighboursBoard2 && !wasFind)
			{
				if (neighboursBoard2->m_node->m_colorIndex == neighboursBoard1->m_node->m_colorIndex && neighboursBoard2->m_node->m_visited == 0)
				{
					neighboursBoard2->m_node->m_visited = 1;
					wasFind = 1;
				}				
				neighboursBoard2 = neighboursBoard2->m_next;
			}
			if (!wasFind)
				return 0;
			wasFind = 0;
			neighboursBoard1 = neighboursBoard1->m_next;
		}
	}
	return 1;
}

/**
 *	@brief	Fonction permettant d'appliquer une couleur au plateau.
 *
 *	@param	p_board			Pointeur sur le plateau.
 *	@param	p_colorIndex	Couleur à appliquer sur le plateau.
 */
void ApplyColor(board ** p_board, unsigned char p_colorIndex)
{
	int i;
	list * pile = NULL;
	list * currentNode = NULL;
	list * neighbours;
	//les voisins ayant comme couleur, oldColor, devront être modifiés en p_colorIndex
	unsigned char oldColor = (*p_board)->m_graph[0]->m_colorIndex;
	
	//Si la couleur a remplacer est la même que la couleur déjà présente, on return
	if (p_colorIndex == oldColor)
		return;

	(*p_board)->m_graph[0]->m_colorIndex = p_colorIndex;
	pile = AddNodeList(pile, (*p_board)->m_graph[0]);
	while (pile)
	{
		currentNode = PopList(&pile);
		neighbours = currentNode->m_node->m_neighbours;

		//Tant que le noeud courant a des voisins
		while (neighbours)
		{
			//Si son voisin est de la meme couleur que lui
			if (neighbours->m_node->m_colorIndex == oldColor)
			{
				//On change sa couleur et on ajoute ses voisins dans la liste des noeuds à traiter (qui auront potentiellement une couleur à changer)
				neighbours->m_node->m_colorIndex = p_colorIndex;
				pile = AddNodeList(pile, neighbours->m_node);
			}

			neighbours = neighbours->m_next;
		}
		FreeList(currentNode);
	}	
}

/**
 *	@brief	Fonction permettant de savoir si le plateau est fini.
 *			La fonction se basera sur un plateau réduit.
 *
 *	@param	p_board			Plateau réduit.
 *
 *	@return	1 si le plateau est fini
 *			0 sinon
 */
int IsFinished(board * p_board)
{
	if (p_board->m_size == 1)
		return 1;
	return 0;
}

/**
 *	@brief	Fonction permettant de libérer un plateau.
 *
 *	@param	p_board		Plateau à désallouer.
 */
void FreeBoard(board * p_board)
{
	if (!p_board)
		return;
	FreeGraph(p_board->m_graph, p_board->m_size);
	free(p_board);
	p_board = NULL;
}



				////////////////////////////////////////////////////
				///        FONCTIONS CONCERNANT LE GRAPHE        ///
				////////////////////////////////////////////////////

/**
 *	@brief	Fonction permettant de vérifier si deux noeuds de graphe sont voisins.
 *
 *	@return	1 si les noeuds sont voisins,
 *			0 sinon.
 */
int IsNeighbours(node * p_node1, node *p_node2)
{
	if (!p_node1 || !p_node2)
		return 0;
	list * neighbours = p_node1->m_neighbours;
	while (neighbours)
	{
		if (neighbours->m_node == p_node2)
			return 1;
		neighbours = neighbours->m_next;
	}
	return 0;
}

/**
 *	@brief	Fonction permettant de libérer un graphe.
 *
 *	@param	p_graph		Graphe à désallouer.
 */
void FreeGraph(node ** p_graph, unsigned int p_size)
{
	if (!p_graph)
		return;
	if (!(*p_graph))
		return;
	int i;
	for (i = 0; i < p_size; i++)
	{
		FreeList(p_graph[i]->m_neighbours);
		free(p_graph[i]);
	}
	free(p_graph);
	p_graph = NULL;
}



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
list * AddNodeList(list * p_list, node * p_node)
{
	if (!p_list)
	{
		list * newNode = (list *)malloc(sizeof(list));
		newNode->m_next = NULL;
		newNode->m_node = p_node;
		return newNode;
	}

	p_list->m_next = AddNodeList(p_list->m_next, p_node);
	return p_list;
}

/**
 *	@brief	Fonction permettant de récupérer la tête de la liste d'adjacence.
 *
 *	@param	p_list		Pointeur sur la liste d'adjacence.
 *
 *	@return Pointeur sur le noeud de liste extrait.
 */
list * PopList(list ** p_list)
{
	list * result = NULL;

	if (!p_list || !*p_list)
		return NULL;

	result = *p_list;
	*p_list = (*p_list)->m_next;
	result->m_next = NULL;

	return result;
}

/**
 *	@brief	Fonction permettant de libérer une liste d'adjacence.
 *
 *	@param	p_list		Liste d'adjacence à désallouer.
 */
void FreeList(list * p_list)
{
	if (!p_list)
		return;

	if (p_list->m_next)
		FreeList(p_list->m_next);

	free(p_list);
}
