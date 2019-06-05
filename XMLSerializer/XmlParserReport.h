// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
#pragma once
#include "XmlElement.h"

namespace XmlParserLib
{
	class XmlParserReport
	{
	public:
		char* endPointer;
		XmlElement* value;
		bool isSucsessful;

		operator XmlElement*();
	};
}