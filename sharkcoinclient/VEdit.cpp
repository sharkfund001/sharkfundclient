// VEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "VEdit.h"


// CVEdit

IMPLEMENT_DYNAMIC(CVEdit, CEdit)

CVEdit::CVEdit()
{
	m_nFontSize = 90;
	m_csFontName = _T("΢���ź�");
	m_pFont = new CFont;
	m_pFont->CreatePointFont(m_nFontSize, m_csFontName);
	m_bItalic = FALSE;
	m_bBold = FALSE;
	m_bUnderLine = FALSE;
	m_clrText = RGB(0, 0, 0);
	m_clrBk = RGB(255, 255, 255);
	m_clrFMBk = RGB(255, 255, 255);
	m_bkBrush.CreateSolidBrush(m_clrBk);
	m_bkFMBrush.CreateSolidBrush(m_clrBk);

}

CVEdit::~CVEdit()
{
	if(m_pFont)
	{
		delete m_pFont;
		m_pFont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CVEdit, CEdit)
	ON_WM_NCCALCSIZE()
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CVEdit ��Ϣ�������




void CVEdit::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(!m_pFont)
	{
		CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
		return ;
	}

	// �����������������
	CString csTest = _T("�༭��VCenterAlign");
	SIZE sz;

	// ����DC�����ò��ռ��������
	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(m_pFont);
	// ���� m_Font ������ַ��߿�
	GetTextExtentPoint32(pDC->GetSafeHdc(), csTest, csTest.GetLength(), &sz);
	pDC->SelectObject(pOldFont);

	// �༭����������ұ߿�Ĭ�ϸ�Ϊ2���أ�����ĸ߶ȼ�������Ҫ��ȥ���±߿�߶�
	int nFrame = 2*2;
	// �༭��Ĭ�Ͽɱ༭������Ҳ��1�����ر߿�
	int nTextTopFrame = 1;

	double dTemp =  (lpncsp->rgrc[0].bottom-lpncsp->rgrc[0].top-sz.cy-nFrame)/2.0 ;
	if(dTemp>(int)dTemp)
		dTemp = (double)( (int)dTemp+1 );

	int nBorder = (int)dTemp -nTextTopFrame;
	if(nBorder<0)
		nBorder = 0;

	lpncsp->rgrc[0].top += nBorder;
	//lpncsp[0].rgrc->bottom = lpncsp[0].rgrc->top + sz.cy + nBorder;

	CEdit::OnNcCalcSize(bCalcValidRects, lpncsp);
}


void CVEdit::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	SetFont(m_pFont);
	SetFontHelper();
	CEdit::PreSubclassWindow();
}

void CVEdit::SetNewFont( int nFontSize, CString csFontName/* = _T("΢���ź�")*/ )
{
	m_nFontSize = nFontSize;
	m_csFontName = csFontName;
	SetFontHelper();
}

void CVEdit::SetFontHelper()
{
	if(m_pFont)
	{
		if(m_pFont->m_hObject)
			m_pFont->DeleteObject();
	}
	m_pFont = new CFont;

	CDC* pDC = this->GetDC();
	LOGFONT lgFont;
	lgFont.lfCharSet = DEFAULT_CHARSET;
	lgFont.lfClipPrecision = 0;
	lgFont.lfEscapement = 0;
	_tcscpy_s(lgFont.lfFaceName, m_csFontName);
	lgFont.lfHeight = m_nFontSize;
	lgFont.lfItalic = m_bItalic;
	lgFont.lfOrientation = 0;
	lgFont.lfOutPrecision = 0;
	lgFont.lfPitchAndFamily = 2;
	lgFont.lfQuality = 0;
	lgFont.lfStrikeOut = 0;
	lgFont.lfUnderline = m_bUnderLine;
	lgFont.lfWidth = 0;
	if(m_bBold)
		lgFont.lfWeight = FW_BOLD;
	else
		lgFont.lfWeight = FW_NORMAL;

	m_pFont->CreatePointFontIndirect(&lgFont);
	SetFont(m_pFont);

	// ͨ�� MoveWindow ���� WM_NCCALCSIZE ��Ϣ�ķ��ͣ���ע����봰�ڴ�С�б仯ʱ���ܴ����ɹ�
	CRect rcClient;
	GetWindowRect(&rcClient);
	GetParent()->ScreenToClient(&rcClient);
	// �ı䴰�ڴ�С������ WM_NCCALCSIZE
	MoveWindow(rcClient.left, rcClient.top, rcClient.Width()+1, rcClient.Height());
	// �ָ����ڴ�С��Ҳ������ WM_NCCALCSIZE
	MoveWindow(rcClient.left, rcClient.top, rcClient.Width()-1, rcClient.Height()); 

}

void CVEdit::SetFontStyle( BOOL bBold/*=TRUE*/, BOOL bItalic/*=FALSE*/, BOOL bUnderLine/*=FALSE*/ )
{
	m_bBold = bBold;
	m_bItalic = bItalic;
	m_bUnderLine = bUnderLine;
	// �ؽ�����
	SetFontHelper();
}


HBRUSH CVEdit::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	// TODO:  �ڴ˸��� DC ���κ�����

	DrawBkAndFrame();
	pDC->SetTextColor(m_clrText);
	//pDC->SetBkColor(m_clrBk);
	pDC->SetBkMode(TRANSPARENT);

	// TODO:  �����Ӧ���ø����Ĵ�������򷵻ط� null ����
	return m_bkBrush;
}

void CVEdit::SetTextColor( COLORREF clrText )
{
	m_clrText = clrText;
}

void CVEdit::SetFMColor(COLORREF clrFMBk)
{
	m_clrFMBk = clrFMBk;
	// �ؽ�������ˢ
	if(m_bkFMBrush.m_hObject)
	{
		m_bkFMBrush.DeleteObject();
	}
	m_bkFMBrush.CreateSolidBrush(m_clrFMBk);
}

void CVEdit::SetBkColor( COLORREF clrBk )
{
	m_clrBk = clrBk;
	// �ؽ�������ˢ
	if(m_bkBrush.m_hObject)
	{
		m_bkBrush.DeleteObject();
	}
	m_bkBrush.CreateSolidBrush(m_clrBk);
}

void CVEdit::DrawBkAndFrame()
{
	CRect rcClient, rcWnd;
	CDC* pDC = this->GetDC();
	GetClientRect(&rcClient);
	GetWindowRect(&rcWnd);
	ScreenToClient(&rcWnd);

	CRect rcFill;
	rcFill = rcWnd;
	rcFill.bottom = rcClient.top ;
	pDC->FillRect(rcFill, &m_bkBrush);

	rcFill = rcWnd;
	rcFill.top = rcClient.bottom ;
	pDC->FillRect(rcFill, &m_bkBrush);

	pDC->FrameRect(rcWnd, &m_bkFMBrush);


	ReleaseDC(pDC);
}
