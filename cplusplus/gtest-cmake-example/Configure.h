#ifndef __CONFIGURE_H__
#define __CONFIGURE_H__

#include <string>
#include <vector>


class Configure
{
public:
    int addItem(std::string str);
    std::string getItem(int index);
    int getSize();

private:
    std::vector<std::string> vItems;
};

#endif 
