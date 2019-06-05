//This is an independent project of an individual developer. Dear PVS-Studio, please check it.

#include <iostream>
#include <list>
#include "XmlParser.h"

using namespace XmlParserLib;
using namespace std;



int main()
{
	//char g[] = "  <block attr1=\'ght\' attr2=\'ght2\'><block1 attr1=\'ght\' attr2=\'ght2\'/><block2 attr1=\'ght\' attr2=\'ght2\'/></block>";
	char gg[] = "  <b> </b>";

	system("pause");
	
	XmlElement* e = XmlParser::ParseXmlTag(gg);
	for (uint32_t i = 0; i < 200000; i++)
		e->GetChildrens()->push_front(XmlParser::ParseXmlTag(gg));
	

	delete e;

	system("pause");

}