// RankDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game.h"
#include "RankDlg.h"
#include "afxdialogex.h"


// CRankDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRankDlg, CDialogEx)

CRankDlg::CRankDlg(CString str, CString m, CString s, CString c, UINT cnt, CString lev, CString rank, UINT point, CWnd* pParent /*=NULL*/)
	: CDialogEx(CRankDlg::IDD, pParent)
{
	member =str ; nMin =m ; nSec =s, nCsec =c, nCnt =cnt ; strLev =lev ; rankName =rank ; nPoint =point ;
	TimePoint () ;	// 생성자로 초기화 함과 동시에 시간계산
}

CRankDlg::~CRankDlg()
{
}

void CRankDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RANKLIST, rankList);
	DDX_Control(pDX, IDC_RANKLIST, rankList);
}


BEGIN_MESSAGE_MAP(CRankDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_EASY, &CRankDlg::OnBnClickedButtonEasy)
	ON_BN_CLICKED(IDC_BUTTON_NORMAL, &CRankDlg::OnBnClickedButtonNormal)
	ON_BN_CLICKED(IDC_BUTTON_HARD, &CRankDlg::OnBnClickedButtonHard)
	ON_BN_CLICKED(IDC_BUTTON_DELALL, &CRankDlg::OnBnClickedButtonDelall)
END_MESSAGE_MAP()


// CRankDlg 메시지 처리기입니다.

BOOL CRankDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect gv_rect ;
	rankList.GetClientRect (&gv_rect) ;	// 리스트 컨트롤의 사이즈 받음

	// 구분자 정의
	rankList.InsertColumn (0, _T ("순 위"), LVCFMT_CENTER, 70) ;		
	rankList.InsertColumn (1, _T ("이 름"), LVCFMT_CENTER, 70) ;	
	rankList.InsertColumn (2, _T ("점 수"), LVCFMT_CENTER, 70) ;
	rankList.InsertColumn (3, _T ("시 간"), LVCFMT_CENTER, 100) ;
	rankList.InsertColumn (4, _T ("클릭 수"), LVCFMT_CENTER, 70) ;
	rankList.InsertColumn (5, _T ("난이도"), LVCFMT_CENTER, gv_rect.Width () -380) ;
	
	// 저장 목록 불러옴
	AutoLoad () ;

	// 새로 추가 된 랭크 기록
	CString str ;
	str.Format (_T ("%u"), nRank) ;
	int cnt =rankList.InsertItem (rankList.GetItemCount (), str) ;	// 순위
	rankList.SetItemText (cnt, 1, member) ;		// 이름
	str.Format (_T ("%u"), nPoint) ;	
	rankList.SetItemText (cnt, 2, str) ;		// 점수
	rankList.SetItemText (cnt, 3, nMin +_T(":")+nSec+_T(":") +nCsec) ;		// 시간
	str.Format (_T ("%u"), nCnt) ;		
	rankList.SetItemText (cnt, 4, str) ;		// 클릭 수
	rankList.SetItemText (cnt, 5, strLev) ;		// 난이도

	SortRank (rankName) ;		// 작성된 리스트를 정렬한다.

	int nFind =rankList.GetItemCount (), idx ;

	for (idx =0 ; idx < nFind ; idx++) {
		if (rankList.GetItemText (idx, 1) == member &&
			_ttoi (rankList.GetItemText (idx, 2)) == nPoint &&
			_ttoi (rankList.GetItemText (idx, 4)) == nCnt) break ;
	}

	rankList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	rankList.SetItemState (idx, LVIS_FOCUSED | LVIS_SELECTED, 0x000F) ;

	return TRUE;  // return TRUE unless you set the focus to a control
}

// 창이 꺼지면 자동으로 저장함
void CRankDlg::AutoSave(void)
{	
	CStdioFile file (rankName, 
		CFile::shareDenyNone /* 다른 프로세서의 파일 접근을 가능하게 한다 */
		/* 파일이 존재하지 않으면 txt 파일을 생성하고, 처리 type은 binary 이다.
			파일이 존재하면 덮어 쓴다. */
		|CStdioFile ::modeCreate 
		|CStdioFile ::modeNoTruncate 
		|CStdioFile ::modeWrite 
		|CStdioFile ::typeBinary) ;	

	// 데이터를 쓴다
	for (int i =0 ; i < rankList.GetItemCount () ; i++)
		for (int j =0 ; j < 6 ; j++) 
			file.WriteString (rankList.GetItemText (i, j) +"\n") ;	

	file.Close () ;
} 

// 창이 켜지면 자동으로 불러옴
void CRankDlg::AutoLoad(void)
{
	// Ranking.txt를 읽기전용으로 열기
	CFile file (rankName, 
		CFile::shareDenyNone 
		|CFile ::modeNoTruncate 
		|CFile ::modeCreate 
		|CFile ::modeRead) ;	
	
	CString str ;

	CArchive *ar =new CArchive (&file, CArchive ::load) ;	
	
	bool TF =true ;		// 한 줄씩 쓰기위한 boolean 변수
	int col =1, row =-1 ;	

	// 데이터를 읽는다.
	while (ar ->ReadString (str)) {
		// 한 줄을 모두 읽었을때
		if (col == 6) {		
			TF =!TF ;		
			col =1 ;
		}

		// 행을 입력받는다.
		if (TF) {	
			row =rankList.GetItemCount () ;
			rankList.InsertItem (row, str) ;
			TF =!TF ;
		}
		// 열을 하나씩 세팅한다.
		else rankList.SetItemText (row, col++, str) ;	
	}
	// 랭크 처리
	if (row < 0)  nRank =1 ;		// row가 -1이면 첫 순위이므로 1/
	else nRank =row +2 ;		// row가 -1이 아니면 순위가 있으므로 겟아이템카운트의 갯수의 2를 더함


	ar ->Close () ; 
	file.Close () ;
}

// 창이 닫힐때 데이터를 저장한다
void CRankDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	AutoSave () ;
}

// 랭크정렬
void CRankDlg::SortRank(CString rankName)
{
	int row =rankList.GetItemCount () ;
	int pt1, pt2, cnt1, cnt2 ;

	// 점수와 클릭수를 기준으로 정렬한다
	for (int i =0 ; i < row -1 ; i++)
		for (int j =i +1 ; j < row ; j++) {

			if(CompareRank (&pt1, &pt2, 2, i, j)) SwapRank(i, j);		
			
			if (pt1 == pt2) 
				if(CompareRank (&cnt1, &cnt2, 4, i, j)) SwapRank(i, j);
		}
	// 정렬
	NaturalMergeSort (0, row) ;
}

// 랭크순위교환
void CRankDlg::SwapRank(int val1, int val2)
{
	CString temp =rankList.GetItemText (val2, 0) ;

	rankList.SetItemText (val2, 0, rankList.GetItemText (val1, 0)) ;
	rankList.SetItemText (val1, 0, temp) ;
}

// 현재 순위의 값과 현재 순위가 맞는지
// return 1 -- 랭크 불일치
// return 0 -- 랭크 일치
int CRankDlg::CompareRank(int * val1, int * val2, int col, int i, int j)
{
	int rank1 =_ttoi (rankList.GetItemText (i, 0)), rank2 =_ttoi (rankList.GetItemText (j, 0)) ;

	*val1 =_ttoi (rankList.GetItemText (i, col)) ; 
	*val2 =_ttoi (rankList.GetItemText (j, col)) ;

	// 순위에 대해
	if (col == 2  && (*val1 < *val2 && rank1 < rank2)) return 1;

	// 클릭 수에 대해
	else if (col == 4 && (*val1 < *val2 && rank1 > rank2)) return 1;
	
	return 0;
}

// 순위를 바꾼후 바뀐 순위대로 재 정렬 자연병합정렬
void CRankDlg::NaturalMergeSort(int top, int bottom)
{
	int i, j, val1, val2 ;	

	// top 부터 순서대로 정렬 되어있는곳 까지 탐색
	for (i =top +1 ; i < bottom ; i++) {
		val1 =_ttoi (rankList.GetItemText (i -1, 0)) ;
		val2 =_ttoi (rankList.GetItemText (i, 0)) ;
		if (val1 > val2) break ;
	}

	if (i >= bottom && !top) return ;		// 만약 top가 0이고 i가 bottom보 작지 않다면 정렬이 되어있는 것이므로 return

	// 탐색 후에 또 다시 순서대로 정렬 되어있는 곳까지 탐색
	for (j =i +1 ; j < bottom ; j++) {
		val1 =_ttoi (rankList.GetItemText (j -1, 0)) ;
		val2 =_ttoi (rankList.GetItemText (j, 0)) ;
		if (val1 > val2) break ;
	}

	// 서브리스트가 하나만 탐색 되지 않을 경우
	if (j <= bottom) QuickSort (top, j -1) ;	// 2개의 탐색된 서브 리스트를 하나로 합쳐 정렬

	if (j < bottom) NaturalMergeSort (j, bottom) ;	// 탐색을 List의 끝까지 하지 않았다면 top를 정렬 시킨 서브 리스트의 다음 Element로 준다.
	else NaturalMergeSort (0, bottom) ;		// 만약 탐색을 List의 끝까지 했다면 top를 0으로 초기화 한후 다시 시작
}

// 퀵 소트
void CRankDlg::QuickSort(int first, int last)
{
    int pivot, left, right ;

     if (first < last) {
         pivot =(last +first) /2 ; 
		 left =first ; right =last ;
         while (left < right) {
			 while (_ttoi (rankList.GetItemText (left, 0)) <= _ttoi (rankList.GetItemText (pivot, 0)) && left < last) left++ ;
             while (_ttoi (rankList.GetItemText (right, 0)) > _ttoi (rankList.GetItemText (pivot, 0))) right-- ;
             if (left < right) SwapItem (left, right) ;	// 교환 
         }
		 SwapItem (pivot, right) ;
     
         QuickSort (first, right -1) ;
         QuickSort (right +1, last) ;
    }
}

// 리스트 컨트롤의 아이템을 교환
void CRankDlg::SwapItem(int val1, int val2)
{
	// val1행의 데이타를 temp에 넣고
	CString temp[] ={ rankList.GetItemText (val1, 0), rankList.GetItemText (val1, 1), 
		rankList.GetItemText (val1, 2), rankList.GetItemText (val1, 3),
		rankList.GetItemText (val1, 4) } ;

	// val1행에 val2행 값을 넣는다.
	for (int i =0 ; i < 5 ; i++) 
		rankList.SetItemText (val1, i, rankList.GetItemText (val2, i)) ;

	// val2행에 val1행 값을 넣는다.
	for (int i =0 ; i < 5 ; i++)
		rankList.SetItemText (val2, i, temp[i]) ;	
}

// Easy 버튼 클릭 이벤트
void CRankDlg::OnBnClickedButtonEasy()
{
	AutoSave () ;		// 이전 목록은 저장
	rankName =_T ("C:\\image\\EasyRanking.txt") ;	// 파일명 정의
	rankList.DeleteAllItems () ;		// 목록을 비우고
	AutoLoad () ;		// 현재 목록 불러오기
}

// Normal 버튼 클릭 이벤트
void CRankDlg::OnBnClickedButtonNormal()
{
	AutoSave () ;		
	rankName =_T ("C:\\image\\NormalRanking.txt") ;	
	rankList.DeleteAllItems () ;	
	AutoLoad () ;		
}

// Hard 버튼 클릭 이벤트
void CRankDlg::OnBnClickedButtonHard()
{
	AutoSave () ;	 
	rankName =_T ("C:\\image\\HardRanking.txt") ;	 
	rankList.DeleteAllItems () ; 
	AutoLoad () ;		 
}

// 초기화 버튼 클릭
void CRankDlg::OnBnClickedButtonDelall()
{
	// 우선 리스트 컨트롤에 있는 값을 비우고, 
	// 해당 리스트 컨트롤의 데이터가 있는 file의 이름을 얻은 다음
	// 해당 파일을 초기화 한다.
	rankList.DeleteAllItems () ;	
	FILE *fp;
	CFile file (rankName, 
		CFile::shareDenyNone 
		| CFile ::modeCreate 
		| CFile ::modeNoTruncate 
		| CFile ::modeRead) ;	
	strLev = file.GetFileName();

	// 파일 내용 초기화
	if(strLev == _T("EasyRanking.txt"))
		fp = fopen("c:\\image\\EasyRanking.txt",  "w");
	else if(strLev == _T("NormalRanking.txt"))
		fp = fopen("c:\\image\\NormalRanking.txt",  "w");
	else 
		fp = fopen("c:\\image\\HardRanking.txt",  "w");
}

// 흘러간 시간을 나타내기 위한 함수
void CRankDlg::TimePoint(void)
{
	// 상급일때 시간 계산
	if (rankName == _T ("C:\\image\\HardRanking.txt")) {
		nMin.Format (_T ("0%d"), 1 -_ttoi (nMin)) ;
		if ((39 -_ttoi (nSec)) > 10) nSec.Format (_T ("%d"), 39 -_ttoi (nSec)) ;
		else nSec.Format (_T ("0%d"), 39 -_ttoi (nSec)) ;

		if ((100 -_ttoi (nCsec)) > 10) nCsec.Format (_T ("%d"), 100 -_ttoi (nCsec)) ;
		else nCsec.Format (_T ("0%d"), 100 -_ttoi (nCsec)) ;
	}

	// 중급일때 시간 계산
	if (rankName == _T ("C:\\image\\NoramlRanking.txt")) {
		if ((49 -_ttoi (nSec)) > 10) nSec.Format (_T ("%d"), 49 -_ttoi (nSec)) ;
		else nSec.Format (_T ("0%d"), 49 -_ttoi (nSec)) ;

		if ((100 -_ttoi (nCsec)) > 10) nCsec.Format (_T ("%d"), 100 -_ttoi (nCsec)) ;
		else nCsec.Format (_T ("0%d"), 100 -_ttoi (nCsec)) ;
	}

	// 초급일때 시간 계산
	if (rankName == _T ("C:\\image\\EasyRanking.txt")) {
		if((19 -_ttoi (nSec)) > 10) nSec.Format (_T ("%d"), 19 -_ttoi (nSec)) ;
		else nSec.Format (_T ("0%d"), 19 -_ttoi (nSec)) ;

		if ((100 -_ttoi (nCsec)) > 10) nCsec.Format (_T ("%d"), 100 -_ttoi (nCsec)) ;
		else nCsec.Format (_T ("0%d"), 100 -_ttoi (nCsec)) ;
	}
}
