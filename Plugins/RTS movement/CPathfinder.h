
// Pathfinding defs
typedef unsigned int cell_t;			// Type for a cell
typedef CPoint point_t;
#define PFCELL_CLEAR			((cell_t)	0	)
#define PFCELL_IMPASSABLE		((cell_t)	-1	)
#define PFCELL_DIAGONAL_COST	((cell_t)	140	)		// Cost for a diagonal move
#define PFCELL_ADJACENT_COST	((cell_t)	100	)		// Cost for an adjacent move
#define PFCELL_OCCUPIED_COST	((cell_t)	500	)		// ~10 cells cost for a stationary unit in a cell
#define PFCELL_TRAVELLING_COST	((cell_t)	3	)		// Cost for a unit taking this path already

// Node directions
enum NodeDir {
	NODEDIR_UP,
	NODEDIR_UPRIGHT,
	NODEDIR_RIGHT,
	NODEDIR_DOWNRIGHT,
	NODEDIR_DOWN,
	NODEDIR_DOWNLEFT,
	NODEDIR_LEFT,
	NODEDIR_UPLEFT,
	NODEDIR_NONE
};

////////////////////////////////
// A* PATHFINDING IMPLEMENTATION
class CPathfinder {
public:
	CPathfinder();
	~CPathfinder();

	struct Node {
		point_t cell;
		Node* parent;

		unsigned int f;
		unsigned int g;
		unsigned int h;
	};

	void			SetMap(cell_t* gridMemory, int _width, int _height);
	bool			FindPath(point_t& origin, point_t& target);
	cell_t			GetCell(int x, int y);
	void			AddToCellUsage(int x, int y);
	bool			CellImpassable(int x, int y);
	void			ClearCellCosts();			// Reset all cells to either 0 or PFCELL_IMPASSABLE

	NodeDir			DirToNode(const Node* a, const Node* b);

	// The final cell:  follow parent nodes back to NULL (start) for the path
	Node			pathEnd;

private:

	void			AddCellToOpenList(int x, int y, int g);

	unsigned int	EstimateH(const point_t& cell);


	cell_t*			pMap;

	int				width;
	int				height;

	// The open list: cells to be considered
	list<Node> openList;

	// The closed list: cells no longer to be considered
	list<Node> closedList;

	Node*		  currentNode;
	point_t		  target;
};