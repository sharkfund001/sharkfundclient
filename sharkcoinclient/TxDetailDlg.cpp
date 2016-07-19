// TxDetailDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sharkcoinclient.h"
#include "TxDetailDlg.h"
#include "afxdialogex.h"


// CTxDetailDlg �Ի���

IMPLEMENT_DYNAMIC(CTxDetailDlg, CDialogBase)

CTxDetailDlg::CTxDetailDlg(CWnd* pParent /*=NULL*/)
	: CDialogBase(CTxDetailDlg::IDD, pParent)
{
	//m_pBmp = NULL ;
}

CTxDetailDlg::~CTxDetailDlg()
{
	/*if( NULL != m_pBmp ) {
	DeleteObject(m_pBmp) ;
	m_pBmp = NULL ;
	}*/
}

void CTxDetailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CTxDetailDlg, CDialogBase)
	ON_BN_CLICKED(IDC_CLOSE, &CTxDetailDlg::OnBnClickedClose)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CTxDetailDlg::OnBnClickedOk)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

string CTxDetailDlg::GetContacrDetail(uistruct::REVTRANSACTION_t tx)
{
	string strCommand,strShowData;
	strCommand = strprintf("%s %s","getaccountinfo" ,tx.desaddr.c_str() );
	CSoyPayHelp::getInstance()->SendRpc(strCommand,strShowData);

	if (strShowData  ==""){		
		return _T("");
	}
	Json::Reader reader;  
	Json::Value root; 

	if (reader.parse(strShowData, root)) {		

		if (strShowData.find("RegID") >0)
		{
			tx.desregid = root["RegID"].asString();
		}
	}

	string txdetail ="";
	string nValue = tx.Contract;
	std::vector<unsigned char> vTemp = CSoyPayHelp::getInstance()->ParseHex(nValue);

	string strContract = CSoyPayHelp::getInstance()->HexStr(vTemp);
	txdetail +=strprintf("%s:   %d\r\n\r\n",_T("��Լ") ,strContract.c_str() );


	return txdetail;
}
// CTxDetailDlg ��Ϣ�������
void CTxDetailDlg::ShowTxDetail(CString jsontx)
{
	if (jsontx == _T(""))
	{
		string strShowData;
		strShowData+=strprintf("%s" ,_T("�˽��ײ�����")) ;
		GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(strShowData.c_str());
		return;
	}
	//string txdetail = tx.ToJson();
	uistruct::REVTRANSACTION_t tx;
	tx.JsonToStruct(jsontx.GetString());
	string txdetail,strShowData;

	string txtype = tx.txtype;
	if (!strcmp(txtype.c_str(),"REWARD_TX"))
	{
		strShowData+=strprintf("%s" ,_T("�ڿ�������")) ;
	}else if (!strcmp(txtype.c_str(),"REG_ACCT_TX"))
	{
		strShowData+=strprintf("%s" ,_T("ע���˻�����")) ;
	}else if (!strcmp(txtype.c_str(),"COMMON_TX"))
	{
		strShowData+=strprintf("%s" ,_T("ת�˽���")) ;
	}else if (!strcmp(txtype.c_str(),"CONTRACT_TX"))
	{
		strShowData+=strprintf("%s" ,_T("��Լ����")) ;
	}else if (!strcmp(txtype.c_str(),"REG_APP_TX"))
	{
		strShowData+=strprintf("%s" ,_T("ע��Ӧ�ý���")) ;
	}
	txdetail=strprintf("%s: %s\r\n\r\n",_T("��������"),strShowData.c_str());
	txdetail+=strprintf("%s: %s\r\n\r\n",_T("����ID"),tx.txhash);
	txdetail+=strprintf("%s:   %d\r\n\r\n",_T("�汾��"),tx.ver);
	txdetail+=strprintf("%s:   %s\r\n\r\n",_T("Դ��ַ"),tx.addr.c_str());
	if (tx.pubkey != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",_T("��Կ"),tx.pubkey.c_str());
	}
	if (tx.miner_pubkey != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",_T("���ڿ�Կ"),tx.miner_pubkey.c_str());
	}
	if (tx.fees != 0)
	{
		txdetail+=strprintf("%s:   %.8f\r\n\r\n",_T("С��"),tx.fees*COIN);
	}
	if (tx.height != 0)
	{
		int height = tx.height +250; 
		txdetail+=strprintf("%s:   %d\r\n\r\n",_T("���׳�ʱ�߶�"),height);
	}
	if (tx.desaddr != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",_T("Ŀ�ĵ�ַ"),tx.desaddr.c_str() );
	}
	if (tx.money != 0)
	{
		txdetail+=strprintf("%s:   %.2f\r\n\r\n",_T("���׽��"),tx.money*COIN);
	}
	if (tx.Contract != "")
	{
		//txdetail.AppendFormat(_T("��Լ����:   %d\r\n\r\n"),tx.Contract.c_str() );
		txdetail+=strprintf("%s",GetContacrDetail(tx).c_str());
		
	}

	if (tx.confirmedHeight != 0)
	{
		txdetail+=strprintf("%s:   %d\r\n\r\n",_T("ȷ�ϸ߶�"),tx.confirmedHeight );
	}
	if (tx.confirmedtime != 0)
	{
		SYSTEMTIME curTime = UiFun::Time_tToSystemTime(tx.confirmedtime);
		txdetail+=strprintf("%s:   %04d-%02d-%02d %02d:%02d:%02d\r\n\r\n",_T("ȷ��ʱ��"),curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute, curTime.wSecond);
	}
	if (tx.blockhash != "")
	{
		txdetail+=strprintf("%s:   %s\r\n\r\n",_T("ȷ��blockHash"),tx.blockhash.c_str());
	}

	if ( theApp.blocktipheight != 0 && tx.confirmedHeight != 0)
	{
		unsigned int quredCount = theApp.blocktipheight - tx.confirmedHeight;
		if (quredCount >=0)
		{
			txdetail+=strprintf("%s:  %d\r\n\r\n",_T("ȷ����"),quredCount );
		}	
	}

	GetDlgItem(IDC_EDIT_TXDETAIL)->SetWindowText(txdetail.c_str());
}

BOOL CTxDetailDlg::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	//SetBkBmpNid(IDB_BITMAP_SETBLACK);

	m_headText.SetWindowText(_T("��������"));
	m_headText.SetFont(100, _T("΢���ź�"));
	m_headText.SetTextColor(RGB(255,255,255));
	m_headText.SetWindowPos( NULL , 5 , 4 , 100, 25  ,SWP_SHOWWINDOW ) ; 

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
	CRect rect ;
	m_rBtnClose.GetClientRect(rect);

	RECT ret;
	GetWindowRect(&ret);
	m_rBtnClose.SetWindowPos(NULL ,(ret.right-ret.left)-rect.Width() - 4 , 2 , 0 , 0 , SWP_NOSIZE); 

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUT1 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText(_T("ȷ��")) ;
	m_rBtnOk.SetFontEx(-12 , _T("΢���ź�"));
	/*
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	*/
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
	m_rBtnOk.SizeToContent();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	ShowTxDetail(theApp.m_strAddress);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}




void CTxDetailDlg::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}



HBRUSH CTxDetailDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_EDIT_TXDETAIL)
	{

		pDC->SetBkMode(TRANSPARENT);
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));

	}


	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CTxDetailDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}

//void CTxDetailDlg::SetBkBmpNid( UINT nBitmapIn ) 
//{
//	if( NULL != m_pBmp ) {
//		::DeleteObject( m_pBmp ) ;
//		m_pBmp = NULL ;
//	}
//	m_pBmp	= NULL ;
//	HINSTANCE	hInstResource = NULL;	
//	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);
//	if( NULL != hInstResource ) {
//		m_pBmp = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);
//	}
//}


//BOOL CTxDetailDlg::OnEraseBkgnd(CDC* pDC)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CRect   rect; 
//	GetClientRect(&rect); 
//
//	if(m_pBmp   !=   NULL) { 
//		BITMAP   bm; 
//		CDC   dcMem; 
//		::GetObject(m_pBmp,sizeof(BITMAP),   (LPVOID)&bm); 
//		dcMem.CreateCompatibleDC(NULL); 
//		HBITMAP     pOldBitmap   =(HBITMAP   )   dcMem.SelectObject(m_pBmp); 
//		pDC-> StretchBlt(rect.left,rect.top-1,rect.Width(),rect.Height(),   &dcMem,   0,   0,bm.bmWidth-1,bm.bmHeight-1,   SRCCOPY); 
//
//		dcMem.SelectObject(pOldBitmap);
//		dcMem.DeleteDC();
//	} else  
//		CWnd::OnEraseBkgnd(pDC); 
//
//	return 1;
//}
