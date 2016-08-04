#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <d3dx9.h>

/*
	This is a VertexBuffer Object:
	Manages and automates the locking and unlocking procedures
*/
class VertexBuffer
{
public:

	VertexBuffer(){}
	VertexBuffer( int vertexSize, int numVertices, int usage, LPDIRECT3DDEVICE9 pDev, bool useSystemMemory);
    ~VertexBuffer();

	void* lock(int offset, int length, DWORD options);
	void  unlock(void);
    void  readData(int offset, int length, void* pDest);
    void  writeData(int offset, int length, const void* pSource );
	
	int mSizeInBytes;
	int mUsage;
	bool mIsLocked;
	int mLockStart;
	int mLockSize;
	bool mSystemMemory;
	bool mSuppressHardwareUpdate;
	int mVertexSize;
	int m_numVertices;

	LPDIRECT3DVERTEXBUFFER9 m_pVB;
};




#endif // VERTEX_BUFFER_H