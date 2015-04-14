// LevelDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game.h"
#include "LevelDlg.h"
#include "GameDoc.h"
#include "GameView.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CLevelDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLevelDlg, CDialog)

CLevelDlg::CLevelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLevelDlg::IDD, pParent)
{
	nRow = nCol = time = 0;
	strLev = _T("초급");
	strRank = ld_font = _T("");
}

CLevelDlg::~CLevelDlg()
{
}

void CLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_FONT, ld_font);
	DDX_Control(pDX, IDC_COMBO_FONT, ld_cFont);
}


BEGIN_MESSAGE_MAP(CLevelDlg, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_EASY, &CLevelDlg::OnBnClickedButtonEasy)
	ON_BN_CLICKED(IDC_BUTTON_NORMAL, &CLevelDlg::OnBnClickedButtonNormal)
	ON_BN_CLICKED(IDC_BUTTON_HARD, &CLevelDlg::OnBnClickedButtonHard)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT, &CLevelDlg::OnCbnSelchangeComboFont)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_COLOR, &CLevelDlg::OnBnClickedButtonColor)
END_MESSAGE_MAP()


// CLevelDlg 메시지 처리기입니다.

// 초급 선택
void CLevelDlg::OnBnClickedButtonEasy()
{
	nRow =3 ; nCol =4 ; time = 19;
	strLev =_T ("초급") ;
	strRank =_T ("C:\\image\\EasyRanking.txt") ;
	CDialog ::OnOK () ;
}

// 중급 선택
void CLevelDlg::OnBnClickedButtonNormal()
{
	nRow =4 ; nCol =5 ; time = 49 ;
	strLev =_T ("중급") ;
	strRank =_T ("C:\\image\\NormalRanking.txt") ;
	CDialog ::OnOK () ;
}

// 상급 선택
void CLevelDlg::OnBnClickedButtonHard()
{
	nRow =5 ; nCol =6 ; time = 99 ; 
	strLev =_T ("상급") ;
	strRank =_T ("C:\\image\\HardRanking.txt") ;
	CDialog ::OnOK () ;
}

// 글꼴 콤보박스 
void CLevelDlg::OnCbnSelchangeComboFont()
{
	int index = ld_cFont.GetCurSel();
	ld_cFont.GetLBText(index, ld_font);
}

// 초기값
BOOL CLevelDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_STATIC_COLOR);
	pStatic->GetWindowRect(&ld_rect);
	ScreenToClient(&ld_rect);
	
	// 리스트박스의 초기값 설정
	ld_cFont.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

// 픽쳐컨트롤에 색칠
void CLevelDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CBrush newBrush, *pOldBrush;
	newBrush.CreateSolidBrush(ld_rectColor);
	pOldBrush = dc.SelectObject(&newBrush);
	dc.Rectangle(&ld_rect);
	dc.SelectObject(pOldBrush);
	newBrush.DeleteObject();
}

// 배경색 클릭이벤트
void CLevelDlg::OnBnClickedButtonColor()
{
	CColorDialog dlg;
	// 색상선택 대화상자 띄우기
	dlg.DoModal();
	ld_rectColor = dlg.GetColor();
	InvalidateRect(&ld_rect);
}


void CLevelDlg::OnCancel()
{
	CDialog::OnCancel();
}
