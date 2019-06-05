// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
#pragma once
#include "XmlParserReport.h"
#include "XmlAttribute.h"


namespace XmlParserLib
{
	class XmlParser
	{
	private:
		enum ParserStatus {
			None = 0,
			TagName = 1,
			AdditionalInfoSector = 2,
			AttributeName,
			AttributeValue,
			ChildrensSection
		};
		static bool isSymbol(char c);

	public:
		static XmlParserReport ParseXmlTag(char* pointer);
	};
}