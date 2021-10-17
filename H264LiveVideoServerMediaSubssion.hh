#ifndef _H264_LIVE_VIDEO_SERVER_MEDIA_SUBSESSION_HH
#define _H264_LIVE_VIDEO_SERVER_MEDIA_SUBSESSION_HH

#include "H264VideoFileServerMediaSubsession.hh"

class H264LiveVideoServerMediaSubssion : public H264VideoFileServerMediaSubsession {

public:
	static H264LiveVideoServerMediaSubssion* createNew(UsageEnvironment& env, Boolean reuseFirstSource, int *datasize, unsigned char*  databuf, bool *dosent);

protected: // we're a virtual base class
	H264LiveVideoServerMediaSubssion(UsageEnvironment& env, Boolean reuseFirstSource, int *datasize, unsigned char*  databuf, bool *dosent);
	~H264LiveVideoServerMediaSubssion();

protected: // redefined virtual functions
	FramedSource* createNewStreamSource(unsigned clientSessionId,unsigned& estBitrate);
public:
	char fFileName[100];

	int *Server_datasize;//��������Сָ��
	unsigned char*  Server_databuf;//������ָ��
	bool *Server_dosent;//���ͱ�ʾ
};
#endif