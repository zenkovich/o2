#pragma once

#include <string>
#include <vector>
#include <map>
#include "pugixml/pugixml.hpp"

#undef GetClassName

using namespace std;

class SyntaxAttributes;
class SyntaxClass;
class SyntaxClassInheritance;
class SyntaxComment;
class SyntaxEnum;
class SyntaxFile;
class SyntaxFunction;
class SyntaxNamespace;
class SyntaxSection;
class SyntaxType;
class SyntaxTypedef;
class SyntaxUsingNamespace;
class SyntaxVariable;

typedef map<string, string>           StringStringDict;
typedef vector<SyntaxAttributes*>     SyntaxAttributesVec;
typedef vector<SyntaxClass*>          SyntaxClassesVec;
typedef vector<SyntaxComment*>        SyntaxCommentsVec;
typedef vector<SyntaxEnum*>           SyntaxEnumsVec;
typedef vector<SyntaxFile*>           SyntaxFilesVec;
typedef vector<SyntaxFunction*>       SyntaxFunctionsVec;
typedef vector<SyntaxSection*>        SyntaxSectionsVec;
typedef vector<SyntaxTypedef*>        SyntaxTypedefsVec;
typedef vector<SyntaxUsingNamespace*> SyntaxUsingNamespacesVec;
typedef vector<SyntaxVariable*>       SyntaxVariablesVec;
typedef vector<string>                StringsVec;

enum class SyntaxProtectionSection { Public, Private, Protected };

// Date time stamp
struct TimeStamp
{
	int year, month, day, hour, minute, second; 

	TimeStamp(int seconds = 0, int minutes = 0, int hours = 0, int days = 0, int months = 0, int years = 0);

	bool operator==(const TimeStamp& wt) const;
	bool operator!=(const TimeStamp& wt) const;

	// Saves data to xml node
	void SaveTo(pugi::xml_node& node) const;

	// Loads data from xml node
	void LoadFrom(pugi::xml_node& node);
};

// Abstract syntax tree file
class SyntaxFile
{
public:
	// Default constructor
	SyntaxFile();

	// Destructor
	~SyntaxFile();

	// Returns file path
	const string& GetPath() const;

	// Returns file's data
	const string& GetData() const;

	// Returns file last edit date
	const TimeStamp& GetLastEditedDate() const;

	// Returns global syntax namespace in this file
	SyntaxNamespace* GetGlobalNamespace() const;

	// Saves data to xml node
	void SaveTo(pugi::xml_node& node) const;

	// Loads data from xml node
	void LoadFrom(pugi::xml_node& node);

protected:
	string           mPath;                      // File path
	string           mData;                      // File data
	TimeStamp        mLastEditedDate;            // Last file edited date
	SyntaxNamespace* mGlobalNamespace = nullptr; // Global syntax namespace in file

	friend class CppSyntaxParser;
	friend class CodeToolApplication;
};

// Syntax expression base
class ISyntaxExpression
{
public:
	// Returns start of expression in owner file data
	int GetBegin() const;

	// Returns length of expression text
	int GetLength() const;

	// Returns end of expression
	int GetEnd() const;

	// Returns line of expression
	int GetLine() const;

	// Returns expression text
	const string& GetData() const;

	// Returns pointer to owner file
	SyntaxFile* GetOwnerFile() const;

protected:
	int         mBegin = 0;      // Data begin position
	int         mLength = 0;     // Data length
	int         mLine = 0;       // Data line number
	string      mData;           // Expression text
	SyntaxFile* mFile = nullptr; // Owner file

	friend class CppSyntaxParser;
};

// Syntax single/multiline comment
class SyntaxComment: public ISyntaxExpression
{
};

// Syntax using namespace ...
class SyntaxUsingNamespace: public ISyntaxExpression
{
public:
	// Returns using namespace name
	const string& GetUsingNamespaceName() const;

	// Returns using namespace (if found)
	SyntaxSection* GetUsingNamespace() const;

	// Saves data to xml node
	void SaveTo(pugi::xml_node& node) const;

	// Loads data from xml node
	void LoadFrom(pugi::xml_node& node);

protected:
	string          mUsingNamespaceName;       // Using namespace name
	SyntaxSection*  mUsingNamespace = nullptr; // Using namespace (if found)

	friend class CodeToolCache;
	friend class CppSyntaxParser;
};

// Syntax typedef X Y;
class SyntaxTypedef: public ISyntaxExpression
{
public:
	// Returns what was defined (X)
	const string& GetWhatName() const;

	// Returns new defined name (Y)
	const string& GetNewDefName() const;

	// Returns what was defined (X)
	SyntaxSection* GetWhat() const;

	// Returns new defined name (Y)
	SyntaxSection* GetNewDef() const;

	// Saves data to xml node
	void SaveTo(pugi::xml_node& node) const;

	// Loads data from xml node
	void LoadFrom(pugi::xml_node& node);

protected:
	string          mWhatName;              // What was used to defined name (X)
	string          mNewDefName;            // What was new defined name (Y)
	SyntaxSection*  mWhatSection = nullptr; // What section used to define (X)

	friend class CodeToolCache;
	friend class CppSyntaxParser;
};

// Syntax names section, base for namespaces or classes
class SyntaxSection: public ISyntaxExpression
{
public:
	// Default constructor
	SyntaxSection();

	// Destructor
	~SyntaxSection();

	// Returns parent section
	virtual SyntaxSection* GetParentSection() const;

	// Returns section name
	virtual const string& GetName() const;

	// Returns full section name including all parent names (something like A::B::C)
	virtual const string& GetFullName() const;

	// Returns array of functions
	virtual const SyntaxFunctionsVec& GetFunctions() const;

	// Returns array of variables
	virtual const SyntaxVariablesVec& GetVariables() const;

	// Returns nested sections
	virtual const SyntaxSectionsVec& GetSections() const;

	// Returns nested enums
	virtual const SyntaxEnumsVec& GetEnums() const;

	// Returns typedefs defined in this section
	virtual const SyntaxTypedefsVec& GetTypedefs() const;

	// Returns using namespaces in this section
	virtual const SyntaxUsingNamespacesVec& GetUsingNamespaces() const;

	// Returns comment in section
	virtual const SyntaxCommentsVec& GetComments() const;

	// Returns comment above or on this line
	SyntaxComment* FindCommentNearLine(int line) const;

	// Returns all inside sections
	virtual SyntaxSectionsVec GetAllSections() const;

	// Returns all inside classes
	virtual SyntaxClassesVec GetAllClasses() const;

	// Returns all enums
	virtual SyntaxEnumsVec GetAllEnums() const;

	// Returns is this section is class
	virtual bool IsClass() const;

	// Returns attributes definitions
	virtual const SyntaxAttributesVec& GetAttributes() const;

	// Saves data to xml node
	virtual void SaveTo(pugi::xml_node& node) const;

	// Loads data from xml node
	virtual void LoadFrom(pugi::xml_node& node);

protected:
	string                   mName;                    // Short name of section
	string                   mFullName;                // Full name of section with all parents names
	SyntaxSection*           mParentSection = nullptr; // Pointer to parent section (nullptr of section is global)
	SyntaxFunctionsVec       mFunctions;               // List of functions
	SyntaxVariablesVec       mVariables;               // List of variables
	SyntaxSectionsVec        mSections;                // List of nested sections (classes or namespaces)
	SyntaxEnumsVec           mEnums;                   // List of enum
	SyntaxCommentsVec        mComments;                // List of comments
	SyntaxTypedefsVec        mTypedefs;                // List of typedefs
	SyntaxUsingNamespacesVec mUsingNamespaces;         // List of using namespaces
	SyntaxAttributesVec      mAttributes;              // List of attributes

	friend class CodeToolCache;
	friend class CppSyntaxParser;
	friend class SyntaxTree;
};

// Syntax namespace
class SyntaxNamespace: public SyntaxSection
{
public:
	SyntaxNamespace();

	friend class CodeToolCache;
	friend class CppSyntaxParser;
	friend class SyntaxTree;
};

// Syntax class inheritance definition
class SyntaxClassInheritance: public ISyntaxExpression
{
public:
	// Default constructor
	SyntaxClassInheritance() {}

	// Constructor by class name and inheritance protection type
	SyntaxClassInheritance(const string& className, SyntaxProtectionSection type);

	// Returns class name
	const string& GetClassName() const;

	// Returns class
	SyntaxClass* GetClass();

	// Returns class inheritance protection type
	SyntaxProtectionSection GetInheritanceType() const;

	// Check equality operator
	bool operator==(const SyntaxClassInheritance& other) const;

	// Saves data to xml node
	void SaveTo(pugi::xml_node& node) const;

	// Loads data from xml node
	void LoadFrom(pugi::xml_node& node);

protected:
	string                  mClassName;       // Inheritance class name
	SyntaxClass*            mClass = nullptr; // Inheritance class (if found)
	SyntaxProtectionSection mInheritanceType; // Inheritance protection type

	friend class CodeToolCache;
	friend class CppSyntaxParser;
	friend class SyntaxTree;
};
typedef vector<SyntaxClassInheritance> SyntaxClassInheritancsVec;

// Syntax class or struct
class SyntaxClass: public SyntaxSection
{
public:
	// Returns array of functions
	const SyntaxFunctionsVec& GetFunctions() const;

	// Returns array of variables
	const SyntaxVariablesVec& GetVariables() const;

	// Returns nested sections
	const SyntaxSectionsVec& GetSections() const;

	// Returns nested enums
	const SyntaxEnumsVec& GetEnums() const;

	// Returns typedefs defined in this section
	const SyntaxTypedefsVec& GetTypedefs() const;

	// Returns using namespaces in this section
	const SyntaxUsingNamespacesVec& GetUsingNamespaces() const;

	// Returns all inside sections
	SyntaxSectionsVec GetAllSections() const;

	// Returns all inside classes
	SyntaxClassesVec GetAllClasses() const;

	// Returns is this section is class
	bool IsClass() const;

	// Returns base classes
	const SyntaxClassInheritancsVec& GetBaseClasses() const;

	// Returns is class template
	bool IsTemplate() const;

	// Returns is class meta (defined as "meta class name { ... };")
	bool IsMetaClass() const;

	// Returns template parameters (if exist)
	const string& GetTemplateParameters() const;

	// Returns parent class protection section
	SyntaxProtectionSection GetClassSection() const;

	// Returns comment definition for attribute (empty for not attribute classes)
	const string& GetAttributeCommentDef() const;

	// Returns short definition for attribute (empty for not attribute classes)
	const string& GetAttributeShortDef() const;

	// Returns attributes definitions
	const SyntaxAttributesVec& GetAttributes() const;

	// Saves data to xml node
	void SaveTo(pugi::xml_node& node) const;

	// Loads data from xml node
	void LoadFrom(pugi::xml_node& node);

protected:
	SyntaxClassInheritancsVec mBaseClasses;             // Base classes
	bool                      mIsMeta = false;          // Is class meta (defined as "meta class name { ... };")
	string                    mTemplateParameters;      // Template parameters (empty if class isn't template)

	SyntaxProtectionSection   mClassSection = SyntaxProtectionSection::Public; // protection section of parent class

	SyntaxClass*              mSourceClass = nullptr;   // Source class for template specialized classes

	string                    mAttributeCommentDef;     // Attribute comment definition
	string                    mAttributeShortDef;       // Attribute short definition

	friend class CodeToolCache;
	friend class CppSyntaxParser;
	friend class SyntaxTree;
};

// Syntax attributes list
class SyntaxAttributes: public ISyntaxExpression
{
public:
	// Returns list of attributes
	const vector<string>& GetAttributesList() const;

protected:
	vector<string> mAttributesList;

	friend class CodeToolCache;
	friend class CppSyntaxParser;
	friend class SyntaxTree;
};

// Syntax variable type
class SyntaxType: public ISyntaxExpression
{
public:
	// Returns name of type
	const string& GetName() const;

	// Returns is type is constant
	bool IsConstant() const;

	// Returns is type is reference
	bool IsReference() const;

	// Returns is type is pointer
	bool IsPointer() const;

protected:
	string mName;               
	bool   mIsContant = false;  
	bool   mIsReference = false;
	bool   mIsPointer = false;  
	bool   mIsMutable = false;

	friend class CppSyntaxParser;
};

// Syntax variable
class SyntaxVariable: public ISyntaxExpression
{
public:
	// Returns type of variable
	const SyntaxType& GetVariableType() const;

	// Returns name of variable
	const string& GetName() const;

	// Returns default value
	const string& GetDefaultValue() const;

	// Returns class definition section
	SyntaxProtectionSection GetClassSection() const;

	// Returns is variable is static
	bool IsStatic() const;

protected:
	SyntaxType              mType;                                           // Type of variable
	string                  mName;                                           // Name of variable
	string                  mDefaultValue;                                   // Default variable value
	SyntaxProtectionSection mClassSection = SyntaxProtectionSection::Public; // Protection section
	bool                    mIsStatic = false;                               // Is variable static

	friend class CppSyntaxParser;
};

// Syntax function
class SyntaxFunction: public ISyntaxExpression
{
public:
	// Default constructor
	SyntaxFunction();

	// Destructor
	~SyntaxFunction();

	// Returns function's returning type
	const SyntaxType& GetReturnType() const;

	// Returns name of function
	const string& GetName() const;

	// Returns list of function's parameters
	const SyntaxVariablesVec& GetParameters() const;

	// Returns protection section
	SyntaxProtectionSection GetClassSection() const;

	// Returns is function template
	bool IsTemplate() const;

	// Returns function templates (if have)
	const string& GetTemplates() const;

	// Returns is function static
	bool IsStatic() const;

protected:
	SyntaxType              mReturnType;									 // Returning type
	string                  mName;											 // Name of function
	SyntaxVariablesVec      mParameters;									 // List of parameters
	SyntaxProtectionSection mClassSection = SyntaxProtectionSection::Public; // Protection Section
	bool                    mIsStatic = false;								 // Is function static
	bool                    mIsVirtual = false;								 // Is function virtual
	bool                    mIsContstant = false;							 // Is function constant
	string                  mTemplates;                                      // Function templates

	friend class CppSyntaxParser;
};

// Syntax enum
class SyntaxEnum: public ISyntaxExpression
{
public:
	// Returns name of enum
	const string& GetName() const;

	// Returns full name of enum with all parent spaces names
	const string& GetFullName() const;

	// Returns enum entries
	const StringStringDict& GetEntries() const;

	// Returns protection section
	SyntaxProtectionSection GetClassSection() const;

	// Returns owner syntax section
	SyntaxSection* GetOwnerSyntaxSection() const;

protected:
	string                  mName;	                 // Name of enum
	string                  mFullName;               // Full enum name with all parent spaces names
	StringStringDict        mEntries;                // Entries of enum
	SyntaxSection*          mOwnerSection = nullptr; // Owner syntax section
	SyntaxProtectionSection mClassSection = SyntaxProtectionSection::Public;

	friend class CppSyntaxParser;
};
