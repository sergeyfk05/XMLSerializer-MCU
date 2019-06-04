// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

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
			delete[] this->tagName;
			uint8_t buf = strlen(tagName) + 1;
			this->tagName = (char*)malloc(buf);
			strcpy_s(this->tagName, buf, tagName);
		}
	}
	char* ToString()
	{
		char* result;
		throw new exception("not realized");
		return result;
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

class XmlParserReport
{
public:
	char* endPointer;
	XmlElement* value;
	bool isSucsessful = true;

	operator XmlElement*()
	{
		return value;
	}
	operator int() { return 1; }
};

static XmlParserReport ParseTag(char* pointer)
{
	bool skipSpace = true;
	char tagName[20];
	char attrName[20];
	char attrValue[20];
	XmlParserReport result;
	XmlElement* value = new XmlElement();
	ParserStatus status = None;

	while (true) 
	{

#pragma region SkippedChars

		if (*pointer == '\0')
		{
			delete value;
			result.isSucsessful = false;
			return result;
		}

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
			value->SetName(tagName);
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
			value->GetAttributes()->push_front(new XmlAttribute(attrName, attrValue));
			pointer++;
			continue;
		}

#pragma endregion

#pragma region CloseTag

		if (((status == TagName) || (status == AdditionalInfoSector)) && (*pointer == '/') && (*(pointer + 1) == '>'))
		{
			value->SetName(tagName);
			pointer += 2;
			break;
		}

		if (((status == TagName) || (status == AdditionalInfoSector)) && (*pointer == '>'))
		{
			value->SetName(tagName);
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
					delete value;
					result.isSucsessful = false;
					return result;
				}

				pointer += buf;
				if (*pointer != '>')
				{
					delete value;
					result.isSucsessful = false;
					return result;
				}

				pointer += 1;
				break;

			}
			else
			{
				//throw new exception("не изменяется указатель на конец потомка");
				XmlParserReport report = ParseTag(pointer);
				pointer = report.endPointer;
				value->GetChildrens()->push_front(report.value);
				continue;
			}
		}

#pragma endregion
			   

		delete value;
		result.isSucsessful = false;
		return result;

	}

	result.value = value;
	result.endPointer = pointer;

	return result;
}


int main()
{
	char g[] = "  <block attr1=\'ght\' attr2=\'ght2\'><block1 attr1=\'ght\' attr2=\'ght2\'/><block2 attr1=\'ght\' attr2=\'ght2\'/></block>";
	char gg[] = "  <block> </block>";
	system("pause");
	XmlElement* e = ParseTag(g);

	for (uint32_t i = 0; i < 200000; i++)
		e->GetChildrens()->push_front(ParseTag(gg));


	system("pause");
	delete e;

	system("pause");

}