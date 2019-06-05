// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
#include "XmlAttribute.h"



namespace XmlParserLib
{

		XmlAttribute::XmlAttribute(const char* name, const char* value)
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

		char* XmlAttribute::GetName()
		{
			return name;
		}

		char* XmlAttribute::GetValue()
		{
			return value;
		}

		XmlAttribute::~XmlAttribute()
		{
			if (name != NULL)
				delete[] name;

			if (value != NULL)
				delete value;
		}
}