#pragma once


// CRankNameDlg 대화 상자입니다.

class CRankNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRankNameDlg)

public:
	CRankNameDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRankNameDlg();
// 대화 상자 데이터입니다.
	enum { IDD = IDD_RANKNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString name;
	virtual BOOL OnInitDialog();
};
