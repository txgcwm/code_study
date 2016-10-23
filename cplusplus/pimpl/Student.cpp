#include <string.h>
#include <stdio.h>

#include "Student.h"

#ifdef USE_STRUCT
struct Student::Pimpl {

	Pimpl(std::string name, int age)
	{
		m_name = name; 
		m_age = age;

		printf("name: %s, age: %d\n", name.c_str(), age);
	}
	~Pimpl(){}

	std::string m_name;
	int m_age;

	void SetName(std::string name)
	{
		m_name = name;
		printf("name: %s\n", name.c_str());
	}

	void SetAge(int age)
	{
		m_age = age;
		printf("age: %d\n", age);
	}
}; 
#else
class StudentPimpl
{
public:
	StudentPimpl(std::string name, int age);
	~StudentPimpl();

	void SetName(std::string name);
	void SetAge(int age);

private:
	std::string m_name;
	int m_age;
};
#endif

Student::Student(std::string name, int age)
#ifdef USE_STRUCT
: m_pimpl(new Pimpl(name, age))
#else
: m_pimpl(new StudentPimpl(name, age))
#endif
{

}

Student::~Student()
{

}

void Student::SetName(std::string name)
{
	m_pimpl->SetName(name);
}

void Student::SetAge(int age)
{
	m_pimpl->SetAge(age);
}


#ifndef USE_STRUCT 
StudentPimpl::StudentPimpl(std::string name, int age)
: m_name(name)
, m_age(age)
{
	printf("name: %s, age: %d\n", name.c_str(), age);
}

StudentPimpl::~StudentPimpl()
{

}

void StudentPimpl::SetName(std::string name)
{
	m_name = name;
	printf("name: %s\n", name.c_str());

	return;
}

void StudentPimpl::SetAge(int age)
{
	m_age = age;
	printf("age: %d\n", age);

	return;
}
#endif
