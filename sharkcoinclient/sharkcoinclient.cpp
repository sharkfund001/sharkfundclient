
// sharkcoinclient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "sharkcoinclient.h"
#include "sharkcoinclientDlg.h"

#include "UiHead.h"
#include "SynchronousSocket.h"
#include "Log.h"
#include "PostMsg.h"
#include "CommonStruct.h"
#include "WalletPassPhrase.h"

#include"tlhelp32.h"
#include <afxsock.h>

#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
// CsharkcoinclientApp

BEGIN_MESSAGE_MAP(CsharkcoinclientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CsharkcoinclientApp ����

CsharkcoinclientApp::CsharkcoinclientApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_bReIndexServer = FALSE;
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	m_nLockStatus = STATUS_DECRYPTED;		//��ʼ��ΪǮ���޼���
	m_bIsDlgCreateFinished = FALSE;			//Ĭ��û�д������
	m_bIsSyncTx = FALSE;					//��������ͬ��
	m_bIsUpdateCheckRunSwitch = TRUE;		//���¼���߳̿��ر�־,Ĭ�Ͽ���״̬  
	m_bIsProcessMsgRunSwitch = TRUE;		//������Ϣ�߳̿��ر�־,Ĭ�Ͽ���״̬  
	m_bIsReceiveMsgRunSwitch = TRUE;		//���շ���������߳̿��ر�־,Ĭ�Ͽ���״̬
    m_bIsDlgCreateFinished = FALSE;			//���Ի���������dlg�Ƿ񴴽���
	m_bIsStartMainDlg = FALSE;				//�Ƿ��������Ի���
	m_bIsSyncTx = FALSE;					//�Ƿ�ͬ������
	m_nNetWorkType = REGTEST;				//��ʼ������Ϊ���ؾ�����       
	m_strServerCfgFileName = "sharkfund.conf";  //�����������ļ���
	m_nSyncHight = 0;                       //ͬ���߶ȳ�ʼ��
	m_bIsCommitDb = FALSE;                  //�Ƿ��ύִ�е����ݿ�
	m_bIsSyncBlock = FALSE;                 //������ͬ��block��־
	m_bIsWalletLocked = FALSE;              //Ǯ�����ܱ�־
	m_bIsHaveLocked = FALSE;                //Ǯ���Ƿ�����״̬
	m_bIsPassLock = TRUE;

	m_reminder = 0;
	m_poptips = true;
}


// Ψһ��һ�� ChuobiApp ����

CsharkcoinclientApp theApp;


// ChuobiApp ��ʼ��

BOOL CsharkcoinclientApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	if (!RunOnlyOneApp())
	{
		return FALSE;
	}

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	if (!AfxSocketInit()){   //��ʼ��SOCKET
		return FALSE ;
	}

	sSubscribeList.clear();
	v_ProcSubThrd.clear();
	InitializeCriticalSection(&(cs_MsgDisptch) ) ;
	GetMoFilename(m_strInsPath, m_strModuleFilename); //��ȡ�ļ�·�����ļ�����

	//��鹤��Ŀ¼�Ƿ�Ϊ�ǿո��Ŀ¼
	CheckPathValid(m_strInsPath);

	//����ά���߳�
	if( !CreateMaintainThrd() ) {
		return FALSE ;
	}

	/// ���������ļ�sharkcoinsclient.conf
	ParseUIConfigFile(m_strInsPath);

	//��ʼ����־���ò���
	InitLogCfg();


	string strRemoteRpcPort = m_rpcPort;
	string strRemoteUIPort = m_uiPort;
	ProductHttpHead(m_strInsPath ,m_strServerCfgFileName, m_rpcPort, m_sendPreHeadstr, m_sendEndHeadstr, m_uiPort, m_nNetWorkType);

	string dbpath = strprintf("%s\\db",m_strInsPath);
	if (!PathIsDirectory(dbpath.c_str()))
	{
		::CreateDirectory(dbpath.c_str(), NULL);
	}
	//��sqlite3���ݿ�
	m_SqliteDeal.InitializationDB();

	//�ж�����ΪԶ�̷���
	if (strcmp(m_severIp.c_str(),_T("127.0.0.1")))
	{
		m_rpcPort = strRemoteRpcPort;
		m_uiPort = strRemoteUIPort;
	}

	//����Ƿ���ڷ����������
	OnCheckSeverfile();

	//�����������
	StartSeverProcess(m_strInsPath);

	//���¼��
	Update();

	Sleep(1000);
	//m_pSocket = NULL ;
	//m_pSocket = new CHSocket(_T("127.0.0.1") , 12345);
	//if ( NULL != m_pSocket ) {
	//	if ( !m_pSocket->OnConnnect()) {
	//		AfxMessageBox("����������ʧ��!");
	//		return FALSE ;
	//	}
	//}

	//���ӵ�������
	CSynchronousSocket te;
	SOCKET nSocket = te.OnblockConnnect(m_severIp.c_str(),atoi(m_uiPort.c_str()) ) ;
	if ( INVALID_SOCKET != nSocket ){
		TRACE("nSocket OK\n");
		theApp.m_uiConnSocket = nSocket;
	}
	else
	{
		TRACE("nSocket Error\n");
	}

	theApp.StartWorkThread();  //����������Ϣ��������Ϣ�߳�

	m_strProgressGifFile =   m_strInsPath + "\\gif\\progress.gif\0";
	m_strProgressOutGifFile =  m_strInsPath + "\\gif\\exit.gif\0";
	//������ϴ˳�ʼ���������������CGIFControl::Load(LPSTCR filePath)����
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Status state = GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL) ;

	pSplashThread = (CSplashThread*) AfxBeginThread(RUNTIME_CLASS(CSplashThread),THREAD_PRIORITY_NORMAL,0, CREATE_SUSPENDED); 
	ASSERT(pSplashThread->IsKindOf(RUNTIME_CLASS(CSplashThread)));
	pSplashThread->ResumeThread(); 
	Sleep(1); 

	while(1)
	{
		if (m_bIsStartMainDlg)
		{
			break;
		}
		Sleep(100);
	}

	CsharkcoinclientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

BOOL CsharkcoinclientApp::RunOnlyOneApp()
{
	HANDLE hMutex = CreateMutex(NULL, FALSE, _T("sharkcoinclient"));
	// ���������
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
	{
		CloseHandle(hMutex);
		AfxMessageBox(_T("�����Ѿ���������") ,MB_OK);
		exit(1);  

		return FALSE;
	}

	return TRUE;
}

BOOL CsharkcoinclientApp::CreateMaintainThrd() 
{
	hMtThrd		= NULL ;
	nMtThrdID	= 0  ;
	hMtStartEvent = ::CreateEvent(0,FALSE,FALSE,0) ; 

	hMtThrd = (HANDLE)_beginthreadex( NULL, 0, &MtProc, this, 0, &nMtThrdID ) ;
	if( INVALID_HANDLE_VALUE == hMtThrd ) {
		::MessageBox( NULL , "ά���̴߳���ʧ��!" , "����" , MB_ICONERROR ) ;
		CloseHandle( hMtStartEvent ) ;
		hMtStartEvent = NULL ;
		return  FALSE;
	}

	::WaitForSingleObject( hMtStartEvent , 100000 ) ;
	CloseHandle( hMtStartEvent ) ;
	hMtStartEvent = NULL ;

	return TRUE ;	
}


UINT __stdcall CsharkcoinclientApp::MtProc(LPVOID pParam)
{
	MSG	msg , msgcpy ;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE) ;

	if(!SetEvent( ((CsharkcoinclientApp*)pParam)->hMtStartEvent ) ) {
		return 1;
	}

	while ( 1 )
	{	
		BOOL MsgReturn	= GetMessage( &msg , NULL , 0 , 0 );
		memset( &msgcpy , 0 , sizeof(MSG) ) ; 
		memcpy( &msgcpy , &msg , sizeof(MSG) );
		if ( MsgReturn )
		{
			switch ( msg.message )
			{
			case MSG_USER_MOBILE_STATEUS:
				{
                    ::PostThreadMessage( ((CsharkcoinclientApp*)pParam)->GetMtHthrdId() , MSG_USER_MOBILE_TIP , msgcpy.wParam , msgcpy.lParam ) ;
				}
				break;
			case MSG_USER_QUITTHREAD:
				{
					std::vector< sThrd >::iterator it ;
					CsharkcoinclientApp* pApp = ((CsharkcoinclientApp*)pParam) ;
					for( it = pApp->v_ProcSubThrd.begin() ; it != pApp->v_ProcSubThrd.end() ; it++ ) {
						::PostThreadMessage( it->hThrdId , MSG_USER_QUITTHREAD , 0 , 0 ) ;
						DWORD exc = 0xffff ;
						while( ::GetExitCodeThread( it->hThrd , &exc ) ) {
							if( STILL_ACTIVE == exc ) {
								;
							}else {
								TRACE( "EXC = %d \n" , exc ) ;
								break;
							}
						}
					}
					pApp->v_ProcSubThrd.clear() ;
					_endthreadex( 0 ) ; 
				}
				break;
			default:
				break;
			}
			((CsharkcoinclientApp*)pParam)->DispatchMsg( ((CsharkcoinclientApp*)pParam)->GetMtHthrdId() , msgcpy.message , msgcpy.wParam , msgcpy.lParam ) ;
		}
	}
}
void CsharkcoinclientApp::RegThrdToMtThrd( HANDLE hThread , unsigned int hThrdId )
{
	if( NULL == hThrdId || NULL == hThread ) {
		return ;
	}
	sThrd sthrd ;
	memset(  &sthrd , 0 , sizeof( sThrd ) ) ;
	sthrd.hThrd = hThread ;
	sthrd.hThrdId = hThrdId ;

	UnRegThrdToMtThrd( sthrd.hThrd , sthrd.hThrdId ) ;
	v_ProcSubThrd.push_back( sthrd ) ;
	TRACE("The thread id=%d RegThrdToMtThrd\n" , hThrdId ) ;
}
void CsharkcoinclientApp::UnRegThrdToMtThrd( HANDLE hThread , unsigned int hThrdId ) 
{
	if( NULL == hThrdId || NULL == hThread ) {
		return ;
	}
	std::vector< sThrd >::iterator it ;
	for( it = v_ProcSubThrd.begin() ; it != v_ProcSubThrd.end() ; it++ ) {
		if( hThrdId == it->hThrdId || 
			hThread == it->hThrd ) {
				TRACE("The thread id=%d UnRegThrdToMtThrd\n" , it->hThrdId ) ;
				v_ProcSubThrd.erase( it );
				break ;
		}
	}
}
void CsharkcoinclientApp::SubscribeMsg( DWORD nThreadId , HWND hWnd, UINT msg , void * relMsgMem )
{
	if( NULL == hWnd ) {
		return ;
	}
	//���ظ�����
	UnSubscribeMsg( nThreadId , hWnd , msg ) ;
	//
	MSG_NOTIFY msgNotify ;
	memset(  &msgNotify , 0 , sizeof( MSG_NOTIFY ) ) ;
	msgNotify.nThreadId = nThreadId ;
	msgNotify.hSubWnd   = hWnd		;
	msgNotify.msg		= msg		;
	msgNotify.relMsgMem	= relMsgMem	;

	EnterCriticalSection( &theApp.cs_MsgDisptch ) ;
	sSubscribeList.push_back( msgNotify ) ;
	LeaveCriticalSection (&theApp.cs_MsgDisptch ) ;
}

void CsharkcoinclientApp::UnSubscribeMsg( DWORD nThreadId , HWND hWnd, UINT msg )
{
	if( NULL == hWnd ) {
		return ;
	}
	std::vector<MSG_NOTIFY>::iterator it ;
	EnterCriticalSection( &theApp.cs_MsgDisptch ) ;
	for( it = sSubscribeList.begin() ; it != sSubscribeList.end() ; it++ ) {
		if( nThreadId == it->nThreadId && \
			hWnd == it->hSubWnd && \
			msg == it->msg ) {
				sSubscribeList.erase( it );
				break ;
		}
	}
	LeaveCriticalSection (&theApp.cs_MsgDisptch ) ;
}

void CsharkcoinclientApp::DispatchMsg( unsigned int threadID , UINT msg , WPARAM wParam , LPARAM lParam ) 
{
	BOOL flag = FALSE ;
	std::vector<MSG_NOTIFY>::iterator it ;

	EnterCriticalSection( &cs_MsgDisptch ) ;
	for( it = sSubscribeList.begin() ; it != sSubscribeList.end() ; it++ ) {
		if( threadID == it->nThreadId && msg == it->msg ) {
			if( NULL != it->relMsgMem ) {
				flag = TRUE ;
				::PostMessage( it->hSubWnd , msg , wParam , (LPARAM)it->relMsgMem ) ;
			} else {
				::PostMessage( it->hSubWnd , msg , wParam , lParam ) ;
			}
		}
	}
	LeaveCriticalSection (&cs_MsgDisptch ) ;

	if( flag ) {
		uiapi::ReleaseP( (void*)lParam );
	}
}

void CsharkcoinclientApp::GetMoFilename( string & path , string & filename ) 
{
	char strPath[MAX_PATH] ;
	GetModuleFileName( AfxGetApp()->m_hInstance , strPath , MAX_PATH );
	CString str=strPath;
	int i=str.ReverseFind('\\');
	path		= str.Left(i);
	filename	= strPath;	
}

void CsharkcoinclientApp::StartSeverProcess(const string& strdir)
{
	CloseProcess(_T("sharkfundd.exe"));

	STARTUPINFOA si; 
	memset(&si, 0, sizeof(STARTUPINFO));  
	si.cb = sizeof(STARTUPINFO);  
	si.dwFlags = STARTF_USESHOWWINDOW;  
	si.wShowWindow =SW_HIDE;

//	CString str;
//	str.Format(_T("sharkcoinsd.exe -datadir=%s -ui=1") ,strdir ) ;  //str = "sharkcoinsd.exe -datadir=D:\code\sharkcoins\bin -ui=1"
	string startBat = strprintf("sharkfundd -datadir=%s -ui=1", strdir);
	if(m_bReIndexServer)
		startBat += _T(" -reindex=1");

	if(!CreateProcessA(NULL,(LPSTR)(startBat.c_str()),NULL,NULL,FALSE,0,NULL,NULL,&si,&sever_pi)){
		int n = GetLastError();
		AfxMessageBox(_T("��������˳���ʧ��\n"));
		exit(1);  
	}
	CloseHandle(sever_pi.hProcess);
	CloseHandle(sever_pi.hThread);
}

void CsharkcoinclientApp::CloseProcess(const string& exename)
{
	HANDLE SnapShot,ProcessHandle;  
	SHFILEINFO shSmall;  
	PROCESSENTRY32 ProcessInfo;   
	int count  = 0;
	while(TRUE) 
	{
		SnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);  
		if(SnapShot!=NULL) {
			ProcessInfo.dwSize=sizeof(ProcessInfo);
			BOOL Status=Process32First(SnapShot,&ProcessInfo); 
			while(Status)
			{
				SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall, sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);
				USES_CONVERSION;
				CString str(ProcessInfo.szExeFile);
				char* pData = str.GetBuffer(str.GetLength());
				str.ReleaseBuffer();
				strlwr(pData);
				for(int i=0;i<3;i++)
					if(!strcmp(exename.c_str(),str)){
						ProcessHandle=OpenProcess(PROCESS_ALL_ACCESS,FALSE,ProcessInfo.th32ProcessID);  
						TerminateProcess(ProcessHandle,0);  
						return; 
					}
				    Status=Process32Next(SnapShot,&ProcessInfo);  
			}
			break;
		}
		Sleep(1000);  
	}
}

//todo �ж�Ǯ���Ƿ��Ѽ���
bool CsharkcoinclientApp::IsLockWallet(){
	
	if (m_bIsPassLock)
	{
		while(m_bIsWalletLocked)
		{
			CWalletPassPhrase walletpassdlg;
			if (walletpassdlg.DoModal() == IDCANCEL)
			{
				break;
			}
			Sleep(100);
		}
	}

	return m_bIsWalletLocked;

}


void CsharkcoinclientApp::OnCheckSeverfile()
{
	string strSeverPath = strprintf("%s\\sharkfundd.exe", m_strInsPath);

	if (PathIsDirectory(strSeverPath.c_str()))
	{
		::CreateDirectory(strSeverPath.c_str(), NULL);
		::RemoveDirectory(strSeverPath.c_str());
		if( (_access( strSeverPath.c_str(), 0 )) == -1 )
		{
			UiFun::MessageBoxEx(_T("��ִ��sharkfundd.exe�ļ�������,����������Ǯ��\r\n") , _T("Error") ,MFB_OK|MFB_ERROR );
			exit(1);
		}
	}

	strSeverPath = strprintf("%s\\temp\\sharkfundd.exe",m_strInsPath);

	if (PathIsDirectory(strSeverPath.c_str()))
	{
		::CreateDirectory(strSeverPath.c_str(), NULL);
		::RemoveDirectory(strSeverPath.c_str());
	}
	//todo �����ļ����
	///// �ж��ļ��Ƿ����
	//if( (_access( strSeverPath.c_str(), 0 )) == -1 )
	//{
	//	UiFun::MessageBoxEx(_T("sharkcoinsd �ļ�������,����������\r\n") , _T("Error") ,MFB_OK|MFB_ERROR );
	//	exit(1);
	//}
}

//У��·���Ϸ���
void CsharkcoinclientApp::CheckPathValid(const string& strDir)
{
	BOOL bExist = FALSE;
	for(int i = 0;i < (int)strDir.length();i++)
	{
		BYTE bchar = (BYTE)strDir.at(i);
		if(bchar == ' ')
		{
			bExist = TRUE;
			break;
		}
	}

	if (bExist)
	{
		UiFun::MessageBoxEx("���򲻿��Է��ں��пո��Ŀ¼��\r\n" , _T("Error") ,MFB_OK|MFB_ERROR );
		exit(0);
	}
}


void CsharkcoinclientApp::StartWorkThread() 
{
	//���շ���˷��͹���message�������䱣����
	m_hReceiveThread = (HANDLE)_beginthreadex(NULL,	0,	ReceiveMessageFromServer, this, 0, &m_uReceiveThreadId);
	//������ϢΪʵ�ʸ�ʽ����������Ϣ������
	m_hPraseJsonToMsgThread = (HANDLE)_beginthreadex(NULL,	0,	PraseJsonToMessage, this, 0, &m_uPraseJsonToMsgThreadId);
	//���ݲ�ͬ��Ϣ��������ͬ��Ӧ
	m_hProcessMsgThread = (HANDLE)_beginthreadex(NULL,	0,	ProcessMessage, this, 0, &m_uProcessMsgThreadId);
	//���ͻ����Ƿ��и���
	m_hCheckUpdateThread = (HANDLE)_beginthreadex(NULL,0, CheckUpdate, this, 0, &m_uCheckUpDateThreadId);
}


//У��json�ַ����ĸ�ʽ�Ƿ���ȷ
bool JsonCheck(string strjson){

	strjson =  UiFun::trimleft(strjson);
	strjson =  UiFun::trimright(strjson);
	if (strjson.at(0) != '{')
	{
		return false;
	}
	int len = strjson.length();
	if (strjson.at(len-1) != '}')
	{
		return false;
	}
	return true;
}

UINT __stdcall CsharkcoinclientApp::ReceiveMessageFromServer(LPVOID pParam)
{
	bool bReConnect(false);
	int nMaxbufferLen(10*1024-1);
	CsharkcoinclientApp * psharkcoinsApp  = (CsharkcoinclientApp*)pParam ;
	if ( NULL != psharkcoinsApp ) {
		while (true)
		{
			Sleep(100);
			if (theApp.m_bIsCloseAppFlag)
			{
				return 1;
			}
			if(bReConnect) {

				CSynchronousSocket te;
				SOCKET nSocket = te.OnblockConnnect((char*)psharkcoinsApp->m_severIp.c_str(),atoi(psharkcoinsApp->m_uiPort.c_str()));
				if( INVALID_SOCKET != nSocket) {
					psharkcoinsApp->m_uiConnSocket = nSocket;
					bReConnect = false;
					TRACE("retry nSocket OK\n");
				}
				else
				{
				TRACE("retry nSocket Error\n");
				}
			}
			else {
				if ( INVALID_SOCKET != psharkcoinsApp->m_uiConnSocket ) {
					char  cRecvbuffer[10*1024];
					int nRecLen = recv( psharkcoinsApp->m_uiConnSocket , cRecvbuffer , nMaxbufferLen , 0);

					if ( nRecLen > 0 ) {
						/*TRACE("recv len %d:",nRecLen);
						for(int i=0; i< nRecLen; ++i) {
							TRACE("%02X",cRecvbuffer[i]);
						}
						TRACE("\n");*/
						if(!psharkcoinsApp->m_noUiMsgBuffer.AddBytesToBuffer(cRecvbuffer, nRecLen)) {
							if (INVALID_SOCKET != psharkcoinsApp->m_uiConnSocket)
							{
								closesocket(psharkcoinsApp->m_uiConnSocket);
								psharkcoinsApp->m_uiConnSocket = INVALID_SOCKET;
							}
							psharkcoinsApp->m_noUiMsgBuffer.ClearBuffer();
							bReConnect = true;
						}
					}
					else if(nRecLen == 0) 
					{
						TRACE0("noui socket has been closed\n");
						if (INVALID_SOCKET != psharkcoinsApp->m_uiConnSocket)
						{
							closesocket(psharkcoinsApp->m_uiConnSocket);
							psharkcoinsApp->m_uiConnSocket = INVALID_SOCKET;
						}
						bReConnect = true;
					}
					else if(nRecLen < 0) 
					{
						 TRACE1("recv failed: %d\n", WSAGetLastError());
						 if (INVALID_SOCKET != psharkcoinsApp->m_uiConnSocket)
						 {
							 closesocket(psharkcoinsApp->m_uiConnSocket);
							 psharkcoinsApp->m_uiConnSocket = INVALID_SOCKET;
						 }
						 bReConnect = true;
					}
				}
				else
				{
						bReConnect = true;
				}
			}
		}
	}
	return 1 ;
}

int GetMsgType(string const m_strData,Json::Value &root)
{
	string m_strType;
	int pos = m_strData.find("type");
	if ( pos >=0 ){
		m_strType =root["type"].asString();
		if ( !strcmp(m_strType.c_str() ,_T("init")))			 //��ʼ��Ǯ������
		{
			return ININTAL_TYPE;
		}
		if ( !strcmp(m_strType.c_str() ,_T("revtransaction")))   //������յ��뱾Ǯ����ַ��ؽ���
		{
			return REV_TRANSATION_TYPE;
		}
		if ( !strcmp(m_strType.c_str() ,_T("blockchanged")))     //�������鷢���ı�
		{
			return BLOCK_CHANGE_TYPE;
		}
		if ( !strcmp(m_strType.c_str() ,_T("rev_app_transaction"))) //������յ��뱾��Ǯ����غ�Լ����
		{
			return APP_TRANSATION_TYPE;
		}
		if(!strcmp(m_strType.c_str(), _T("notify")))             //����˷��͸�Ǯ�������֪ͨ��Ϣ
		{
			return SERVER_NOTIYF_TYPE;
		}
		if(!strcmp(m_strType.c_str(), _T("SyncTx")))             //������ͬ�����100����Ľ���
		{
			return SERVER_SYNC_TX;
		}
		if(!strcmp(m_strType.c_str(), _T("releasetx")))          //����˽����״��������ͷţ�֪ͨ���������Ϣ
		{
			return RELEASE_TX;
		}
		if(!strcmp(m_strType.c_str(), _T("rmtx")))               //����˽����Ϸ��Ľ��״�mempool�������֪ͨ���������Ϣ 
		{
			return REMOVE_TX;
		}
		if(!strcmp(m_strType.c_str(), _T("SyncTxHight")))        //�������Ǯ������ͬ���߶� 
		{
			return SYSTX_HEIGHT;
		}
	}
	return  -1;
}


bool ProcessMsgJson(Json::Value &msgValue, CsharkcoinclientApp* pApp) 
{
	string objstr = msgValue.toStyledString();
	int type = GetMsgType(objstr.c_str(),msgValue);
	LogPrint("RECIVEUI", "MESG:%s\n",objstr.c_str());
	switch(type)
	{
	case ININTAL_TYPE:
		{
			string msg = msgValue["msg"].asString();
			TRACE("MEST:%s\r\n",msg.c_str());
			if (!strcmp(msg.c_str(),"Verifying blocks..."))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,1);
				pApp->m_msgQueue.push(postmsg);
			}
			else if (!strcmp(msg.c_str(),"Verifying Finished"))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,2);
				pApp->m_msgQueue.push(postmsg);
			}
			else if (!strcmp(msg.c_str(),"Loading addresses..."))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,3);
				pApp->m_msgQueue.push(postmsg);
			}
			else if (!strcmp(msg.c_str(),"initialize end"))
			{
				CPostMsg postmsg(MSG_USER_STARTPROCESS_UI,4);
				pApp->m_msgQueue.push(postmsg);				
			}
//			postmsg.SetData(msg);
//			pApp->m_msgQueue.push(postmsg);
//		TRACE("type: %s   mag: %s\r\n" , postmsg.Getm_strType() ,msg);
			break;
		}
	case SERVER_SYNC_TX:
		{

			Json::Value obj =msgValue["msg"]; 
			//���뵽���ݿ�
			string txData ;
			txData =strprintf("%s" , obj.toStyledString().c_str() );
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_SYNC_TRANSACTION);
			postmsg.SetData(txData);
			pApp->m_msgQueue.push(postmsg);
			
		}
		break;
	case SYSTX_HEIGHT:
		{
			Json::Value obj =msgValue["msg"]; 
			//���뵽���ݿ�
			if (!obj.isNull())
			{
				int height = obj["syncheight"].asInt();
				pApp->m_nSyncHight = height;
				pApp->DeleteItemTransaction(height);
			}
		}
		break;
	case REV_TRANSATION_TYPE:
		{

			SYSTEMTIME curTime ;
			memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			GetLocalTime( &curTime ) ;
			static int RecivetxtxTimeLast =0;
			int tempTimemsg= (int)UiFun::SystemTimeToTimet(curTime);
			/// ����Ǯ��
			CPostMsg postuimsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRESS);
			if ((tempTimemsg - RecivetxtxTimeLast)>10 || RecivetxtxTimeLast == 0)
			{	
				pApp->m_msgQueue.push(postuimsg);
				RecivetxtxTimeLast = tempTimemsg;
			}

		//	LogPrint("INFO", "REV_TRANSATION %s\n",msgValue.toStyledString().c_str());
			const Json::Value& txArray = msgValue["transation"]; 
			//���뵽���ݿ�
			string strHash =txArray["hash"].asString();
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
			postmsg.SetData(strHash);
			pApp->m_msgQueue.push(postmsg);

			}
			break;
		case APP_TRANSATION_TYPE:
		 {
			 Json::Value obj =msgValue["transation"]; 
			 CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_APP_TRANSATION);
			 postmsg.SetData(obj.toStyledString().c_str());
			 pApp->m_msgQueue.push(postmsg);
		 }
		  break;
	case RELEASE_TX:
		{
			//���뵽���ݿ�
			string strHash =  msgValue["hash"].asString() ;
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_RELEASETX);
			postmsg.SetData(strHash);
			pApp->m_msgQueue.push(postmsg);
		}
		break;
	case REMOVE_TX:
		{
			//���뵽���ݿ�
			string strHash = msgValue["hash"].asString() ;
	
			CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REMOVETX);
			postmsg.SetData(strHash);
			pApp->m_msgQueue.push(postmsg);
		}
		break;
	case BLOCK_CHANGE_TYPE:
		{
			SYSTEMTIME curTime ;
			memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
			GetLocalTime( &curTime ) ;
			static int RecivetxMsgTimeLast =0;
			int tempTimemsg=(unsigned int) UiFun::SystemTimeToTimet(curTime);
		
			TRACE("change:%s\r\n","blockchanged");
			uistruct::BLOCKCHANGED_t      m_Blockchanged ;
			m_Blockchanged.type = msgValue["type"].asString();
			m_Blockchanged.tips = msgValue["tips"].asInt();
			m_Blockchanged.high = msgValue["high"].asInt64() ;
			m_Blockchanged.hash = msgValue["hash"].asString();
			m_Blockchanged.connections = msgValue["connections"].asInt();
			m_Blockchanged.fuelrate = msgValue["fuelrate"].asInt();

			string strJson = m_Blockchanged.ToJson();
			CPostMsg postmsg(MSG_USER_UP_PROGRESS,0);
			postmsg.SetData(strJson.c_str());

			pApp->m_msgQueue.push(postmsg);
			
			
		//	if ((tempTimemsg - RecivetxMsgTimeLast)>10 || RecivetxMsgTimeLast == 0)
			if(m_Blockchanged.high == m_Blockchanged.tips)
			{	
				/// ����tipblock hash
				CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_BlLOCKTIP);
				string msg = msgValue.toStyledString();
				postblockmsg.SetData(msg);
				pApp->m_msgQueue.push(postblockmsg);  

			
				/// ����Ǯ����ַ
				CPostMsg postuimsg(MSG_USER_GET_UPDATABASE,WM_UP_ADDRESS);
				pApp->m_msgQueue.push(postuimsg);

				RecivetxMsgTimeLast = tempTimemsg;
			}
			break;
		}
	case SERVER_NOTIYF_TYPE:
		{
			string msg = msgValue["msg"].asString();
			TRACE("MEST:%s\r\n",msg.c_str());
			if (!strcmp(msg.c_str(),"server closed"))
			{
				theApp.m_bIsServerClose = false;
			}else if (!strcmp(msg.c_str(),"Lock"))
			{
				CPostMsg postmsg(MSG_USER_UP_PROGRESS,WM_LOCKSTATE);
				postmsg.Setm_strType(msg);
				pApp->m_msgQueue.push(postmsg);
				pApp->m_bIsWalletLocked = TRUE;
				pApp->m_nLockStatus =2;

				//// ����block
				CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_UPDATABlLOCK);
				pApp->m_msgQueue.push(postblockmsg);  
			}else if (!strcmp(msg.c_str(),"UnLock"))
			{
				CPostMsg postmsg(MSG_USER_UP_PROGRESS,WM_LOCKSTATE);
				postmsg.Setm_strType(msg);
				pApp->m_msgQueue.push(postmsg);
				pApp->m_bIsWalletLocked = FALSE;
				pApp->m_nLockStatus =1;

				//// ����block
				CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_UPDATABlLOCK);
				pApp->m_msgQueue.push(postblockmsg);  
			}else if (msg.find("connections")>=0)
			{
				CPostMsg postmsg(MSG_USER_UP_PROGRESS,WM_CONNECTNET);
				postmsg.Setm_strType(msg);
				pApp->m_msgQueue.push(postmsg);
			}
			//TRACE("type: %s   mag: %s\r\n" , postmsg.Getm_strType() ,msg);
			break;
		}
	default:
		break;
	}
	return true;
}

UINT __stdcall CsharkcoinclientApp::PraseJsonToMessage(LPVOID pParam)
{
	CsharkcoinclientApp * psharkcoinsApp  = (CsharkcoinclientApp*)pParam;
	if ( NULL != psharkcoinsApp ) {
		while (true)
		{
		    Sleep(100); 
			if (theApp.m_bIsCloseAppFlag && !psharkcoinsApp->m_noUiMsgBuffer.HaveNoUiMsg())
			{
				return 1;
			}
			while(psharkcoinsApp->m_noUiMsgBuffer.HaveNoUiMsg()) {
				string strMsg;
				psharkcoinsApp->m_noUiMsgBuffer.GetNoUiMsg(strMsg);
				Json::Reader reader;  
				Json::Value jsonValue; 
				if (!JsonCheck(strMsg))
				{
					TRACE("JsonCheck noui msg error,msg content:%s\n", strMsg.c_str());
					continue;
				}
				if (strMsg == _T(""))
				{
					continue;
				}
				if (!reader.parse(strMsg, jsonValue)) 
					continue;
				ProcessMsgJson(jsonValue, psharkcoinsApp);
			}
		}
	}
	return 1;
}

bool CsharkcoinclientApp::Update()
{
			CString sPath;
			sPath.Format(_T("%s\\update.exe"),m_strInsPath.c_str());
			sPath.Replace("\\\\","\\");
			CFileFind find;
			if(!find.FindFile(sPath)) return false;
			SHELLEXECUTEINFO ShRun = {0}; 
			ShRun.cbSize = sizeof(SHELLEXECUTEINFO); 
			ShRun.fMask = SEE_MASK_NOCLOSEPROCESS; 
			ShRun.hwnd = NULL; 
			ShRun.lpVerb = NULL; 
			ShRun.lpFile = sPath;
			ShRun.lpParameters = _T("-checkforupdates"); 
			ShRun.nShow = SW_HIDE; 
			ShRun.hInstApp = NULL; 
			ShellExecuteEx(&ShRun); 
			WaitForSingleObject(ShRun.hProcess, 120000); 
			ULONG lResult = 0; 
			if (!GetExitCodeProcess(ShRun.hProcess, &lResult)) return false;

			if (lResult == 0) return false;
			CString sMsg;
			sMsg.Format(_T("�����%d�ļ���Ҫ����,�����Ƿ�Ҫ����?"),lResult);
			
		/*	if ( IDYES != UiFun::MessageBoxEx(sMsg, UiFun::UI_LoadString("COMM_MODULE" , "COMM_TIP" ,theApp.gsLanguage) , MFB_YESNO|MFB_TIP ) ){
				((ChuobiDlg*)(pApp->m_pMainWnd))->ClosWalletWind();
				return false;
			}*/

			if(m_pMainWnd != NULL)
				SendMessage(((CsharkcoinclientDlg*)(m_pMainWnd))->m_hWnd,MSG_USER_CLOSEPROCESS,NULL,NULL);
			CloseProcess("sharkfundd.exe");
			DWORD exc;
			m_bIsProcessMsgRunSwitch = false;
			m_bIsReceiveMsgRunSwitch = false;
			m_bIsUpdateCheckRunSwitch = false; 
			closesocket(theApp.m_uiConnSocket);
			/// �ȴ�������Ϣ�����˳�
			while( ::GetExitCodeThread( theApp.m_hProcessMsgThread , &exc ) ) {

				if( STILL_ACTIVE == exc ) {
					;
				}else {
					TRACE( "EXC = %d \n" , exc ) ;
					break;
				}
				Sleep(100);
			}
			ShRun.lpParameters = NULL; 
			ShRun.nShow = SW_SHOW;
			ShellExecuteEx(&ShRun);
			CloseProcess("sharkfundclient.exe");
			return true;
}


UINT __stdcall CsharkcoinclientApp::CheckUpdate(LPVOID lpParam){

	CsharkcoinclientApp * pApp  = (CsharkcoinclientApp*)lpParam; 

	while(TRUE)
	{
		///һ����
		static int nCount = 0;
		Sleep(1);
		if (theApp.m_bIsCloseAppFlag)
		{
			return 1;
		}

		nCount++;
		if(nCount == 30000 ){
			nCount =0;
			if(false == pApp->Update())
				continue;
	
		}
	}
	return true;
}

UINT __stdcall CsharkcoinclientApp::ProcessMessage(LPVOID pParam) {
	CsharkcoinclientApp * pApp  = (CsharkcoinclientApp*)pParam ;
	
	while ( true)
	{
		CPostMsg Postmsg ;
		/// ��Ϣ������ ���ܹ��˳�
		if (!theApp.m_bIsProcessMsgRunSwitch && !pApp->m_msgQueue.pop(Postmsg))
		{
			return 1;
		}

		if(!pApp->m_msgQueue.pop(Postmsg)){
			Sleep(100); 
			continue;
		}
		CsharkcoinclientDlg *pAppDlg = (CsharkcoinclientDlg*)(pApp->m_pMainWnd);
		if ((pAppDlg == NULL || !theApp.m_bIsDlgCreateFinished) && Postmsg.GetUItype() != MSG_USER_STARTPROCESS_UI&&Postmsg.GetDatatype() !=WM_SYNC_TRANSACTION){
			pApp->m_msgQueue.push(Postmsg);
			Sleep(100); 
			continue;
		}
		switch (Postmsg.GetUItype() )
		{
		case MSG_USER_STARTPROCESS_UI:
			{
				LogPrint("PROCESSMSG", "MSG_USER_STARTPROCESS_UI ��:%d\n",Postmsg.GetDatatype());
				theApp.DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_STARTPROCESS_UI ,Postmsg.GetDatatype(),0);
				if (Postmsg.GetDatatype() == 4)
				{
					theApp.m_bIsStartMainDlg = true;
				}
				if(theApp.m_bIsSyncTx){
					theApp.m_bIsSyncTx = FALSE;
					theApp.m_bIsCommitDb =theApp.m_SqliteDeal.CommitDbTransaction();
					TRACE("Sync Tx commit transaction\n");
					LogPrint("INFO", "Sync Tx commit transaction\n");
				}
			}
			break;
		case MSG_USER_GET_UPDATABASE:
			{
			
				switch (Postmsg.GetDatatype())
				{
				case WM_UP_ADDRESS:
					{
						LogPrint("PROCESSMSG", "WM_UP_ADDRESS\n");
						//����Ǯ����ַ���ݿ�
						pApp->UpdateAddressData();
					}
					break;
				case WM_SYNC_TRANSACTION:
					{
						
						if(!theApp.m_bIsSyncTx) {
							theApp.m_bIsSyncTx = TRUE;
							theApp.m_SqliteDeal.BeginDBTransaction();
						}
						string txData = Postmsg.GetData();
						if ( "" != txData ) {
							pApp->SyncTransaction(txData) ;
						}
						LogPrint("PROCESSMSG", "WM_SYNC_TRANSACTION ����ͬ������:%s\n",txData.c_str());
					}
					break;
				case WM_REVTRANSACTION:
					{

						//������ʷ���׼�¼���ݿ�
						string pHash = Postmsg.GetData();
						if ( "" != pHash ) {
							string strCondition("");
							strCondition = strprintf(" hash = '%s' ", pHash.c_str());
							int nItem =  pApp->m_SqliteDeal.GetTableCountItem(_T("t_transaction") ,strCondition);
							if (nItem == 0)
							{
								pApp->InsertTransaction(pHash) ;
							}else{
								pApp->UpdateTransaction(pHash);
							}
						}
						LogPrint("PROCESSMSG", "WM_REVTRANSACTION ��ȡ��Ǯ���йصĽ���:%s\n",pHash.c_str());
					}
					break;
				case WM_APP_TRANSATION:
					{
						string txDetail = Postmsg.GetData();
						if ( "" != txDetail ) {
							pApp->UpdateAppRecord(txDetail);
							pApp->InsertAppTransaction(txDetail);
						}
						LogPrint("PROCESSMSG", "WM_APP_TRANSATION ��ȡ��Ǯ���йص�Ӧ�ý���:%s\n",txDetail.c_str());
					}
					break;
				case WM_UP_BlLOCKTIP:
					{
						LogPrint("PROCESSMSG", "WM_UP_BlLOCKTIP ����block���¸߶�\n");
						//��������blocktip���ݿ�
						string data = Postmsg.GetData();
						if (data == "")
						{
							break;
						}
						if ( pApp->m_SqliteDeal.ClearTableData(_T("t_chain_tip") ) ) {
							uistruct::BLOCKCHANGED_t block;
							if (block.JsonToStruct(data))
							{
								string strinsert;
								strinsert = strprintf("'%s','%d','%d','%d','%d'",block.hash, block.high, block.time, block.fuelrate, pApp->m_nLockStatus);
								if (!pApp->m_SqliteDeal.InsertTableItem(_T("t_chain_tip") ,strinsert ))
								{
									TRACE("insert into table t_chain_tip error");
								}
							}
						}
					}
					break;
				case WM_UP_UPDATABlLOCK:
					{
						uistruct::CHAIN_TIP_T chainblock;
						pApp->m_SqliteDeal.GetTipBlockHash(&chainblock);
						string strWhere =strprintf("block_hash='%s'",chainblock.blockhash);
						string updatastr = strprintf("lock=%d",pApp->m_nLockStatus);
						if (!pApp->m_SqliteDeal.UpdateTableItem(_T("t_chain_tip") ,updatastr,strWhere ))
						{
							TRACE("insert into table t_chain_tip error");
						}
					}
					break;
				case WM_UP_ADDRBOOK:
					{
						LogPrint("PROCESSMSG", "WM_UP_ADDRBOOK ���µ�ַ�����ݿ�\n");
						string josnaddr = Postmsg.GetData();
						uistruct::ADDRBOOK_t addr;
						if (addr.JsonToStruct(josnaddr))
						{
							string strCond;
							strCond = strprintf(" address='%s' ", addr.address.c_str());
							
							int nItem =  pApp->m_SqliteDeal.GetTableCountItem(_T("t_address_book"), strCond);
							if (0 == nItem)
							{
								pApp->InsertAddrBook(addr) ;
							}else{
								pApp->UpdateAddrBook(addr);
							}
						}
					}
					break;
				case WM_UP_DELETERBOOK:
					{
						LogPrint("PROCESSMSG", "WM_UP_DELETERBOOK ɾ����ַ�����ݿ�\n");
						string josnaddr = Postmsg.GetData();
						uistruct::ADDRBOOK_t addr;
						if (addr.JsonToStruct(josnaddr))
						{
							string strCond;
							strCond = strprintf(" address='%s' ", addr.address.c_str());
							pApp->m_SqliteDeal.DeleteTableItem(_T("t_address_book"),strCond);
						}
					}
					break;
				case WM_RELEASETX:
					{
						LogPrint("PROCESSMSG", "WM_RELEASETX �յ��������·ŵ�mempool\n");
						//������ʷ���׼�¼���ݿ�
						string pHash = Postmsg.GetData();
						if ( _T("") != pHash ) {
		
							string strCondition("");
							strCondition = strprintf(" hash = '%s' ", pHash.c_str());
							int nItem =  pApp->m_SqliteDeal.GetTableCountItem(_T("t_transaction") ,strCondition);
							if (nItem != 0)
							{
								string updataValue="confirm_height=0,confirmed_time=0,block_hash=''";
								LogPrint("INFO","WM_RELEASETX:%s",pHash.c_str());
								pApp->m_SqliteDeal.UpdateTableItem(_T("t_transaction") , strCondition , updataValue ) ;
								nItem =  pApp->m_SqliteDeal.GetTableCountItem(_T("t_app_transaction") ,strCondition);
								if (nItem != 0)
								{
									pApp->m_SqliteDeal.UpdateTableItem(_T("t_app_transaction") , strCondition , updataValue ) ;
								}
								theApp.m_SqliteDeal.UpdataAllTableData();   /// ����Ӧ�ñ��
							}
						}
					}
					break;
				case WM_REMOVETX:
					{
						string pHash = Postmsg.GetData();
						if ( _T("") != pHash ) {

							string strCondition(_T(""));
							strCondition = strprintf(" hash = '%s' ", pHash.c_str());
							int nItem =  pApp->m_SqliteDeal.GetTableCountItem(_T("t_transaction") ,strCondition);
							if (nItem != 0)
							{
								LogPrint("INFO","WM_REMOVETX:%s",pHash.c_str());
								pApp->m_SqliteDeal.DeleteTableItem(_T("t_transaction"),strCondition);
								///ɾ����app��صĽ���
								pApp->m_SqliteDeal.DeleteTableItem(_T("t_app_transaction"),strCondition);
								theApp.m_SqliteDeal.UpdataAllTableData();   /// ����Ӧ�ñ��
							}
						}
						/// ֪ͨ��ʾ���׽���
						CPostMsg Postmsg(MSG_USER_TRANSRECORD_UI,WM_REMOVETX);
						pApp->m_msgQueue.push(Postmsg);
						LogPrint("PROCESSMSG", "WM_REMOVETX ɾ������:%s\n",pHash.c_str());
					}
					break;
				default:
					break;
				}
			}
			break;
		case MSG_USER_UP_PROGRESS:
			{
				switch(Postmsg.GetDatatype())
				{
				case WM_LOCKSTATE:
					{
						LogPrint("PROCESSMSG", "MSG_USER_UP_PROGRESS WM_LOCKSTATE ������Ϣ\n");
						pApp->m_msgLockStatusQueue.push(Postmsg);
					}
					break;
				case WM_CONNECTNET:
					{
						LogPrint("PROCESSMSG", "MSG_USER_UP_PROGRESS WM_CONNECTNET ����������Ϣ\n");
						pApp->m_msgConnCountQueue.push(Postmsg);
					}
					break;
				default:
					{
						TRACE("change:%s\r\n","MSG_USER_UP_PROGRESS");
						
						pApp->m_msgProcessBarQueue.push(Postmsg);
						LogPrint("PROCESSMSG", "MSG_USER_UP_PROGRESS  ���½�������Ϣ:%s\n",Postmsg.GetData().c_str());
					}
					break;
				}	
			}
			break;
		case MSG_USER_INSERT_DATA:   //// ��������
			{
				LogPrint("PROCESSMSG", "MSG_USER_INSERT_DATA ����������Ϣ\n");
				uistruct::DATABASEINFO_t   pDatabase; // = (uistruct::DATABASEINFO_t *)Postmsg.GetStrPoint();
				string strTemp = Postmsg.GetData();
				pDatabase.JsonToStruct(strTemp.c_str());
				pApp->m_SqliteDeal.InsertTableItem(pDatabase.strTabName.c_str() ,pDatabase.strSource.c_str()) ;
				if ( !strcmp(pDatabase.strTabName.c_str() , _T("t_p2p_quiz")) ){
					Postmsg.SetType(MSG_USER_INSERT_DATA,WM_P2P_BET_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("t_dark_record")) )
				{
					Postmsg.SetType(MSG_USER_INSERT_DATA,WM_DARK_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("t_wallet_address")))
				{
					uistruct::LISTADDR_t addr; 
					addr.JsonToStruct(pDatabase.strcutjson.c_str());
					string Temp = addr.ToJson();
					pApp->SendUIMessage((int)WM_UP_NEWADDRESS,Temp.c_str());
					Postmsg.SetType(MSG_USER_INSERT_DATA,WM_UP_ADDRESS);
				}

			}
			break;
		case MSG_USER_UPDATA_DATA:   /// �������ݿ�
			{
					LogPrint("PROCESSMSG", "MSG_USER_UPDATA_DATA ����������Ϣ\n");
				uistruct::DATABASEINFO_t pDatabase;// = (uistruct::DATABASEINFO_t *)Postmsg.GetStrPoint();
				string strTemp = Postmsg.GetData();
				pDatabase.JsonToStruct(strTemp);

				if ( !pApp->m_SqliteDeal.UpdateTableItem(pDatabase.strTabName.c_str() , pDatabase.strSource.c_str() , pDatabase.strWhere.c_str() ) ){
					TRACE(_T("t_p2p_quiz���ݸ���ʧ��!") );
				}

				if ( !strcmp(pDatabase.strTabName.c_str() , _T("t_p2p_quiz")) ){
					Postmsg.SetType(MSG_USER_UPDATA_DATA,WM_P2P_BET_RECORD);
				}else if (!strcmp(pDatabase.strTabName.c_str() , _T("t_dark_record")) )
				{
					Postmsg.SetType(MSG_USER_UPDATA_DATA,WM_DARK_RECORD);
				}

			}
			break;
		default:
			break;
		}
		if (theApp.SendPostThread(Postmsg.GetDatatype()) == 0)
		{
			pApp->DispatchMsg( pApp->GetMtHthrdId() , Postmsg.GetUItype(), Postmsg.GetDatatype() , 0) ;
		}
		
	}
	return 1 ;
}


void CsharkcoinclientApp::SendUIMessage(int message, string data)
{
	CPostMsg Postmsg(MSG_USER_MAIN_UI, message);
	Postmsg.SetData(data);	
	m_msgUIRecDlgQueue.push(Postmsg);
	m_msgUISendDlgQueue.push(Postmsg);

	CPostMsg msg(MSG_USER_RECIVE_UI, message);
	m_msgQueue.push(msg);

	CPostMsg msg1(MSG_USER_SEND_UI, message);
	m_msgQueue.push(msg1);
}


int CsharkcoinclientApp::SendPostThread(DWORD msgtype)
{
	CsharkcoinclientDlg *pDlg = (CsharkcoinclientDlg*)(((CsharkcoinclientApp*)this)->m_pMainWnd) ;
	if (pDlg == NULL)
	{
		return 0 ;
	}

	switch (msgtype)
	{
	case WM_REVTRANSACTION:
		{
			GetMainDlgStruct();
			//DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MAIN_UI , 0,0);

		}
		break;
	case WM_UPWALLET:
		{
			DispatchMsg( theApp.GetMtHthrdId() , MSG_USER_MAIN_UI ,WM_UPWALLET,0);
		}
		break;
	default:
		return 0;
	}
	return 1 ;
}

void CsharkcoinclientApp::GetMainDlgStruct()
{
	uistruct::MINDLG_T maindlg;
	string strCommand,strShowData;
	strCommand = "0";
	double nmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_wallet_address") ,_T("money"), _T(" 1=1 "));
	if (nmoney < 0)
	{
		maindlg.money = "0.0";
	}else{
		string strmoney;
		strmoney = strprintf("%.3lf",nmoney);
		maindlg.money = strmoney;
	}

	string strCond;
	strCond = " confirm_height = 0 ";

	nmoney =  theApp.m_SqliteDeal.GetTableItemSum(_T("t_transaction") , _T("money") , strCond) ;

	if (nmoney <0)
	{
		maindlg.unconfirmmoney = _T("0.0");
	}else{
		CString strmoney;
		strmoney.Format(_T("%.3lf"),nmoney);
		maindlg.unconfirmmoney = strmoney;
	}

	int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction"), _T(" 1=1 "));

	strCommand = strprintf("%d",nItem);
	maindlg.itemcount = strCommand;

	uistruct::TRANSRECORDLIST pTransaction;
	strCond = " tx_type='COMMON_TX' order by confirmed_time desc limit 6";
	theApp.m_SqliteDeal.GetTransactionList(strCond, &pTransaction);


	int i = 1;
	if (pTransaction.size() != 0  ) {
		std::vector<uistruct::REVTRANSACTION_t>::const_iterator const_it;
		for (const_it = pTransaction.begin(); const_it != pTransaction.end()&&i<7 ; const_it++ ) {
			uistruct::REVTRANSACTION_t temp = *const_it;
			if(i == 1)
			maindlg.addr1 = temp.ToJson();
			if(i == 2)
				maindlg.addr2 = temp.ToJson();
			if(i == 3)
				maindlg.addr3 = temp.ToJson();
			if(i ==4)
				maindlg.addr4 = temp.ToJson();
			if(i == 5)
				maindlg.addr5 = temp.ToJson();
			if(i == 6)
				maindlg.addr6 = temp.ToJson();

			i++;
		}
	}

	CPostMsg Postmsg(MSG_USER_MAIN_UI,0);
	string msg =maindlg.ToJson();
	Postmsg.SetData(msg.c_str());	
	m_msgUIMainDlgQueue.push(Postmsg);

	CPostMsg msg1(MSG_USER_MAIN_UI,0);
	m_msgQueue.push(msg1);
}

void CsharkcoinclientApp::InsertAppTransaction(string txdetail)
{
	uistruct::REVTRANSACTION_t transcion;
	if (transcion.JsonToStruct(txdetail))
	{
		/// �������õ���ص�id
		map<string,string>::const_iterator item = m_mapAppId.find(transcion.desregid);
		if (item == m_mapAppId.end())
		{
			return;
		}
		string strSourceData;
		strSourceData = strprintf("'%s' , '%s' ,'%d' ,'%s' ,'%s' , '%.8f' ,'%d' ,'%s' ,'%s', '%.8f' ,'%s' ,'%d','%d','%s'" , transcion.txhash.c_str() ,\
			transcion.txtype.c_str() ,transcion.ver ,transcion.addr.c_str() ,transcion.regid,transcion.fees,transcion.height,\
			transcion.desaddr.c_str(), transcion.desregid.c_str(),transcion.money,transcion.Contract.c_str(),transcion.confirmedHeight,transcion.confirmedtime,transcion.blockhash.c_str()) ;
		m_SqliteDeal.InsertTableItem(_T("t_app_transaction") ,strSourceData ) ;
	}
}

void  CsharkcoinclientApp::ParseUIConfigFile(const string& strExeDir){
	string configpath = "";
	configpath += strprintf("%s",strExeDir);
	configpath+= strprintf("\\%s","sharkfundclient.conf");

	if (PathFileExistsA(configpath.c_str()))
	{
		CJsonConfigHelp::getInstance()->Init();
		CJsonConfigHelp::getInstance()->ReadJsonConfig(configpath);
		/*
		CScriptCfg scriptCfg;
		CJsonConfigHelp::getInstance()->GetScriptCfgData(scriptCfg);
		m_strFreeze5MonthId = scriptCfg.m_strFreeze5MonthAppId;
		m_strFreeze10MonthId= scriptCfg.m_strFreeze10MonthAppId;
		m_strFreeze20MonthId = scriptCfg.m_strFreeze20MonthAppId;
		*/
		

		//CJsonConfigHelp::getInstance()->GetClosConfig(m_reminder);
		//CJsonConfigHelp::getInstance()->GetPopTipsConfig(m_poptips);
		//CJsonConfigHelp::getInstance()->GetNewestScriptData(m_neststcriptid);
		//CJsonConfigHelp::getInstance()->GetP2pBetStep(m_p2pbetstep);
		//CJsonConfigHelp::getInstance()->GetRedPacketStep(m_redPackestep);
		//CJsonConfigHelp::getInstance()->GetListAppConfig(m_listapp);
		//CJsonConfigHelp::getInstance()->GetHelpUrl(helpurlen,helpurlcn);
		CNetParamCfg netParm;
		CJsonConfigHelp::getInstance()->GetClosConfig(m_reminder);
		CJsonConfigHelp::getInstance()->GetPopTipsConfig(m_poptips);
		CJsonConfigHelp::getInstance()->GetNetParamCfgData(netParm);
		CJsonConfigHelp::getInstance()->GetListAppConfig(m_listapp);
		CJsonConfigHelp::getInstance()->GetInsertAppId(m_mapAppId);
		CJsonConfigHelp::getInstance()->GetAppBalance(m_AppBalanceCfg);

		m_severIp = netParm.server_ip;
		m_uiPort = netParm.server_ui_port;
		m_rpcPort = netParm.rpc_port;
		m_rpcUser = netParm.rpc_user;
		m_rpcPassWord = netParm.rpc_password;
	}
}
