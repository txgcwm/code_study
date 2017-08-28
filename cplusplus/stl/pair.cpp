    #include <iostream>
    
    
    int main(int argc, char *argv[])
    {
        typedef std::pair<int, std::string> Student;
        //初始化pair的第一种方式 通过构造函数初始化
        Student stu_first(2011, "hahaya");
        //初始化pair的第二种方式 通过初始化成员变量初始化
        Student stu_second;
        stu_second.first = 2012;
        stu_second.second = "ToSmile";
        //初始化pair的第三种方式 通过辅助函数std::make_pair来初始化
        Student stu_third = std::make_pair(2013, "http://hahaya.github.com");
        //std::pair对象的输出
        std::cout << "number:" << stu_first.first << "\tname:" << stu_first.second << std::endl;
        std::cout << "number:" << stu_second.first << "\tname:" << stu_second.second << std::endl;
        std::cout << "number:" << stu_third.first << "\tname:" << stu_third.second << std::endl;
        
        return 0;
    }
