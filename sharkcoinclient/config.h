#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <string>
using namespace std;

#define MSG_USER_QUITTHREAD		   WM_USER+1	
#define MSG_USER_MOBILE_STATEUS    WM_USER+2
#define MSG_USER_MOBILE_TIP        WM_USER+3


#define WM_BN_CLICK						WM_USER+801

typedef struct tagMSG_NOTIFY {
	DWORD	nThreadId	;
	HWND	hSubWnd		;
	UINT	msg			;
	void	*relMsgMem	;	//�ؽ��ڴ��ַ
} MSG_NOTIFY ;

typedef struct tagsThrd{
	HANDLE hThrd		 ;
	unsigned int hThrdId ;
} sThrd ;

//// �̶�����ͼƬ����
//string button_out_pic ini.bmp;      // ��ʼ��ťͼƬ����
//string button_in_pic Install.bmp;     // δ��װӦ��������ȥ��ͼƬ
//string Installbutton_in_pic  UnInstall.bmp;     // ��װӦ��������ȥ��ͼƬ
//string Installbutton_in_pic  click.bmp;     // δ��װӦ�������ͼƬ
/// Сͼ ��ͼ����
typedef struct ADD_APP{
	string   appname;             /// ��ִ���ļ�����
	bool     isLagerPic;          // true �Ǵ�ͼ false ��Сͼ
	bool     isInstall;           //true �Ѱ�װ false δ��װ
	int      type;                // Ӧ�ó������� 0 ��ʾ��ͼƬ 1 ��ͨ���� 2 ��Ϸ����Ϸ
	int      version;             /// 10000
	ADD_APP(){
		appname ="";   //address
		isLagerPic =false  ;				//���
		isInstall = false;
		type = 0;  
		version =1000;
	}
}ADD_APP_DATA;


typedef void (*BLOCKDATA)(char *pRecBuffer, int nRecLen , WPARAM wParam); 
#endif