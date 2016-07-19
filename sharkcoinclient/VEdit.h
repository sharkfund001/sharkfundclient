#pragma once


// CVEdit

class CVEdit : public CEdit
{
	DECLARE_DYNAMIC(CVEdit)

public:
	CVEdit();
	virtual ~CVEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void PreSubclassWindow();
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);

public:
	CFont* m_pFont; // ����ָ��
	int m_nFontSize; // �����С
	CString m_csFontName; // ��������
	COLORREF m_clrText; // �ı���ɫ
	COLORREF m_clrBk; // ������ɫ
	BOOL m_bItalic; // б��
	BOOL m_bBold; // ����
	BOOL m_bUnderLine; // �»���
	CBrush m_bkBrush; // ������ˢ
	COLORREF m_clrFMBk;	//�߿򱳾�
	CBrush m_bkFMBrush;	//�߿�ˢ

protected:
	void SetFontHelper(); // ���´�������������
	void DrawBkAndFrame(); // �������ͱ߿�

public:
	void SetTextColor(COLORREF clrText); // �����ı���ɫ
	void SetBkColor(COLORREF clrBk); // ���ñ�����ɫ
	void SetFMColor(COLORREF clrFMBk);	//���ñ߿򱳾���ɫ
	void SetNewFont(int nFontSize, CString csFontName = _T("΢���ź�")); // ����������
	void SetFontStyle(BOOL bBold=TRUE, BOOL bItalic=FALSE, BOOL bUnderLine=FALSE); // ����������ʽ

};


