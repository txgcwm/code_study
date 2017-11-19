#include "SmartPtr.h"



int main(int argc, char** argv)
{
    int *i = new int(2);
    {
        SmartPtr<int> ptr1(i);
        {
            SmartPtr<int> ptr2(ptr1);
            {
                SmartPtr<int> ptr3 = ptr2;

                std::cout << *ptr1 << std::endl;
                *ptr1 = 20;
                std::cout << *ptr2 << std::endl;
            }
        }
    }

    return 0;
}