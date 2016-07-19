#pragma once


// CSet �Ի���

class CSet : public CDialogBar
{
	DECLARE_DYNAMIC(CSet)

public:
	CSet();   // ��׼���캯��
	virtual ~CSet();

// �Ի�������
	enum { IDD = IDD_DIALOG_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
public:
	CShadeButtonST         m_rBtnRpc;
	CShadeButtonST         m_rBtnQbjm;
	CShadeButtonST         m_rBtnXgmm;
	CShadeButtonST         m_rBtnQbsd;
	CShadeButtonST         m_rBtnQbbf;
	CShadeButtonST         m_rBtnDcsy;
	CShadeButtonST         m_rBtnDrsy;
	CShadeButtonST         m_rBtnSzyyid;
	CShadeButtonST         m_rBtnFfmrsz;
	CShadeButtonST         m_rBtnGbts;
public:
	void SetBkBmpNid( UINT nBitmapIn ) ;
	bool GetFileName(CString &fileName,CString strexe );
	void AddImportWalletAndBookAddr(CString fileName);
	void WriteExportWalletAndBookAddr(CString fileName);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg void OnBnClickedButtonPrc();				//rpc����
	afx_msg void OnBnClickedButtonQbjm();				//Ǯ������
	afx_msg void OnBnClickedButtonXgmm();				//�޸�����
	afx_msg void OnBnClickedButtonQbsd();				//Ǯ������
	afx_msg void OnBnClickedButtonQbbf();				//Ǯ������
	afx_msg void OnBnClickedButtonDcsy();				//����˽Կ
	afx_msg void OnBnClickedButtonDrsy();				//����˽Կ
	afx_msg void OnBnClickedButtonYyid();				//Ӧ��ID
	afx_msg void OnBnClickedButtonHfmrsz();				//�ָ�Ĭ������
	afx_msg void OnBnClickedButtonGbts();				//�ر���ʾ
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
