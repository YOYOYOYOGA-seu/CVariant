#include "CVariant.h"
#include <iostream>
#include <vector>
int main(void)
{
  /* 基础类型 */
	gva::CVariant test;  
	int outputInt; 
	double outDouble;
	std::string outputStr;
	test = 3.14;   
	if (test.getType() == static_cast<datatype_t>(gva::dataKind<double>::type))
	{
		test.get(outDouble);
		std::cout << "现在test是double:"<< outDouble<<std::endl;
	}
	test = 23213; 
  
	if (test.getType() == static_cast<datatype_t>(gva::dataKind<int>::type))
	{
		test.get(outputInt);
		std::cout << "现在test是Int:" << outputInt << std::endl;
	}

  

	test = "string test"; 
	if (test.getType() == static_cast<datatype_t>(gva::dataKind<std::string>::type))
	{
		test.get(outputStr);
		std::cout << "现在test是string:" << outputStr << std::endl;
	}

  /* 调用+= */
	test = 32;
	test += 453;
	test += 5645;
	test += 8798;
	test.get(outputInt);
	std::cout << "现在test的值ֵ:" << outputInt << std::endl;

   /* string基础类型 */
	std::string tempStr("test str+=:");
	test = tempStr;
	test += "sdasda"; 
	test.get(outputStr);
	std::cout << "现在test的值ֵ:" << outputStr << std::endl;

  /* 调用append升级为向量 */
  test.append("第二项");
  test += ("第三项");
  test.append ("第四项");
	std::cout << "现在test中的值";
	for (unsigned int i = 0; i < test.getSize(); i++)
	{
		if (test[i].get(outputStr) == true)
			std::cout << outputStr << ",";
	}
	std::cout << std::endl;

   /* 调用get取数据 */
	if(test[3].get(outDouble) == false)
		std::cout <<"类型不符"<< std::endl;

  /* 向量类型构造 */
	gva::CVariant test2 = std::vector<short>{ 1,45,76,99 };
	test = test2;

  /* 调用insert函数 */
	short cVctShort[] = { 122,123,124 };
	test.insert((short)23423, 2);
	test.insert((short)23423, 2);
	test.insert(cVctShort,sizeof(cVctShort)/sizeof(short), 2);
	short ouputShort;
  std::cout << "现在test中的值";
	for (unsigned int i = 0; i < test.getSize(); i++)
	{
		if (test[i].get(ouputShort) == true)
			std::cout << ouputShort << ",";
	}
  std::cout << std::endl;

  /* 赋值、+= -= 、setValue()、类型转换联合测试 */
  test2 = -3;
  test = -3.14;
  test += test2;
  std::cout <<test.value<int>()<< std::endl;
  test += -2;
  test -= 3;
  test -= test2;
  std::cout <<test.value<double>()<< std::endl;
  std::cout <<test.value<unsigned short>()<< std::endl;
  test.setValue(43);
  std::cout <<test.value<double>()<< std::endl;

  double castTest = test;
  std::cout << castTest << std::endl;

  test = static_cast<char>(3);
  test2 = 3.00000000;
  if(test == test2)
    std::cout <<"test == test2"<< std::endl;

  std::vector<std::string> strList1 = {"a", "b", "csqada"};
  test = strList1;
  test2 = strList1;
  if(test == test2)
    std::cout <<"test == test2 (vector compare)"<< std::endl;

  test2.append(2);
  std::cout << "现在test2中的值";
	for (unsigned int i = 0; i < test2.getSize(); i++)
	{
		if (test2[i].get(outDouble) == true)
			std::cout << outDouble << ",";
	}
  std::cout << std::endl;

  /* 拷贝构造、赋值构造测试 */
  gva::CVariant test3;
  test3 = test2;
  test2.append(5);
  test2.clear();
  std::cout << "现在test3中的值";
	for (unsigned int i = 0; i < test3.getSize(); i++)
	{
		if (test3[i].get(outDouble) == true)
			std::cout << outDouble << ",";
	}
  std::cout << std::endl;
  test3 = test;
  test = static_cast<char>('a');

  test3.clear();
  test3 = gva::CVariant(test);
  test = static_cast<char>('c');
  test.clear();
  std::cout << "现在test3中的值";
    std::cout << test3.value<char>() << std::endl;
  std::cout << std::endl;
  test3.clear();

  /* toString() 测试*/
  test = static_cast<int>(123123);
  std::cout << "toString :" << test.toString() << std::endl;
  std::cout << "toString hex: 0x" << test.toString("hex") << std::endl;
  test = true;
  std::cout << "toString :" << test.toString() << std::endl;
  test = 'c';
  std::cout << "toString :" << test.toString() << std::endl;
  test = 436346.324233123;
  std::cout << "toString :" << test.toString() << std::endl;
  std::cout << "toString prec = 3:" << test.toString(3) << std::endl;
  std::vector<double> doubleList = {436346.324233123, 3432.5464, -12312.43, 34534.123};
  test = doubleList;
  std::cout << "toString :" << test.toString(3) << std::endl;
  test = strList1;
  std::cout << "toString :" << test.toString() << std::endl;
  test = std::vector<bool>{true, false ,false, true};
  std::cout << "toString :" << test.toString() << std::endl;
	while (1)
	{

	}
	return 0;
}