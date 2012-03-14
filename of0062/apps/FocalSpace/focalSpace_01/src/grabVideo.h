#pragma once
#include <iostream>
#include <vector>

#include <tchar.h>
#include <strsafe.h>
#include <conio.h>

// For functions and definitions used to create output file
#include <dmo.h>       // Mo*MediaType
#include <uuids.h>     // FORMAT_WaveFormatEx and such
#include <mfapi.h>     // FCC
//#include <windows.h>

#include "resource.h"
#include "NuiApi.h"

// For configuring DMO properties
#include <wmcodecdsp.h>
// For discovering microphone array device
#include <MMDeviceApi.h>
#include "NuiSensor.h"

const int                     VIDEO_WIDTH = 640;
const int                     VIDEO_HEIGHT= 480;
const int                     DEPTH_WIDTH = 640;
const int                     DEPTH_HEIGHT= 480;
#define                       NUI_SKELETON_COUNT 6

// store audio data in CStaticMediaBuffer
class CStaticMediaBuffer : public IMediaBuffer {
public:
   CStaticMediaBuffer() {}
   CStaticMediaBuffer(BYTE *pData, ULONG ulSize, ULONG ulData) :
      m_pData(pData), m_ulSize(ulSize), m_ulData(ulData), m_cRef(1) {}
   STDMETHODIMP_(ULONG) AddRef() { return 2; }
   STDMETHODIMP_(ULONG) Release() { return 1; }
   STDMETHODIMP QueryInterface(REFIID riid, void **ppv) {
      if (riid == IID_IUnknown) {
         AddRef();
         *ppv = (IUnknown*)this;
         return NOERROR;
      }
      else if (riid == IID_IMediaBuffer) {
         AddRef();
         *ppv = (IMediaBuffer*)this;
         return NOERROR;
      }
      else
         return E_NOINTERFACE;
   }
   STDMETHODIMP SetLength(DWORD ulLength) {m_ulData = ulLength; return NOERROR;}
   STDMETHODIMP GetMaxLength(DWORD *pcbMaxLength) {*pcbMaxLength = m_ulSize; return NOERROR;}
   STDMETHODIMP GetBufferAndLength(BYTE **ppBuffer, DWORD *pcbLength) {
      if (ppBuffer) *ppBuffer = m_pData;
      if (pcbLength) *pcbLength = m_ulData;
      return NOERROR;
   }
   void Init(BYTE *pData, ULONG ulSize, ULONG ulData) {
        m_pData = pData;
        m_ulSize = ulSize;
        m_ulData = ulData;
    }
protected:
   BYTE *m_pData;
   ULONG m_ulSize;
   ULONG m_ulData;
   ULONG m_cRef;
};



class KinectGrabber{
public:
	HANDLE                    m_hThNuiProcess;
	HANDLE                    m_hEvNuiProcessStop;
	HANDLE                    m_hNextSkeletonEvent;
	INuiSensor*               m_pNuiSensor;
	BSTR                      m_instanceId;
	CRITICAL_SECTION          cs;
	bool                      tryLock();
	void                      unlock();
	void                      lock();
	bool                      getWriteTurn();
	void                      setWriteTurn(bool val);
	bool                      writeTurn;
	DWORD                     m_SkeletonIds[NUI_SKELETON_COUNT];
	DWORD                     m_TrackedSkeletonIds[NUI_SKELETON_MAX_TRACKED_COUNT];
	void                      Nui_SetApplicationTracking();
	void                      Nui_SetTrackedSkeletons(int skel1, int skel2);

	//---------------------------------------------------------------------/
	// Sensor Initialization and Update
	//---------------------------------------------------------------------/
    HRESULT					  Kinect_Init();
    void                      Kinect_UnInit( );
	void                      Kinect_Zero();
	static DWORD WINAPI 	  Kinect_Update(LPVOID pParam);
	DWORD WINAPI 			  Kinect_Update();
    void                      Kinect_GotVideoAlert( );
	void                      Kinect_GotDepthAlert( );
    void                      Kinect_GotSkeletonAlert( );	
	//---------------------------------------------------------------------/
	// Pixel Buffers
	//---------------------------------------------------------------------/
	RGBQUAD*		          Kinect_getDepthPixels();    //store RGBQUARD (3 bytes) pointers pointing to (R,G,B as a whole) of each pixel; in total it has 1* width * height
	BYTE*			          Kinect_getAlphaPixels(); 
	BYTE*			          Kinect_getRGBBuffer();      // store byte pointers pointing to R,G,B,A for each pixel; in total it has 4 * width * height
	USHORT*			          Kinect_getDepthBuffer();    // store byte pointers pointing to R,G,B for each pixel;   in total it has 3 * width * height
	USHORT*			          Kinect_getPlayerBuffer();
	void			          Kinect_makeRGBFromRGBA();   // store unsigned short (2 bytes) pointers pointing to each pixels's depth in milimeter; in total it has 1 * width * height
	//---------------------------------------------------------------------/
	// Audio
	//---------------------------------------------------------------------/
	HRESULT					    DShowRecord();      // samples audio and reads out the location
	double                      soundPixel;         // Pixel approximation of the source of audio. (x dimention)
	int                         minDiscrepancyIdx;  // Index of the tracked skeleton that most closely machtes the audio.

	void						recordAudioStart();
	void						recordAudioInit();
	void						recordAudioEnd();
	LPCTSTR                     szOutputFile;
	TCHAR                       szOutfileFullName[MAX_PATH];
	HANDLE                      hFile;
	DWORD                       writtenWAVHeader;
	int                         totalBytesWritten;
	CStaticMediaBuffer          outputBuffer;
    INuiAudioBeam*              pSC;
	DWORD                       cOutputBufLen;
    BYTE*                       pbOutputBuffer;
	DMO_OUTPUT_DATA_BUFFER      OutputBufferStruct;
	ULONG                       cbProduced;
	DWORD                       dwStatus;
	
	bool                        audioRecordEnded;   // Index of the tracked skeleton that most closely machtes the audio.
	//---------------------------------------------------------------------/
	//Skeleton tracking
	//---------------------------------------------------------------------/
	void		           	 getJointsPoints();
	void			         Kinect_ColorFromDepth(LONG depthX, LONG depthY, LONG *pColorX, LONG *pColorY);
	bool                     isSkeletonTracked;  //flag to indicate if there are skeletons being tracked
	// Joint location data for last tracked player
	int                      headJoints_x;
	int                      headJoints_y;
	int                      headJoints_z;
	int                      handLeft_x;
	int                      handLeft_y;
	int                      handRight_x;
	int                      handRight_y;
	int                      shoulderLeft_x;
	int                      shoulderLeft_y;
	int                      shoulderRight_x;
	int                      shoulderRight_y;
	// X and Depth values for the head of all players
	int				         headXValues[NUI_SKELETON_COUNT];
	int                      headYValues[NUI_SKELETON_COUNT];
	int				         headZValues[NUI_SKELETON_COUNT];

	int                      leftShoulderXValues[NUI_SKELETON_COUNT];
	int                      leftShoulderYValues[NUI_SKELETON_COUNT];

	int                      rightShoulderXValues[NUI_SKELETON_COUNT];
	int                      rightShoulderYValues[NUI_SKELETON_COUNT];
	
	int                      leftHandXValues[NUI_SKELETON_COUNT];
	int                      leftHandYValues[NUI_SKELETON_COUNT];
	
	int                      rightHandXValues[NUI_SKELETON_COUNT];
	int                      rightHandYValues[NUI_SKELETON_COUNT];


private:

	//---------------------------------------------------------------------/
	// Audio stuff
	//---------------------------------------------------------------------/
	HANDLE			        mmHandle;
	IMediaObject*	        pDMO;  
    IPropertyStore*	        pPS;
	double                  dConf;               // Values for audio localization
	double                  dBeamAngle, dAngle;  // Values for audio localization

	//---------------------------------------------------------------------/
	// Nui and video handle and events
	//---------------------------------------------------------------------/
	HANDLE                  m_hNextSkeletonFrameEvent;
    HANDLE                  m_hNextDepthFrameEvent;
    HANDLE                  m_hNextVideoFrameEvent;
    HANDLE                  m_hNextDepthPlayerFrameEvent;
	HANDLE                  m_pDepthStreamHandle;
	HANDLE                  m_pDepthPlayerStreamHandle;
    HANDLE                  m_pVideoStreamHandle;
	RGBQUAD                 Kinect_DepthToRGB( USHORT s );
	//---------------------------------------------------------------------/
	// Video stuff
	//---------------------------------------------------------------------/
	// adjusts format of pixel data from kinect to put into a RGBA OF texture
	void		            Kinect_FormatRGBForOutput();	
	//pixel buffers
	int                     m_LastFramesTotal;
	BYTE 		            m_rgbBuffer[4*VIDEO_HEIGHT*VIDEO_WIDTH];
	BYTE 		            m_rgb_noalpa_Buffer[3*VIDEO_HEIGHT*VIDEO_WIDTH];	
	RGBQUAD		            m_rgbDepth[VIDEO_HEIGHT*VIDEO_WIDTH];
	USHORT		            m_depthBuffer[DEPTH_HEIGHT*DEPTH_WIDTH];
	USHORT		            m_playerBuffer[DEPTH_HEIGHT*DEPTH_WIDTH];
};