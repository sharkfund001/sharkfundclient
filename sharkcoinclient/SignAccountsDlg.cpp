// SignAccountsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sharkcoinclient.h"
#include "SignAccountsDlg.h"
#include "afxdialogex.h"


// CSignAccountsDlg �Ի���

IMPLEMENT_DYNAMIC(CSignAccountsDlg, CDialogBase)

CSignAccountsDlg::CSignAccountsDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSignAccountsDlg::IDD, pParent)
{
}
CSignAccountsDlg::~CSignAccountsDlg()
{

}

void CSignAccountsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SA_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDC_BUTTON_SA_SEND, m_rBtnSend);
	DDX_Control(pDX, IDC_ST_ADDR, m_addr);
	DDX_Control(pDX, IDC_FEE, m_fee);
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_COMBO2, m_cboUnit);
}


BEGIN_MESSAGE_MAP(CSignAccountsDlg, CDialogBase)
	ON_BN_CLICKED(IDC_BUTTON_SA_SEND, &CSignAccountsDlg::OnBnClickedButtonSend)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SA_CLOSE, &CSignAccountsDlg::OnBnClickedButtonClose)
END_MESSAGE_MAP()



void CSignAccountsDlg::OnBnClickedButtonSend()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	if (theApp.IsLockWallet())
	{
		return ;
	}

	CString address;
	GetDlgItem(IDC_EDIT_SA_ADDRESS)->GetWindowText(address);
	if ( _T("") != address ) {
		string strCommand , strShowData;CString strFee ;

		Json::Reader reader;  
		Json::Value root; 
		//if (!reader.parse(strShowData.GetString(), root)) 
		//	return  ;


		GetDlgItem(IDC_EDIT_FEE)->GetWindowText(strFee);
		strCommand = strprintf("%s %s %lld","registaccounttx" ,address  , (INT64)REAL_MONEY(strtod(strFee,NULL)) );

		if(!CSoyPayHelp::getInstance()->SendRpc(strCommand,root))
		{
			TRACE("OnBnClickedButtonSend rpccmd registaccounttx error");
			return;
		}

		strShowData = root.toStyledString();
		string strData;
		int pos = strShowData.find("hash");

		if ( pos >=0 ) {
			//���뵽���ݿ�
			string strHash;
			strHash =root["hash"].asString();
			string strCond;
			strCond = strprintf(" hash = '%s' ", (LPSTR)(LPCTSTR)strHash.c_str());
			int nItem =  theApp.m_SqliteDeal.GetTableCountItem(_T("t_transaction") , strCond) ;

			if ( 0 == nItem ) {

				CPostMsg postmsg(MSG_USER_GET_UPDATABASE,WM_REVTRANSACTION);
				postmsg.SetData(strHash);
				theApp.m_msgQueue.push(postmsg);

			}
		}

		if ( pos >=0 ) {
			strData = strprintf("%s\n%s" , _T("����׷��ͳɹ�����ȴ�1-2����ȷ�ϼ����") ,root["hash"].asCString() ) ;
		}else{
			strData= _T("�����˻�ʧ��") ;
		}
		if ( IDOK == UiFun::MessageBoxEx(strData.c_str(), _T("��ʾ") , MFB_OK|MFB_TIP ) ){
			EndDialog(IDOK);
		}
	}

}
void CSignAccountsDlg::SetShowAddr(CString addr)
{
	((CComboBox*)GetDlgItem(IDC_COMBO2))->SetCurSel(0);
	GetDlgItem(IDC_EDIT_SA_ADDRESS)->SetWindowText(addr);
}



BOOL CSignAccountsDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetWindowText(_T("�����ַ"));
	m_headText.SetFont(90, _T("΢���ź�"));
	m_headText.SetTextColor(RGB(255,255,255));	
	m_headText.SetWindowPos( NULL , 5 , 4 , 100, 25  ,SWP_SHOWWINDOW ) ; 
	// TODO:  �ڴ����Ӷ���ĳ�ʼ��
	//m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE , RGB(255, 255, 0) , IDB_BITMAP_CLOSE2 , RGB(255, 255, 255) );
	
	m_rBtnClose.SetBitmaps( IDB_BITMAP_CLOSE2 , RGB(0, 0, 0) , IDB_BITMAP_CLOSE1 , RGB(0, 0, 0) );
	m_rBtnClose.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnClose.SetWindowText("") ;
	m_rBtnClose.SetFontEx(-12 , _T("΢���ź�"));
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
	//m_rBtnClose.SetWindowPos(NULL ,476-26 , 0 , 0 , 0 , SWP_NOSIZE);
	CRect rect ;
	m_rBtnClose.GetClientRect(rect);

	RECT ret;
	GetWindowRect(&ret);
	m_rBtnClose.SetWindowPos(NULL ,(ret.right-ret.left)-rect.Width() - 4 , 2 , 0 , 0 , SWP_NOSIZE); 

	m_rBtnSend.SetBitmaps( IDB_BITMAP_BUT1 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnSend.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnSend.SetWindowText(_T("��  ��")) ;
	m_rBtnSend.SetFontEx(-12 , _T("΢���ź�"));
	/*
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	*/
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
	m_rBtnSend.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
	m_rBtnSend.SizeToContent();

	SetShowAddr( theApp.m_strAddress);

	m_fontGrid.CreatePointFont(100,_T("������"));

	GetDlgItem(IDC_EDIT_FEE)->SetWindowTextA("0.0001");

	//GetDlgItem(IDC_FEE)->SetWindowText(_T("��ʾ"));
	//GetDlgItem(IDC_ST_ADDR)->SetWindowText(_T("��ַ:"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

HBRUSH CSignAccountsDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//if (nCtlColor == CTLCOLOR_STATIC)
	//{
	//	pDC->SetBkMode(TRANSPARENT);
	//	pDC->SelectObject(&m_fontGrid);
	//	hbr = (HBRUSH)CreateSolidBrush(RGB(240,240,240));
	//}


	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CSignAccountsDlg::OnBnClickedButtonClose()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	EndDialog(IDOK);
}
