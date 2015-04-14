// RankName.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game.h"
#include "RankNameDlg.h"
#include "afxdialogex.h"


// CRankNameDlgDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRankNameDlg, CDialogEx)

CRankNameDlg::CRankNameDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRankNameDlg::IDD, pParent)
	, name(_T(""))
{

}

CRankNameDlg::~CRankNameDlg()
{
}

void CRankNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NAME, name);
}


BEGIN_MESSAGE_MAP(CRankNameDlg, CDialogEx)
END_MESSAGE_MAP()


BOOL CRankNameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetDlgItem (IDC_NAME) ->SetFocus () ;	// 이름 입력창을 포커싱
	return TRUE;  // return TRUE unless you set the focus to a control
}
