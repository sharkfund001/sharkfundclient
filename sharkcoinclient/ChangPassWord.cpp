// ChangPassWord.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sharkcoinclient.h"
#include "ChangPassWord.h"
#include "afxdialogex.h"


// CChangPassWord �Ի���

IMPLEMENT_DYNAMIC(CChangPassWord, CDialogBase)

CChangPassWord::CChangPassWord(CWnd* pParent /*=NULL*/)
	: CDialogBase(CChangPassWord::IDD, pParent)
{

}

CChangPassWord::~CChangPassWord()
{
}

void CChangPassWord::DoDataExchange(CDataExchange* pDX)
{
	CDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATICODLE, m_oldpassword);
	DDX_Control(pDX, IDC_STATICNEW, m_newpassword);	
	DDX_Control(pDX, IDC_STATICCOMFIRED, m_comfiredpassword);	
	DDX_Control(pDX, IDC_STATIC4, m_headText);	
	DDX_Control(pDX, IDC_CLOSE, m_rBtnClose);
	DDX_Control(pDX, IDCANCEL, m_rBtnCancel);
	DDX_Control(pDX, IDOK, m_rBtnOk);
}


BEGIN_MESSAGE_MAP(CChangPassWord, CDialogBase)
	ON_BN_CLICKED(IDOK, &CChangPassWord::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CLOSE, &CChangPassWord::OnBnClickedClose)
END_MESSAGE_MAP()


// CChangPassWord ��Ϣ�������


void CChangPassWord::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString inputOld = _T("");
	CString confiedNew= _T("");
	CString confiedPassWord = _T("");
	GetDlgItem(IDC_EDIT_OLDPASSWORD)->GetWindowText(inputOld);
	GetDlgItem(IDC_EDIT_NEWPASSWORD)->GetWindowText(confiedNew);
	GetDlgItem(IDC_EDIT_CONFIREDPASSWORD)->GetWindowText(confiedPassWord);
	int pos = confiedNew.Find(_T(" "));
	if (pos >=0)
	{
		UiFun::MessageBoxEx(_T("�����������в����пո�") , _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	if (inputOld == _T(""))
	{
		UiFun::MessageBoxEx(_T("�����������") ,   _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (confiedNew == _T(""))
	{
		UiFun::MessageBoxEx(_T("������������") ,   _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (confiedPassWord == _T(""))
	{
		UiFun::MessageBoxEx(_T("������ȷ��������") ,   _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}
	if (IsAllDigtal(confiedNew))
	{
		UiFun::MessageBoxEx(_T("���벻��ȫ������") ,   _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	if (confiedNew.GetLength()<6)
	{
		UiFun::MessageBoxEx(_T("�������������λ"),   _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	if (strcmp(confiedPassWord,confiedNew) != 0)
	{
		UiFun::MessageBoxEx(_T("ȷ��������������벻һ��"),   _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}

	string strCommand;
	strCommand = strprintf("%s %s %s",_T("walletpassphrasechange"),inputOld,confiedNew);

	Json::Value root; 
	if (!CSoyPayHelp::getInstance()->SendRpc(strCommand.c_str(),root))
	{
		UiFun::MessageBoxEx(_T("�����޸Ĵ���,��鿴�������Ƿ���ȷ"),   _T("��ʾ") ,MFB_OK|MFB_TIP );
		TRACE("rpccmd walletpassphrasechange error");
		return;
	}

	bool isEntryp = root["chgpwd"].asBool();
	if (!isEntryp)
	{
		UiFun::MessageBoxEx(_T("��������벻��ȷ,����������") ,   _T("��ʾ") ,MFB_OK|MFB_TIP );
		return;
	}else{
		UiFun::MessageBoxEx(_T("��ϲ�޸�����ɹ�")  ,   _T("��ʾ") ,MFB_OK|MFB_TIP );
	}
	
	CDialogBase::OnOK();
}


void CChangPassWord::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogBase::OnCancel();
}


BOOL CChangPassWord::OnInitDialog()
{
	CDialogBase::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_headText.SetFont(100, _T("΢���ź�"));
	m_headText.SetTextColor(RGB(255,255,255));	
	m_headText.SetWindowText(_T("�޸�����"));
	m_headText.SetWindowPos( NULL , 5 , 4 , 100, 25  ,SWP_SHOWWINDOW ) ; 
	
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

	m_rBtnOk.SetBitmaps( IDB_BITMAP_TC_LAN , RGB(255, 255, 0) , IDB_BITMAP_TC_LAN , RGB(255, 255, 255) );
	m_rBtnOk.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnOk.SetWindowText(_T("ȷ��")) ;
	//m_rBtnOk.SetFontEx(12 , _T("����"));
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

	m_rBtnCancel.SetBitmaps( IDB_BITMAP_TC_ORG , RGB(255, 255, 0) , IDB_BITMAP_TC_ORG , RGB(255, 255, 255) );
	m_rBtnCancel.SetAlign(CButtonST::ST_ALIGN_OVERLAP);
	m_rBtnCancel.SetWindowText(_T("ȡ��")) ;
	//m_rBtnCancel.SetFontEx(12 , _T("����"));
	/*
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(0, 0, 0));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(0, 0, 0));
	*/
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_OUT , RGB(255, 255, 255));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_IN , RGB(200, 75, 60));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, RGB(255, 255, 255));
	m_rBtnCancel.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255, 255, 255));
	m_rBtnCancel.SizeToContent();

	GetDlgItem(IDC_STATICODLE)->SetWindowText(_T("������:"));
	GetDlgItem(IDC_STATICNEW)->SetWindowText(_T("������:"));
	GetDlgItem(IDC_STATICCOMFIRED)->SetWindowText(_T("ȷ��������:"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
