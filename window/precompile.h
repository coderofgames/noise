//-----------------------------------------------------------------------------------------------
//
// PRECOMPILE.h : 
//-----------------------------------------------------------------------------------------------

#ifndef PRECOMPILE_H
#define PRECOMPILE_H

#define MAX_LOADSTRING 100

// 
#define DEBUG_LEVEL 1
#undef DEBUG_LEVEL

#ifdef DEBUG_LEVEL
	#define ERROR  OutputMessage::DebugPrint
#else
	#define ERROR  OutputMessage::MessageBoxPrint
#endif


//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
#define LOG				OutputMessage::Log
#define FORMAT_TEXT		OutputMessage::Format
#define MESSAGE			OutputMessage::Message

// TODO : Get the standard error done 
//#define STD_ERROR(a		FORMAT_TEXT(
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------


#define TO_STRING(a)		(#a)
//#define TOKEN_PASTER(a)		(##a)   // broken

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
// normal safe release/delete macros
#define SAFE_RELEASE(p)				if(p){/*unnessesary whitespace*/\
										p->Release();\
										p=NULL;}

#define SAFE_DELETE_ARRAY(p)		if(p){\
										delete [] p;\
										p=NULL;}

#define SAFE_DELETE(p)				if(p){\
										delete  p;\
										p=NULL;}

// additional/enhanced versions 
#define SAFE_RELEASE_EXCEPTION(p)	if(p){\
										HRESULT hr = p->Release();\
										if( SUCCEEDED(hr)){\
											ERROR("RELEASED: %s",TO_STRING(p));\
										}\
										p=NULL;}

#define SAFE_RELEASE_LOG(p)			if(p){\
										HRESULT hr = p->Release();\
										if( SUCCEEDED(hr)){\
											ERROR("RELEASED: %s",TO_STRING(p));\
										}\
										p=NULL;}

#define SAFE_RELEASE_LOG(p,c)		if(p){\
										HRESULT hr = p->Release();\
										if( SUCCEEDED(hr)){\
											ERROR("RELEASED: %s, RefID: %d",TO_STRING(p),c);\
										}\
										p=NULL;}

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------


// a quick and dirty reference number, not tracked globally 
#define ADD_REF_NUMBER( a ) { static Int32 refs = 0; a = refs++; } 

// prevent copying ... 
#define NO_COPY(a)	private: \
					a(const a& src); \
					a& operator=(const a& src);\

//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
// dissallow copying of this object, *and* create a static instance counter
#define INSTANCE_ONLY(a)	NO_COPY(a)\
							Int32 i32##a##IdCount;\
							static InstanceCounter ms_##a##InstCounter;\
							void Incr(void){ i32##a##IdCount = ms_##a##InstCounter.Incr();}\
							void Decr(void){ ms_##a##InstCounter.Decr();}\
							public:Int32 GetInstanceCount(){return ms_##a##InstCounter.GetValue();}\
							Int32 GetinstId(){return i32##a##IdCount; }\

// if the above macro is called, then this one has to be called too in the .cpp file associated with the class
#define DECLARE_INSTANCE_TRACKER(a) InstanceCounter a::ms_##a##InstCounter
//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------






#endif