#pragma once
class Tag
{
public:
	enum VarType { None, Char, UChar, Bool, Short, Integer, Long, Single, Double, Binary };
	union VarValue { char ch; bool b; short s; int i; __int64 l; float f; double d; }; 
	Tag(void);
	~Tag(void);
	CString m_strName;
	VarType m_varType;
	VarValue m_varValue;
};

