#ifndef __STUDENT_PIMPL_H__
#define __STUDENT_PIMPL_H__

#include <string>



class Student;

class StudentPimpl
{
public:
	StudentPimpl(std::string name, int age);
	~StudentPimpl();

	void SetName(std::string name);
	void SetAge(int age);

private:
	std::auto_ptr<Student> m_pimpl;
	
};

#endif
