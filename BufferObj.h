#pragma once

namespace CrossHandlers
{

	template <class T>

	class BufferObject2
	{

		private:
			T* buffer;
			unsigned int totalSize;
			unsigned int size;
			unsigned int increment;

		public:
			BufferObject2(unsigned int startSize);
			unsigned int Add(T object);
			T* GetBuffer() const;
			unsigned int GetBufferSize() const;
			~BufferObject2();

	};
}

using namespace CrossHandlers;

template <class T>
BufferObject2<T>::BufferObject2(unsigned int startSize)
{

	totalSize = startSize;
	increment = startSize;
	size = 0;
	buffer = new T[totalSize];

}

template <class T>
unsigned int BufferObject2<T>::Add(T object)
{

	if (size == totalSize)
	{

		totalSize += increment;
		T* newBuffer = new T[totalSize];

		for (unsigned int i = 0; i < size; i++)
		{

			newBuffer[i] = buffer[i];

		}

		delete[] buffer;
		buffer = newBuffer;

	}

	buffer[size] = object;
	size++;
	return size - 1;

}

template <class T>
T* BufferObject2<T>::GetBuffer() const
{

	return buffer;

}

template <class T>
unsigned int BufferObject2<T>::GetBufferSize() const
{

	return size;

}

template <class T>
BufferObject2<T>::~BufferObject2()
{

	delete[] buffer;
}