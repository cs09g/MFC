
#pragma once
#include "afxwin.h"
#include "atltypes.h"


// CLevelDlg 대화 상자입니다.

class CLevelDlg : public CDialog
{
	DECLARE_DYNAMIC(CLevelDlg)

public:
	CLevelDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLevelDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LEVEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	int nRow;
	int nCol;
	int time;

	afx_msg void OnBnClickedButtonEasy();
	afx_msg void OnBnClickedButtonNormal();
	afx_msg void OnBnClickedButtonHard();
	afx_msg void OnCbnSelchangeComboFont();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonColor();
	virtual BOOL OnInitDialog();

	CString strLev;	// 랭킹을 저장할 txt파일명
	CString strRank;	// 경로를 포함한 파일명
	CString ld_font;

	CComboBox ld_cFont;
	CRect ld_rect;
	COLORREF ld_rectColor;
	virtual void OnCancel();
};
