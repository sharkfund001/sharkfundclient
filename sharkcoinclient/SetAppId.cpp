// SetAppId.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sharkcoinclient.h"
#include "SetAppId.h"
#include "afxdialogex.h"


// CSetAppId �Ի���

IMPLEMENT_DYNAMIC(CSetAppId, CDialogEx)

CSetAppId::CSetAppId(CWnd* pParent /*=NULL*/)
	: CDialogBase(CSetAppId::IDD, pParent)
{

}

CSetAppId::~CSetAppId()
{
}

void CSetAppId::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_BETID, m_P2Pid);
	DDX_Control(pDX, IDC_STATIC_GRBID, m_redpackeid);	
	DDX_Control(pDX, IDC_STATIC_IPOID, m_ipoid);	
	DDX_Control(pDX, IDC_HEAD, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CSetAppId, CDialogBase)
	ON_BN_CLICKED(IDOK, &CSetAppId::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CSetAppId::OnBnClickedClose)
END_MESSAGE_MAP()


// CSetAppId ��Ϣ�������

void CSetAppId::SetDaluft()
{
	string strShow = "";

	//GetDlgItem(IDC_BETID)->SetWindowText(theApp.m_betScritptid.c_str());
	//GetDlgItem(IDC_GRBID)->SetWindowText(theApp.m_redPacketScriptid.c_str());
	//GetDlgItem(IDC_IPOID)->SetWindowText(theApp.m_ipoScritptid.c_str());
}
bool CSetAppId::IsRegId(const char* pData)
{
	for(int i = 0;pData[i] != '\0';i++)
	{
		if (!(pData[i] >= '0' && pData[i] <='9') && pData[i] !='-')
			return false;
	}

	return true;
}
void CSetAppId::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (PathFileExistsA(theApp.m_strInsPath.c_str()))
	{
		string configpath = "";
		configpath = strprintf("%s",theApp.m_strInsPath);
		configpath+= strprintf("\\%s","sharkfundclient.conf");
		string strFile = CJsonConfigHelp::getInstance()->GetConfigRootStr(configpath);
		if (strFile == _T(""))
		{
			return;
		}
		Json::Reader reader;  
		Json::Value root; 

		if (!reader.parse(strFile, root)) 
			return;

		CString strTemp = _T("");

		GetDlgItem(IDC_BETID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				UiFun::MessageBoxEx(_T("������AppID��ʽ����ȷ") , _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				Json::Value setscriptid = root["script"];
				ASSERT(!setscriptid.isNull());
				setscriptid["betscript"]= strTemp.GetString();
				root["script"]=setscriptid;
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_GRBID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				UiFun::MessageBoxEx(_T("�����AppID��ʽ����ȷ"), _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				Json::Value setscriptid = root["script"];
				ASSERT(!setscriptid.isNull());
				setscriptid["redpacketscript"]= strTemp.GetString();;
				root["script"]=setscriptid;
			}
		}

		strTemp = _T("");
		GetDlgItem(IDC_IPOID)->GetWindowText(strTemp);
		if (strTemp != _T(""))
		{
			if (!IsRegId(strTemp))
			{
				
				UiFun::MessageBoxEx(_T("IPO AppID��ʽ����ȷ"), _T("Error") ,MFB_OK|MFB_ERROR );
			}else
			{
				Json::Value setscriptid = root["script"];
				ASSERT(!setscriptid.isNull());
				setscriptid["iposcript"]= strTemp.GetString();
				root["script"]=setscriptid;
			}
		}

		CStdioFile  File;
		string strpath = theApp.m_strInsPath;
		strpath+="\\sharkfundclient.conf";
		File.Open((LPCTSTR)(LPSTR)strpath.c_str(),CFile::modeWrite | CFile::modeCreate); 
		string strfile = root.toStyledString();
		File.WriteString(strfile.c_str());
		File.Close();
	}

	theApp.ParseUIConfigFile(theApp.m_strInsPath);
	CDialogBase::OnOK();
}


BOOL CSetAppId::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	m_headText.SetFont(100, _T("΢���ź�"));
	m_headText.SetTextColor(RGB(255,255,255));	
	m_headText.SetWindowText(_T("����Ӧ��ID"));
	
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
	CRect rect ;
	m_rBtnClose.GetClientRect(rect);

	RECT ret;
	GetWindowRect(&ret);
	m_rBtnClose.SetWindowPos(NULL ,(ret.right-ret.left)-rect.Width() - 4 , 2 , 0 , 0 , SWP_NOSIZE); 

	m_rBtnOk.SetBitmaps( IDB_BITMAP_BUT1 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText(_T("ȷ ��")) ;
	//m_rBtnOk.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnOk.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnOk.SizeToContent();

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_BUT1 , RGB(255, 255, 0) , IDB_BITMAP_BUT1 , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText(_T("ȡ ��")) ;
	//m_rBtnCancel.SetFontEx(20 , _T("΢���ź�"));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	m_rBtnCancel.SizeToContent();

	GetDlgItem(IDC_STATIC_BETID)->SetWindowText(_T("������ID:"));
	GetDlgItem(IDC_STATIC_GRBID)->SetWindowText(_T("�����ID"));

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDaluft();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSetAppId::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}
