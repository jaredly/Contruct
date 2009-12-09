#include "stdafx.h"
#include "ChronoEventEditor\ChronoEventEditor.h"
#include "Event Sheet Editor\Event Sheet Editor.h"

#include "..\Structure\Export\Export.h"

CString CEditorParam::CreateFormattedParamString(EventSheetEditor* pEd, CEditorAction* pAction, CEditorCondition* pCondition)
{
	CApplication* pApp = pEd->application;
	CObjTypeMap& typemap = pApp->object_types;

	valid = true;
	CString ret;

	for( int a = 0; a < ttokens.size(); a++)
	{
		Token& tokenitem = ttokens[a];
	
		if(tokenitem.t == T_IDENTIFIER && tokenitem.tsub == TT_OBJECTNAME)	// object param
		{
			long number = tokenitem.oidOwner;

			CObjType* pType = pEd->application->FindObjTypeFromNumber(number);
			if(pType)
			{
				int specialvalue = 0;
				if(pAction)
				{
					pAction->m_imgs.push_back(CEditorImage());
					CEditorImage* image = &pAction->m_imgs.back();
					image->tok = &tokenitem;
					specialvalue = pAction->m_imgs.size()-1;
				}
				if(pCondition)
				{
					pCondition->m_imgs.push_back(CEditorImage());
					CEditorImage* image = &pCondition->m_imgs.back();
					image->tok = &tokenitem;
					
					specialvalue = pCondition->m_imgs.size()-1;
				}		
				ret += pType->GetName();
				ret += " <special_image>";
				ret.AppendFormat("%d", specialvalue);
				ret += "</special_image> ";
				
				
			}
			else
				valid = false;
			//else - we need some sort of tempory name if we cant find one or the user could lose information!
		
		}
		else {
			CString string = tokenitem.str;

			string.Replace("&", "&amp;");
			string.Replace("<", "&lt;");
			string.Replace(">", "&gt;");

			if (tokenitem.t == T_STRINGLITERAL)
				string = CString("\"") + string + "\"";
			if (tokenitem.t == T_VARIABLENAME)
			{
				string = CString("'");
				if(tokenitem.oidOwner == -1)
				{
					CApplication::GlobalVariable* global = pApp->GetGlobalVariable(tokenitem.id);
					if(global)
						string += global->name;
					else
						valid = false;
				}
				else if(CObjType* pType = pApp->FindObjTypeFromNumber(tokenitem.oidOwner))
				{
					PrivateValue* Private = pType->GetPrivateVariable(tokenitem.id);
					if(Private)
						string += Private->name;
					else {

						// Not found; look up by name instead (valid expressions sometimes get here for families - AG)
						vector<PrivateValue>::iterator i = pType->m_PrivateValues.begin();
						bool found = false;
						CString l = tokenitem.str;
						l.MakeLower();

						for ( ; i != pType->m_PrivateValues.end(); i++) {
							CString r = i->name;
							r.MakeLower();

							if (l == r) {
								found = true;
								string += i->name;
								break;
							}
						}

						if (!found)
							valid = false;
					}
				}
				else
					valid = false; //added to prevent family bug where deleting a family wont remove params refering to it
					
				string += "'";
			}

			ret += string;
		}

		/*
		if(tokenitem.type == PTOK_COLOR) // for later when we have color tokens 
		{
			int specialvalue = 0;
			if(pAction)
			{
				pAction->m_imgs.push_back(CEditorImage());
				CEditorImage* image = &pAction->m_imgs.back();
				image->tok = &tokenitem;
				specialvalue = pAction->m_imgs.size()-1;
			}
			if(pCondition)
			{
				pCondition->m_imgs.push_back(CEditorImage());
				CEditorImage* image = &pCondition->m_imgs.back();
				image->tok = &tokenitem;
					
				specialvalue = pCondition->m_imgs.size()-1;
			}				


			ret += "<special_image>";
			ret.AppendFormat("%d", specialvalue);
			ret += "</special_image> ";
		}
		*/
	}

	// Okay we have now recreated our token and everything...lets check what type of parameter it is!

	// Final step: 
	if(m_type == 9 || m_type == 15)
	{
		CString formatted;
		formatted.Format("<b>%s</b>", m_reserved);
		return formatted;
	}

	// Private variable
	if(m_type == 14)
	{
		int varid = atoi(ret);
		CObjType* pType = pApp->FindObjTypeFromNumber(this->oid);

		for(vector<PrivateValue>::iterator v = pType->m_PrivateValues.begin(); v!=  pType->m_PrivateValues.end(); v++)
		{
			if(v->identifier == varid)
			{
				CString formatted;
				formatted.Format("'%s'", v->name);
				return formatted;
			}
		}	
	}

	// Global variable
	if (m_type == 13) {
		int varid = atoi(ret);

		for(list<CApplication::GlobalVariable>::iterator v = pApp->global_variables.begin(); v !=  pApp->global_variables.end(); v++)
		{
			if(v->identifier == varid)
			{
				CString formatted;
				formatted.Format("'%s'", v->name);
				return formatted;
			}
		}	
	}
		
	if(m_type == 6) // object..translate the parameter to a new number
	{
		int id = atoi(ret);
		CString newstring;
		newstring.Format("%d", id);
		if(ret == newstring)	// will fail if its an attribute	
		{
/*
			int iconid;
			if(!pEd->m_familyhbitmaps.Lookup(id, iconid))
				pEd->m_objecthbitmaps.Lookup(id, iconid);


			ret.Format("<img>%d</img>", iconid);
				
*/
			long number = atol(ret);

			CObjType* pType;
	

			if(pType = pApp->FindObjTypeFromNumber(number))
			{
				int specialvalue = 0;
				if(ttokens[0].t == T_INTEGER)
				{
				//	ttokens[0].tsub = TT_OBJECTNAME;
				//	ttokens[0].t = T_IDENTIFIER;
					ttokens[0].oidOwner = number;
					ttokens[0].id = number;
					//ttokens[0].trans = false;
				}
				
				if(pAction)
				{
					pAction->m_imgs.push_back(CEditorImage());
					CEditorImage* image = &pAction->m_imgs.back();
					image->tok = &ttokens[0];
					specialvalue = pAction->m_imgs.size()-1;
				}
				if(pCondition)
				{
					pCondition->m_imgs.push_back(CEditorImage());
					CEditorImage* image = &pCondition->m_imgs.back();
					image->tok = &ttokens[0];
					
					specialvalue = pCondition->m_imgs.size()-1;
				}				

				ret = "<b>";
				ret += pType->GetName();
				ret += "</b>";
				ret += " <special_image>";
				ret.AppendFormat("%d", specialvalue);
				ret += "</special_image> ";

				return ret;
				
			}
			else
				valid = false;


		}
		else
		{
			// attribute..


		}
		return ret;

	

	}
	if(m_type == 3) // colors
	{
		int id = atoi(ret);
		CString newstring;
		newstring.Format("%d", id);
		ttokens[0].t = T_COLOR;
		if(ret == newstring)	// will fail if its an attribute	
		{
			int specialvalue = 0;
			if(pAction)
			{
				pAction->m_imgs.push_back(CEditorImage());
				CEditorImage* image = &pAction->m_imgs.back();
				image->tok = &ttokens[0];
				image->tok->id = atoi(image->tok->str);
				specialvalue = pAction->m_imgs.size()-1;
			}
			if(pCondition)
			{
				pCondition->m_imgs.push_back(CEditorImage());
				CEditorImage* image = &pCondition->m_imgs.back();
				image->tok = &ttokens[0];
				image->tok->id = atoi(image->tok->str);					
				specialvalue = pCondition->m_imgs.size()-1;
			}				


			ret = "<special_image>";
			ret.AppendFormat("%d", specialvalue);
			ret += "</special_image> ";

			return ret;
		}


	}
	// otherwise just return ret ;p
	CString format;
	format.Format("<i>%s</i>", ret);

	return format;
}



CString CEditorParam::CreateParamString(CChronoEventEditor* Ed)
{
	return CreateParamString(((EventSheetEditor*)Ed)->application);
}

CString CEditorParam::CreateParamString(CExport* Ed)
{
	return CreateParamString(Ed->application);
}

CString CEditorParam::CreateParamString(CApplication* pApp)
{
	CObjTypeMap& typemap = pApp->object_types;

	CString ret;

	valid = true;

	vector<Token>::iterator i = ttokens.begin();
	vector<Token>::iterator ttokens_end = ttokens.end();

	ttokens_end = ttokens.end();

	for ( ; i != ttokens_end; i++) {
		switch (i->t) {
		case T_IDENTIFIER:

			switch (i->tsub) {
			case TT_OBJECTNAME:
				{
				if(CObjType* pType = pApp->FindObjTypeFromNumber(i->oidOwner))
					ret += pType->GetName();
				else
					valid = false;

				break;
				}
			case TT_FAMILYNAME:
				ret += i->str;
				break;
			case TT_MOVEMENTNAME:
				{
				CObjType* pType;
				CBehavior* pMov;

				if(pType = pApp->FindObjTypeFromNumber(i->oidOwner))
					ret += pType->GetName();
				else
				{
					valid = false;
					break;
				}

				ret += "[";

				if (pMov = pType->GetBehaviorByUniqueID(i->id))
					ret += pMov->Text + "]";
				else
					valid = false;

				break;
				}				
			case TT_NORMAL:
				ret += i->str;
				break;
			}
			break;
		case T_VARIABLENAME:
			ret += "'";
			if(i->oidOwner == -1)
			{
				CApplication::GlobalVariable* pGlobal = pApp->GetGlobalVariable(i->id);
				if(pGlobal)
					ret += pGlobal->name;	

				else
					valid = false;
			}
			else if(CObjType* pType = pApp->FindObjTypeFromNumber(i->oidOwner))
			{
				PrivateValue* pValue = pType->GetPrivateVariable(i->id);
				if(pValue)
					ret += pValue->name;
				else
					valid = false;
			}
			ret += "'";
			break;
		case T_STRINGLITERAL:
			ret += "\"";
			ret += i->str;
			ret += "\"";
			break;
		default:
			ret += i->str;
			break;
		}
	}


	return ret;


	// Davos code

	//CString ret;
	//for( int a = 0; a < tokens.size(); a ++)
	//{
	//	CEditorParamToken tokenitem = tokens[a];
	//	if(tokenitem.trans)
	//	{
	//		if(tokenitem.type == 0)	// normal
	//			ret += tokenitem.tok;
	//		else if(tokenitem.type == 1)	// object name
	//		{
	//			
	//			long number = atol(tokenitem.tok);

	//			CObjType* pType;
	//		
	//			if(typemap.Lookup(number,pType))
	//				ret += pType->m_name;
	//			//else - we need some sort of tempory name if we cant find one or the user could lose information!
	//		}
	//			

	//			
	//		
	//		
	//		else if(tokenitem.type == 2)	// family
	//		{
	//			ret += tokenitem.tok;
	//			/*
	//			long number = atol(tokenitem.tok);
	//			//list<CFamily>::iterator i = pApp->m_Families.begin();
	//			//advance(i, number);
	//			CFamily* i = pApp->FindFamilyFromNumber(number);
	//			// TODO error checking...what if a family is removed?
	//			ret += i->m_Name;
	//			*/
	//		}
	//		else
	//			ret += tokenitem.tok;
	//	}
	//	
	//	else
	//	{
	//		ret += tokenitem.tok;
	//	}
	//}

	//// TODO: Reform objectID tokens into object name
	//return ret;
}

void CEditorParam::Serialize( CArchive& ar, CChronoEventEditor* Ed)
{
	CString ExpectedName = "CEditorParam";
	int     Version      = 1;

	if (!SerializeClassHeader(ExpectedName, Version, ar))
		return;
	if(!ar.IsLoading())
	{
		valid = true;

		ar << m_type;
		ar << m_reserved;
		ar << oid;

		int size = ttokens.size();

		ar << size;
		for( int a = 0; a < size; a ++ )
		{
			ar << (int)ttokens[a].t;
			ar << ttokens[a].length;
			ar << ttokens[a].oidOwner;
			ar << ttokens[a].str;
			ar << ttokens[a].id;
			ar << (int)ttokens[a].tsub;
		}
	}
	else
	{
		ar >> m_type;
		ar >> m_reserved;
		ar >> oid;
		
		ttokens.clear();

		int size = 0;

		ar >> size;
		for( int a = 0; a < size; a ++ )
		{
			int temp;
			ttokens.push_back(Token());
			Token& tok = ttokens.back();
			ar >> temp;
			tok.t = (ExpToken)temp;
			ar >> tok.length;
			ar >> tok.oidOwner;
			ar >> tok.str;
			ar >> tok.id;
			ar >> temp;
			tok.tsub = (TokenSubType)temp;
		}
	}
}
bool CEditorParam::CharPartOfName(char i)
{
	//Basically a sprite name can consist of any of these letters.
	if( i >= 'a' && i <= 'z')
		return true;
	if( i >= 'A' && i <= 'Z')
		return true;
	if( i >= '1' && i <= '9')
		return true;
	if( i == '0')
		return true;

	return false;
}

void CEditorParam::CreateFromString(CString src, CApplication* pApp, int type, CString reserved, bool exporting)
{
	m_type = type;
	m_reserved = reserved;

	///////////////////
	// Object types
	///////////////////

	
	CObjTypeMap& typemap = pApp->object_types;

	int unused;

	// Tokenise the expression with editormode
	CExpression tokeniser;
	tokeniser.pCallerType = pApp->FindObjTypeFromNumber(oid);
	tokeniser.oidCaller = oid;
	tokeniser.ParseUserString(src, &unused, !exporting);
	ttokens = tokeniser.toks;

	ProcessEdittimeExpressionInformation(ttokens, pApp, oid);


	/*


	// Davo's version

	
	tokens.clear();
	// We need to create
	CString copy;
	copy = src;
	char* f = copy.LockBuffer();
	char* c = f;
	char* tmp = c;

	CString name;
	CString tok;
	bool quote = false;
	while((char)(*c)) // c has not reached null
	{
		name = "";
		// test if we are reading a name
		if( *c == '"')
			quote = 1 - quote;
		while(CharPartOfName(*c) && !quote)	// post incr c
		{
			name += (char)(*c);
			c++;
		}
		if(name != "")
		{
		
			long number;
			bool found = false;

			POSITION pos = typemap.GetStartPosition();
			CObjType* obj;
			bool IsFamily = false;
			while(pos)
			{
				typemap.GetNextAssoc(pos, number, obj);
				if(obj->m_name == name)
				{
					found = true;
					break;
				}
				
			}
			for(list<CFamily>::iterator i = pApp->m_Families.begin(); i != pApp->m_Families.end(); i++)
			{
				CString l = i->m_Name;
				CString r = name;
				l.MakeLower();
				r.MakeLower();

				if(l == r)
				{
					found = true;
					IsFamily = true;
					break;
				}
			}

			if(found)	// we have found an object name
			{
				//save last token
				if(tok != "")
				{
					CEditorParamToken token;
					token.tok = tok;
					token.type = PTOK_STRING;
					token.trans = true;
					tokens.push_back(token);
				}
				// now record the object name
				if(IsFamily) // family
				{
					CEditorParamToken token;	
					token.tok = name;
					token.type = PTOK_FAMILYNAME;
					token.trans = true;
					tokens.push_back(token);
				}	
				else // object
				{
					CEditorParamToken token;	
					token.tok.Format("%d", number);
					token.type = PTOK_OBJECTNAME;
					token.trans = true;
					tokens.push_back(token);
				}	
				//Clear tokens			
				name = "";
				tok = "";
			}
			tok += name;
		}
		tok += (char)(*c);

		//next letter unless we have already reached it
		if((char)(*c))
			c++;
	}
	//save last token
	if(tok != "")
	{
		CEditorParamToken token;
		token.tok = tok;
		token.type = PTOK_STRING;
		token.trans = true;
		tokens.push_back(token);
	}

	copy.ReleaseBuffer();
	*/

}


CEditorParam::~CEditorParam()
{

}