// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
#include "XmlElement.h"



namespace XmlParserLib
{
		XmlElement::XmlElement()
		{
			childrens = new list<XmlElement*>();
			attributes = new list<XmlAttribute*>();
		}
		XmlElement::XmlElement(char* tagName) : XmlElement()
		{
			SetName(tagName);			
		}

		list<XmlElement*>* XmlElement::GetChildrens() {
			return childrens;
		}
		list<XmlAttribute*>* XmlElement::GetAttributes() {
			return attributes;
		}
		void XmlElement::SetName(const char* tagName)
		{
			if (tagName != NULL)
			{
				delete[] this->tagName;
				uint8_t buf = strlen(tagName) + 1;
				this->tagName = (char*)malloc(buf);
				strcpy_s(this->tagName, buf, tagName);
			}
		}
		char* XmlElement::ToString()
		{
			char* result;
			throw new exception("not realized");
			return result;
		}

		XmlElement::~XmlElement() {

			if (childrens != NULL)
				for (XmlElement* el : *childrens)
					delete el;
			delete childrens;

			if (attributes != NULL)
				for (XmlAttribute* el : *attributes)
					delete el;
			delete attributes;


			if (tagName != NULL)
				delete[] tagName;
		}

}