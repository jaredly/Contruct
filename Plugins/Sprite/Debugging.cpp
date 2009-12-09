// Include StdAfx
#include "StdAfx.h"

// Debugging
void ExtObject::DebuggerUpdateDisplay(ExpStore*& pPrivateVars)
{
	if (m_Animation.pAnim)
	{
		if(m_Animation.pAnim->is_angle && m_Animation.pAnim->parent)
			pRuntime->AddDebuggerItem("Animation", m_Animation.pAnim->parent->name, true);
		else
			pRuntime->AddDebuggerItem("Animation", m_Animation.pAnim->name, true);
	}
	pRuntime->AddDebuggerItem("Animation Frame", m_Animation.curFrame, true);

	if (!privateVars.empty())
		pPrivateVars = &(privateVars.front()); 
}

void ExtObject::OnDebuggerValueChanged(const char* name, const char* value)
{
	// Check if a private variable was modified
	if (CheckDebugVariableChange(name, value, privateVars, this, pRuntime))
		return;
}