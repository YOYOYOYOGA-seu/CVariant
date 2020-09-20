# CVariant

## 1. Introduction
</br>

CVariant is a objective class which acts like a union for most base data type in C++, just like std::any, boost::variant or QVariant in Qt. The most between CVariant and those preoccupant liberary is CVariant support a inside vector type for all base data type,and fade the operate difference between variant and variant vector, and can operate it like std::vector. For exmaple, you can easily define a vector type variant or transform a single variant to a variant vector:
```cpp
CVariant var = std::vector<int>{1,2,3,4}; //now var is int[] type
int temp;
var[3].get(temp);  //then temp = 4

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
  For a base type(not a vector type), the += is same as the base type's own operator +=; and if the CVariant object's type is a vector type, the += call the function `CVariant::append`.  
* **operator-=:**  
  Call the data type's own operator -=, only vaild when the type is base type(not a vector type).
* **operator==:**  
  only for base type CVariant compare, return true when `type = type(or can do a cast)`, `data = data`.
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
Set a CVariant object's value, different form operator = ,it can't change the CVariant object's type, so if the input's type is different form object's current type and can't do a static_cast, the operate will not success. In other words, this function will do type checking and not a copy assignment function
```cpp
bool get(T& retVal);     
```
Get value of a CVariant object. for a vector type, you can call as: `var[i].get`.return true if success.
```cpp
T value(void)
```
Directly return the value,but if failed, will abort the whole proccess(current type of CVariant object can't cast to the given type T)
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
append elements to the end of a vector type CVariant object, if the CVariant is a base type(but must same as the input or can do a cast), the function will upgrade the CVariant to vector type.
```cpp
bool insert(...);             
```
insert elements to a vector type CVariant(must has the same base type)
```cpp
bool clear(); 
```
Clear, will release memory.

## 4. matters need attention

</br>

* Although the CVariant will auto do some cast dynamicly, but if you pay attention to the program's performance, you'd better do a statement when using template function. That mean's you'd better do a pre-cast and insure the input type of function(or operator) are strictly same as the CVariant object's current type, to avoid a dynamic cast by the CVariant's internal function. for example:
```cpp
CVariant<double> var(static_cast<double>(5));
var += static_cast<double>(3); //or var += 3.0
var.append<double>(123);
var.insert<double>(0,'s');
var = static_cast<char>32;
...

```
* What happens a internal dynamicly auto cast:
A cast will only happen between numeric type(like char, double ,unsigned int).For example, there are 3 CVariant objects:
```cpp
CVariant var1 = 3; // int
CVariant var2 = static_cast<char>(3); // char
CVariant var3 = 3.14; //double
```
(1)when do a arithmetical operate:
```cpp
var3 += 2;    // do a cast (2 to 2.00000 and add to var3)
var3 += var1   // do a cast (var.type from int to double and add to var3)
var1 -= var2;  // do a cast (var.type from char to int and minus from var3)
...
```
(2)when call setValue:
```cpp
var3.setValue(2);    // do a cast (2 to 2.00000 and add to var3)
var3.setValue('a');   // do a cast ( from char to double and add to var3)
...
```
You must pay attention that setValue a different form operator=, while operator= more like a copy function and will change the caller object's type, like `var3 = 2;` that will change `var3` form double to int.

(3)when call value:
```cpp
int a = var3.value<int>();    // do a cast (double to int and return it)
double b = var2.setValue<double>();   // do a cast ( from char to double and return it)
...
```
You must pay attention that when the cast is invaild(fail) among program running time, it will cause the program abort. So recommend to use `CVariant::get` like `if(var3.get(a) == false) return;`.
(4)when append or insert a  numerical value:
```cpp
var3.append(2) //do a cast (2 to 2.00000 and and append it to var3)
var3.insert(0,'q'); // do a cast ( from char to double and insert it to var3)
...
```
You must pay attention that when append or insert a CVariant object, the auto cast will not happen(don't like the arithmetical operate), Because `arithmetical cast are understandable, but elements'type must be strict conformance in a vector`.
```cpp
var3.append(var2);   //return fail (not do cast from char to double)
var3.insert(var1);   //return fail (not do cast from int to double)
...
```

