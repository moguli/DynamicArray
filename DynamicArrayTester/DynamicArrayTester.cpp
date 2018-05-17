// DynamicArrayTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include "../DynamicArray/dynamic.h"



void test1();
void test2();
void pr_int(int handle);
int test3();
int test4();

int main()
{
	//test1();
	//test2();
	test3();
	test4();

	return 0;
}





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


// ASSET LIST CSV PARSER PROTOTYPING






//#include<vector>
typedef double var;

// An asset module is an asset list line item with eight extra parameters.
typedef struct AssetListEntry
{
	char sName[32];		// Asset name
	var vPrice;
	var vSpread;
	var vRollLong;
	var vRollShort;
	var vPip;
	var vPipCost;
	var vMarginCost;
	var vLeverage;
	var vLotAmount;
	var vCommission;
	char sSymbol[32];
} AssetListEntry;

AssetListEntry* ParseAssetLine(const char* line)
{
	char* line_temp = (char*)malloc(strlen(line) + 1);
	strcpy(line_temp, line);
	const char skip[2] = ",";
	char* token;
	static AssetListEntry output;
	memset(&output, 0, sizeof(output));
	/* get the first token */
	token = strtok(line_temp, skip);

	int i;
	/* walk through other tokens */
	for (i = 0; ((token != NULL) && (i < 12)); i++)   // no more than 12 arguments will be accepted.
	{
		switch (i)
		{
		case 0: sscanf(token, "%s", &output.sName); break;
		case 1: sscanf(token, "%lf", &output.vPrice); break;
		case 2: sscanf(token, "%lf", &output.vSpread); break;
		case 3: sscanf(token, "%lf", &output.vRollLong); break;
		case 4: sscanf(token, "%lf", &output.vRollShort); break;
		case 5: sscanf(token, "%lf", &output.vPip); break;
		case 6: sscanf(token, "%lf", &output.vPipCost); break;
		case 7: sscanf(token, "%lf", &output.vMarginCost); break;
		case 8: sscanf(token, "%lf", &output.vLeverage); break;
		case 9: sscanf(token, "%lf", &output.vLotAmount); break;
		case 10: sscanf(token, "%lf", &output.vCommission); break;
		case 11: sscanf(token, "%s", &output.sSymbol); break; // optional
		default: break;
		}
		token = strtok(NULL, skip);
	}
	free(line_temp);								// Cleanup
	if (i < 11) return NULL;						// Symbol is optional.  The other 11 fields are required.
	else if (!strlen(output.sName)) return NULL;	// Sanity check
	else return &output;

}

// returns handler to dynamic array of AssetListEntries or NULL if failure
int ParseAssetListString(const char* AssetListString)
{
	// this header must match.  Otherwise, the parse attempt is rejected.
	const char header[128] = "Name,Price,Spread,RollLong,RollShort,PIP,PIPCost,MarginCost,Leverage,LotAmount,Commission,Symbol";

	// temporary string
	char* str = (char*)malloc(strlen(AssetListString) + 1);
	strcpy(str, AssetListString);

	int hLines = 0, hList = 0;

	// first, make sure the header is correct.
	if (strstr(str, header) != str)
	{
		free(str);
		return NULL;
	}

	const int line_size = 256;
	hLines = da_new(line_size);					// new dynamic array: Tokenized lines
	hList = da_new(sizeof(AssetListEntry));		// new dynamic array: AssetListEntry structs
	if (!hLines || !hList)
	{
		da_delete(hLines);
		da_delete(hList);
		free(str);
		return NULL;
	}

	// step 1 - tokenize the lines and save them in a separate store of memory.
	const char skip[4] = "\r\n ";
	char *token;
	token = strtok(str, skip);

	int i; bool reset = false;// char duplicate[128];
	for (i = 0; token != NULL; i++)
	{
		std::cout << "Token: " << token << std::endl;
		if (i)
		{
			da_push_back(hLines, token);
			//lines.push_back(token);
		}
		if (!reset) token = strtok(NULL, skip);
		else token = strtok(token, skip);
		reset = false;
	}

	// sanity check: were any lines tokenized?
	if (!da_size(hLines))
	{
		da_delete(hLines);
		da_delete(hList);
		free(str);
		return NULL;
	}
	std::cout << da_size(hLines) << " lines to be parsed." << std::endl;

	// step 2 - parse the individual stores of memory for a separate tokenizer.

	AssetListEntry* pEntry = NULL;
	//for (const auto& line : lines)
	for(i = 0; i<da_size(hLines); i++)
	{
		//pEntry = ParseAssetLine(line.c_str());
		pEntry = ParseAssetLine((const char*)da_data(hLines, i));
		if (!pEntry) continue;
		//else list.push_back(*pEntry);
		else da_push_back(hList, pEntry);
	}

	// cleanup
	da_delete(hLines);
	free(str);

	// successful? check size.
	if (da_size(hList))
		return hList;
	else
	{
		da_delete(hList);
		return NULL;
	}
	
}

int test3()
{
	const char str[128] = "USD/JPY,113.225,0.017,0.0678,-0.1783,0.01,0.000088,0.04,25.004,1,0,USD/JPY";
	//const char str[128] = "USD/JPY,113.225,0.017,0.0678,-0.1783,0.01,0.000088,0.04,25.004,1,0"; // symbol field is blank

	AssetListEntry* output = ParseAssetLine(str);
	if (!output) return 0;

	std::cout
		<< output->sName << ", "
		<< output->vPrice << ", "
		<< output->vSpread << ", "
		<< output->vRollLong << ", "
		<< output->vRollShort << ", "
		<< output->vPip << ", "
		<< output->vPipCost << ", "
		<< output->vMarginCost << ", "
		<< output->vLeverage << ", "
		<< output->vLotAmount << ", "
		<< output->vCommission << ", "
		<< output->sSymbol
		<< std::endl;
	return(0);
}

int test4()
{
	char str[1028] = "Name,Price,Spread,RollLong,RollShort,PIP,PIPCost,MarginCost,Leverage,LotAmount,Commission,Symbol\r\nAUD/USD,0.76738,0.00019,0.0321,-0.3726,0.0001,0.0001,0.023,33.342,1,0,AUD/USD\r\nEUR/USD,1.05523,0.00014,-0.178,-0.0471,0.0001,0.0001,0.0211,50.007,1,0,EUR/USD\r\nGBP/USD,1.24394,0.00024,-0.1609,-0.0848,0.0001,0.0001,0.0622,20.004,1,0,GBP/USD";
	//std::vector<AssetListEntry> list = ParseAssetListString(str);
	int hList = ParseAssetListString(str);

	//std::cout << list.size() << " entries added to list." << std::endl;
	std::cout << da_size(hList) << " entries added to list." << std::endl;
	//for (auto& a : list)
	int i; AssetListEntry* pEntry = NULL;
	for(i=0; i<da_size(hList); i++)
	{
		pEntry = (AssetListEntry*)da_data(hList, i);
		std::cout
			<< "Entry: "
			<< pEntry->sName << ", "
			<< pEntry->vPrice << ", "
			<< pEntry->vSpread << ", "
			<< pEntry->vRollLong << ", "
			<< pEntry->vRollShort << ", "
			<< pEntry->vPip << ", "
			<< pEntry->vPipCost << ", "
			<< pEntry->vMarginCost << ", "
			<< pEntry->vLeverage << ", "
			<< pEntry->vLotAmount << ", "
			<< pEntry->vCommission << ", "
			<< pEntry->sSymbol
			<< std::endl;
	}

	return 0;
}

