#include "aStar.h"

/**
*	@brief	Fonction permettant de cr�er (allouer) un noeud de liste utilis�e pour A*.
*
*	@param	p_board		Plateau r�duit du jeu.
*	@param	p_g			Distance d�j� parcourue entre l'�tat initial et l'�tat courant
*	@param	p_f			Evaluation de la distance � parcourir entre l'�tat initial et l'�tat final (g + h).
*	@param	p_color		Derni�re couleur appliqu�e pour arriver � ce plateau.
*	@param	p_prev		Pointeur vers le noeud correspondant au plateau pr�c�dent dans la solution.
*
*	@return	Le noeud de liste allou� et initialis�.
*/
aStar * CreateNodeAStar(board * p_board, int p_g, int p_f, unsigned int p_color, aStar * p_prev)
{
	aStar * result = NULL;

	result = (aStar*)malloc(sizeof(aStar));
	result->m_board = p_board;
	result->m_color = p_color;
	result->m_f = p_f;
	result->m_g = p_g;
	result->m_prev = p_prev;
	result->m_next = NULL;

	return result;
}

/**
*	@brief	Fonction permettant d'ins�rer un noeud dans un liste de mani�re tri�e ou non.
*
*	@param	p_list		Liste utilis�e pour A*.
*	@param	p_node		Noeud � ins�rer dans la liste.
*	@param	p_tri		1 si l'insertion doit se faire de mani�re tri�, 0 sinon.
*/
void InsertNodeAStar(aStar ** p_list, aStar * p_node, int p_tri)
{
	if (!p_tri)//non tri� -> insertion en tete
	{
		if (!p_list)
			return;
		
		if (!(*p_list))
		{
			(*p_list) = p_node;
			//p_node->m_next = NULL;
			return;
		}
		p_node->m_next = (*p_list);
		(*p_list) = p_node;	
	}
	else//tri�
	{
		aStar ** liste = p_list;
		while (*liste)
		{
			if ((*liste)->m_f < p_node->m_f)
			{
				liste = &((*liste)->m_next);
			}
			else
			{
				p_node->m_next = *liste;
				*liste = p_node;
				return;
			}
		}
		(*liste) = p_node;
		//Iteratif au lieu de r�curcif pour �viter les stack overflow
		/*
		if (!p_list)
			return;
		if (!(*p_list) || p_node->m_f < (*p_list)->m_f)
		{
			p_node->m_next = (*p_list);
			(*p_list) = p_node;
			return;
		}
		InsertNodeAStar(&(*p_list)->m_next, p_node, p_tri);
		*/
	}
}

/**
*	@brief	Fonction permettant de r�cup�rer la t�te d'une liste utilis�e pour A*.
*
*	@param	p_list		Pointeur sur la liste pour A*.
*
*	@return Pointeur sur le noeud de liste extrait.
*/
aStar * PopListAStar(aStar ** p_list)
{
	// TODO ...
	aStar * result = NULL;

	if (!p_list || !*p_list)
		return NULL;

	result = *p_list;
	*p_list = (*p_list)->m_next;
	result->m_next = NULL;

	return result;
}

/**
*	@brief	Fonction permettant de savoir si un plateau se trouve dans une liste A*.
*
*	@param	p_list		Liste A*.
*	@param	p_board		Plateau � chercher.
*
*	@return Noeud dans lequel on trouve le plateau pass� en param�tre (pointeur sur le pointeur dans la liste).
*/
aStar ** IsInList(aStar ** p_list, board * p_board)
{
	// TODO ...
	if (!p_list)
		return NULL;

	if (!(*p_list))
		return NULL;

	aStar **parcours = p_list;
	while ((*parcours))
	{
		if (SameBoards((*parcours)->m_board, p_board))
			return parcours;
		(parcours) = &((*parcours)->m_next);
	}
	return NULL;
	//return IsInList(&((*p_list)->m_next), p_board);
}

/**
*	@brief	Fonction de r�solution du jeu en utilisant l'algorithme A*.
*
*	@param	p_board				Plateau � r�soudre (plateau r�duit).
*	@param	p_colors[OUT]		Tableau des couleurs de la solution.
*	@param	p_nbColors[OUT]		Nombre de couleurs dans la solution.
*
*	@return	 1	si le plateau a �t� r�solu,
*			-1	si une erreur est sruvenue,
*			 0	sinon.
*/
int SolveGame(board * p_board, unsigned int ** p_colors, unsigned int * p_nbColors)
{
	unsigned char colorUsed[NB_MAX_COLOR] = { 0 }; //Permet d'obtimiser le choix des couleurs voisines (ne pas appliquer 2 fois la meme)
	aStar * opened = NULL;	
	aStar * closed = NULL;
	aStar * currentNode = NULL;
	list * neighbours = NULL;
	int travelled = 1;

	//On cr�� un noeud aStar a partir du plateau a r�soudre et on l'ajoute dans opened
	InsertNodeAStar(&opened, CreateNodeAStar(p_board, 0, H(p_board), p_board->m_graph[0]->m_colorIndex, NULL), 1);

	while (!IsFinished((currentNode = PopListAStar(&opened))->m_board)) //Tant que le plateau retir� de opened n'est pas fini
	{
		//On insert le noeud retir� dans closed
		InsertNodeAStar(&closed, currentNode, 0);
		neighbours = currentNode->m_board->m_graph[0]->m_neighbours;
		while (neighbours)//Pour chacun de ses voisins
		{			
			if (neighbours->m_node->m_colorIndex != currentNode->m_board->m_graph[0]->m_colorIndex && !colorUsed[neighbours->m_node->m_colorIndex])//Si la couleur d'un des voisins n'a pas d�j� �t� appliqu�e
			{
				//On applique la couleur, r�duit le graph, libere l'ancien graph non r�duit, incr�mente de 1 les mouvements effectu�s et on cr�� le nouveau noeud aStar
				board * newBoard = CopyBoard(currentNode->m_board);
				board * newReducedBoard = NULL;
				ApplyColor(&newBoard, neighbours->m_node->m_colorIndex);
				colorUsed[neighbours->m_node->m_colorIndex] = 1;
				ReduceBoard(newBoard, &newReducedBoard);
				FreeBoard(newBoard);
				travelled = currentNode->m_g + 1;
				aStar * newNodeAStar = CreateNodeAStar(newReducedBoard, travelled, travelled + H(newReducedBoard), neighbours->m_node->m_colorIndex, currentNode);
				if (IsInList(&closed, newNodeAStar->m_board) != NULL)//Si le noeud cr�� est d�j� dans closed, inutile de le traiter une deuxi�me fois, on le lib�re.
				{
					FreeAStarList(&newNodeAStar);
				}
				else
				{
					aStar ** nodeInList = NULL;
					if ((nodeInList = IsInList(&opened, newNodeAStar->m_board)) != NULL)//S'il est d�j� dans opened
					{
						if ((*nodeInList)->m_f > travelled + H((*nodeInList)->m_board))//S'il est plus interessant que celui d�j� pr�sent
						{
							//On le retire, le recalcul et on le r�ins�re de mani�re tri�e
							aStar *tmp = (*nodeInList);
							(*nodeInList) = tmp->m_next;
							tmp->m_next = NULL;
							tmp->m_f = currentNode->m_g + H(tmp->m_board);
							InsertNodeAStar(&opened, tmp, 1);
						}
						else//Sinon on lib�re le noeud 
						{
							FreeAStarList(&newNodeAStar);
							newNodeAStar = NULL;
						}
					}
					else//Si le noeud est ni dans closed, ni dans opened, on l'ajoute dans opened pour le traiter plus tard
					{
						InsertNodeAStar(&opened, newNodeAStar, 1);
					}
				}
			}
			neighbours = neighbours->m_next;//On passe au voisin d'apr�s
		}
		int i;
		for (i = 0; i < NB_MAX_COLOR;i++)
		{
			colorUsed[i] = 0;
		}
	}

	InsertNodeAStar(&closed, currentNode, 0);
	(*p_nbColors) = (closed->m_g);
	int i = (*p_nbColors)-1;
	(*p_colors) = (int*)malloc(sizeof(int) * (closed->m_g));
	while (i >= 0)
	{
		(*p_colors)[i] = currentNode->m_color;
		currentNode = currentNode->m_prev;
		i--;
	}
	FreeAStarList(&opened);
	FreeAStarList(&closed);
}

/**
*	@brief	Fonction de calcul de l'heuristique pour l'algorithme A*.
*
*	@param	p_board			Plateau du jeu.
*
*	@return	Valeur de l'heuristique.
*/
int H(board * p_board)
{
	int i;
	unsigned char nbColors = 0;
	unsigned char color[NB_MAX_COLOR] = { 0 };
	for (i = 1; i < p_board->m_size;i++)
	{
		color[p_board->m_graph[i]->m_colorIndex] = 1;
	}
	for (i = 0; i < NB_MAX_COLOR; i++)
	{
		if (color[i])
			nbColors++;
	}
	return nbColors;
}

void FreeAStarList(aStar **p_list)
{
	if (!p_list)
		return;
	if (!(*p_list))
		return;
	FreeAStarList(&(*p_list)->m_next);
	FreeBoard((*p_list)->m_board);
	free(*p_list);
}