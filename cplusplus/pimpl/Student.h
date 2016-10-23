#ifndef __STUDENT_H__
#define __STUDENT_H__

#include <memory>
#include <string>



#define USE_STRUCT

class Student
{
public:
	Student(std::string name, int age);
	~Student();

	void SetName(std::string name);
	void SetAge(int age);

private:
#ifdef USE_STRUCT
	struct Pimpl;
	std::auto_ptr<Pimpl> m_pimpl;
#else
	class StudentPimpl;
	std::auto_ptr<StudentPimpl> m_pimpl;
#endif
};

#endif
