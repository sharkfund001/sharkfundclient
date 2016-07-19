// Rece.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sharkcoinclient.h"
#include "Rece.h"
#include "afxdialogex.h"
#include "NewAddressDlg.h"
#include "SignAccountsDlg.h"
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorksheet.h"
#include "CWorkbooks.h"
#include "CWorksheets.h"
#include "sharkcoinclientDlg.h"

// CRece �Ի���

IMPLEMENT_DYNAMIC(CRece, CDialogBar)

CRece::CRece()
{
   m_pBmp = NULL ;
}

CRece::~CRece()
{
	if( NULL != m_pBmp ) {
		DeleteObject(m_pBmp) ;
		m_pBmp = NULL ;
	}
}

void CRece::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_listCtrl);
	DDX_Control(pDX, IDC_BUTTON_JHDZ, m_rBtnAcitve);
	DDX_Control(pDX, IDC_BUTTON_NEWADDRESS, m_rBtnNewaddr);
	DDX_Control(pDX, IDC_COPYADDRESS, m_rBtnCopyaddr);
	DDX_Control(pDX, IDC_EXPORT_EXEL2, m_rBtnExcel);
}


BEGIN_MESSAGE_MAP(CRece, CDialogBar)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(MSG_USER_RECIVE_UI , &CRece::OnShowListCtrl )
	ON_BN_CLICKED(IDC_BUTTON_JHDZ, &CRece::OnBnClickedButtonJhdz)
	ON_BN_CLICKED(IDC_BUTTON_NEWADDRESS, &CRece::OnBnClickedButtonNewaddress)
	ON_BN_CLICKED(IDC_COPYADDRESS, &CRece::OnBnClickedCopyaddress)
	ON_BN_CLICKED(IDC_EXPORT_EXEL2, &CRece::OnBnClickedExportExel2)
END_MESSAGE_MAP()

void CRece::ShowListInfo()
{
	theApp.m_SqliteDeal.GetWalletAddressList(_T(" 1=1 order by reg_id"), &m_MapAddrInfo);

	if ( 0 == m_MapAddrInfo.size() ) return  ;

	m_listCtrl.DeleteAllItems();

	//���ص�ComBox�ؼ�
	int nSubIdx = 0 , i = 0 ;
	string strShowData = "";
	std::map<string,uistruct::LISTADDR_t>::const_iterator const_it;
	for ( const_it = m_MapAddrInfo.begin() ; const_it != m_MapAddrInfo.end() ; const_it++ ) {
		nSubIdx = 0;
		string strOrder="";
		strOrder = strprintf("%d", i+1);
		m_listCtrl.InsertItem(i,strOrder.c_str());

		uistruct::LISTADDR_t address = const_it->second;

		//m_listCtrl.SetItemText(i , ++nSubIdx , address.RegID.c_str() ) ;


		m_listCtrl.SetItemText( i , ++nSubIdx, address.Label.c_str()) ;
		//m_listCtrl.SetItemData(item , (DWORD_PTR)&(*const_it)) ;

		m_listCtrl.SetItemText(i , ++nSubIdx , address.address.c_str() ) ;

		if (address.bSign == 1)
		{
			strShowData=_T("�Ѽ���") ;
		}else{
			strShowData=_T("δ����");
		}

		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		
		/*if (address.nColdDig== 1)
		{
		strShowData=_T("֧��");
		}else{
		strShowData=_T("��֧��");
		}
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;*/
		

		strShowData = strprintf("%.2f" , address.fMoney ) ;
		m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

		i++;
	}
	if ( 17 <= m_MapAddrInfo.size() )  {
		m_listCtrl.SetColumnWidth(5 , 86 ); 
	}else{
		m_listCtrl.SetColumnWidth(5 , 104 ); 
	}
}

// CRece ��Ϣ�������
void CRece::SetBkBmpNid( UINT nBitmapIn ) 
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

BOOL CRece::OnEraseBkgnd(CDC* pDC)
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


int CRece::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	SetBkBmpNid( IDB_BITMAP_SETBLACK ) ;
	return 0;
}


BOOL CRece::Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID)
{
	// TODO: �ڴ����ר�ô����/����û���

	BOOL bRes = CDialogBar::Create(pParentWnd, nIDTemplate, nStyle, nID);
	if ( bRes ) {
		UpdateData(FALSE);
		struct LISTCol {
			CString		name ;
			UINT		size ;
		} listcol[5]  = {
			{"���" ,      80},
			//{"�˺�ID" ,      100},
			{"��ǩ" ,      157},
			{"��ַ" ,      310}, 
			{"����״̬" ,       134}, 
			//{"���ڿ�" ,      66},
			{"���" ,      180}
		};
		m_listCtrl.SetBkColor(RGB(255,255,255));       
		m_listCtrl.SetRowHeigt(35);               
		m_listCtrl.SetHeaderHeight(1.9);         
		m_listCtrl.SetHeaderFontHW(15,0);
		m_listCtrl.SetHeaderBKColor(/*224,65,18,*/35, 109, 206, 0); 
		m_listCtrl.SetHeaderTextColor(RGB(255,255,255)); 
		m_listCtrl.SetTextColor(RGB(0,0,0));  
		for( int i = 0 ; i < 5 ; i++  ) {
			m_listCtrl.InsertColumn(i,listcol[i].name,LVCFMT_CENTER,listcol[i].size);
		}

		m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );// |LVS_SINGLESEL  );


		m_rBtnExcel.SetBitmaps( IDB_BITMAP_BUT1, RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		m_rBtnExcel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnExcel.SetWindowText("����Excel") ;
		m_rBtnExcel.SetFontEx(-12 , _T("΢���ź�"));
		m_rBtnExcel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255,255,255));
		m_rBtnExcel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255,255,255));
		m_rBtnExcel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255,255));
		m_rBtnExcel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255,255,255));
		m_rBtnExcel.SizeToContent();

		m_rBtnAcitve.SetBitmaps( IDB_BITMAP_BUT1 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		m_rBtnAcitve.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnAcitve.SetWindowText("�����ַ") ;
		m_rBtnAcitve.SetFontEx(-12 , _T("΢���ź�"));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255,255,255));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255,255,255));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255,255));
		m_rBtnAcitve.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255,255,255));
		m_rBtnAcitve.SizeToContent();

		m_rBtnNewaddr.SetBitmaps( IDB_BITMAP_BUT1 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		m_rBtnNewaddr.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnNewaddr.SetWindowText("�½���ַ") ;
		m_rBtnNewaddr.SetFontEx(-12 , _T("΢���ź�"));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255,255,255));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnNewaddr.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnNewaddr.SizeToContent();

		m_rBtnCopyaddr.SetBitmaps( IDB_BITMAP_BUT1 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
		m_rBtnCopyaddr.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
		m_rBtnCopyaddr.SetWindowText("���Ƶ�ַ") ;
		m_rBtnCopyaddr.SetFontEx(-12 , _T("΢���ź�"));
		m_rBtnCopyaddr.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
		m_rBtnCopyaddr.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(255,255,255));
		m_rBtnCopyaddr.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
		m_rBtnCopyaddr.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
		m_rBtnCopyaddr.SizeToContent();

		ShowListInfo();


		theApp.SubscribeMsg( theApp.GetMtHthrdId() , GetSafeHwnd() , MSG_USER_RECIVE_UI ) ;

	}
	return bRes ;
}


void CRece::OnSize(UINT nType, int cx, int cy)
{
	CDialogBar::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if( NULL != GetSafeHwnd() ) {
		CRect rc  ;
		GetClientRect( rc ) ;

		CWnd *pWnd = NULL;
		pWnd = (CWnd *)GetDlgItem(IDC_STATIC_RECE_GROUP);
		if(NULL != pWnd)
		{
			pWnd->SetWindowPos(NULL, 10, 6, 880, 80, SWP_SHOWWINDOW);
		}

		pWnd = (CWnd*)GetDlgItem(IDC_LIST);
		if ( NULL != pWnd ) {
			pWnd->SetWindowPos( NULL ,9 , 100 , 880, 350  ,SWP_SHOWWINDOW ) ; 
		}

		pWnd = (CWnd*)GetDlgItem(IDC_EXPORT_EXEL2);
		if ( NULL != pWnd ) {
			pWnd->GetWindowRect(rc);
			pWnd->SetWindowPos( NULL ,40 , 36 , rc.Width(), rc.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		pWnd = (CWnd*)GetDlgItem(IDC_BUTTON_JHDZ);
		if ( NULL != pWnd ) {
			pWnd->GetWindowRect(rc);
			pWnd->SetWindowPos( NULL ,130 , 36 , rc.Width(), rc.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		pWnd = (CWnd*)GetDlgItem(IDC_BUTTON_NEWADDRESS);
		if ( NULL != pWnd ) {
			pWnd->GetWindowRect(rc);
			pWnd->SetWindowPos( NULL ,220 , 36 , rc.Width(), rc.Height()  ,SWP_SHOWWINDOW ) ; 
		}
		pWnd = (CWnd*)GetDlgItem(IDC_COPYADDRESS);
		if ( NULL != pWnd ) {
			pWnd->GetWindowRect(rc);
			pWnd->SetWindowPos( NULL ,310 , 36 , rc.Width(), rc.Height()  ,SWP_SHOWWINDOW ) ; 
		}

	}
}


void CRece::OnBnClickedButtonJhdz()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if (!theApp.m_bIsSyncBlock)
	{
		
		UiFun::MessageBoxEx(_T("ͬ��δ���,���ܷ��ͼ����"),_T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	

	CString StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		CString str = m_listCtrl.GetItemText(nRow, 2);
		string addr = strprintf("%s",str);
		if(!m_MapAddrInfo.count(addr))
			{
					TRACE("ERROR");
				StrShow.Format(_T("��ַ������"));
				
				UiFun::MessageBoxEx(StrShow ,_T("��ʾ") ,MFB_OK|MFB_TIP );
				return;
		   }
		uistruct::LISTADDR_t te =  m_MapAddrInfo[addr];
		//uistruct::LISTADDR_t * pAddrItem = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
		if (te.fMoney <=0)
		{
			StrShow.Format(_T("�˻����Ϊ��,���ܼ���!"));
			
			UiFun::MessageBoxEx(StrShow ,_T("��ʾ") ,MFB_OK|MFB_TIP );
			return;
		}
		if(te.bSign) 
		{
			StrShow.Format(_T("�˻��Ѽ���!"));
			
			UiFun::MessageBoxEx(StrShow, _T("��ʾ") ,MFB_OK|MFB_TIP );
			return;
		}
		theApp.m_strAddress.Format(_T("%s") ,te.address.c_str() ) ;
		CSignAccountsDlg dlg;
		dlg.DoModal();

		/*m_accountDlg->SetShowAddr(pDbbetData->address);
		CRect rcWindow;
		GetWindowRect(&rcWindow);
		m_accountDlg->MoveWindow(rcWindow.right/2+50,rcWindow.top+200,400,rcWindow.Height()/2);
		m_accountDlg->ShowWindow(SW_SHOW);*/
	}else{
		StrShow.Format(_T("��ѡ���ַ!"));
		
		UiFun::MessageBoxEx(StrShow , _T("��ʾ"),MFB_OK|MFB_TIP );
	}
}


void CRece::OnBnClickedButtonNewaddress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CNewAddressDlg dlg;
	dlg.DoModal();
}


void CRece::OnBnClickedCopyaddress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	string StrShow;
	POSITION pos = m_listCtrl.GetFirstSelectedItemPosition() ;
	if ( pos ) {
		int nRow = m_listCtrl.GetNextSelectedItem(pos) ;
		//uistruct::LISTADDR_t * pDbbetData = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(nRow) ;
		CString source =m_listCtrl.GetItemText(nRow, 2);
		//�ı����ݱ�����source������
		if(OpenClipboard())
		{
			HGLOBAL clipbuffer;
			char * buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, source.GetLength()+1);
			buffer = (char*)GlobalLock(clipbuffer);
			strcpy(buffer, LPCSTR(source));
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT,clipbuffer);
			CloseClipboard();
			StrShow = _T("��ַ�Ѹ��Ƶ�������");

			UiFun::MessageBoxEx(StrShow.c_str() , _T("��ʾ") ,MFB_OK|MFB_TIP );
		}
	}
	else{
		StrShow = _T("��ѡ���ַ!");

		UiFun::MessageBoxEx(StrShow.c_str() , _T("��ʾ") ,MFB_OK|MFB_TIP );
	}
}


BOOL CRece::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//�ж��Ƿ��¼���Enter��
		if (pMsg->wParam == VK_CONTROL || pMsg->wParam == VK_ESCAPE ||pMsg->wParam == VK_SHIFT )
		{
			return true;
		}
	}

	if(pMsg->message == WM_KEYDOWN) 
	{  

		switch(pMsg->wParam) 
		{  

		case VK_RETURN: //�س�   
			return TRUE;  

		case VK_ESCAPE: //ESC  
			return TRUE;  

		}  

	}  
	return CDialogBar::PreTranslateMessage(pMsg);
}

void   CRece::ModifyListCtrlItem()
{
	CPostMsg postmsg;
	if (!theApp.m_msgUIRecDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());



	if(m_MapAddrInfo.count(addr.address) <= 0)
	{
		TRACE("map ModifyListCtrlItem ERROR");
		return;
	}
	
	m_MapAddrInfo[addr.address]=addr;
	
	int count = m_listCtrl.GetItemCount();
	if ( 17 <= count )  {
		m_listCtrl.SetColumnWidth(5 , 86 ); 
	}else{
		m_listCtrl.SetColumnWidth(5 , 104 ); 
	}
	for(int i = 0; i < count; i++)
	{
		CString str = m_listCtrl.GetItemText(i, 2); // ����������������ˣ�����ȡ��ÿ��item��0�е�ֵ
		if(strcmp(str,addr.address.c_str()) == 0)
		////uistruct::LISTADDR_t *pListAddr = (uistruct::LISTADDR_t*)m_listCtrl.GetItemData(i);
		//if (!memcmp(pListAddr->address,addr.address,sizeof(pListAddr->address)) &&\
		//	(pListAddr->fMoney != addr.fMoney || pListAddr->bSign != addr.bSign))
		{
			/*pListAddr->fMoney = addr.fMoney;
			pListAddr->bSign = addr.bSign;*/
		
			int nSubIdx = 0;
			string  strShowData;
		
			//m_listCtrl.SetItemText(i , ++nSubIdx , addr.RegID.c_str() ) ;
	
			m_listCtrl.SetItemText(i , ++nSubIdx , addr.Label.c_str() ) ;
			//m_listCtrl.SetItemData(i , (DWORD_PTR)&(*m_pListaddrInfo.rbegin())) ;


			m_listCtrl.SetItemText(i , ++nSubIdx , addr.address.c_str() ) ;

			if (addr.bSign == 1)
			{
				strShowData=_T("�Ѽ���");
			}else{
				strShowData = _T("δ����") ;
			}

			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

			
			/*if (addr.nColdDig== 1)
			{
			strShowData = _T("֧��") ;
			}else{
			strShowData = _T("��֧��");
			}
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str()) ;*/
			

			strShowData = strprintf("%.2f" ,addr.fMoney ) ;
			m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;
			break;
		}
	}
}
void   CRece::InsertListCtrlItem()
{
	CPostMsg postmsg;
	if (!theApp.m_msgUIRecDlgQueue.pop(postmsg))
	{
		return ;
	}

	uistruct::LISTADDR_t addr; 
	string strTemp = postmsg.GetData();
	addr.JsonToStruct(strTemp.c_str());



	if(m_MapAddrInfo.count(addr.address) > 0){
		TRACE("map InsertListCtrlItem ERROR");
		return ;
	}

	int count = m_listCtrl.GetItemCount();

	int nSubIdx = 0,i =count;

	string strOrder(_T(""));
	strOrder =strprintf("%d", ++count);
	
	m_listCtrl.InsertItem( i , strOrder.c_str());  //���
	string  strShowData;

	//m_listCtrl.SetItemText(i , ++nSubIdx , addr.RegID.c_str() ) ;


	m_listCtrl.SetItemText( i , ++nSubIdx, addr.Label.c_str() ) ; //��ǩ
	
	m_MapAddrInfo[addr.address]=addr;

	    //��ַ
	m_listCtrl.SetItemText( i , ++nSubIdx, addr.address.c_str() ); 

	if (addr.bSign == 1)
	{
		strShowData= _T("�Ѽ���") ;
	}else{
		strShowData= _T("δ����") ;
	}

	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;
	
	/*if (addr.nColdDig== 1)
	{
	strShowData= _T("֧��");
	}else{
	strShowData= _T("��֧��") ;
	}
	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;*/
	

	strShowData =strprintf("%.2f" , addr.fMoney ) ;
	m_listCtrl.SetItemText(i , ++nSubIdx , strShowData.c_str() ) ;

}

LRESULT  CRece::OnShowListCtrl(  WPARAM wParam, LPARAM lParam )
{
	int type = (int)wParam;
	switch(type)
	{
	case WM_UP_ADDRESS:
		{
			ModifyListCtrlItem();
			break;
		}
		break;
	case WM_UP_NEWADDRESS:
		{
			InsertListCtrlItem();
			break;
		}
		break;
	default:
		break;

	}
	//ShowListInfo();
	return 1;
}


void CRece::OnBnClickedExportExel2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int count = m_listCtrl.GetItemCount();
	if (count == 0)
	{
		UiFun::MessageBoxEx(_T("û�м�¼���Ե�����") ,_T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	CFileDialog dlg(FALSE,NULL,_T("���ռ�¼"),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST ,"�ļ� (*.xls)|*.xls||");
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	CString strFile = dlg.GetPathName();
	if (!((CsharkcoinclientDlg*)(theApp.m_pMainWnd))->GetFileName(strFile,_T(".xls")))
	{
		return;
	}

	struct LISTCol {
		string		name ;
		UINT		size ;
	} listheadr[5]  = {
		{_T("���") ,      50},
		{_T("��ǩ") ,    30},
		{_T("��ַ") ,    40},
		{_T("����״̬") ,    10},
		{_T("���") ,  40}
	};

	COleVariant

	covTrue((short)TRUE),

	covFalse((short)FALSE),

	covOptional((long)DISP_E_PARAMNOTFOUND,   VT_ERROR);

	CApplication   app;

	CWorkbooks   books;

	CWorkbook   book;

	CWorksheets   sheets;

	CWorksheet   sheet;

	CRange   range;

	CFont0   font;



	if (!app.CreateDispatch(_T("Excel.Application")))

	{

		UiFun::MessageBoxEx(_T("������û��װoffice ���´���ʧ�ܣ�") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;

	}



	//Get   a   new   workbook.

	books = app.get_Workbooks();

	book = books.Add(covOptional);



	sheets = book.get_Worksheets();

	sheet = sheets.get_Item(COleVariant((short)1));


	int   iCol;

	int   m_cols   =   5;

	int   m_rows = count;

	HDITEM   hdi;

	TCHAR     lpBuffer[256];

	bool       fFound   =   false;



	hdi.mask   =   HDI_TEXT;

	hdi.pszText   =   lpBuffer;

	hdi.cchTextMax   =   256;

	CString   colname;

	CString strTemp;

	for(iCol=0;   iCol <m_cols;   iCol++)//���б�ı���ͷд��EXCEL

	{

		UiFun::GetCellName(1 ,iCol + 1, colname);

		range   =   sheet.get_Range(COleVariant(colname),COleVariant(colname));

		//pmyHeaderCtrl-> GetItem(iCol,   &hdi);

		range.put_Value2(COleVariant(listheadr[iCol].name.c_str()));

		int   nWidth   = listheadr[iCol].size;  //m_listCtrl.GetColumnWidth(iCol)/6;

		//�õ���iCol+1��  

		range.AttachDispatch(range.get_Item(_variant_t((long)(iCol+1)),vtMissing).pdispVal,true);  

		//�����п� 

		range.put_ColumnWidth(_variant_t((long)nWidth));

	}

	range   =   sheet.get_Range(COleVariant( _T("A1 ")),   COleVariant(colname));

	range.put_RowHeight(_variant_t((long)50));//�����еĸ߶�

	font = range.get_Font();

	font.put_Bold(covTrue);

	range.put_VerticalAlignment(COleVariant((short)-4108));//xlVAlignCenter   =   -4108



	COleSafeArray   saRet;

	DWORD   numElements[]={m_rows,m_cols};       //5x2   element   array

	saRet.Create(VT_BSTR,   2,   numElements);

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range = range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));

	long   index[2];

	range   =   sheet.get_Range(COleVariant( _T("A2 ")),covOptional);

	range   =   range.get_Resize(COleVariant((short)m_rows),COleVariant((short)m_cols));

	for(int i = 0; i < count; i++)
	{
		CString strNo = m_listCtrl.GetItemText(i, 0);
		BSTR   bstr   =   strNo.AllocSysString();
		index[0] = i;
		index[1] = 0;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);

		CString strLabel = m_listCtrl.GetItemText(i, 1);
		bstr   =   strLabel.AllocSysString();
		index[0] = i;
		index[1] = 1;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);

		CString strAddress = m_listCtrl.GetItemText(i, 2);
		bstr   =   strAddress.AllocSysString();
		index[0] = i;
		index[1] = 2;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);

		CString strActive = m_listCtrl.GetItemText(i, 3);
		bstr   =   strActive.AllocSysString();
		index[0] = i;
		index[1] = 3;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);
		CString strMoney = m_listCtrl.GetItemText(i, 4);
		bstr   =   strMoney.AllocSysString();
		index[0] = i;
		index[1] = 4;
		saRet.PutElement(index,bstr);

		SysFreeString(bstr);
	}



	range.put_Value2(COleVariant(saRet));


	saRet.Detach();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	book.SaveCopyAs(COleVariant(strFile));

	//       cellinterior.ReleaseDispatch();

	book.put_Saved(true);

	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch();
	books.ReleaseDispatch();

	//book.ReleaseDispatch();  

	//books.ReleaseDispatch();  
	book.Close (covOptional, covOptional,covOptional);// �ر�Workbook����
	books.Close(); 
	app.Quit();

	app.ReleaseDispatch();
}
