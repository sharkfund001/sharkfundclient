// RPCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sharkcoinclient.h"
#include "RPCDlg.h"
#include "afxdialogex.h"
#include "CommonStruct.h"

// CRPCDlg �Ի���

IMPLEMENT_DYNAMIC(CRPCDlg, CDialogBase)

CRPCDlg::CRPCDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CRPCDlg::IDD, pParent)
{

}

CRPCDlg::~CRPCDlg()
{
}

void CRPCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_RPC, m_rpccommand);
	DDX_Control(pDX, IDC_COMBO_INPUT, m_comboxinput);
	DDX_Control(pDX, IDC_CLOSE1, m_rBtnClose);
	DDX_Control(pDX, IDC_BTN_CLEAR, m_rBtnClear);
	DDX_Control(pDX, IDC_HEAD, m_headText);
}


BEGIN_MESSAGE_MAP(CRPCDlg, CDialogBase)
	ON_MESSAGE(WM_SHOW_RECV_DATA,OnShowRecvData)
	ON_MESSAGE(WM_SHOW_SEND_DATA,OnShowSendData)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CRPCDlg::OnBnClickedBtnClear)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CLOSE1, &CRPCDlg::OnBnClickedClose1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRPCDlg ��Ϣ�������
BOOL CRPCDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	CRect rcWindow;
	theApp.m_pMainWnd->GetWindowRect(&rcWindow);
	CRect rc;
	GetWindowRect(&rc);
    MoveWindow(rcWindow.right,rcWindow.top-20,rc.Width(), rc.Height());
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_headText.SetFont(100, _T("΢���ź�"));
	m_headText.SetTextColor(RGB(255,255,255));	
	m_headText.SetWindowText("����");
	m_headText.SetWindowPos( NULL , 5 , 4 , 100, 25  ,SWP_SHOWWINDOW ) ; 

	//m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	
	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE2 , RGB(0, 0, 0) , IDB_BITMAP_CLOSE1 , RGB(0, 0, 0) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	//m_rBtnClose.SetFontEx(-12 , _T("��Բ"));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClose.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnClose.SizeToContent();
	
	/*
	m_rBtnClose.SetIcon(IDI_ICON_CLOSE3, 0, 0, IDI_ICON_CLOSE1, 0, 0);
	m_rBtnClose.DrawBorder(FALSE); 
	m_rBtnClose.DrawTransparent(TRUE);
	m_rBtnClose.SizeToContent();
	*/
	CRect rect ;
	m_rBtnClose.GetClientRect(rect);

	RECT ret;
	GetWindowRect(&ret);
	m_rBtnClose.SetWindowPos(NULL ,276 , 2 , 0 , 0 , SWP_NOSIZE); 
	m_rBtnClear.SetWindowText("���");
	m_rBtnClear.SetBitmaps( IDB_BITMAP_TC_LAN , RGB(255, 255, 0) , IDB_BITMAP_TC_LAN , RGB(255, 255, 255) );
	m_rBtnClear.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	/*
	m_rBtnClear.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnClear.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClear.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnClear.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	*/
	m_rBtnClear.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
	m_rBtnClear.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnClear.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
	m_rBtnClear.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
	m_rBtnClear.SizeToContent();

	//GetDlgItem(IDC_STATIC_INPUT)->SetWindowText(UiFun::UI_LoadString("RPC" , "RPC_INPUT" ,theApp.gsLanguage));
	CRect rcClient;
	GetClientRect(&rcClient);
	m_rpccommand.MoveWindow(0, 27, rcClient.Width(), rcClient.Height()- 80);

	//m_rpccommand.SetBkColor(RGB(205,225,243));
	//CEdit* pShowInfoEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOWINFO);
	//ASSERT(pShowInfoEdit);
	//pShowInfoEdit->MoveWindow(5,10,rcClient.Width()-60,rcClient.Height()-130);

	//CTreeCtrl* pShowInfoEdit = (CTreeCtrl*)GetDlgItem(IDC_TREE_RPC);
	//ASSERT(pShowInfoEdit);
	//pShowInfoEdit->MoveWindow(5,27,rcClient.Width()-60,rcClient.Height()-130);

	//CStatic* pStaticInput = (CStatic*)GetDlgItem(IDC_STATIC_INPUT);
	//ASSERT(pStaticInput);
	//pStaticInput->MoveWindow(5,rcClient.Height()-110,40,20);

	//CComboBox* pInputEdit = (CComboBox*)GetDlgItem(IDC_COMBO_INPUT);
	//ASSERT(pInputEdit);
	//pInputEdit->MoveWindow(50,rcClient.Height()-103,rcClient.Width()-160,25);

	//CButton* pBtnClear = (CButton*)GetDlgItem(IDC_BTN_CLEAR);
	//ASSERT(pBtnClear);
	//pBtnClear->MoveWindow(rcClient.Width()-95,rcClient.Height()-103,30,25);
	m_comboxinput.EnableAutoCompletion();
	AutoCombo.Init(&m_comboxinput);

	CStringA configpath = "";
	configpath.AppendFormat("%s",theApp.m_strInsPath.c_str());
	configpath.AppendFormat("\\%s","sharkfundclient.conf");

	if (PathFileExistsA(configpath))
	{
		CJsonConfigHelp::getInstance()->AddItemString(configpath,AutoCombo);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CRPCDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		//if (pMsg->hwnd == GetDlgItem(IDC_COMBO_INPUT)->GetSafeHwnd())
		if(m_comboxinput.IsChild(GetFocus()))
		{
			CString strCommand;
			GetDlgItem(IDC_COMBO_INPUT)->GetWindowText(strCommand);
			GetDlgItem(IDC_COMBO_INPUT)->SetWindowText(_T(""));

			string strcom =strprintf("%s",strCommand);
			string strSendData,strretshow;
			RPCCommandToJson(strcom,strSendData);
			CSoyPayHelp::getInstance()->SendRpcDlg(strSendData,strretshow);
			ShowJson(strcom,strretshow);
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CRPCDlg::OnBnClickedBtnClear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//GetDlgItem(IDC_EDIT_SHOWINFO)->SetWindowText(_T(""));
	m_rpccommand.DeleteAllItems();
}
LRESULT CRPCDlg::OnShowRecvData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	UpdateEditContent(str,_T("RecvData:"));
	return TRUE;
}
LRESULT CRPCDlg::OnShowSendData(WPARAM wparam,LPARAM lparam)
{
	CStringA str = *(CStringA*)wparam;
	UpdateEditContent(str,_T("SendData:"));
	return TRUE;
}
void CRPCDlg::UpdateEditContent(const CStringA& strNewData,const CString& strKeyWord)
{
	/*CString strShow;
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SHOWINFO);
	pEdit->GetWindowText(strShow);
	if(strShow.GetLength()> 1024*10)
	strShow.Empty();
	strShow += _T("\r\n");
	strShow += strKeyWord;
	strShow += _T("\r\n");
	strShow += CString(strNewData.GetString());
	pEdit->SetWindowText(strShow);

	int nLines = pEdit->GetLineCount();
	pEdit->LineScroll(nLines);*/
}


void CRPCDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( NULL != GetSafeHwnd() ) {
		const int div = 100 ;
		CRect rc ;
		GetClientRect( rc ) ;
		CWnd *pst = GetDlgItem( IDC_TREE_RPC ) ;
		if ( NULL != pst ) {
			pst->SetWindowPos( NULL , 0 , 27 , rc.Width(), 90*rc.Height()/div ,SWP_SHOWWINDOW ) ;
		}
		pst = GetDlgItem( IDC_STATIC_INPUT ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , 2 , 95*rc.Height()/div , rect.Width(), rect.Height() ,SWP_SHOWWINDOW ) ;
		}
		pst = GetDlgItem( IDC_COMBO_INPUT ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , (14*rc.Width()/div)+5  , 95*rc.Height()/div - 2 , 58*rc.Width()/div, 65*rc.Height()/div ,SWP_SHOWWINDOW ) ;
		}
		pst = GetDlgItem( IDC_BTN_CLEAR ) ;
		if ( NULL != pst ) {
			CRect rect ;
			pst->GetClientRect( rect ) ;
			pst->SetWindowPos( NULL , 75*rc.Width()/div  , 95*rc.Height()/div - 4 , rect.Width(), rect.Height() ,SWP_SHOWWINDOW ) ;
		}
	}
}

void CRPCDlg::ShowJson(string rpcname,string jsomstr){
	if (jsomstr == _T(""))
	{
		return;
	}
	Json::Reader reader;  
	Json::Value root; 
	if (!reader.parse(jsomstr, root)) 
		return ;
	m_rpccommand.Expand(m_rpccommand.GetRootItem(),TVE_COLLAPSE);
	HTREEITEM hItemRoot = m_rpccommand.InsertItem(rpcname.c_str(),TVI_ROOT);
	CSoyPayHelp::getInstance()->Jiexi(root,hItemRoot,m_rpccommand);
	m_rpccommand.Expand(hItemRoot,TVE_EXPAND);
}

void CRPCDlg::OnBnClickedClose1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}


HBRUSH CRPCDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	if (nCtlColor == CTLCOLOR_STATIC)
	{

		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));

	}
	return hbr;
}
