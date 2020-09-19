# CVariant

## 1. Introduction
</br>

CVariant is a objective class which acts like a union for most base data type in C++, just like std::any, boost::variant or QVariant in Qt. The most between CVariant and those preoccupant liberary is CVariant support a inside vector type for all base data type,and fade the operate difference between variant and variant vector, and can operate it like std::vector. For exmaple, you can easily define a vector type variant or transform a single variant to a variant vector:
```cpp
CVariant var = std::vector<int>{1,2,3,4}; //now var is int[] type
int temp;
var[3].getValue(temp);  //then temp = 4

var =3.14; //now var is double type
var += 2;
var.append(5.12); //now var upgarde to double[] type:{5.14, 5.12}
var.insert(1,4.44); //{5.14, 4.44, 5.12}
...
```
In addition, all type is defined dynamicly, it means you can change it's type any time.  
For more information, please read section 3 for more detail.

</br>
</br>

## 2. Update log
</br>

* Sep. 19, 2020 : Upload repositories.
  
</br>
</br>

## 3. How to use
</br>

### (1) support type:
> * bool  
> * char, unsigned char  
> * short, unsigned short  
> * int, unsigned int  
> * long long, unsigned long long  
> * string  
> * `vector of all type above`

</br>

### (2) construct:
```cpp
CVariant();
CVariant(datatype_t type);
CVariant(const T var);
CVariant(std::vector<T>& var);
CVariant(const T* var, size_t n);
CVariant(const CVariant& var);
```
The constructor do a deepin copy.

</br>

### (3) operator:
* **operator=:**
 ```cpp
 operator=(const CVariant& var);
 operator=(const char* var);
 operator=(const T var);
 operator=(const std::vector<T>& var);
 ```
 All of though assignment do deepin copy.
* **operator+=:**  
  For a base type(not a vector type), the += is same as the base type's own; and if the CVariant object's type is a vector type, the += call the function `CVariant::append`.  
* **operator-=:**  
  Call the data type's own operator -=, only vaild when the type is base type(not a vector type).
* **operator==:**  
  only for base type CVariant compare, return true when `type = type`, `data = data`.
* **operator[]:**  
  for base type , return itself, for vector type, return the number n CVariant object. 

</br>

### (4) member function (API):
```cpp
static bool ifBaseType(int type);    
```
Check if the input type index a base type.

```cpp
static bool ifVectorType(int type);  
```
Check if the input type index a vector type .
```cpp
static bool ifType(int type);        
```
Check if the type index a vaild type
```cpp
bool setType(datatype_t tp); 
```
Set a CVariant to a pointed type, and will call clear() if the CVariant object is not empty.
```cpp
datatype_t getType(void);     
```
Get a CVariant object's type
```cpp
unsigned int getSize(void);   
```
Get a vector type CVariant object's size ( base type always 1)
```cpp
bool setValue(...);           
```
Set a CVariant object's value, different form operator = ,it can't change the CVariant object's type, so if the input's type is different form object's current type, the operate will not success. In other words, this function will do type checking and not a copy assignment function
```cpp
bool getValue(T& retVal);     
```
Get value of a CVariant object. for a vector type, you can call as: `var[i].getValue` 
```cpp
const T* getPtr(void);       
```
Get the ptr to the origin data memory area.
```cpp
bool erease(...);             
```
Delete a pointed element or a range of elements in vector type CVariant object
```cpp
bool append(...);             
```
append elements to the end of a vector type CVariant object, if the CVariant is a base type(but must same as the input), the function will upgrade the CVariant to vector type.
```cpp
bool insert(...);             
```
insert elements to a vector type CVariant(must has the same base type)
```cpp
bool clear(); 
```
Clear, will release memory.