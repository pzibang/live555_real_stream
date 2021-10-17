#ifndef _H264FRAMEDLIVESOURCE_HH
#define _H264FRAMEDLIVESOURCE_HH

#include <FramedSource.hh>


class H264FramedLiveSource : public FramedSource
{
public:
	static H264FramedLiveSource* createNew(UsageEnvironment& env, int *datasize, unsigned char*  databuf, bool *dosent, unsigned preferredFrameSize = 0, unsigned playTimePerFrame = 0);

protected:
	H264FramedLiveSource(UsageEnvironment& env, int *datasize, unsigned char*  databuf, bool *dosent, unsigned preferredFrameSize, unsigned playTimePerFrame);
	~H264FramedLiveSource();

private:
	virtual void doGetNextFrame();
	int TransportData(unsigned char* to, unsigned maxSize);

protected:
	int *Framed_datasize;//��������Сָ��
	unsigned char *Framed_databuf;//������ָ��
	bool *Framed_dosent;//���ͱ�ʾ

	int readbufsize;//��¼�Ѷ�ȡ��������С
	int bufsizel;//��¼��������С
};

#endif