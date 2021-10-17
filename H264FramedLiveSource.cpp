#include "H264FramedLiveSource.hh"

H264FramedLiveSource::H264FramedLiveSource(UsageEnvironment& env, int *datasize, unsigned char*  databuf, bool *dosent, unsigned preferredFrameSize, unsigned playTimePerFrame)
: FramedSource(env)
{
	Framed_datasize = datasize;//��������Сָ��
	Framed_databuf = databuf;//������ָ��
	Framed_dosent = dosent;//���ͱ�ʾ
}

H264FramedLiveSource* H264FramedLiveSource::createNew(UsageEnvironment& env, int *datasize, unsigned char*  databuf, bool *dosent, unsigned preferredFrameSize, unsigned playTimePerFrame)
{
	H264FramedLiveSource* newSource = new H264FramedLiveSource(env, datasize, databuf, dosent, preferredFrameSize, playTimePerFrame);
	return newSource;
}

H264FramedLiveSource::~H264FramedLiveSource()
{
}

void H264FramedLiveSource::doGetNextFrame()
{
	if (*Framed_dosent == true)
	{
		*Framed_dosent = false;
		bufsizel = *Framed_datasize;
		readbufsize = 0;

		fFrameSize = fMaxSize;
		memcpy(fTo, Framed_databuf + readbufsize, fFrameSize);
		readbufsize += fFrameSize;
	}
	else
	{
		if (bufsizel - readbufsize>fMaxSize)
		{
			fFrameSize = fMaxSize;
			memcpy(fTo, Framed_databuf + readbufsize, fFrameSize);
			readbufsize += fFrameSize;
		}
		else
		{
			fFrameSize= bufsizel - readbufsize;//fFrameSize�ǻ����live555�����������Ĵ�С�����븳����ȷ��ֵ			memcpy(fTo, Framed_databuf + readbufsize,fFrameSize);
			*Framed_dosent = true;
		}
	}

	nextTask() = envir().taskScheduler().scheduleDelayedTask(0,(TaskFunc*)FramedSource::afterGetting, this);//��ʾ�ӳ�0�����ִ�� afterGetting ����
	return;
}