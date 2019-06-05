// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
#include "XmlParser.h"


namespace XmlParserLib
{

		bool XmlParser::isSymbol(char c)
		{
			if ((c >= 48) && (c <= 57))
				return true;

			if ((c >= 65) && (c <= 90))
				return true;

			if ((c >= 97) && (c <= 122))
				return true;

			return false;
		}

		XmlParserReport XmlParser::ParseXmlTag(char* pointer)
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

				#pragma region Ñhildrens

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
						XmlParserReport report = ParseXmlTag(pointer);
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
}