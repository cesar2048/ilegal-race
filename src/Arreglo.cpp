#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
Areglo, contiene un puntero a el arreglo de datos
y permite realojarse en memoria para aumentar o reducir su tamaño

Metodos a implementar>
 - Push back ( T element)
 - Push front ( T element )
 - get (i)
 - remove(i)
 - removeLast()
 - size()
 - empty()
 - Constructor
 - Destructor
 */

template <class T>
class Arreglo {
  private:
		T *ptr;
		int iSize;

  public:
		Arreglo();
		~Arreglo();
		
		void pushBack(T element);
		void pushFront(T element);
		T *at(int i);
		int empty();
		int size();
		void remove(int i);
		void removeLast();
};

template <class T>
Arreglo<T>::Arreglo(){
	this->iSize = 0;
	this->ptr = NULL;
}

template <class T>
Arreglo<T>::~Arreglo(){
	if( this->ptr != NULL ){
		free( this->ptr );
		this->iSize = 0;
		this->ptr = NULL;
	}
}

template <class T>
int Arreglo<T>::empty(){
	return this->iSize == 0;
}

template <class T>
void Arreglo<T>::pushBack(T element){
	if( this->empty() ){
		this->ptr = (T*) malloc( sizeof(T) );
		memcpy( this->ptr, &element, sizeof(T) );
	} else {
		this->ptr = (T*) realloc(this->ptr, sizeof(T) * (this->iSize + 1) );
		memcpy( this->ptr + this->iSize, &element, sizeof(T) );
	}
	iSize++;
}

// revisar
template <class T>
void Arreglo<T>::pushFront(T element){
	if( this->empty() ){
		this->ptr = (T*) malloc( sizeof(T) );
		memcpy( this->ptr, &element, sizeof(T) );
	} else {
		this->ptr = (T*) realloc( this->ptr, sizeof(T) * (this->iSize + 1) );
		memmove( this->ptr + 1,  this->ptr, sizeof(T) * (this->iSize - 1) );
		memcpy( this->ptr, &element, sizeof(T) );
	}
	iSize++;
}

template <class T>
T *Arreglo<T>::at(int i){
	if ( !this->empty() && i < this->iSize ) {
		return this->ptr + i;
	}
	return NULL;
}

template <class T>
int Arreglo<T>::size(){
	return iSize;
}



