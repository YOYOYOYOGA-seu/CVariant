#include "CVariant.h"
#include <iostream>
#include <vector>
int main(void)
{
	gva::CVariant test;  
	int outputInt; 
	double outDouble;
	std::string outputStr;
	test = 3.14;   
	if (test.getType() == static_cast<datatype_t>(gva::dataKind<double>::type))
	{
		test.getValue(outDouble);
		std::cout << "现在test是double:"<< outDouble<<std::endl;
	}
	test = 23213; 
	if (test.getType() == static_cast<datatype_t>(gva::dataKind<int>::type))
	{
		test.getValue(outputInt);
		std::cout << "现在test是Int:" << outputInt << std::endl;
	}

	test = "string test"; 
	if (test.getType() == static_cast<datatype_t>(gva::dataKind<std::string>::type))
	{
		test.getValue(outputStr);
		std::cout << "现在test是string:" << outputStr << std::endl;
	}

	test = 32;
	test += 453;
	test += 5645;
	test += 8798;
	test.getValue(outputInt);
	std::cout << "现在test的值ֵ:" << outputInt << std::endl;

	std::string tempStr("test str+=:");
	test = tempStr;
	test += "sdasda";
	test.getValue(outputStr);
	std::cout << "现在test的值ֵ:" << outputStr << std::endl;

  test.append("第二项");
  test += ("第三项");
  test.append ("第四项");
	std::cout << "现在test中的值";
	for (unsigned int i = 0; i < test.getSize(); i++)
	{
		if (test[i].getValue(outputStr) == true)
			std::cout << outputStr << ",";
	}
	std::cout << std::endl;


	if(test[3].getValue(outDouble) == false)
		std::cout <<"类型不符"<< std::endl;

	gva::CVariant test2 = std::vector<short>{ 1,45,76,99 };
	test = test2;
	short cVctShort[] = { 122,123,124 };
	test.insert((short)23423, 2);
	test.insert((short)23423, 2);
	test.insert(cVctShort,sizeof(cVctShort)/sizeof(short), 2);

	short ouputShort;
  std::cout << "现在test中的值";
	for (unsigned int i = 0; i < test.getSize(); i++)
	{
		if (test[i].getValue(ouputShort) == true)
			std::cout << ouputShort << ",";
	}
  std::cout << std::endl;
	while (1)
	{

	}
	return 0;
}