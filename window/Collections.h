#ifndef COLLECTIONS_H
#define COLLECTIONS_H

template <class T>
class ArrayList
{
public:
	
	ArrayList<T>()
	{
		data = new T[100];
		last = 100;
		m_size = 0;
	}

	ArrayList<T>(int _size)
	{
		data = new T[_size];
		last = _size;
		m_size = 0;
	}
	~ArrayList<T>()
	{
		delete [] data;
	}

	void AddElem( T elem )
	{
		if( m_size < last )
		{
			memcpy((void*)(data+m_size), (void*)&elem, sizeof(T));
			m_size++;
		}
	}

	T &GetElem( int i ){ return data[i]; }

	T& operator[](int i)
	{
		return data[i];
	}

	int size()
	{
		return m_size;
	}

private:
	T* data;
	int m_size;
	int last;


};


template <class T>
class Array2D
{
public:
	Array2D<T>()
	{
		data = new T*[100];
		for( int i = 0; i < 100; i++ )
		{
			*data = new T[100];
		}
		sizeX = 100;
		sizeY = 100;

	}
	Array2D<T>(int _size)
	{
		data = new T*[_size];
		for( int i = 0; i < _size; i++ )
		{
			*data = new T[_size];
		}		
		sizeX = _size;
		sizeY = _size;

	}
	Array2D<T>(int _sizeX, int _sizeY)
	{
		data = new T*[_sizeX];
		for( int i = 0; i < _sizeX; i++ )
		{
			*data = new T[_sizeY];
		}		
		sizeX = _sizeX;
		sizeY = _sizeY;

	}

	~Array2D<T>()
	{
		for( int i = 0; i < sizeX; i++ )
		{
			delete [] data[i];
		}
		delete [] *data;
	}


	void AddElem( T &elem, int i, int j )
	{
		try
		{
			if( i < sizeX && j < sizeY )
			{
				//data[i][j] = elem;
				memcpy( (void*)(data + i*sizeX +  j), (void*)&elem, sizeof(T));
			}
		}
		catch(...)
		{
			ERROR( "cannot add memory" );
		}
	}

	T& operator() (int i, int j){ return data[i][j];}

	// unsafe array operator - the usage would be
	// for data[i] to be a pointer indexed by j
	// so the user can call list[i][j] and return a T data type
	T* operator[](int i){return data[i];}

	T** data;
	int sizeX;
	int sizeY;
};



#endif