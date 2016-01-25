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
			void Add(BufferObject2<T>* object);
			unsigned int Add(T object);
			T* GetBuffer() const;
			unsigned int GetBufferSize() const;
			void Reset();
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
void BufferObject2<T>::Add(BufferObject2<T>* object)
{

	while (size + object->GetBufferSize() >= totalSize)
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

	T* sourceBuffer = object->GetBuffer();
	memcpy(&(buffer[size]), sourceBuffer, object->GetBufferSize() * sizeof(T));
	size += object->GetBufferSize();

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
void BufferObject2<T>::Reset()
{

	size = 0;

}

template <class T>
BufferObject2<T>::~BufferObject2()
{

	delete[] buffer;

}