
// sharkcoinclientDlg.h : ͷ�ļ�
//

#pragma once

#include "IndTitleBar.h"
#include "ProgStatusBar.h"
#include "MainDlg.h"
#include "Send.h"
#include "Rece.h"
#include "Deal.h"
#include "Ipo.h"
#include "Set.h"
#include "BalloonTip.h"
#include "RPCDlg.h"
#include "OutGifDlg.h"

// CsharkcoinclientDlg �Ի���
class CsharkcoinclientDlg : public CDialogEx
{
// ����
public:
	CsharkcoinclientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_sharkcoinCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIndTitleBar		*m_pTitleBar;				//*m_pIndToolbar;			
	CProgStatusBar      *m_pStatusBar;				//״̬��					

	CMainDlg            *m_pMainDlg;				//������			
	CSend               *m_pSendDlg;				//���ͽ���
	CRece               *m_pReceDlg;				//���ս���
	CDeal               *m_pDealDlg;				//��������
	CIpo                *m_pIpoDlg;					//Ӧ�����
	CSet                *m_pSetDlg;					//���ý���
	CRPCDlg             *m_pRPCDlg;					//RPC����
	COutGifDlg          *m_pOutGifDlg;				
public:
	std::map< UINT , CDialogBar * >  m_dlgMap ; 
	void       ShowDialog(UINT dlgid)  ;
	void       DestroyDlg();
	void       ClosWallet();
	void       CloseThread();
	void       StopSever();
	void       CloseApp();
	void       DeleteTray();
	CDialogBar *p_CurSelDlg   ;   //��ǰѡ��Ի���ָ��
	int        dlgType;
public:
	static void  BlockDataProc(char *pRecBuffer, int nRecLen ,WPARAM wParam);
public:
	afx_msg void OnBnClickedButtonMain();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonRece();
	afx_msg void OnBnClickedButtonDeal();
	afx_msg void OnBnClickedButtonIpo();
	afx_msg void OnBnClickedButtonSet();

	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonMin();
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnShowTask(WPARAM wParam,LPARAM lParam) ;
	afx_msg LRESULT OnPopupBar(WPARAM wParam,LPARAM lParam) ;
public:
	void InitialRpcCmd();      //��ʼ��RPC��������
	void LoadWalletAddress();  //����Ǯ����ַ 
	bool GetFileName(CString &fileName,CString strexe );
	void  ClosWalletWind();
	void OnCloseWriteAppFee();
	void ToTray();
	void ShowStateTip(UINT nButtonID);

	CBalloonTip*  m_pBalloonTip; //ð����ʾ
	CMyQueue m_barpoomesg;
	CMenu menu;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

