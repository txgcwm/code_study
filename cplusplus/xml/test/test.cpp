#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

#include "tinyxml.h"

using namespace std;



void printSchoolXml(const char* xmlFile)
{
	TiXmlDocument doc;  
	
	if (doc.LoadFile(xmlFile)) {  	
		doc.Print();  
	} else {
		cout << "can not parse xml conf/school.xml" << endl;
	}
}

int write_school_xml(const char* xmlFile)
{
	TiXmlDocument doc;  
	TiXmlDeclaration * decl = new TiXmlDeclaration("1.0", "", "");  
	TiXmlElement * schoolElement = new TiXmlElement( "School" );  
	TiXmlElement * classElement = new TiXmlElement( "Class" );  
	classElement->SetAttribute("name", "C++");

	TiXmlElement * stu1Element = new TiXmlElement("Student");
	stu1Element->SetAttribute("name", "tinyxml");
	stu1Element->SetAttribute("number", "123");
	TiXmlElement * stu1EmailElement = new TiXmlElement("email");
	stu1EmailElement->LinkEndChild(new TiXmlText("tinyxml@163.com") );
	TiXmlElement * stu1AddressElement = new TiXmlElement("address");
	stu1AddressElement->LinkEndChild(new TiXmlText("中国"));
	stu1Element->LinkEndChild(stu1EmailElement);
	stu1Element->LinkEndChild(stu1AddressElement);

	TiXmlElement * stu2Element = new TiXmlElement("Student");
	stu2Element->SetAttribute("name", "jsoncpp");
	stu2Element->SetAttribute("number", "456");
	TiXmlElement * stu2EmailElement = new TiXmlElement("email");
	stu2EmailElement->LinkEndChild(new TiXmlText("jsoncpp@163.com"));
	TiXmlElement * stu2AddressElement = new TiXmlElement("address");
	stu2AddressElement->LinkEndChild(new TiXmlText("美国"));
	stu2Element->LinkEndChild(stu2EmailElement);
	stu2Element->LinkEndChild(stu2AddressElement);

	classElement->LinkEndChild(stu1Element);  
	classElement->LinkEndChild(stu2Element);  
	schoolElement->LinkEndChild(classElement);  
	
	doc.LinkEndChild(decl);  
	doc.LinkEndChild(schoolElement); 
	doc.SaveFile(xmlFile);

	return 0; 
}

int read_school_xml(const char* xmlFile)
{
	TiXmlDocument doc;

	if (doc.LoadFile(xmlFile)) {  	
		// doc.Print();  
	} else {
		cout << "can not parse xml!" << endl;
		return -1;
	}

	TiXmlElement* rootElement = doc.RootElement();  //School元素  
	TiXmlElement* classElement = rootElement->FirstChildElement();  // Class元素
	TiXmlElement* studentElement = classElement->FirstChildElement();  //Students

	cout << classElement->Value() << endl;

	for (; studentElement != NULL; studentElement = studentElement->NextSiblingElement() ) {
		cout << endl << studentElement->Value() << endl;

		TiXmlAttribute* attributeOfStudent = studentElement->FirstAttribute();  //获得student的name属性  

		for (;attributeOfStudent != NULL; attributeOfStudent = attributeOfStudent->Next() ) {  
			cout << attributeOfStudent->Name() << " : " << attributeOfStudent->Value() << endl;  
		}

		TiXmlElement* studentContactElement = studentElement->FirstChildElement();//获得student的第一个联系方式 
		for (; studentContactElement != NULL; studentContactElement = studentContactElement->NextSiblingElement() ) {
			string contactType = studentContactElement->Value();
			string contactValue = studentContactElement->GetText();
			cout << contactType  << " : " << contactValue << endl;  
		}
	}

	return 0; 
}

int main(int argc, char *argv[])
{
	int res = -1;
	int action = 1;
	char filename[20] = {0};

	while((res = getopt(argc, argv, "?a:f:h")) != -1) {
        switch(res) {
        case 'a':
            action = atoi(optarg);
            break;

        case 'f':
            memcpy(filename, optarg, strlen(optarg));
            break;

        case 'h':
        default:
            // usage(argv[0]);
            return -1;
        }
    }

    if(action == 1) {
    	read_school_xml(filename);
    } else if(action == 2) {
    	write_school_xml(filename);
    }

	return 0;
}

