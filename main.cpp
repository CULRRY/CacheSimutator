#include <iostream>
#include "CacheSimulator.h"
#include <windows.h>
#include <list>

using namespace std;


int main()
{

	CacheSimulator c{};

	char* p = new char;

	uint64 d = reinterpret_cast<uint64>(p) & ~(0xFFFULL);

	p = (char*)d;


	char* a[10];

	for (int i = 0; i < 10; i++)
	{
		a[i] = p + 4096 * i;
		c.Access(*a[i]);
		c.PrintHitRate(*a[i]);
	}


	c.Access(*a[0]);
	c.Access(*a[1]);
	c.Access(*a[0]);
	c.Access(*a[9]);

	c.PrintTotalHitRate();
	


	return 0;

}