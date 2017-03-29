#include <stdlib.h>
#include <iostream>
#include <string>


using namespace std;

void display(const int *a)
{
 	for(int i=0;a[i];i++)
  		cout<<a[i]<<" ";

 	cout<<endl;
}

void cat(const string &str,int *temp,int &a)
{
 	int count=0,k=0;
 	unsigned int i=0;
 	char strtemp[255]={0};

 	while(i<str.size()) {
  		while(isdigit(str[i])&&i<str.size()) {
   			strtemp[k++]=str[i++];
		}

  		strtemp[k]='\0';
  		temp[count++]=atoi(strtemp);
  		strtemp[0]='\0';
  		k=0;
  		i++;
 	}

 	temp[count]='\0';
 	a=count;

	return;
}

int main(int argc, char **argv)
{
 	string temp="444 43 2343 23432 33";
 	int aa[255];
 	int a=0;
 	cout<<temp<<endl;
 	cat(temp,aa,a);
 	display(aa);
 	cout<<"共"<<a<<"次！"<<endl;

 	return 0;
}
