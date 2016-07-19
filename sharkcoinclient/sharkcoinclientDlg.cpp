
// sharkcoinclientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sharkcoinclient.h"
#include "sharkcoinclientDlg.h"
#include "afxdialogex.h"
#include "Reminderdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsharkcoinclientDlg �Ի���

CsharkcoinclientDlg::CsharkcoinclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsharkcoinclientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pTitleBar = NULL ;
	m_pStatusBar = NULL ;
	m_pMainDlg = NULL ;
	m_pSendDlg = NULL ;
	m_pReceDlg = NULL ;
	m_pDealDlg = NULL ;
	m_pIpoDlg = NULL ;
	m_pSetDlg = NULL ;
	m_pRPCDlg = NULL;
	m_pOutGifDlg = NULL;

	p_CurSelDlg = NULL  ;
	dlgType = 0;
	m_dlgMap.clear();
	m_pBalloonTip = NULL;
}

void CsharkcoinclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsharkcoinclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_MAIN, &CsharkcoinclientDlg::OnBnClickedButtonMain)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CsharkcoinclientDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_RECE, &CsharkcoinclientDlg::OnBnClickedButtonRece)
	ON_BN_CLICKED(IDC_BUTTON_DEAL, &CsharkcoinclientDlg::OnBnClickedButtonDeal)
	ON_BN_CLICKED(IDC_BUTTON_APP, &CsharkcoinclientDlg::OnBnClickedButtonIpo)
	ON_BN_CLICKED(IDC_BUTTON_SET, &CsharkcoinclientDlg::OnBnClickedButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CsharkcoinclientDlg::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_MIN, &CsharkcoinclientDlg::OnBnClickedButtonMin)
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_MESSAGE(WM_POPUPBAR,OnPopupBar)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// ChuobiDlg ��Ϣ�������

BOOL CsharkcoinclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//if ( NULL != theApp.m_pSocket ) {
	//	theApp.m_pSocket->OnRecv( BlockDataProc , (WPARAM)this);
	//}
	if ( ((CsharkcoinclientApp*)AfxGetApp())->pSplashThread != NULL)
	{ 
		((CsharkcoinclientApp*)AfxGetApp())->pSplashThread->PostThreadMessage(WM_QUIT,NULL,NULL);
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CsharkcoinclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CsharkcoinclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CsharkcoinclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CsharkcoinclientDlg::GetFileName(CString &fileName,CString strexe )
{
	int spcace = fileName.Find(" ");
	if (spcace >=0)
	{
		UiFun::MessageBoxEx(_T("·������Ϊ��") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return false;
	}
	int pos = fileName.Find(".",0);
	if (pos <0)
	{
		fileName.AppendFormat(strexe);
	}
	pos = fileName.Find(".",0);
	fileName = fileName.Left(pos);
	fileName.AppendFormat(strexe);
	if(PathFileExistsA(fileName)){
		CString strDisplay;
		strDisplay=_T("���ļ��Ѿ������Ƿ�Ҫ����") ;
		if (IDOK == UiFun::MessageBoxEx(strDisplay , _T("��ʾ") , MFB_OKCANCEL|MFB_TIP ) )
		{
			return TRUE;
		}
	}

	return TRUE;
}

void CsharkcoinclientDlg::ShowDialog(UINT dlgid) 
{
	CRect rc , rt;
	GetClientRect(&rc);
	std::map< UINT , CDialogBar * >::iterator it ;
	for( it = m_dlgMap.begin() ; it != m_dlgMap.end() ; it++ ) {
		if ( NULL != it->second ) {
			if ( dlgid == it->first ) {
				(it->second)->SetWindowPos(NULL , rc.left , 100  , rc.Width() ,rc.Height() - 100 - 40 , SWP_SHOWWINDOW) ;
				(it->second)->ShowWindow( SW_SHOW ) ;
			}else{
				(it->second)->ShowWindow( SW_HIDE ) ;
			}
		}
	}
	p_CurSelDlg = m_dlgMap[dlgid] ;  //��ǰѡ��Ի���ָ��
	dlgType = dlgid;

	//CPostMsg postkmsg(MSG_USER_UPDATA_UI,0);
	//theApp.m_msgQueue.push(postkmsg); 
}

void CsharkcoinclientDlg::InitialRpcCmd()
{
	CSoyPayHelp::getInstance()->InitialRpcCmd(theApp.m_severIp,theApp.m_sendPreHeadstr,theApp.m_sendEndHeadstr,theApp.m_rpcPort);
}

//����Ǯ����ַ
void CsharkcoinclientDlg::LoadWalletAddress() 
{
	string strCommand;
	strCommand = strprintf("%s","listaddr");
	string strShowData ="";

	Json::Value root; 
	if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
	{
		TRACE("SyncAddrInfo rpccmd listaddr error");
		return;
	}

	map<string,uistruct::LISTADDR_t> pListInfo;
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 "), (&pListInfo));

	/// ���ݿ���û�еĵ�ַҪ����
	map<string,int> SListInfo;

	for(unsigned int i = 0; i < root.size(); ++i){
		//address
		uistruct::LISTADDR_t listaddr;
		string address = root[i]["addr"].asString();	
		SListInfo[address] = i;

		//address
		listaddr.address = root[i]["addr"].asString(); 
		//RegID

		listaddr.RegID = root[i]["regid"].asString() ;
		//���
		double fmoney = 0.0 ;  
		fmoney = root[i]["balance"].asDouble(); 
		listaddr.fMoney = fmoney ;
		//�Ƿ�֧�����ڿ�
		listaddr.nColdDig = root[i]["haveminerkey"].asBool() ;

		//�Ƿ�ע��GetLength();
		if ( 3 <= listaddr.RegID.length() ) {
			listaddr.bSign    = 1 ;
		}else{
			listaddr.bSign   = 0 ;
		}

		string strCond;
		strCond=strprintf(" address = '%s' ", listaddr.address.c_str());
		uistruct::LISTADDR_t addrsql;
		int item = theApp.m_SqliteDeal.GetWalletAddressItem(strCond, &addrsql) ;

		if (addrsql.address.length() == 0 )
		{
			string strData;
			strData= strprintf("'%s' , '%s' , '%.8f' , '%d' ,'%d','%s'" , listaddr.address.c_str() ,listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign,listaddr.Label.c_str() ) ;
			if (!theApp.m_SqliteDeal.InsertTableItem(_T("t_wallet_address") ,strData ))
			{
				TRACE("INSERT t_wallet_address data failed!\n");
			}
		}else{
			if (listaddr.fMoney != addrsql.fMoney || listaddr.bSign != addrsql.bSign)
			{
				string strSourceData,strWhere;
				strSourceData =strprintf("reg_id = '%s', money = %.8f ,cold_dig =%d, sign =%d" ,listaddr.RegID.c_str() ,listaddr.fMoney ,listaddr.nColdDig ,listaddr.bSign ) ;
				strWhere=strprintf("address = '%s'" , listaddr.address.c_str()  ) ;
				if ( !theApp.m_SqliteDeal.UpdateTableItem(_T("t_wallet_address") , strSourceData , strWhere ) ){
					TRACE("UPDATE t_wallet_address data failed!\n");
				}
			}
		}
	}

	////// �޳����ݿ���Ǯ��û�еĵ�ַ
	map<string,uistruct::LISTADDR_t>::const_iterator it;
	for (it= pListInfo.begin();it != pListInfo.end();it++)
	{
		if (SListInfo.count(it->first) <= 0)
		{
			string strCond;
			strCond = strprintf(" address='%s' ", it->first.c_str());
			int item = theApp.m_SqliteDeal.DeleteTableItem(_T("t_wallet_address"), strCond);
		}
	}
}

int CsharkcoinclientDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	InitialRpcCmd();
	LoadWalletAddress();
	
	//SetWindowPos(NULL, 0, 0, 800, 716, 0);
	SetWindowPos(NULL, 0, 0, 900, 600, 0);
	CenterWindow();

	CRect rect ;
	GetClientRect( rect ) ;

	if ( NULL == m_pRPCDlg ) {
		m_pRPCDlg = new CRPCDlg;
		m_pRPCDlg->Create(CRPCDlg::IDD,this);
		m_pRPCDlg->ShowWindow(SW_HIDE);
	}

	//TOP
	if( NULL == m_pTitleBar ){
		m_pTitleBar = new CIndTitleBar ;
		m_pTitleBar->Create(this, IDD_DIALOGBAR_TOOL,CBRS_ALIGN_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,1) ;
		m_pTitleBar->SetWindowPos(NULL , 0 , 0  , rect.Width() ,100 , SWP_SHOWWINDOW) ;
		m_pTitleBar->ShowWindow(SW_SHOW) ;
	}
	//״̬��
	if( NULL == m_pStatusBar ){
		m_pStatusBar = new CProgStatusBar ;
		m_pStatusBar->Create(this, IDD_DIALOG_STATUS,CBRS_ALIGN_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,2) ;
		m_pStatusBar->SetWindowPos(NULL , 0 , rect.Height() - 40  , rect.Width() ,41 , SWP_SHOWWINDOW) ;
		m_pStatusBar->ShowWindow(SW_SHOW) ;
	}
	//��ҳ
	if( NULL == m_pMainDlg ){
		m_pMainDlg = new CMainDlg ;
		m_pMainDlg->Create(this, CMainDlg::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,3) ;
		m_pMainDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pSendDlg ){
		m_pSendDlg = new CSend ;
		m_pSendDlg->Create(this, CSend::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,4) ;
		m_pSendDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pReceDlg ){
		m_pReceDlg = new CRece ;
		m_pReceDlg->Create(this, CRece::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,5) ;
		m_pReceDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pDealDlg ){
		m_pDealDlg = new CDeal ;
		m_pDealDlg->Create(this, CDeal::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,6) ;
		m_pDealDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pIpoDlg ){
		m_pIpoDlg = new CIpo ;
		m_pIpoDlg->Create(this, CIpo::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,7) ;
		m_pIpoDlg->ShowWindow(SW_HIDE) ;
	}
	if( NULL == m_pSetDlg ){
		m_pSetDlg = new CSet ;
		m_pSetDlg->Create(this, CSet::IDD, CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE,8) ;
		m_pSetDlg->ShowWindow(SW_HIDE) ;
	}

	if( NULL == m_pOutGifDlg ){
		m_pOutGifDlg = new COutGifDlg ;
		m_pOutGifDlg->Create(COutGifDlg::IDD , this) ;
		m_pOutGifDlg->ShowWindow(SW_HIDE) ;
	}
	//���뵽map
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CMainDlg::IDD , m_pMainDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CSend::IDD , m_pSendDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CRece::IDD , m_pReceDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CDeal::IDD , m_pDealDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CIpo::IDD , m_pIpoDlg)) ; 
	m_dlgMap.insert( std::map<UINT , CDialogBar *>::value_type( CSet::IDD , m_pSetDlg)) ; 
	//��ʾ��ҳ
	ShowDialog(CMainDlg::IDD) ;
	theApp.m_bIsDlgCreateFinished = TRUE;

	menu.CreatePopupMenu();
	menu.AppendMenu(MF_STRING,IDC_BUTTON_CLOSE,"�ر�"); 



	return 0;
}
void CsharkcoinclientDlg::OnBnClickedButtonMain()
{
	ShowDialog(CMainDlg::IDD) ;
	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_MOBILE_STATEUS , CMainDlg::IDD , 0 ) ;
}
void CsharkcoinclientDlg::OnBnClickedButtonSend()
{
	ShowDialog(CSend::IDD) ;
	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_MOBILE_STATEUS , CSend::IDD , 0 ) ;
}
void CsharkcoinclientDlg::OnBnClickedButtonRece()
{
	ShowDialog(CRece::IDD) ;
	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_MOBILE_STATEUS , CRece::IDD , 0 ) ;
}
void CsharkcoinclientDlg::OnBnClickedButtonDeal()
{
	ShowDialog(CDeal::IDD) ;
	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_MOBILE_STATEUS , CDeal::IDD , 0 ) ;
}
void CsharkcoinclientDlg::OnBnClickedButtonIpo()
{
	ShowDialog(CIpo::IDD) ;
	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_MOBILE_STATEUS , CIpo::IDD , 0 ) ;
}
void CsharkcoinclientDlg::OnBnClickedButtonSet()
{
	ShowDialog(CSet::IDD) ;
	::PostThreadMessage( theApp.GetMtHthrdId() , MSG_USER_MOBILE_STATEUS , CSet::IDD , 0 ) ;
}
void CsharkcoinclientDlg::OnBnClickedButtonClose()
{
	/*
	EndWaitCursor();
	PostMessage( WM_CLOSE ); 	
	*/
	ClosWallet();
}

void  CsharkcoinclientDlg::ClosWallet()
{
	if (theApp.m_reminder == 0 || theApp.m_reminder == 2)
	{
		CReminderdlg remindlg(this,theApp.m_reminder);
		remindlg.DoModal();
	}else if (theApp.m_reminder == 1)  ///��С��
	{
		ToTray();                              /// ��С��
	}else if (theApp.m_reminder == 3){        /// �رճ���
		
		OnCloseWriteAppFee();
		BeginWaitCursor();
		if ( NULL != m_pOutGifDlg ) {
			CRect rc;
			GetWindowRect(&rc);	
			m_pOutGifDlg->LoadGifing(TRUE);
			m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
			m_pOutGifDlg->ShowWindow(SW_SHOW) ;
		}
		
		SetTimer( 0x10 , 2000 , NULL ) ; 
		
	}

}

void CsharkcoinclientDlg::ToTray() 
{ 

	NOTIFYICONDATA nid; 
	memset(&nid,0,sizeof(NOTIFYICONDATA));
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this->m_hWnd; 
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE ; //NIF_MESSAGE|
	nid.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ���� 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	memset(nid.szTip,0,128);
	strcpy_s(nid.szTip,"sharkcoin"); //��Ϣ��ʾ�� 
	Shell_NotifyIcon(NIM_ADD,&nid); //�����������ͼ�� 
	ShowWindow(SW_HIDE); //���������� 
} 

void CsharkcoinclientDlg::OnBnClickedButtonMin() 
{
	ToTray();
}

void CsharkcoinclientDlg::BlockDataProc(char *pRecBuffer, int nRecLen ,WPARAM wParam)
{
	for(int i=0; i< nRecLen; ++i) {
		TRACE("%02X",pRecBuffer[i]);
	}
}

void CsharkcoinclientDlg::OnCloseWriteAppFee()
{/*
	if (PathFileExistsA(theApp.m_strInsPath.c_str()))
	{
		string configpath = "";
		configpath = strprintf("%s",theApp.m_strInsPath);
		configpath+= strprintf("\\%s","sharkcoinsclient.conf");
		string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile, root)) 
			return;

		int pos =strFile.find("p2pbet");
		if (pos>=0)
		{
			Json::Value setbet = root["p2pbet"];
			ASSERT(!setbet.isNull());
			setbet["SendBetFee"]= theApp.m_P2PBetCfg.SendBetFee;
			setbet["AcceptBetnFee"]= theApp.m_P2PBetCfg.AcceptBetnFee;
			setbet["OpenBetnFee"]= theApp.m_P2PBetCfg.OpenBetnFee;
			setbet["GetAppAmountnFee"]= theApp.m_P2PBetCfg.GetAppAmountnFee;
			setbet["GetRechangeFee"]= theApp.m_P2PBetCfg.GetRechangeFee;
			root["p2pbet"]=setbet;
		}else{
			Json::Value setbet ;
			setbet["SendBetFee"]= theApp.m_P2PBetCfg.SendBetFee;
			setbet["AcceptBetnFee"]= theApp.m_P2PBetCfg.AcceptBetnFee;
			setbet["OpenBetnFee"]= theApp.m_P2PBetCfg.OpenBetnFee;
			setbet["GetAppAmountnFee"]= theApp.m_P2PBetCfg.GetAppAmountnFee;
			setbet["GetRechangeFee"]= theApp.m_P2PBetCfg.GetRechangeFee;
			root["p2pbet"]=setbet;
		}

		CStdioFile  File;
		string strpath = theApp.m_strInsPath;
		strpath+="\\sharkcoinsclient.conf";
		if (File.Open((LPCTSTR)(LPSTR)strpath.c_str(),CFile::modeWrite | CFile::modeCreate))
		{
			string strfile = root.toStyledString();
			File.WriteString(strfile.c_str());
			File.Close();
		} 

	}
	*/
}

LRESULT CsharkcoinclientDlg::OnPopupBar(WPARAM wParam,LPARAM lParam) 
{
	///�����˲���ʾ��Ϣ
	if (!theApp.m_poptips)
	{
		return 0;
	}
	char* message = (char*)(lParam);
	string strmessage =strprintf("%s",message);

	if (m_pBalloonTip!= NULL && CBalloonTip::nBalloonInstances != 0)
	{
		CPostMsg Postmsg ;
		Postmsg.SetData(strmessage);
		m_barpoomesg.push(Postmsg);
		return 0;
	}


	RECT ret;
	GetWindowRect(&ret);


	LOGFONT lf;
	::ZeroMemory (&lf, sizeof (lf));
	lf.lfHeight = -12;
	lf.lfWeight = FW_BOLD;
	lf.lfUnderline = FALSE;
	strcpy((char*)lf.lfFaceName, "΢���ź�");

	//// �Ի���û�б����ز���Ҫ����ǰ��
	if (IsWindowVisible() && this->IsTopParentActive())
	{
		m_pBalloonTip =CBalloonTip::Show(
			CPoint(ret.right -90, ret.bottom-10),         // Point on the screen where the tip will be shown
			CSize(270, 150),          // Size of the total rectangle encompassing the balloon 
			_T(message), // Message to be shown in the balloon
			lf,                               // LOGFONT structure for font properties 
			15,                 // Time in seconds to show the balloon
			TRUE              // TRUE  == Balloon is up(Balloon Tip is down) 
			// FALSE ==  Balloon is down(Balloon Tip is up)
			);
	}else{
		int x=GetSystemMetrics(SM_CXSCREEN); //�õ�x����
		int y=GetSystemMetrics(SM_CYSCREEN);//�õ�y����
		m_pBalloonTip =CBalloonTip::Show(
			CPoint(x-90, y-10),         // Point on the screen where the tip will be shown
			CSize(270, 150),          // Size of the total rectangle encompassing the balloon 
			_T(message), // Message to be shown in the balloon
			lf,                               // LOGFONT structure for font properties 
			15,                 // Time in seconds to show the balloon
			TRUE              // TRUE  == Balloon is up(Balloon Tip is down) 
			// FALSE ==  Balloon is down(Balloon Tip is up)
			);
	}

	return 0;
}

void  CsharkcoinclientDlg::ClosWalletWind()
{
	OnCloseWriteAppFee();
	BeginWaitCursor();
	
	if ( NULL != m_pOutGifDlg ) {
		CRect rc;
		GetWindowRect(&rc);	
		m_pOutGifDlg->LoadGifing(TRUE);
		m_pOutGifDlg->SetWindowPos(NULL , (rc.left + rc.right)/2 - 300/2 , (rc.top + rc.bottom)/2 - 100/2  , 300 ,100 , SWP_SHOWWINDOW);
		m_pOutGifDlg->ShowWindow(SW_SHOW) ;
	}
	
	SetTimer( 0x10 , 2000 , NULL ) ; 
	
}

//wParam���յ���ͼ���ID����lParam���յ���������Ϊ 
LRESULT CsharkcoinclientDlg::OnShowTask(WPARAM wParam,LPARAM lParam) 
{ 
	if(wParam!=IDR_MAINFRAME) 
		return 1; 
	switch(lParam) 
	{ 
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա� 
		{ 
			CPoint pt;
			::GetCursorPos(&pt);//�õ����λ�� 

			menu.TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
		} break; 
	case WM_LBUTTONDBLCLK: //˫������Ĵ��� 
		{ 
			this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
			//DeleteTray(); 
		} break; 
	case  WM_LBUTTONDOWN:
		{
			if (IsWindowVisible())
			{
				this->ShowWindow(SW_HIDE);//�򵥵���ʾ���������¶�
			}else{
				SetWindowPos(&this->wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);  
				this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶� 
				SetWindowPos(&this->wndNoTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

				if(m_pBalloonTip != NULL&& m_pBalloonTip->nBalloonInstances==1)
				{
					CBalloonTip::Hide(m_pBalloonTip);
				}
			}
	case WM_MOUSEFIRST:
		{
			int b =5;
		}
		}
	default: break; 
	} 
	return 0; 
} 


void CsharkcoinclientDlg::DestroyDlg()
{
	if ( NULL != m_pTitleBar ) 
	{
		delete m_pTitleBar ;
		m_pTitleBar = NULL ;
	}
	if ( NULL != m_pStatusBar ) 
	{
		delete m_pStatusBar ;
		m_pStatusBar = NULL ;
	}
	if ( NULL != m_pMainDlg ) 
	{
		delete m_pMainDlg ;
		m_pMainDlg = NULL ;
	}
	if ( NULL != m_pSendDlg ) 
	{
		delete m_pSendDlg ;
		m_pSendDlg = NULL ;
	}

	if( NULL != m_pReceDlg)
	{
		delete m_pReceDlg;
		m_pReceDlg = NULL;
	}

	if( NULL != m_pDealDlg)
	{
		delete m_pDealDlg;
		m_pDealDlg = NULL;
	}

	if( NULL != m_pIpoDlg)
	{
		delete m_pIpoDlg;
		m_pIpoDlg = NULL;
	}

	if ( NULL != m_pRPCDlg ) 
	{
		delete m_pRPCDlg ;
		m_pRPCDlg = NULL ;
	}

	if(NULL != m_pSetDlg)
	{
		delete m_pSetDlg;
		m_pSetDlg = NULL;
	}
}

void CsharkcoinclientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if( 0x10 == nIDEvent ) {
		KillTimer(0x10);
		StopSever();

		DestroyDlg();
		LogPrint("INFO","OnTimer start \n");
		if ( NULL != m_pOutGifDlg ) {
			m_pOutGifDlg->ShowWindow(SW_HIDE) ;
			m_pOutGifDlg->LoadGifing(FALSE);
			delete m_pOutGifDlg ;
			m_pOutGifDlg = NULL ;
		}
		theApp.m_bIsCloseAppFlag = TRUE ;
		CloseThread();
		CloseApp();
	}else if (0x11  == nIDEvent)
	{
		CPostMsg Postmsg ;
		if (m_pBalloonTip!= NULL && CBalloonTip::nBalloonInstances == 0 && m_barpoomesg.pop(Postmsg))
		{
			string message = Postmsg.GetData();
			::SendMessage(theApp.m_pMainWnd->m_hWnd,WM_POPUPBAR,0,(LPARAM)message.c_str());	
		}
	}else if (0x12  == nIDEvent) /// ��������������Ƿ�������û�����
	{
		NOTIFYICONDATA nid; 
		memset(&nid,0,sizeof(NOTIFYICONDATA));
		nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
		nid.hWnd=this->m_hWnd; 
		nid.uID=IDR_MAINFRAME; 
		nid.uFlags=NIF_ICON|NIF_TIP|NIF_MESSAGE ; //NIF_MESSAGE|
		nid.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ���� 
		nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
		memset(nid.szTip,0,128);
		strcpy_s(nid.szTip,"sharkcoin"); //��Ϣ��ʾ�� 
		if ( !Shell_NotifyIcon( NIM_MODIFY, &nid ) )  
			Shell_NotifyIcon( NIM_ADD, &nid );  

	}
	CDialogEx::OnTimer(nIDEvent);
}

void CsharkcoinclientDlg::CloseThread()
{
	DWORD exc;
	theApp.m_bIsProcessMsgRunSwitch = 0;
	closesocket(theApp.m_uiConnSocket);

	while( ::GetExitCodeThread( theApp.m_hReceiveThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hReceiveThread );
	while( ::GetExitCodeThread( theApp.m_hPraseJsonToMsgThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hPraseJsonToMsgThread );
	while( ::GetExitCodeThread( theApp.m_hProcessMsgThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hProcessMsgThread);

	while( ::GetExitCodeThread( theApp.m_hCheckUpdateThread , &exc ) ) {

		if( STILL_ACTIVE == exc ) {
			;
		}else {
			TRACE( "EXC = %d \n" , exc ) ;
			break;
		}
		Sleep(100);
	}
	CloseHandle(theApp.m_hCheckUpdateThread);
}

void  CsharkcoinclientDlg::StopSever()
{
	string strCommand;
	strCommand = strprintf("%s",_T("stop"));
	string strSendData;
	string strret = _T("sharkfundd server stopping");

	SYSTEMTIME curTime ;
	memset( &curTime , 0 , sizeof(SYSTEMTIME) ) ;
	GetLocalTime( &curTime ) ;
	int RecivetxTimestart =0;
	RecivetxTimestart=(int) UiFun::SystemTimeToTimet(curTime);
	bool nRecStopCmd = false;
	while(TRUE){
		if(!nRecStopCmd) 
		{
			CSoyPayHelp::getInstance()->SendRpc(strCommand,strSendData);
		}
		if (strSendData.find(strret) >=0)
		{
			nRecStopCmd = true;
		}
		GetLocalTime( &curTime ) ;
		int RecivetxTimeLast =0;
		RecivetxTimeLast= (int)UiFun::SystemTimeToTimet(curTime);
		if ((RecivetxTimeLast - RecivetxTimestart) > 5)
		{
			return;
		}
		Sleep(5);
	}

}
void CsharkcoinclientDlg::CloseApp()
{
	if (!theApp.m_bIsPassLock)
	{
		EndDialog(IDD_DIALOG_SETWALLETENTRP);
	}
	DeleteTray() ;
	EndWaitCursor();
	//	PostMessage( WM_QUIT ) ; 
	PostMessage( WM_CLOSE ); 	
	//DestroyWindow();
	Sleep(200);
}

void CsharkcoinclientDlg::DeleteTray() 
{ 
	NOTIFYICONDATA nid; 
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA); 
	nid.hWnd=this->m_hWnd; 
	nid.uID=IDR_MAINFRAME; 
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ; 
	nid.uCallbackMessage=WM_SHOWTASK; //�Զ������Ϣ���� 
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	strcpy_s(nid.szTip,"sharkcoin"); //��Ϣ��ʾ��Ϊ���ƻ��������ѡ� 
	Shell_NotifyIcon(NIM_DELETE,&nid); //��������ɾ��ͼ�� 
} 

void CsharkcoinclientDlg::ShowStateTip(UINT nButtonID)
{
	if ( NULL == m_pTitleBar  ) return ;
	m_pTitleBar->MobileTip(nButtonID , FALSE );
}
