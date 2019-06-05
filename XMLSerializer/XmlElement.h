// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
#pragma once
#include "XmlAttribute.h"
#include <list>

using namespace std;


namespace XmlParserLib
{
	class XmlElement
	{
	private:
		list<XmlElement*>* childrens;
		char* tagName;
		list<XmlAttribute*>* attributes;

	public:
		XmlElement();
		XmlElement(char* tagName);

		list<XmlElement*>* GetChildrens();
		list<XmlAttribute*>* GetAttributes();
		void SetName(const char* tagName);
		char* ToString();

		~XmlElement();
	};

}