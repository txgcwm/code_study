#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>


using namespace std;

/// 求从数组a[1..n]中任选m个元素的所有组合。
/// a[1..n]表示候选集，n为候选集大小，n>=m>0。
/// b[1..M]用来存储当前组合中的元素(这里存储的是元素下标)，
/// 常量M表示满足条件的一个组合中元素的个数，M=m，这两个参数仅用来输出结果。
void combine(int a[], int n, int m, int b[], const int M)
{
 	for(int i = n; i >= m; i--) { // 注意这里的循环范围
 
  		b[m-1] = i - 1;
  		if (m > 1) {
   			combine(a, i-1, m-1, b, M);
  		} else {                     // m == 1, 输出一个组合
  		 
   			for(int j = M-1; j >= 0; j--) {
    			cout << a[b[j]] << " ";
   			}

   			cout << endl;
  		}
 	}

 	return;
}

/// 求从数组a[1..n]中任选m个元素的所有组合。
/// a[1..n]表示候选集，m表示一个组合的元素个数。
/// 返回所有组合的总数。
int combine(int a[], int n, int m)
{  
	int i = 0;

	m = m > n ? n : m;

	int* order = new int[m+1];   
	for(i=0; i<=m; i++) {
		order[i] = i-1;    // 注意这里order[0]=-1用来作为循环判断标识
	}

	int count = 0;                               
	int k = m;
	bool flag = true;           // 标志找到一个有效组合
	while(order[0] == -1) {
	  	if(flag) {                // 输出符合要求的组合
	   		for(i=1; i<=m; i++) {
	   			cout << a[order[i]] << " ";
	   		}                  

	   		cout << endl;
	   		count++;
	   		flag = false;
	  	}

	  	order[k]++;                // 在当前位置选择新的数字
	  	if(order[k] == n) {         // 当前位置已无数字可选，回溯
	   		order[k--] = 0;
	   		continue;
	  	}    
	 
	  	if(k < m) {               // 更新当前位置的下一位置的数字 
	   		order[++k] = order[k-1];
	   		continue;
	  	}
	 
	  	if(k == m) {
	   		flag = true;
	  	}
	}

	delete[] order;
	
	return count;
}

int PaiLie()
{
	int ia[] = {1,2,3,4};
 	vector<int> iv(ia,ia+sizeof(ia)/sizeof(int));

 	copy(iv.begin(),iv.end(),ostream_iterator<int>(cout," "));
 	cout << endl;
 
 	while(next_permutation(iv.begin(),iv.end())) {
  		copy(iv.begin(),iv.end(),ostream_iterator<int>(cout," "));
  		cout << endl;
 	}

 	return 0;
}

int main(int argc, char **argv)
{
 	const int N = 4;
 	const int M = 3;
 	int a[N];

 	for(int i=0;i<N;i++) {
  		a[i] = i+1;
 	}

 	// 回溯方法
 	combine(a, N, 3);

 	cout << endl;

 	// 递归方法
 	int b[M];
 	combine(a, N, M, b, M);

 	return 0;
}



// http://www.jb51.net/article/54443.htm