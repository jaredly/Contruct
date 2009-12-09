
///////////////////////////
// Eventlist structures
// Max params
#define CRUNTIME_MAX_PARAMS 18



// Action
struct action {
	ACROUTINE routine;			// Action SDK routine address
	CRunObjType* pType;			// Pointer to object type
	int constantParams;			// Nonzero if all params return true for Constant()
	class CRuntime* pRuntime;

	ExpressionList params;		// Parameters
	mutable ExpReturn actParamList[CRUNTIME_MAX_PARAMS];

	int oid;					// object ID
	int actID;					// action ID

	void Do();		// Run
	void EvaluateParams();
	void EvaluateParams(class CRunObject* selObj, int solindex);
	void Destroy();
};

typedef vector<action>							ActionList;
typedef vector<action>::iterator					ActionIterator;
typedef const vector<action>::const_iterator		ActionConstIterator;

// Condition type
enum ConditionType {
	CONDITIONTYPE_SYSTEM,
	CONDITIONTYPE_SINGLE,
	CONDITIONTYPE_MULTIPLE,	// for containers
	CONDITIONTYPE_MULTIPLEFAMILY,	// for 'contained' family movements
	CONDITIONTYPE_FORCE_DWORD = 0x7fffffff
};

// Condition
struct condition {
	ConditionType cndType;	// Condition type
	ACROUTINE routine;		// Condition SDK routine address
	CRunObjType* pType;		// Pointer to obj type
	int isNegated;			// Nonzero if negated
	int isStatic;		// Is a Static condition
	int constantParams;		// Nonzero if all params return true for Constant()
	class CRuntime* pRuntime;

	ExpressionList params;	// Parameters
	mutable ExpReturn cndParamList[CRUNTIME_MAX_PARAMS];

	int number;			// Condition number
	int oid;			// object ID
	int cndID;			// condition ID

	int Do();			// Run condition (gateway to Do* functions)

	int DoSystem();		// Process no object SOL
	int DoSingle();		// Process single object SOL
	int DoMultiple();	// Process multiple object SOLs
	int DoMultipleFamily(); // Process multiple family contained SOLs

	int DoStatic(ObjList& o);

	void Destroy();

	void EvaluateParams();
	void EvaluateParams(class CRunObject* selObj, int solindex);
};

/////////////////////////////////
// Event tree defs
enum EVENTTYPE {
	EVENT_UNKNOWN,
	EVENT_NORMAL,
	EVENT_PARENT,
	EVENT_GROUP,
};

struct EventTriggerInfo {
	int oid;
	int cndID;
	short fasttrigger;
	condition* pCnd;
	ConditionList* pvConditions;
};

struct EventParametersInfo {
	ConditionList* pCnds;
	ActionList* pActs;
	EventList* pSubEvents;
};

/////////////////////////////////
// Abstract base class CEventLine
// An event line: could be group, eventparent, or event
class __declspec(novtable) CEventLine {
public:
	virtual void Run() = 0;		// Run the eventline
	virtual void Run(int oid, CRunObject* pCaller) = 0;
	virtual void RunFast() = 0; // Run via GenerateEventFast, skip 1st condition

	virtual EVENTTYPE GetType() = 0;
	virtual EventTriggerInfo GetTriggerInfo() = 0;
	virtual ExpressionList* GetParamsPtr() = 0;
	virtual void GetEventInfo(EventParametersInfo& ei) = 0;
	virtual void GetSolModifiers(CRunLayout* frame) = 0;

	virtual bool TriggerMatch(condition* pCnd) = 0;	// For matching triggers

	class CRuntime* pRuntime;

	ConditionIterator* pCurCnd;	// Current condition iterator (only for event/parent)

	int modifiesSol;			// Nonzero if makes SOL modifications
	ObjTypeList* modifiers;		// SOL modifiers (pointer; if modifies unknown, will point to CRuntime::objects)

	int number;					// Event line number
	int eventSheet;				// Index to event sheet

	bool trigger;				// Triggered line?
	bool fasttrigger;			// Fast trigger?
	bool manualSolModifier;		// True if manually modifies the SOLs

	CEventLine() { pCurCnd = NULL; }
};

// CEvent: single event with no sub events
class CEvent : public CEventLine {
public:
	void Run();					// Execute this event
	void Run(int oid, CRunObject* pCaller);
	void RunFast();

	EVENTTYPE GetType();
	EventTriggerInfo GetTriggerInfo();
	ExpressionList* GetParamsPtr();
	void GetEventInfo(EventParametersInfo& ei);
	void GetSolModifiers(CRunLayout* frame);

	bool TriggerMatch(condition* pCnd);

	ConditionList cnds;		// Conditions to check
	ActionList	  acts;		// Actions to run

	CEventLine* elseEvent;

	CEvent() : elseEvent(NULL) {}
	~CEvent();
};

// CEventParent: event which may contain:
// 1.  Sub events
// 2.  Else clauses
class CEventParent : public CEventLine {
public:
	void Run();
	void Run(int oid, CRunObject* pCaller);
	void RunFast();

	EVENTTYPE GetType();
	EventTriggerInfo GetTriggerInfo();
	ExpressionList* GetParamsPtr();
	void GetEventInfo(EventParametersInfo& ei);
	void GetSolModifiers(CRunLayout* frame);

	bool TriggerMatch(condition* pCnd);

	ConditionList cnds;			// This event's conditions
	ActionList	  acts;			// This event's actions

	int HasSubEvents;			// Nonzero if has sub events
	EventList subEvents;		// The sub events

	CEventLine* elseEvent;

	CEventParent() : elseEvent(NULL) {}
	~CEventParent();
};

// CEventGroup:  A group of events
class CEventGroup : public CEventLine {
public:
	void Run();
	void Run(int oid, CRunObject* pCaller);
	void RunFast();

	EVENTTYPE GetType();
	EventTriggerInfo GetTriggerInfo();
	ExpressionList* GetParamsPtr();
	void GetEventInfo(EventParametersInfo& ei);
	void GetSolModifiers(CRunLayout* frame);

	bool TriggerMatch(condition* pCnd);

	int Activated;				// Nonzero if group is active
	bool isTopLevel;			// Can run events as inline (faster)

	EventList events;			// Event lines in this group

	CEventGroup() : isTopLevel(false) {}
	~CEventGroup();
};

// Helper function for GetSolModifiers: adds an entry if it doesnt already exist
void AddSolModifier(ObjTypeList* otl, CRunObjType* type);