#include <iostream>
#include <stdio.h>
#include <liveMedia.hh>
#include <BasicUsageEnvironment.hh>
#include <GroupsockHelper.hh>
#include "H264LiveVideoServerMediaSubssion.hh"
#include "H264FramedLiveSource.hh"
#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"

#define BUFSIZE 1024*200

#ifdef __unix
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),  (mode)))
#endif

static void announceStream(RTSPServer* rtspServer, ServerMediaSession* sms,char const* streamName)//��ʾRTSP������Ϣ
{
	char* url = rtspServer->rtspURL(sms);
	UsageEnvironment& env = rtspServer->envir();
	env <<streamName<< "\n";
	env << "Play this stream using the URL \"" << url << "\"\n";
	delete[] url;
}

int main(int argc, char** argv) 
{
	//���û���
	UsageEnvironment* env;
	Boolean reuseFirstSource = False;//���Ϊ��true������������Ŀͻ��˸���һ���ͻ��˿���һ������Ƶ�������������ͻ��˽����ʱ�����²���
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	env = BasicUsageEnvironment::createNew(*scheduler);

	//����RTSP������
	UserAuthenticationDatabase* authDB = NULL;
	RTSPServer* rtspServer = RTSPServer::createNew(*env, 8554, authDB);
	if (rtspServer == NULL) {
		*env << "Failed to create RTSP server: " << env->getResultMsg() << "\n";
		exit(1);
	}
	char const* descriptionString= "Session streamed by \"testOnDemandRTSPServer\"";

	//ģ��ʵʱ��������ر���
	int datasize;//����������
	unsigned char*  databuf;//������ָ��
	databuf = (unsigned char*)malloc(1024*1024);
	bool dosent;//rtsp���ͱ�־λ��Ϊtrue���ͣ������˳�

	//���ļ��п���1M���ݵ��ڴ�����Ϊʵʱ���紫���ڴ�ģ�⣬���ʵʱ���紫��Ӧ����˫�߳̽ṹ���ǵ�����������߳���
	//����ʵʱ��������ݿ���Ӧ���Ƿ�����H264FramedLiveSource�ļ��У���������ֻ���������µĴ�ָ���ȥ����
	FILE *pf;
	fopen_s(&pf, "test.264", "rb");
	fread(databuf, 1, BUFSIZE, pf);
	datasize = BUFSIZE;
	dosent = true;
	fclose(pf);

	//����Ĳ��ֳ���ģ�����紫��Ĳ����������Ļ�����live555�ṩ��demoһ�������������޸�Ϊ���紫�����ʽ��Ϊ����дaddSubsession�ĵ�һ����������ļ�
	char const* streamName = "h264ESVideoTest";
	ServerMediaSession* sms = ServerMediaSession::createNew(*env, streamName, streamName,descriptionString);
	sms->addSubsession(H264LiveVideoServerMediaSubssion::createNew(*env, reuseFirstSource, &datasize, databuf,&dosent));//�޸�Ϊ�Լ�ʵ�ֵ�H264LiveVideoServerMediaSubssion
	rtspServer->addServerMediaSession(sms);

	announceStream(rtspServer, sms, streamName);//��ʾ�û�����������Ϣ
	env->taskScheduler().doEventLoop(); //ѭ���ȴ�����

	free(databuf);//�ͷŵ��ڴ�
	return 0;
}