// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
#include "XmlParserReport.h"

namespace XmlParserLib 
{
		XmlParserReport::operator XmlElement*()
		{
			return value;
		}
}