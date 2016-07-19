// ProgStatusBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sharkcoinclient.h"
#include "ProgStatusBar.h"
#include "afxdialogex.h"
#include "sharkcoinclientDlg.h"
#include  "BalloonTip.h"
#include <WinVer.h>
// CProgStatusBar �Ի���
#pragma comment(lib, "Version.lib ")
IMPLEMENT_DYNAMIC(CProgStatusBar, CDialogBar)

	CProgStatusBar::CProgStatusBar()
{
	m_pBmp = NULL ;
	m_bProgressType = FALSE;
	m_bIsShowProgress = FALSE;
	m_ProgressWnd = NULL ;
	m_nSigIndex = 0 ;
	m_nLockIndex = 0;
	m_bIsWalletUI = FALSE;
	m_strNetName = _T("");
	memset(m_rectSignal , 0 , sizeof(CRect));
	memset(m_rectLock , 0 , sizeof(CRect));
	m_progress.ShowPercent(FALSE);
	m_progress.ShowDefineText(TRUE);
	m_nConnectCount = 0;
}

CProgStatusBar::~CProgStatusBar()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
	if ( NULL != m_ProgressWnd ) {
		CGIFControl *gifcontrol= ((CGIFControl*)m_ProgressWnd);
		delete gifcontrol ;
		gifcontrol = NULL ;
	}
}

void CProgStatusBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_ST_NET_TB, m_strNeting);
	DDX_Control(pDX, IDC_ST_HEIGHT, m_strHeight);
	DDX_Control(pDX, IDC_ST_VERSION, m_strVersion);
	DDX_Control(pDX, IDC_ST_QQ, m_vlinkCtrlQQ);
	DDX_Control(pDX, IDC_ST_BLOCK, m_vlinkCtrlBlock);
}


BEGIN_MESSAGE_MAP(CProgStatusBar, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(MSG_USER_UP_PROGRESS , &CProgStatusBar::OnShowProgressCtrl  )
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CProgStatusBar ��Ϣ�������
void CProgStatusBar::SetBkBmpNid( UINT nBitmapIn ) 
{
	if( NULL != m_pBmp ) {
		::DeleteObject( m_pBmp ) ;
		m_pBmp = NULL ;
	}
	m_pBmp	= NULL ;
	HINSTANCE	hInstResource = NULL;	
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);
	if( NULL != hInstResource ) {
		m_pBmp = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);
	}
}
void CProgStatusBar::LoadGifing( BOOL bState )
{
	if( NULL != m_ProgressWnd ) {
		if( m_ProgressWnd->GetSafeHwnd() ) {
			if( TRUE == bState ) {
				if( TRUE == ((CGIFControl*)m_ProgressWnd)->Load((char*)theApp.m_strProgressGifFile.c_str()) ) {
					CRect rc ;
					GetClientRect( rc ) ;
					Invalidate() ;
					m_ProgressWnd->SetWindowPos( NULL , rc.Width()+880 , rc.Height()+12 , 0 , 0 , \
						SWP_SHOWWINDOW|SWP_NOSIZE ) ;
					((CGIFControl*)m_ProgressWnd)->Play();
				}
			}else{
				((CGIFControl*)m_ProgressWnd)->Stop() ;	
			}
		}
	}
}

BOOL CProgStatusBar::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect   rect; 
	GetClientRect(&rect); 

	if(m_pBmp   !=   NULL) { 
		BITMAP   bm; 
		CDC   dcMem; 
		::GetObject(m_pBmp,sizeof(BITMAP),   (LPVOID)&bm); 
		dcMem.CreateCompatibleDC(NULL); 
		HBITMAP     pOldBitmap   =(HBITMAP   )   dcMem.SelectObject(m_pBmp); 
		pDC-> StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 

		dcMem.SelectObject(pOldBitmap);
		dcMem.DeleteDC();
	} else  
		CWnd::OnEraseBkgnd(pDC); 

	return 1;
}


int CProgStatusBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid( IDB_BITMAP_STATUS_BJ) ;
	ModifyStyle(WS_BORDER,   0); 
	ModifyStyleEx(WS_EX_WINDOWEDGE,   0); 

	OnIniLockParam();         //�������ı���
	return 0;
}


void CProgStatusBar::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CRect rect ;
	int Width = 0;
	CWnd *pst = GetDlgItem( IDC_ST_NET_TB ) ;
	if ( NULL != pst ) {
		pst->GetWindowRect( rect ) ;
		pst->SetWindowPos( NULL ,8 , 12, rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		Width += rect.Width();
	}

	pst = GetDlgItem(IDC_ST_HEIGHT);
	if( NULL != pst)
	{
		pst->GetWindowRect(rect);
		pst->SetWindowPos( NULL ,Width + 20, 12, rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		Width += rect.Width();
	}
	/*
	pst = GetDlgItem(IDC_PROGRESS);
	if( NULL != pst)
	{
		pst->GetWindowRect(rect);
		pst->SetWindowPos( NULL ,Width , rect.top, rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
		Width += rect.Width();
	}*/

	int nVerWidth = 0;
	pst = GetDlgItem(IDC_ST_VERSION);
	if(NULL != pst)
	{
		pst->GetWindowRect(rect);
		nVerWidth = rect.Width();
		pst->SetWindowPos( NULL ,900 - 80 - nVerWidth - 25, 12, rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
	}

	int nQQWidth = 0;
	if(::IsWindow(m_vlinkCtrlQQ.m_hWnd))
	{
		m_vlinkCtrlQQ.GetWindowRect(rect);
		nQQWidth = rect.Width();
		m_vlinkCtrlQQ.SetWindowPos( NULL ,900 - 80 - nVerWidth - nQQWidth - 33, 12, rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
	}
	
	int nblockWidth = 0;
	if(::IsWindow(m_vlinkCtrlBlock.m_hWnd))
	{
		m_vlinkCtrlBlock.GetWindowRect(rect);
		nblockWidth = rect.Width();
		m_vlinkCtrlBlock.SetWindowPos( NULL ,900 - 80 - nVerWidth - nQQWidth - nblockWidth - 45 , 12, rect.Width(), rect.Height()  ,SWP_SHOWWINDOW ) ; 
	}

}


BOOL CProgStatusBar::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		UpdateData(0);

		HCURSOR cur = ::LoadCursor(NULL, IDC_HAND);
		m_vlinkCtrlQQ.SetWindowText("�ٷ�QQȺ:529196488");
		m_vlinkCtrlQQ.SetColors(RGB(125, 182, 255), RGB(125,182, 255));
		//m_vlinkCtrlQQ.SetFont(90 , _T("΢���ź�"));
		m_vlinkCtrlQQ.SetUnderline(FALSE);
		m_vlinkCtrlQQ.SetURL("http://www.sharkfund.net/");
		m_vlinkCtrlQQ.SetCursor(cur);

		m_vlinkCtrlBlock.SetWindowText("Block chain");
		m_vlinkCtrlBlock.SetColors(RGB(125,182, 255), RGB(125,182, 255));
		m_vlinkCtrlBlock.SetUnderline(FALSE);
		m_vlinkCtrlBlock.SetURL("http://www.sharkfund.net/");
		m_vlinkCtrlBlock.SetCursor(cur);

		m_strNeting.SetFont(90 , _T("΢���ź�"));				//������ʾ����ʹ�С
		m_strNeting.SetTextColor(RGB(125,182,255));			    //������ɫ
	
		if (theApp.m_nNetWorkType == MAIN)
		{
			m_strNetName = "��" ; 
		}else if (theApp.m_nNetWorkType == REGTEST)
		{
			m_strNetName = "����" ; 
		}else if (theApp.m_nNetWorkType == TESTNET)
		{
			m_strNetName = "����"; 
		}
		string strTemp ="";
		strTemp =strprintf("%s%s%s", "��ȡ", m_strNetName, "��������");
		m_strNeting.SetWindowText(strTemp.c_str()) ;
			

		m_strHeight.SetFont(90 , _T("΢���ź�"));				//������ʾ����ʹ�С
		m_strHeight.SetTextColor(RGB(125,182,255));			    //������ɫ
		m_strHeight.SetWindowText("�߶�:") ;
		m_strHeight.ShowWindow(SW_HIDE) ;
		
		m_strVersion.SetFont(90 , _T("΢���ź�"));				//������ʾ����ʹ�С
		m_strVersion.SetTextColor(RGB(125,182,255));	    //������ɫ

		string ver = strprintf("�汾��v%s RC", GetUIVersion());
		m_strVersion.SetWindowText(ver.c_str()) ;
		//m_strVersion.SetWindowText(_T("�汾:v1.0.2.0 RC")) ;

		if ( NULL == m_ProgressWnd ) {
			m_ProgressWnd = new CGIFControl ;
			m_ProgressWnd->Create(_T("") , WS_CHILD | SS_OWNERDRAW | WS_VISIBLE | SS_NOTIFY , \
				CRect(20,20,36,36) , this, 111 ) ;
		}
		
		m_SignalBmpArray[0].LoadBitmap(IDB_BITMAP_SIGNAL0);  
		m_SignalBmpArray[1].LoadBitmap(IDB_BITMAP_SIGNAL1);  
		m_SignalBmpArray[2].LoadBitmap(IDB_BITMAP_SIGNAL2);  
		m_SignalBmpArray[3].LoadBitmap(IDB_BITMAP_SIGNAL3);  

		m_LockBmpArray[0].LoadBitmap(IDB_BITMAP_LOCK); 
		m_LockBmpArray[1].LoadBitmap(IDB_BITMAP_UNLOCK); 

		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_UP_PROGRESS ) ;

		//m_progress.SendMessage(PBM_SETBKCOLOR, 0, RGB(66, 65, 63));//����ɫ
		//m_progress.SendMessage(PBM_SETBARCOLOR, 0, RGB(254, 153, 0));//ǰ��ɫ
		
		m_progress.SetTextColor(RGB(0, 0, 0));
		m_progress.SetBarBkColor(RGB(255, 255, 255));
		m_progress.SetBarColor(RGB(252, 215, 127));

		LoadGifing(TRUE);

		//CPostMsg postmsg(MSG_USER_UP_PROGRESS,0);
		//theApp.m_MsgQueue.pushFront(postmsg);
	}
	return bRes ;
}
int CProgStatusBar::ShowProgressCtrl(){
	CPostMsg postmsg;
	if (!theApp.m_msgProcessBarQueue.pop(postmsg))
	{
		return 1;
	}
	
	uistruct::BLOCKCHANGED_t pBlockchanged; 
	string strTemp = postmsg.GetData();
	pBlockchanged.JsonToStruct(strTemp.c_str());
	LogPrint("CProgStatusBar", "MSG_USER_UP_PROGRESS WM_CONNECTNET ���½�������Ϣ:%s\n",strTemp.c_str());
	if (pBlockchanged.tips <= 0)
	{
		return 1;
	}

	//// blocktip�߶�
	theApp.m_nBlockTipHight = pBlockchanged.tips ;
	if (!m_bProgressType)
	{
		string strTemp = "";
		strTemp = strprintf("%s%s",m_strNetName , "����ͬ����...");
		m_strNeting.SetWindowText(strTemp.c_str());
		m_strNeting.ShowWindow(SW_HIDE);
		m_strNeting.ShowWindow(SW_SHOW);

		m_progress.SetRange32( 0 , 100); 
		int  setpos =(int)((pBlockchanged.high*1.0/pBlockchanged.tips)*100) ;
		setpos = setpos>100?100:setpos;
		//���ý�������ֵ
		m_progress.SetPos(setpos);
		CString strText;
		strText.AppendFormat("%s ~%d", "ʣ��", pBlockchanged.tips-pBlockchanged.high);
		strText.AppendFormat(" %s","��û��ͬ��������");
		m_progress.SetDefinedStr(strText);
		m_bProgressType = TRUE;
		m_nSigIndex =pBlockchanged.connections>3?3:pBlockchanged.connections;

		m_nConnectCount = pBlockchanged.connections;

		if (pBlockchanged.tips==pBlockchanged.high)
		{
			//theApp.IsSyncAppTx = TRUE;             /// ͬ��app����
		}

		if ((pBlockchanged.tips-pBlockchanged.high)<10 && !m_bIsWalletUI)
		{
			TRACE("ok:%s\r\n","OnShowProgressCtrl");
			//// ����Ǯ��ͬ�����
			CPostMsg postblockmsg(MSG_USER_MAIN_UI,WM_UPWALLET);
			theApp.m_msgQueue.pushFront(postblockmsg); 
			LoadGifing(false);
			m_bIsWalletUI = true;
			theApp.m_bIsSyncBlock = true;
		}
		Invalidate(); 
		//InvalidateRect(m_bmpsig);
		//		return 1;
	}

	m_nSigIndex = pBlockchanged.connections>3?3:pBlockchanged.connections;
	int  setpos =(int) ((pBlockchanged.high*1.0/pBlockchanged.tips)*100) ;
	setpos = setpos>100?100:setpos;
	//���ý�������ֵ
	m_progress.SetPos(setpos);
	CString strText;
	strText.AppendFormat("%s ~%d", "ʣ��", pBlockchanged.tips-pBlockchanged.high);
	strText.AppendFormat(" %s","��û��ͬ��������");
	m_progress.SetDefinedStr(strText);
	m_progress.Invalidate();

	if (pBlockchanged.tips==pBlockchanged.high)
	{
		//theApp.IsSyncAppTx = TRUE;             /// ͬ��app����
	}
	if ((pBlockchanged.tips-pBlockchanged.high)<10&& !m_bIsWalletUI)
	{
		TRACE("ok:%s\r\n","OnShowProgressCtrl");
		//// ����Ǯ��ͬ�����
		CPostMsg postblockmsg(MSG_USER_MAIN_UI,WM_UPWALLET);
		theApp.m_msgQueue.pushFront(postblockmsg); 
		LoadGifing(false);
		m_bIsWalletUI = true;
		theApp.m_bIsSyncBlock = true;
	}
	if ( m_bIsWalletUI && !m_bIsShowProgress) {
		string strTemp = "";
		strTemp =strprintf("%s%s",m_strNetName , "����ͬ����...");
		m_strNeting.SetWindowText(strTemp.c_str()) ;
		m_strNeting.ShowWindow(SW_HIDE);
		m_strNeting.ShowWindow(SW_SHOW);

		m_progress.ShowWindow(SW_HIDE);
		if ( NULL != m_ProgressWnd ) {
			m_ProgressWnd->ShowWindow(SW_HIDE) ;
		}
		if (m_bIsShowProgress == false)
		{
			m_bIsShowProgress =true;
		}else{
			m_bIsShowProgress =false;
		}
	}

	if (m_bIsWalletUI && m_bIsShowProgress)
	{
		string strTemp;
		strTemp = strprintf("%s����",m_strNetName);
		m_strNeting.SetWindowText(strTemp.c_str());
		m_strNeting.ShowWindow(SW_HIDE);
		m_strNeting.ShowWindow(SW_SHOW);

		string strTips;
		strTips = strprintf("%s:%d" ,"��ǰ�߶�" ,pBlockchanged.tips );
		m_strHeight.SetWindowText(strTips.c_str()) ;
		m_strHeight.ShowWindow(SW_HIDE);
		m_strHeight.ShowWindow(SW_SHOW);

		CRect rc;
		m_strNeting.GetWindowRect(&rc);
		m_strNeting.MoveWindow(30 , 12, 58, rc.Height());
	}
	InvalidateRect(m_rectSignal);

	SetAppStepfee(pBlockchanged.fuelrate);
	return 1;
}
void CProgStatusBar::ShowLockCtrl()
{
	CPostMsg postmsg;
	if (!theApp.m_msgLockStatusQueue.pop(postmsg))
	{
		return ;
	}
	string strTemp = postmsg.GetStrType();
	if (!strcmp(strTemp.c_str(),"Lock"))
	{
		theApp.m_bIsHaveLocked = TRUE;
		m_nLockIndex = 0;
	}else if(!strcmp(strTemp.c_str(),"UnLock")){
		theApp.m_bIsHaveLocked = TRUE;
		m_nLockIndex = 1;
	}
	InvalidateRect(m_rectLock);
}

void CProgStatusBar::ShowNetCount()
{
	CPostMsg postmsg;
	if (!theApp.m_msgConnCountQueue.pop(postmsg))
	{
		return ;
	}
	string strTemp = postmsg.GetStrType();
	int pos = strTemp.find("connections") ;
	if (pos >=0)
	{
		pos = strTemp.find("=") ;
		if (pos >=0)
		{
			strTemp = strTemp.substr(pos+1,strTemp.length());
			strTemp =  UiFun::trimleft(strTemp);
			strTemp =  UiFun::trimright(strTemp);
			int netCount = atoi(strTemp.c_str());
			m_nSigIndex = netCount>3?3:netCount;
			InvalidateRect(m_rectSignal);

			m_nConnectCount = netCount;
			if (netCount == 0 )
			{
				string strTemp ="";
				strTemp =strprintf("%s%s%s","��ȡ",m_strNetName, "��������");
				m_strNeting.SetWindowText(strTemp.c_str()) ;
				Invalidate(); 
			}
		}	
	}
}
LRESULT CProgStatusBar::OnShowProgressCtrl( WPARAM wParam, LPARAM lParam ) 
{
//	TRACE("OnShowProgressCtrl:%s\r\n","OnShowProgressCtrl");
	if (wParam == WM_LOCKSTATE)
	{
		ShowLockCtrl();
	}else if (wParam == WM_CONNECTNET){
		ShowNetCount();
	}else{
		ShowProgressCtrl();
	}
	return 1;
}
//Invalidate(); 
void CProgStatusBar::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogBar::OnPaint()
	CDC memDC;  
	memDC.CreateCompatibleDC(&dc);  
	CRect rc;  
	GetClientRect(&rc);  

	BITMAP bm1;
	m_SignalBmpArray[m_nSigIndex].GetObject(sizeof(BITMAP),&bm1);

	int width = bm1.bmWidth; //ͼƬ�Ŀ�ȣ��߼���λ���� 
	int height=	bm1.bmHeight; ////ͼƬ�ĸ߶ȣ��߼���λ����

	HBITMAP hOldbmp = (HBITMAP)memDC.SelectObject(m_SignalBmpArray[m_nSigIndex]); 
	dc.BitBlt(900-58, 7, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);  
	CRect rc1(900-58, 7, rc.Width(), rc.Height());
	m_rectSignal = rc1;
	memDC.SelectObject(hOldbmp);  
//	memDC.DeleteDC();  
	if (theApp.m_bIsHaveLocked)
	{
		HBITMAP hlockbmp = (HBITMAP)memDC.SelectObject(m_LockBmpArray[m_nLockIndex]); 
		dc.BitBlt(900-50-30, 8, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);  
		CRect rc2(900-50-30, 8, rc.Width(), rc.Height());
		m_rectLock = rc2;
		memDC.SelectObject(hlockbmp);  
	}
	memDC.DeleteDC();  
}
void CProgStatusBar::OnIniLockParam()
{
	string strCommand;
	strCommand = strprintf("%s",_T("islocked"));
	string strShowData ="";
	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("OnIniLockParam rpccmd islocked error");
		return;
	}

	int state = root["islock"].asInt();
	if (state == 0)     /// û����
	{
		theApp.m_bIsHaveLocked = FALSE;
		theApp.m_bIsWalletLocked =FALSE;
	}else if(state == 1){   /// ����
		theApp.m_bIsWalletLocked =FALSE;
		m_nLockIndex = 1;
		theApp.m_bIsHaveLocked = TRUE;
	}else if (state == 2)    /// ����״̬
	{
		theApp.m_bIsWalletLocked =TRUE;
		m_nLockIndex = 0;
		theApp.m_bIsHaveLocked = TRUE;
	}

	theApp.m_nLockStatus = state; 
	//// ����block
	CPostMsg postblockmsg(MSG_USER_GET_UPDATABASE,WM_UP_UPDATABlLOCK);
	theApp.m_msgQueue.push(postblockmsg);  
}

void CProgStatusBar::SetAppStepfee(int fuelrate)
{
	if (fuelrate == 0)
	{
		return;
	}

}

void CProgStatusBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect temp = m_rectSignal;
	//ClientToScreen(&temp);
	CsharkcoinclientDlg *pDlg = (CsharkcoinclientDlg*)(theApp.m_pMainWnd);
	if (temp.PtInRect(point))
	{
		RECT ret;
		GetWindowRect(&ret);


		LOGFONT lf;
		::ZeroMemory (&lf, sizeof (lf));
		lf.lfHeight = -12;
		lf.lfWeight = FW_BOLD;
		lf.lfUnderline = FALSE;
		strcpy((char*)lf.lfFaceName, "΢���ź�");

		string strShow = strprintf("%s:%d","��ǰ����������", m_nConnectCount);
		if (IsWindowVisible()&& pDlg->m_pBalloonTip->nBalloonInstances !=1)
		{
			pDlg->m_pBalloonTip=CBalloonTip::Show(
				CPoint(ret.right -50, ret.bottom),         // Point on the screen where the tip will be shown
				CSize(85, 80),          // Size of the total rectangle encompassing the balloon 
				_T(strShow.c_str()), // Message to be shown in the balloon
				lf,                               // LOGFONT structure for font properties 
				30,                 // Time in seconds to show the balloon
				FALSE ,             // TRUE  == Balloon is up(Balloon Tip is down) 
				 FALSE // ==  Balloon is down(Balloon Tip is up)
				);
		 }
	}else if (pDlg->m_pBalloonTip != NULL && pDlg->m_pBalloonTip->nBalloonInstances ==1){
		int pos = pDlg->m_pBalloonTip->m_strMessage.Find("��������");
		if (pos >=0)
		{
			CBalloonTip::Hide(pDlg->m_pBalloonTip);
		}
	}

	CDialogBar::OnMouseMove(nFlags, point);
}
string CProgStatusBar::GetUIVersion()
{
	string strRet = "";
	char szAppFullPath[_MAX_PATH] = {0};

	GetModuleFileName(NULL,szAppFullPath,MAX_PATH);//�õ�����ģ�����ƣ�ȫ·��

	//��ȡ��ǰ�ļ��İ汾��Ϣ
	DWORD dwLen = GetFileVersionInfoSize(szAppFullPath,NULL); 
	char *pszAppVersion = new char[dwLen+1];
	if(pszAppVersion)
	{
		memset(pszAppVersion,0,sizeof(char)*(dwLen+1));
		GetFileVersionInfo(szAppFullPath,NULL,dwLen,pszAppVersion);
		CString strVersion;
		UINT nLen(0);
		VS_FIXEDFILEINFO *pFileInfo(NULL);
		VerQueryValue(pszAppVersion,"\\",(LPVOID*)&pFileInfo,&nLen);
		if(pFileInfo)
		{
			//��ȡ�汾��
			strRet = strprintf("%d.%d.%d.%d",HIWORD(pFileInfo->dwFileVersionMS),
				LOWORD(pFileInfo->dwFileVersionMS),
				HIWORD(pFileInfo->dwFileVersionLS),
				LOWORD(pFileInfo->dwFileVersionLS));
		}
	}
	delete pszAppVersion;
	return strRet;
}