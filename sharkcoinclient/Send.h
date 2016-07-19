#pragma once


// CSend �Ի���
#include "CJFlatComboBox.h"
#include "vedit.h"

class CSend : public CDialogBar
{
	DECLARE_DYNAMIC(CSend)

public:
	CSend();   // ��׼���캯��
	virtual ~CSend();

// �Ի�������
	enum { IDD = IDD_DIALOG_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CRoundButton     m_rBtnAddbook;
	map<string,uistruct::LISTADDR_t> m_mapAddrInfo;
	CShadeButtonST         m_rBtnSend;
	CStaticTrans           m_strTx1  ; 
	CCJFlatComboBox		   m_addrbook;
	CCJFlatComboBox        m_cboUnit;
private:
	BOOL AddListaddrDataBox();
	void ModifyComboxItem();
	void InsertComboxIitem();
public:
	HBITMAP		     m_pBmp; 
public:
	void		     SetBkBmpNid( UINT nBitmapIn ) ;
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg void OnBnClickedButtonSelect();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnCbnSelchangeComboPayAddress();
	afx_msg LRESULT OnShowListaddrData(  WPARAM wParam, LPARAM lParam ) ;
	// Ŀ�ĵ�ַ
	CVEdit m_editDestAddr;
	// ��ǩ
	CVEdit m_editLabel;
	// ���
	CVEdit m_editAmout;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
