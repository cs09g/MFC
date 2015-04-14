#pragma once
#include "afxcmn.h"
#include "atltypes.h"


// CTimeDlg 대화 상자입니다.

class CTimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTimeDlg)

public:
	CTimeDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTimeDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TIME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 프로그래스 변수
	CProgressCtrl timeCtrl;
	virtual BOOL OnInitDialog();
	COLORREF color;	// view의 배경색을 받을 변수
	CRect rectView;	// View의 크기를 받을 변수
	afx_msg void OnClose();
};
