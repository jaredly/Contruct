
#include "StdAfx.h"

////////////////////////////////////////////////////////////////////////////////
//		Pathfinding.cpp
//		A* pathfinding algorithm implementation (with hacks)
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// CPATHFINDER
CPathfinder::CPathfinder()
:	width(0),
	height(0),
	pMap(NULL),
	currentNode(NULL),
	target(0, 0)
{}

CPathfinder::~CPathfinder()
{
}

void CPathfinder::SetMap(cell_t* gridMemory, int _width, int _height)
{
	pMap = gridMemory;
	width = _width;
	height = _height;
}

cell_t CPathfinder::GetCell(int x, int y)
{
	//if (x < 0 || y < 0 || x >= width || y >= height)
	//	return PFCELL_IMPASSABLE;

	return pMap[y * width + x];
}

void CPathfinder::AddToCellUsage(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return;

	cell_t& c = pMap[y * width + x];

	if (c + PFCELL_TRAVELLING_COST < PFCELL_IMPASSABLE)
		c += PFCELL_TRAVELLING_COST;
}

bool CPathfinder::CellImpassable(int x, int y)
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return true;

	return GetCell(x, y) == PFCELL_IMPASSABLE;
}

// Reset all cells not impassable to 0
void CPathfinder::ClearCellCosts()
{
	int size = width * height;
	cell_t* i = pMap;
	cell_t* end = pMap + size;

	for ( ; i != end; ++i) {
		if (*i != PFCELL_IMPASSABLE)
			*i = 0;
	}
}

/////////////////////////////
// Find a path through pfMap
bool CPathfinder::FindPath(point_t& origin, point_t& targetcell)
{
	target = targetcell;
	pathEnd.parent = NULL;

	// If not initialised, fail.
	if (pMap == NULL)
		return false;

	// Check the box made by the start and dest cells.
	// If the complete box is empty, allow a direct move to target.
	int minX = min(origin.x, targetcell.x);
	int maxX = max(origin.x, targetcell.x);
	int minY = min(origin.y, targetcell.y);
	int maxY = max(origin.y, targetcell.y);

	bool directlyClear = true;

	// Out of bounds: no calculatable path
	if (minX < 0 || minY < 0 || maxX >= width || maxY >= height)
		return false;
	
	for (int cx = minX; cx <= maxX; cx++) {
		for (int cy = minY; cy <= maxY; cy++) {

			// Obstacle in box
			if (GetCell(cx, cy) != 0) {

				directlyClear = false;
				
				// Break top loop
				cx = maxX + 1;

				break;
			}
		}
	}

	// A "direct" path is available (box is empty)
	if (directlyClear) {

		// Make path end
		pathEnd.cell = target;
		pathEnd.parent = NULL;

		// Path found
		return true;
	}

	// Reset lists
	openList.resize(0);
	closedList.resize(0);

	// Add the start node to the open list
	Node startNode;
	startNode.cell = origin;
	startNode.f = 0;
	startNode.g = 0;
	startNode.h = 0;
	startNode.parent = NULL;

	openList.push_back(startNode);

	// While there are nodes on the open list
	while (!openList.empty()) {

		// Add best F value to closed list
		closedList.push_front(openList.front());

		// Are we there yet?
		if (closedList.front().cell.x == target.x && closedList.front().cell.y == target.y) {


			// Hilight all the cells back to the start
			//currentNode = &(closedList.front());
			pathEnd = closedList.front();

			/*
			while(currentNode->parent != NULL) {

				PFATCELL(currentNode->cell.x, currentNode->cell.y) = PF_HILIGHT;

				currentNode = currentNode->parent;
			}
			*/

			return true;
		}

		// Delete best F value
		openList.pop_front();

		// Get current node
		currentNode = &(closedList.front());
		point_t* pCell = &(currentNode->cell);

		int x = pCell->x;
		int y = pCell->y;

		// Check adjacent 8 nodes.  No diagonals allowed if either cell being crossed is obstacle.
		AddCellToOpenList(x - 1, y, PFCELL_ADJACENT_COST);

		if (!CellImpassable(x - 1, y) && !CellImpassable(x, y - 1))
			AddCellToOpenList(x - 1, y - 1, PFCELL_DIAGONAL_COST);

		AddCellToOpenList(x, y - 1, PFCELL_ADJACENT_COST);

		if (!CellImpassable(x + 1, y) && !CellImpassable(x, y - 1))
			AddCellToOpenList(x + 1, y - 1, PFCELL_DIAGONAL_COST);

		AddCellToOpenList(x + 1, y, PFCELL_ADJACENT_COST);

		if (!CellImpassable(x + 1, y) && !CellImpassable(x, y + 1))
			AddCellToOpenList(x + 1, y + 1, PFCELL_DIAGONAL_COST);

		AddCellToOpenList(x, y + 1, PFCELL_ADJACENT_COST);

		if (!CellImpassable(x - 1, y) && !CellImpassable(x, y + 1))
			AddCellToOpenList(x - 1, y + 1, PFCELL_DIAGONAL_COST);

	}//while open list has cells

	// No path found :(
	return false;
}

// Make a guess at the distance from the cell to the target
unsigned int CPathfinder::EstimateH(const point_t& cell)
{
	int xDistance = abs(cell.x - target.x);
	int yDistance = abs(cell.y - target.y);

	return xDistance * PFCELL_ADJACENT_COST + yDistance * PFCELL_ADJACENT_COST;
	
	//if (xDistance > yDistance)
	//	return 14 * yDistance + 10 * (xDistance - yDistance);
	//else
	//	return 14 * xDistance + 10 * (yDistance - xDistance);
		
}

// Add a cell to the open list, if necessary
void CPathfinder::AddCellToOpenList(int x, int y, int g)
{
	// Impossible for function to be called for an outside 
	//if (x < 0 || y < 0) return;
	//if (x >= width || y >= height) return;

	// Is this cell an obstacle?  Ignore
	if (CellImpassable(x, y))
		return;

	// Is this cell on the closed list?  Ignore
	list<Node>::iterator i = closedList.begin();
	list<Node>::iterator end = closedList.end();

	for ( ; i != end; i++) {
		if ((x == i->cell.x) && (y == i->cell.y))
			return;
	}//for

	// Cell costs can be increased by changing the number in the map
	int curCellCost = GetCell(x, y);

	// If this cell already on the open list?
	i = openList.begin();
	end = openList.end();

	for ( ; i != end; i++) {
		if ((x == i->cell.x) && (y == i->cell.y)) {

			// Is this a better path?
			if (currentNode->g + g + curCellCost < i->g) {

				// Update F, G and H and update parent
				i->parent = currentNode;
				i->g = currentNode->g + g + curCellCost;
				i->h = EstimateH(i->cell);
				i->f = i->g + i->h;

				Node node = *i;

				// This node's F has changed:  Delete it then re-insert it in the right place
				openList.erase(i);

				if (openList.empty()) {
					openList.push_back(node);
					return;
				}
				if (node.f >= openList.back().f)
					openList.push_back(node);
				else {
					list<Node>::iterator j = openList.begin();
					list<Node>::iterator end = openList.end();

					for ( ; j != end; ++j) {
						if (node.f < j->f) {
							openList.insert(j, node);
							break;
						}// if
					}//for each openlist
				}// else
			}//if better path
			//else
			//	++i;

			return;
		}//if on open list
	}//for

	// Not on the open list; add it in the right place
	Node node;
	node.cell.x = x;
	node.cell.y = y;
	node.h = EstimateH(node.cell);
	node.g = currentNode->g + g + curCellCost;
	node.f = node.h + node.g;
	node.parent = currentNode;

	// Insert this node sorted in the open list
	// The loop below won't add new largest F values
	if (openList.empty()) {
		openList.push_back(node);
		return;
	}
	if (node.f >= openList.back().f)
		openList.push_back(node);
	else {
		// Else add it in the correct place
		for (i = openList.begin(); i != openList.end(); i++) {
			if (node.f < i->f) {
				openList.insert(i, node);
				return;
			}
		}//for
	}//else
}

// Direction to node
NodeDir CPathfinder::DirToNode(const Node* a, const Node* b)
{
	const int& ax = a->cell.x;
	const int& ay = a->cell.y;
	const int& bx = b->cell.x;
	const int& by = b->cell.y;

	if (ax == bx) {
		if (by > ay) return NODEDIR_DOWN;
		if (by < ay) return NODEDIR_UP;
		if (ay == by) return NODEDIR_NONE;
	}
	else if (ay == by) {
		if (bx > ax) return NODEDIR_RIGHT;
		if (by < ax) return NODEDIR_LEFT;
	}
	else {
		if (bx < ax && by < ay) return NODEDIR_UPLEFT;
		if (bx > ax && by < ay) return NODEDIR_UPRIGHT;
		if (bx < ax && by > ay) return NODEDIR_DOWNLEFT;
		if (bx > ax && by > ay) return NODEDIR_DOWNRIGHT;
	}
	return NODEDIR_NONE;
}
