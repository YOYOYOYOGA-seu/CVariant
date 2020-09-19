#ifndef __VARIANT_H
#define __VARIANT_H
#include <string>
#include <vector>

typedef enum {
  /* 基础数据类型（兼容 OPEN62541） */
  DATATYPEKIND_BOOLEAN = 0,
  DATATYPEKIND_SBYTE = 1,
  DATATYPEKIND_BYTE = 2,
  DATATYPEKIND_INT16 = 3,
  DATATYPEKIND_UINT16 = 4,
  DATATYPEKIND_INT32 = 5,
  DATATYPEKIND_UINT32 = 6,
  DATATYPEKIND_INT64 = 7,
  DATATYPEKIND_UINT64 = 8,
  DATATYPEKIND_FLOAT = 9,
  DATATYPEKIND_DOUBLE = 10,
  DATATYPEKIND_STRING = 11,
  DATATYPE_BASE_END,
  /* 向量数据类型 */
  DATATYPEKIND_BOOLEAN_VECTOR = 100,
  DATATYPEKIND_SBYTE_VECTOR = 101,
  DATATYPEKIND_BYTE_VECTOR = 102,
  DATATYPEKIND_INT16_VECTOR = 103,
  DATATYPEKIND_UINT16_VECTOR = 104,
  DATATYPEKIND_INT32_VECTOR = 105,
  DATATYPEKIND_UINT32_VECTOR = 106,
  DATATYPEKIND_INT64_VECTOR = 107,
  DATATYPEKIND_UINT64_VECTOR = 108,
  DATATYPEKIND_FLOAT_VECTOR = 109,
  DATATYPEKIND_DOUBLE_VECTOR = 110,
  DATATYPEKIND_STRING_VECTOR = 111,
  DATATYPE_VECTOR_END,
  DATATYPEKIND_NOTYPE = 255,
#define MAX_DATA_TYPE_INDEX  11
}datatype_t;

namespace gva
{
  /* Used to limit the input type of a template function while allowing 
  the template function to automatically get a type number based on the 
  parameter(typedatatype_t) */
  template <typename T>struct dataKind;  
  template <>struct dataKind<bool> { enum { type = DATATYPEKIND_BOOLEAN }; };
  template <>struct dataKind<char> { enum { type = DATATYPEKIND_SBYTE }; };
  template <>struct dataKind<unsigned char> { enum { type = DATATYPEKIND_BYTE }; };
  template <>struct dataKind<short> { enum { type = DATATYPEKIND_INT16 }; };
  template <>struct dataKind<unsigned short> { enum { type = DATATYPEKIND_UINT16 }; };
  template <>struct dataKind<int> { enum { type = DATATYPEKIND_INT32 }; };
  template <>struct dataKind<unsigned int> { enum { type = DATATYPEKIND_UINT32 }; };
  template <>struct dataKind<long long> { enum { type = DATATYPEKIND_INT64 }; };
  template <>struct dataKind<unsigned long long> { enum { type = DATATYPEKIND_UINT64 }; };
  template <>struct dataKind<float> { enum { type = DATATYPEKIND_FLOAT }; };
  template <>struct dataKind<double> { enum { type = DATATYPEKIND_DOUBLE }; };
  template <>struct dataKind<std::string> { enum { type = DATATYPEKIND_STRING }; };



  class cVariant {
  private:
    void* data;
    datatype_t type;
    /* for base type, the size is 1; for vector type, the size is the vector size.
      It will only refreash after call cVariant::getSize() */
    unsigned int size;  
    /* insert a object to a vector type cVariant object, internal function, and must 
    check if insertable before call this function*/
    inline void _insert(cVariant& var, unsigned int locate) {
        unsigned int n = var.getSize();
        cVariant temp;
        for (unsigned int i = 0; i < n; i++)
        {
          temp = var[i];
          
          static_cast<std::vector<cVariant>*>(data)->insert(static_cast<std::vector<cVariant>*>(data)->begin()+locate, temp);
          locate++;
        }
        getSize(); //refreash size
    }
    template <typename T>inline void _insert(const T var, unsigned int locate) {
      cVariant temp;
      temp = (var);
      static_cast<std::vector<cVariant>*>(data)->insert(static_cast<std::vector<cVariant>*>(data)->begin()+locate, temp);
      getSize(); //refreash size
    }
    template <typename T> inline void _insert(const std::vector<T>& var, unsigned int locate) {
      for (int i = 0; i < var.size(); i++)
      {
        cVariant temp;
        temp = var[i];
        static_cast<std::vector<cVariant>*>(data)->insert(static_cast<std::vector<cVariant>*>(data)->begin()+locate, temp);
        locate++;
      }
      getSize(); //refreash size
    }
    template <typename T> inline void _insert(const T* var, unsigned int n, unsigned int locate) {
      for (unsigned int i = 0; i < n; i++)
      {
        cVariant temp;
        temp = var[i];
        static_cast<std::vector<cVariant>*>(data)->insert(static_cast<std::vector<cVariant>*>(data)->begin()+locate, temp);
        locate++;
      }
      getSize(); //refreash size
    }

    /* upgrade a base type cVariant obj to a corresponding vetor type obj */
    void _upgrade(void);

  public:

    cVariant();
    cVariant(datatype_t tp);
    template<typename T> cVariant(const T var)
    {
      data = nullptr;
      size = 0;
      type = DATATYPEKIND_NOTYPE;
      if (static_cast<datatype_t>(dataKind<T>::type) < DATATYPE_BASE_END)
      {
        operator=(var);
      }
    }
    template<typename T> cVariant(const std::vector<T>& var)
    {
      data = nullptr;
      size = 0;
      type = DATATYPEKIND_NOTYPE;
      if (static_cast<datatype_t>(dataKind<T>::type) < DATATYPE_BASE_END)
      {
        operator=(var);
      }
    }
    template<typename T> cVariant(const T* var, unsigned int n)
    {
      if (static_cast<datatype_t>(dataKind<T>::type) < DATATYPE_BASE_END)
      {
        clear();
        type = static_cast<datatype_t>(dataKind<T>::type+ DATATYPEKIND_BOOLEAN_VECTOR);
        data = new std::vector<cVariant>;
        size = n;
        cVariant temp;
        for (int i = 0; i < n; i++)
        {
            temp = var[i];
            static_cast<std::vector<cVariant>*>(data)->push_back(temp);
        }
      }
    }
    cVariant(const cVariant& var);

    ~cVariant();
    void clear();

    cVariant& operator=(const cVariant& var);
    cVariant& operator=(const char* var);
    template<typename T> cVariant& operator=(const T var)
    {
      if (static_cast<datatype_t>(dataKind<T>::type) < DATATYPE_BASE_END)
      {
        if (static_cast<datatype_t>(dataKind<T>::type) == type && data != nullptr)
        {
          *static_cast<T*>(data) = var;
        }
        else
        {
          clear();
          type = static_cast<datatype_t>(dataKind<T>::type);
          data = new T(var);
        }
        size = 1;
      }
      return *this;
    }
  template<typename T> cVariant& operator=(const std::vector<T>& var)
    {
        if (static_cast<datatype_t>(dataKind<T>::type) < DATATYPE_BASE_END)
        {
          clear();
          type = static_cast<datatype_t>(dataKind<T>::type + DATATYPEKIND_BOOLEAN_VECTOR);
          data = new std::vector<cVariant>;
          size = var.size();
          cVariant temp;
          for (unsigned int i = 0; i < var.size(); i++)
          {
            temp = var[i];
            static_cast<std::vector<cVariant>*>(data)->push_back(temp);
          }
        }
        return *this;
    }
    
    /* for base type , it amount to the data type's own +=(like int += int; string+=string;)
      for a vector type, it amount to call cVariant::append*/
    cVariant& operator+=(const char* var);
    template<typename T> cVariant& operator+=(T var)
    {
      if (static_cast<datatype_t>(dataKind<T>::type) == type && type < DATATYPE_BASE_END)
        * static_cast<T*>(data) += var;
      else if (static_cast<datatype_t>(dataKind<T>::type + DATATYPEKIND_BOOLEAN_VECTOR) == type)
        append(var);
      return *this;
    }
    template<typename T> cVariant& operator+=(const std::vector<T>& var)
    {
      append(var);
      return *this;
    }
    cVariant& operator+=(const cVariant& var);

    /* it amount to the data type's own -=(like int += int;),and only vaild when the data type can -=*/
    template<typename T> cVariant& operator-=(T var)
    {
      if (static_cast<datatype_t>(dataKind<T>::type) == type && type < DATATYPE_BASE_END)
        * static_cast<T*>(data) -= var;
      return *this;
    }
    cVariant& operator-=(const cVariant& var);

    /* only for base type, return true when type = type, data = data */
    bool operator==(const cVariant& var);
    
    /* for base type , return itself, for vector type, return the number n cVariant object */
    cVariant& operator[](std::size_t n);

    bool insert(const char* var, unsigned int locate);
    template <typename T> bool insert(const T var, unsigned int locate)
    {
      if (static_cast<datatype_t>(dataKind<T>::type + DATATYPEKIND_BOOLEAN_VECTOR) == type)
      {
        _insert(var, locate);
        return true;
      }
      return false;
    }
    template<typename T> bool insert(const std::vector<T>& var, unsigned int locate)
    {
      if (static_cast<datatype_t>(dataKind<T>::type + DATATYPEKIND_BOOLEAN_VECTOR) == type)
      {
        _insert(var, locate);
        return true;
      }
        return false;
    }
    template<typename T> bool insert(const T* var, unsigned int n, unsigned int locate)
    {
      if (static_cast<datatype_t>(dataKind<T>::type + DATATYPEKIND_BOOLEAN_VECTOR) == type)
      {
        _insert(var, n, locate);
        return true;
      }
        return false;
    }
    bool insert(const cVariant& var, unsigned int locate);

    bool append(const char* var);
    template<typename T>bool append(const T var)
    {
      if (static_cast<datatype_t>(dataKind<T>::type) == type)
      {
        _upgrade();
      }
      if (static_cast<datatype_t>(dataKind<T>::type + DATATYPEKIND_BOOLEAN_VECTOR) == type)
      {
        _insert(var, getSize());
        return true;
      }
      
      return false;
    }
    template<typename T>bool append(const std::vector<T>& var)
    {
      if (static_cast<datatype_t>(dataKind<T>::type) == type)
      {
        _upgrade();
      }
      if (static_cast<datatype_t>(dataKind<T>::type + DATATYPEKIND_BOOLEAN_VECTOR) == type)
      {
        _insert(var, getSize());
        return true;
      }
      return false;
    }
    bool append(const cVariant& var);
    
    bool erease(unsigned int locate);
    bool erease(unsigned int first, unsigned int last);
    
    const void* getPtr(void);
    template<char> const char* getPtr(void); 
    template<typename T> const T* getPtr(void)
    {
      if (static_cast<datatype_t>(dataKind<T>::type) == type)
        return static_cast<const T*>(data);
      return nullptr;
    }


    template<typename T>bool getValue(T& retVal) {
      if (static_cast<datatype_t>(dataKind<T>::type) == type)
      {
        retVal = *static_cast<T*>(data);
        return true;
      }
      return false;
    }

    bool setValue(void* dat, size_t size);
    bool setValue(const char* value);
    template<typename T>bool setValue(T value) {
      if (static_cast<datatype_t>(dataKind<T>::type) == type)
      {
        *static_cast<T*>(data) = value;
        return true;
      }
      return false;
    }

    bool setType(datatype_t tp);
    datatype_t getType(void);
    unsigned int getSize(void);

    static inline bool ifBaseType(int type) {
      return (type >= DATATYPEKIND_BOOLEAN && type < DATATYPE_BASE_END);
    }
    static inline bool ifVectorType(int type) {
      return(type >= DATATYPEKIND_BOOLEAN_VECTOR && type < DATATYPE_VECTOR_END);
    }
    static inline bool ifType(int type) {
      return (ifVectorType(type) || ifBaseType(type));
    }
  };
}
#endif