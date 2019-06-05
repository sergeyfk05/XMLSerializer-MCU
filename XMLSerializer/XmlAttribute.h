// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
#pragma once
#include <stdint.h>
#include <cstring>
#include <cstdlib>


namespace XmlParserLib
{
	class XmlAttribute
	{
	private:
		char* name;
		char* value;

	public:
		XmlAttribute(const char* name, const char* value);

		char* GetName();

		char* GetValue();

		~XmlAttribute();
	};
}