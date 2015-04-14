// TimeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game.h"
#include "TimeDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"
// CTimeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTimeDlg, CDialogEx)

CTimeDlg::CTimeDlg (CWnd* pParent /*=NULL*/)
	: CDialogEx(CTimeDlg::IDD, pParent)
{

}

CTimeDlg::~CTimeDlg()
{
}

void CTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_TIME, timeCtrl);
}


BEGIN_MESSAGE_MAP(CTimeDlg, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CTimeDlg 메시지 처리기입니다.


BOOL CTimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	CRect rectTime ;
	GetClientRect (&rectTime) ;		// 자신의 크기를 받는다.

	// View의 위치에 따라 상대적인 위치를 갖는다.
	SetWindowPos (NULL, 
		rectView.Width () -rectTime.Width (), 
		(rectView.Height () -rectTime.Height ()) /2,
		rectView.Width (), 
		(rectView.Height () -rectTime.Height ()) /2 +rectTime.Height (), 
		SWP_NOSIZE) ;
	
	// 색 처리
	timeCtrl.SetBarColor (0x00FFFFFF & ~color) ;	// 바의 색
	//timeCtrl.SetBkColor (color) ;		// 바의 배경색
	SetBackgroundColor (color) ;		// 다이알로그 배경색
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CTimeDlg::OnClose()
{
	DestroyWindow () ;
	CDialogEx::OnClose();
}
