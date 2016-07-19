#pragma once
#include "DialogBase.h"
#include "StaticTrans.h"
#include "BmpBtn.h"
#include "ShadeButtonST.h"
#include "VEdit.h"

// CTxDetailDlg �Ի���

class CTxDetailDlg : public CDialogBase
{
	DECLARE_DYNAMIC(CTxDetailDlg)

public:
	CTxDetailDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTxDetailDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHOWTXDETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void ShowTxDetail(CString jsontx);
	string GetContacrDetail(uistruct::REVTRANSACTION_t tx);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedClose();
public:
	CStaticTrans     m_headText;
	CShadeButtonST   m_rBtnClose;
	CShadeButtonST   m_rBtnOk;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	void SetBkBmpNid( UINT nBitmapIn );

//public:
//	HBITMAP		     m_pBmp; 
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
