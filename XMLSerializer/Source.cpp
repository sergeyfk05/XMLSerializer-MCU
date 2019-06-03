#include <iostream>
#include <list>

using namespace std;

static bool isSymbol(char c)
{
	if ((c >= 48) && (c <= 57))
		return true;

	if ((c >= 65) && (c <= 90))
		return true;

	if ((c >= 97) && (c <= 122))
		return true;

	return false;
}

class XmlAttribute
{
private:
	char* name;
	char* value;

public:
	XmlAttribute(const char* name, const char* value)
	{
		if (name != NULL)
		{
			uint8_t buf = strlen(name) + 1;
			this->name = (char*)malloc(buf);
			strcpy_s(this->name, buf, name);
		}

		if (value != NULL)
		{
			uint8_t buf = strlen(value) + 1;
			this->value = (char*)malloc(buf);
			strcpy_s(this->value, buf, value);
		}
	}

	char* GetName()
	{
		return name;
	}

	char* GetValue()
	{
		return value;
	}

	~XmlAttribute()
	{
		if (name != NULL)
			delete[] name;

		if (value != NULL)
			delete value;
	}
};

class XmlElement
{
private:
	list<XmlElement*>* childrens;
	char* tagName;
	list<XmlAttribute*>* attributes;

public:
	XmlElement()
	{
		childrens = new list<XmlElement*>();
		attributes = new list<XmlAttribute*>();
	}
	XmlElement(char* tagName) : XmlElement()
	{
		SetName(tagName);
	}


	list<XmlElement*>* GetChildrens() {
		return childrens;
	}
	list<XmlAttribute*>* GetAttributes() {
		return attributes;
	}
	void SetName(const char* tagName)
	{
		if (tagName != NULL)
		{
			uint8_t buf = strlen(tagName) + 1;
			this->tagName = (char*)malloc(buf);
			strcpy_s(this->tagName, buf, tagName);
		}
	}


	~XmlElement() {

		if (childrens != NULL)
			for (XmlElement* el : *childrens)
				delete el;
		delete childrens;

		if (attributes != NULL)
			for (XmlAttribute* el : *attributes)
				delete el;
		delete attributes;

		delete[] tagName;
	}
};

enum ParserStatus {
	None = 0,
	TagName = 1, 
	AdditionalInfoSector = 2,
	AttributeName,
	AttributeValue, 
	ChildrensSection
};

static XmlElement* ParseTag(char* pointer)
{
	bool skipSpace = true;
	char tagName[20];
	char attrName[20];
	char attrValue[20];
	XmlElement* result = new XmlElement();
	ParserStatus status = None;

	while (true) 
	{

#pragma region SkippedChars

		if (*pointer <= 31) {
			pointer++;
			continue;
		}
		if ((skipSpace) && (*pointer == ' '))
		{
			pointer++;
			continue;
		}

#pragma endregion

#pragma region OpenTag

		if ((status == None) && (*pointer == '<'))
		{
			status = TagName;
			skipSpace = false;
			tagName[0] = '\0';
			pointer++;
			continue;
		}

#pragma endregion

#pragma region TagName

		if ((status == TagName) && isSymbol(*pointer))
		{
			strncat_s(tagName, pointer, 1);
			pointer++;
			continue;
		}

		if ((status == TagName) && (*pointer == ' '))
		{
			status = AdditionalInfoSector;
			result->SetName(tagName);
			skipSpace = true;
			pointer++;
			continue;
		}

#pragma endregion

#pragma region Attributes

		if ((status == AdditionalInfoSector) && (isSymbol(*pointer)))
		{
			attrName[0] = '\0';
			strncat_s(attrName, pointer, 1);
			status = AttributeName;
			skipSpace = false;
			pointer++;
			continue;
		}

		if ((status == AttributeName) && (isSymbol(*pointer)))
		{
			strncat_s(attrName, pointer, 1);
			pointer++;
			continue;
		}

		if ((status == AttributeName) && (*pointer == '=') && (*(pointer + 1) == '\''))
		{
			attrValue[0] = '\0';
			skipSpace = false;
			status = AttributeValue;
			pointer += 2;
			continue;
		}

		if ((status == AttributeValue) && isSymbol(*pointer))
		{
			strncat_s(attrValue, pointer, 1);
			pointer++;
			continue;
		}

		if ((status == AttributeValue) && (*pointer == '\''))
		{
			skipSpace = true;
			status = AdditionalInfoSector;
			result->GetAttributes()->push_front(new XmlAttribute(attrName, attrValue));
			pointer++;
			continue;
		}

#pragma endregion

#pragma region CloseTag

		if (((status == TagName) || (status == AdditionalInfoSector)) && (*pointer == '/') && (*(pointer + 1) == '>'))
		{
			pointer += 2;
			break;
		}

		if (((status == TagName) || (status == AdditionalInfoSector)) && (*pointer == '>'))
		{
			skipSpace = true;
			status = ChildrensSection;
			pointer++;
			continue;
		}

#pragma endregion

#pragma region Сhildrens

		if ((status == ChildrensSection) && (*pointer == '<'))
		{
			if (*(pointer + 1) == '/')
			{
				pointer += 2;
				uint8_t buf = strlen(tagName);
				if (strncmp(pointer, tagName, buf) != 0)
				{
					delete result;
					return NULL;
				}

				pointer += buf;
				if (*pointer != '>')
				{
					delete result;
					return NULL;
				}

				pointer += 1;
				continue;

			}
			else
			{
				throw new exception("не изменяется указатель на конец потомка");
				result->GetChildrens()->push_front(ParseTag(pointer));
				continue;
			}
		}

#pragma endregion
			   

		delete result;
		return NULL;

	}
	return result;
}


int main()
{
	char g[] = "  <block attr1=\'ght\' attr2=\'ght2\'><block1 attr1=\'ght\' attr2=\'ght2\'/><block2 attr1=\'ght\' attr2=\'ght2\'/></block>";
	char gg[] = "  <block attr1=\'ght\' />";
	ParseTag(g);
	system("pause");
	XmlAttribute* a = new XmlAttribute(NULL, g);
	delete a;
	system("pause");
	XmlElement* e = new XmlElement();

	for (uint32_t i = 0; i < 100000; i++)
		e->GetChildrens()->push_front(ParseTag(g));


	system("pause");
	delete e;

	system("pause");

}