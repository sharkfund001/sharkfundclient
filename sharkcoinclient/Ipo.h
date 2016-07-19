#pragma once


// CIpo �Ի���
#include "CJFlatComboBox.h"
#include "VEdit.h"

class CIpo : public CDialogBar
{
	DECLARE_DYNAMIC(CIpo)

public:
	CIpo();   // ��׼���캯��
	virtual ~CIpo();

// �Ի�������
	enum { IDD = IDD_DIALOG_IPO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	HBITMAP		     m_pBmp; 
public:
	CListCtrlCl        m_listCtrl;
	CShadeButtonST     m_rBtnSelect;
	CShadeButtonST     m_rBtnDraw;
	CP2PBetHelp		   m_P2PBetHelp;
	string             m_appid;
	CCJFlatComboBox    m_listapp;
	CVEdit			   m_editAddr;

public:
	void  SetBkBmpNid( UINT nBitmapIn ) ;
	double GetFreeMoney(CString addr);
	void OnShowListCtrol(CString addr);
	string GetAppID(string AppName);
	void InitCombox();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL Create(CWnd* pParentWnd, UINT nIDTemplate, UINT nStyle, UINT nID);
	afx_msg void OnBnClickedButtonSelect();
	afx_msg void OnBnClickedButtonDraw();
	afx_msg void OnCbnSelchangeComboAppid();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
