#include <cstdlib>	//malloc, free
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
using namespace std;

void* myAlloc(size_t size)
{  
	return malloc(size); 
}

void myFree(void* ptr)
{
	free(ptr); 
	return;
}

//A user-defined operator new() / operator delete() which can count allocated memory size and counts.
//This can only be bypassed if the program directly call malloc() 
static  size_t countNew = 0;
static  size_t countDel = 0;	    
static  size_t countArrayNew = 0;
static  size_t countArrayDel = 0;
static  size_t timesNew = 0;

inline void* operator new(size_t size) 	 
{
	//cout << " global new(), \t" << size << "\t"; 
	countNew += size; 
	++timesNew;  
	
	//void* p = myAlloc( size ); 
	//cout << p << endl; 
	//return p;
	
	return myAlloc( size ); 	    
}

inline void* operator new[](size_t size)  
{ 
	//cout << " global new[](), \t" << size << "\t";
	countArrayNew += size; 				
	
	void* p = myAlloc( size ); 
	//cout << p << endl; 
	return p;
	
	//return myAlloc( size ); 	    
}	

//(1) 
inline void  operator delete(void* ptr, size_t size)  
{  	
//	cout << "global delete(ptr,size), \t" << ptr << "\t" << size << endl;	
	countDel += size;  	
	myFree( ptr ); 			
}
//(2)
inline void  operator delete(void* ptr)  
{  	
//	cout << "global delete(ptr), \t" << ptr << endl;	 	
	myFree( ptr ); 			
}	

//(1)
inline void  operator delete[](void* ptr, size_t size) 
{ 
//	cout << "global delete[](ptr,size), \t" << ptr << "\t" << size << endl;
	countArrayDel += size; 
	myFree( ptr ); 			
}	
//(2)
inline void  operator delete[](void* ptr) 
{ 
//	cout << "global delete[](ptr), \t" << ptr << endl;
	myFree( ptr ); 			
}

void resetCount()
{
	countNew = 0;
	countDel = 0;
	countArrayNew = 0;
	countArrayDel = 0;
	timesNew = 0;
}

const int gsMapCount = 1000*1000;
namespace jj08
{
	//-------------	
	void test_overload_global_new()
	{
		{
			resetCount();
			string* p = new string("A");
			delete p;
		}
		{
			resetCount();
			string*  p = new string[3];

			delete[] p;
		}
		{
			resetCount();
			vector<int> vec(10);

			vec.push_back(1);
			vec.push_back(1);
			vec.push_back(1);
		}
					
		{
			resetCount();
			list<int> lst;		
			lst.push_back(1); 	
			lst.push_back(1); 	
			lst.push_back(1); 					
		}
		{
			resetCount();
			map<size_t, size_t> lmap;
			for (size_t i = 0; i < gsMapCount; i++)
			{
				lmap[i] = i;
			}
		}
		{
			resetCount();
			unordered_map<size_t, size_t> lmap;
			for (size_t i = 0; i < gsMapCount; i++)
			{
				lmap[i] = i;
			}
		}
									 
	}
}
int main(int argc, char** argv) 
{
	cout << __cplusplus << endl;
	jj08::test_overload_global_new();
	return 0;
}