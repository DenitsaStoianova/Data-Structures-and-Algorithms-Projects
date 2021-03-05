#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

template <class T>
class DynamicArray
{
public:
	DynamicArray();
	DynamicArray(const DynamicArray& other);
	DynamicArray<T>& operator=(const DynamicArray& other);
	~DynamicArray();

public:
	void add(const T& element);
	void remove(unsigned index);

public:
	const T& operator[](int index) const;
	T& operator[](int index);

	size_t getSize() const;
	T getLast() const;

	bool isEmpty() const;
	void print() const;

private:
	size_t size;
	size_t capacity;
	T* elements;

private:
	void copy(const DynamicArray& other);
	void del();
	void resize();
};

template <class T>
DynamicArray<T>::DynamicArray()
{
	this->size = 0;
	this->capacity = 8;
	this->elements = new T[this->capacity];
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
{
	this->copy(other);
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other)
{
	if (this != &other)
	{
		this->del();
		this->copy(other);
	}
	return *this;
}

template <class T>
DynamicArray<T>::~DynamicArray()
{
	this->del();
}

template <class T>
void DynamicArray<T>::add(const T& element)
{
	if (this->size == this->capacity)
	{
		this->resize();
	}

	this->elements[this->size] = element;
	this->size++;
}

template <class T>
void DynamicArray<T>::remove(unsigned index)
{
	for (size_t i = index; i < this->size - 1; i++)
	{
		this->elements[i] = this->elements[i + 1];
	}
	this->size--;
}


template <class T>
const T& DynamicArray<T>::operator[](int index) const
{
	return this->elements[index];
}

template <class T>
T& DynamicArray<T>::operator[](int index)
{
	return this->elements[index];
}

template <class T>
unsigned DynamicArray<T>::getSize() const
{
	return this->size;
}

template <class T>
void DynamicArray<T>::copy(const DynamicArray& other)
{
	this->size = other.size;
	this->capacity = other.capacity;
	this->elements = new T[this->capacity];
	for (size_t i = 0; i < this->size; i++)
	{
		this->elements[i] = other.elements[i];
	}
}

template <class T>
void DynamicArray<T>::del()
{
	delete[] this->elements;
}

template <class T>
void DynamicArray<T>::resize()
{
	this->capacity *= 2;
	T* tempArray = new T[this->capacity];
	for (size_t i = 0; i < this->size; i++)
	{
		tempArray[i] = this->elements[i];
	}
	delete[] this->elements;
	this->elements = tempArray;
}

template <class T>
bool DynamicArray<T>::isEmpty() const
{
	return this->size == 0;
}

template <class T>
T DynamicArray<T>::getLast() const
{
	return this->elements[this->size - 1];
}

template <class T>
void DynamicArray<T>::print() const
{
	for (size_t i = 0; i < this->size; i++)
	{
		std::cout << this->elements[i] << "\n";
	}
}

#endif // !DYNAMIC_ARRAY_H
