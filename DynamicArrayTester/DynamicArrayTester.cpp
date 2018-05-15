// DynamicArrayTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "../DynamicArray/dynamic.h"

typedef struct
{
	bool a;
	int b;
	char c[32];
	double d;

} EXAMPLE;

void test1()
{
	std::cout << sizeof(char) << std::endl;
	std::cout << sizeof(EXAMPLE) << std::endl;

	int h = da_new(sizeof(EXAMPLE));
	EXAMPLE e1, e2, e3;
	memset(&e1, 0, sizeof(EXAMPLE));
	memset(&e2, 0, sizeof(EXAMPLE));
	memset(&e3, 0, sizeof(EXAMPLE));
	e1 = { true,20,"banana",24.5 };
	e2 = { false,-50,"orange", 51.4 };
	e3 = { true,30,"cherry", 68.5 };
	da_reserve(h, 5);
	da_push_back(h, (void*)&e1);
	da_push_back(h, (void*)&e2);
	da_push_back(h, (void*)&e3);
	da_push_back(h, (void*)&EXAMPLE({ false, -10, "bud light lime", 58.4 }));
	int size = da_size(h);
	std::cout << "size: " << size << std::endl;
	EXAMPLE* pEx = (EXAMPLE*)da_data(h, 0);
	for (int i = 0; i < size; i++)
	{
		const char* tf;
		if (pEx[i].a) tf = "true";
		else tf = "false";
		printf("i: %d, %s, %d, %s, %.2f\n", i, tf, pEx[i].b, pEx[i].c, pEx[i].d);
	}

	pEx = (EXAMPLE*)da_data(h, 3);
	char* pExc = pEx[0].c;
	for (int j = 0; j < 32; j++)
	{
		char a = pExc[j];
		if (a == '\0') putchar('_');
		else putchar(pExc[j]);
	}
	printf("\r\n");
	da_delete_all();
}

// print int dy-array
void pr_int(int handle)
{
	int sz = da_size(handle);
	if (!da_data(handle,0)) {
		std::cout << "No handle found!" << std::endl;
		return;
	}
	printf("%p", da_data(handle, 0));
	std::cout << " [";
	for (int i = 0; i < sz; i++)
	{
		if (i) std::cout << ",";
		int* pVal = (int*)da_data(handle, i);
		std::cout << std::to_string(*pVal);
	}
	std::cout
		<< "] ("
		<< da_size(handle)
		<< "/"
		<< da_capacity(handle)
		<< ")"
		<< std::endl;

}
void test2()
{
	int h = da_new(sizeof(int));
	da_reserve(h, 20);
	pr_int(h);
	int a[] = { 45, 378, 3287, 45984, -2397, 379 };
	da_insert(h, &a, 0, sizeof(a) / sizeof(int));
	pr_int(h);
	int b[] = { 901,902,903,904 };
	da_insert(h, &b, 3, sizeof(b) / sizeof(int));
	pr_int(h);
	da_shrink_to_fit(h);
	pr_int(h);
	da_reserve(h,1);
	pr_int(h);
	int ten = 10;
	da_push_back(h, &ten);
	pr_int(h);
	//da_resize(h, 5);
	//pr_int(h);
	da_resize(h, 8);
	da_shrink_to_fit(h);
	pr_int(h);
	da_erase(h, 1, 3);
	pr_int(h);
	da_erase(h, 0, 200);
	pr_int(h);
	da_clear(h);
	pr_int(h);
	da_delete(h);
	pr_int(h);
}

int main()
{
	test1();
	test2();

	return 0;
}

