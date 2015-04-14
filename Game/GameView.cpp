
// GameView.cpp : CGameView 클래스의 구현
//
#pragma comment(lib, "winmm.lib")
#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"
#include "LevelDlg.h"
#include "RankNameDlg.h"
#include "RankDlg.h"
#include "TimeDlg.h"

#include <MMSystem.h>	// 음악 재생을 위한 헤더
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGameView

IMPLEMENT_DYNCREATE(CGameView, CView)

BEGIN_MESSAGE_MAP(CGameView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGameView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_COMMAND(ID_WHITE, &CGameView::OnWhite)
	ON_COMMAND(ID_BLACK, &CGameView::OnBlack)
	ON_COMMAND(ID_USER, &CGameView::OnUser)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CGameView 생성/소멸

CGameView::CGameView()
{
	/* bool */	gv_TF =gv_scoreTF 
	/* UINT */	=gv_cnt =gv_point =gv_timeUpper 
	/* int */		=gv_min =gv_levCnt = gv_tempMin =gv_tempSec =gv_nMatchCount 
	/* DWORD */	=gv_timeDV =gv_bgDV =gv_failDV =gv_successDV
					=FALSE;
	isInit = TRUE;
	gv_sec = 19;
	gv_csec = 100;
	gv_lev = _T("초급");
	gv_rank = _T("C:\\image\\EasyRanking.txt");
	gv_font = _T("HY나무B");
	gv_minStr =gv_secStr =gv_csecStr =_T ("") ;
	CLevelDlg pLevel ;
	gv_bgColor =pLevel.ld_rectColor ;		// 배경화면 초기화
}

CGameView::~CGameView()
{
}


BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.
	return CView::PreCreateWindow(cs);
}

// CGameView 그리기

void CGameView::OnDraw(CDC* pDC)
{
	CGameDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//**** 배경색 *************
	CBrush brush (gv_bgColor);
	CBrush *pOldB =pDC ->SelectObject (&brush) ;
	CRect rect;
	GetClientRect(&rect);
	pDC->Rectangle(rect);
	pDC->SelectObject(pOldB); 
	//***********************

	// 랜덤하지 않으면 랜덤함수 호출
	if(pDoc->gd_bRandom) pDoc->OnRandom();

	int nCount = 0;
go:
	CDC memDC;
	CBitmap* pOldBmp;

	if(isInit){
		// Font 설정 Part
		CFont myFont, *pOldFont ;
		myFont.CreateFont (40, 0, 0, 0, FW_BOLD, true, false, false, 
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, DEFAULT_PITCH, gv_font) ;
		pOldFont =pDC->SelectObject (&myFont) ;

		// Text 설정 Part
		CString str =_T ("HINT") ;
		pDC->SetBkColor (gv_bgColor) ;		// 글자의 배경은 바탕과 같이 
		pDC->SetTextColor (0x00FFFFFF & ~gv_bgColor) ;		// 글자 색을 바탕화면의 반대 색으로
		pDC->SetTextAlign (TA_CENTER) ;		// 가운데 출력
		pDC->TextOutW (rect.Width () /2, rect.top +20, str) ;

		// 보여주는 파트
		pDoc->gd_nBmpFirstID = pDoc->gd_nBmpSecondID = 0 ;		// 비트맵의 이름을 초기화
		pDoc->gd_bMouse = FALSE;

		for(int i=0;i<pDoc->gd_nRow;i++)
			for(int j=0;j<pDoc->gd_nCol;j++)
				pDoc->gd_bShow[i][j] = TRUE;
	}
	memDC.CreateCompatibleDC(pDC);

	for(int n=0;n<pDoc->gd_nRow;n++){
		for(int m=0;m<pDoc->gd_nCol;m++){
			// 이미지 출력 
			int imgStartPtrX = rect.right/(pDoc->gd_nCol+2);
			int imgStartPtrY = rect.bottom/(pDoc->gd_nRow+2);
			/* 이미지 출력 */
			if(pDoc->gd_bShow[n][m]){
				pDoc->gd_bmp[n][m].LoadBitmapW(IDB_BITMAP1+pDoc->gd_nRnd[nCount]);
				pOldBmp = memDC.SelectObject(&pDoc->gd_bmp[n][m]); 
				pDC->StretchBlt(
					imgStartPtrX*(m+1), imgStartPtrY*(n+1),
					imgStartPtrX, imgStartPtrY, 
					&memDC, 0, 0, 
					pDoc->gd_bmCell.cx, pDoc->gd_bmCell.cy, SRCCOPY);
				pDoc->gd_bmp[n][m].DeleteObject();
			}
			/* 커버 출력 */
			if(!pDoc->gd_bShow[n][m]){
				CBitmap bmp;
				bmp.LoadBitmapW(IDB_BITMAP1);	// 커버 출력
				pOldBmp = memDC.SelectObject(&bmp);
				 
				pDC->StretchBlt(
					imgStartPtrX*(m+1), imgStartPtrY*(n+1),
					imgStartPtrX, imgStartPtrY, 
					&memDC, 0, 0, 
					pDoc->gd_bmCell.cx, pDoc->gd_bmCell.cy, SRCCOPY);
				bmp.DeleteObject();
			}
			nCount++;
		}
	}
	memDC.SelectObject(pOldBmp);
	pDoc->ResizeWindow();

	// 게임이 시작된 시점에서의 처리
	if(isInit){
		isInit = FALSE;

		for(int i=0;i<pDoc->gd_nRow;i++)
			for(int j=0;j<pDoc->gd_nCol;j++)
				pDoc->gd_bShow[i][j] = FALSE;

		// 음악 디바이스 열기
		gv_timeDV =OpenSounds (_T ("C:\\image\\심장소리.mp3")) ;	
		gv_successDV =OpenSounds (_T ("C:\\image\\박수갈채.mp3")) ;
		gv_failDV =OpenSounds (_T ("C:\\image\\언더테이커_등장.mp3")) ;	
		gv_bgDV =OpenSounds (_T ("C:\\image\\Terranigma-Light_Side_Field.mp3")) ;

		// 타임바 범위 정의
		gv_timeUpper = gv_min*60 + gv_sec;
		pTimeDlg ->timeCtrl.SetRange32 (0, gv_timeUpper) ;	// 범위
		pTimeDlg ->timeCtrl.SetPos (gv_min*60 +gv_sec) ;		// 초기위치

		PlaySound (L"C:\\image\\즐거운시간되십시오.wav", NULL, SND_SYNC) ;
		
		// Hint time
		int time;
		if(gv_lev == _T("초급")) time = 600;
		else if(gv_lev == _T("중급")) time = 1000;
		else time = 1800;

		pTimeDlg ->ShowWindow (SW_SHOW) ;	// 모달리스 시작
		Sleep(time);
		pDC ->TextOutW (rect.Width () /2, rect.top +20, _T ("             ")) ;
		PlaySounds (gv_bgDV) ;	// 슬립 끝나고 배경음
		SetTimer (true, 1, NULL) ;
		goto go;
	}
}

// CGameView 인쇄
void CGameView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}
BOOL CGameView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}
void CGameView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}
void CGameView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}
void CGameView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}
void CGameView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

// CGameView 진단
#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}
void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
CGameDoc* CGameView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameDoc)));
	return (CGameDoc*)m_pDocument;
}
#endif //_DEBUG

// CGameView 메시지 처리기
// 왼쪽버튼 클릭시
void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CGameDoc* pDoc = GetDocument();
	CRect rect;
	GetClientRect(&rect);

	// 현재 마우스가 가진 인덱스
	pDoc->gd_nRowIndex = point.y / (rect.bottom/(pDoc->gd_nRow+2))-1;
	pDoc->gd_nColIndex = point.x / (rect.right/(pDoc->gd_nCol+2))-1;

	// 같은곳 두번클릭, 두번이상 클릭, 범위밖 클릭
	if(pDoc->gd_bShow[pDoc->gd_nRowIndex][pDoc->gd_nColIndex]
		|| pDoc->gd_nBmpSecondID
		|| pDoc->gd_nRowIndex > pDoc->gd_nRow - 1
		|| pDoc->gd_nColIndex > pDoc->gd_nCol - 1
		|| pDoc->gd_nRowIndex < 0 
		|| pDoc->gd_nColIndex < 0)
		return ;

	// 마우스가 가지고 있는 인덱스 값에 그림을 보여줌
	gv_TF =!gv_TF ;		// 클릭을 두번 할 때 마다 클릭수 증가시키기 위함

	PlaySound (L"C:\\image\\카메라.wav", NULL, SND_ASYNC) ;	// 클릭 했을 때 사진 넘김 소리
	pDoc->gd_bShow[pDoc->gd_nRowIndex][pDoc->gd_nColIndex] = TRUE;

	SetRect(&rect, pDoc->gd_nRowIndex*(pDoc->gd_bmCell.cx + 1),
		pDoc->gd_nColIndex*(pDoc->gd_bmCell.cy + 1),
		pDoc->gd_nRowIndex*(pDoc->gd_bmCell.cx + 2),
		pDoc->gd_nColIndex*(pDoc->gd_bmCell.cy + 2));
	InvalidateRect(rect);

	// 마우스가 클릭한 곳의 인덱스 및 그림 아이디 저장
	// 첫번째 클릭
	if(pDoc->gd_nBmpFirstID==0){
		pDoc->gd_nBmpFirstID = pDoc->gd_nBmpID[pDoc->gd_nRowIndex][pDoc->gd_nColIndex];
		gv_nRowTempIndex = pDoc->gd_nRowIndex;
		gv_nColTempIndex = pDoc->gd_nColIndex;
		pDoc->gd_bMouse = TRUE;
	}
	// 두번째 클릭
	else{
		pDoc->gd_nBmpSecondID = pDoc->gd_nBmpID[pDoc->gd_nRowIndex][pDoc->gd_nColIndex];
		pDoc->gd_bMouse = FALSE;
	}

	// 클릭 카운트 계산
	if (gv_TF) gv_cnt++ ;
	
	// 같은그림인지 판단
	OnMatching();
	CView::OnLButtonDown(nFlags, point);
}

// 카드 맞추는 도중
void CGameView::OnMatching(void)
{
	CGameDoc* pDoc = GetDocument();

	// 그림이 같으면
	if(pDoc->gd_nBmpFirstID == pDoc->gd_nBmpSecondID && pDoc->gd_bMouse == FALSE){
		PlaySound (L"C:\\image\\키스쪽.wav", NULL, SND_ASYNC) ;		// 그림이 맞았을 때 나는 소리
		
		// 클릭 이벤트 초기화
		pDoc->gd_bShow[gv_nRowTempIndex][gv_nColTempIndex] 
		= pDoc->gd_bShow[pDoc->gd_nRowIndex][pDoc->gd_nColIndex] = TRUE;
		pDoc->gd_nBmpFirstID = pDoc->gd_nBmpSecondID 
		= gv_nRowTempIndex = gv_nColTempIndex = 0;

		gv_nMatchCount ++;
		gv_point+= 10 ;		// 그림을 맞추었을 때 10점 추가
	}
	else if(pDoc->gd_nBmpFirstID != pDoc->gd_nBmpSecondID && pDoc->gd_bMouse == FALSE){
		// 다른 그림이어도 보여주긴함
		CDC* pDC = GetDC();
		CDC memDC;
		CBitmap bmp;

		memDC.CreateCompatibleDC(pDC);
		bmp.LoadBitmapW(IDB_BITMAP1 + pDoc->gd_nBmpSecondID);

		CRect rect;
		GetClientRect(&rect);

		CBitmap* pOldBmp = memDC.SelectObject(&bmp);
		pDC->StretchBlt(rect.right/(pDoc->gd_nCol+2)*(pDoc->gd_nColIndex + 1),
			rect.bottom/(pDoc->gd_nRow+2)*(pDoc->gd_nRowIndex + 1),
			rect.right/(pDoc->gd_nCol+2), rect.bottom/(pDoc->gd_nRow+2), 
			&memDC, 0, 0, pDoc->gd_bmCell.cx, pDoc->gd_bmCell.cy, SRCCOPY);

		bmp.DeleteObject();
		memDC.SelectObject(pOldBmp);
		// 카드 보여주는 시간
		Sleep(400);

		// 클릭 이벤트 초기화
		pDoc->gd_bShow[gv_nRowTempIndex][gv_nColTempIndex] = FALSE;
		pDoc->gd_bShow[pDoc->gd_nRowIndex][pDoc->gd_nColIndex] = FALSE;
		pDoc->gd_nBmpFirstID = pDoc->gd_nBmpSecondID = 0;
		gv_nRowTempIndex = gv_nColTempIndex = 0;
	}

	Invalidate();
	if(gv_nMatchCount == (pDoc->gd_nRow*pDoc->gd_nCol)/2) OnSuccess() ;
}

// 성공했을때
void CGameView::OnSuccess(void)
{
	// 랭크 기록 여부
	KillTimer (true) ;	// 메세지 박스를 선택 안해도 시간이 흐르지 않도록
	int res;
	StopSounds (gv_bgDV) ;
	StopSounds (gv_timeDV) ;		// 시간이 지나거나 성공하면 음악을 끔
	
	if(gv_sec != 0){
		PlaySounds (gv_successDV) ;
		gv_scoreTF =!gv_scoreTF ;		// 스코어 타이머 가동
		gv_tempMin =gv_min ; gv_tempSec =gv_sec ;

		SetTimer (true, 40, NULL) ;	// 점수에 남은 시간에 대한 포인트를 더한다.
		res =AfxMessageBox (_T ("축하드립니다! 랭크를 기록 하시겠습니까?"), MB_YESNO) ;
		
		// 기록 할 때
		if (res == IDYES) {
			CRankNameDlg dlgName ;
			dlgName.DoModal () ;

			CRankDlg *mem =new CRankDlg (dlgName.name, gv_minStr, gv_secStr, gv_csecStr, gv_cnt, gv_lev, gv_rank, gv_point) ;
			mem ->DoModal () ;
		}
	}
	else{
		PlaySounds (gv_failDV) ;
		res =AfxMessageBox (_T ("안타깝네요. 다시 하시겠습니까?"), MB_YESNO) ;
		gv_TF =true ;		// 재시작을 다시 묻지 않기 위해
		StopSounds (gv_failDV) ;		// 대화상자를 닫으면 음악을 끄게한다.
	}
	// 재시작 여부
	if(gv_TF == false){		// 게임 끝내면 gv_TF는 원래 값으로 되돌아옴
		KillTimer (true) ;		
		gv_scoreTF =!gv_scoreTF ;		// 스코어 타이머 해제
		StopSounds (gv_successDV) ;		// 대화상자가 뜨기전 음악을 끈다.
		res = AfxMessageBox(_T("다시 시작하시겠습니까?"), MB_YESNO);
	}

	if(res == IDYES){
		gv_cnt = gv_point = gv_min = 0 ;
		gv_csec =100 ;
		CGameDoc* pDoc = GetDocument();
		gv_sec = pDoc->gd_time;
		pDoc->gd_bRandom = TRUE; // 랜덤하게 그림의 ID변경

		for(int n=0;n<pDoc->gd_nRow;n++)
			for(int m=0;m<pDoc->gd_nCol;m++)
				pDoc->gd_bShow[n][m] = FALSE;
		
		// 아무 그림도 선택하지 않은것으로 인식하기 위한 초기화
		pDoc->gd_nBmpFirstID = pDoc->gd_nBmpSecondID = 0 ;
		pDoc->gd_bMouse = FALSE;
		gv_nMatchCount = 0; 
		isInit = TRUE;
		Invalidate();
	}
	else{ // 다시 하지 않으면 종료
		PlaySound (L"C:\\image\\벌써가게흐흥.wav", NULL, SND_SYNC) ;
		PostMessageW(WM_QUIT);
	}
}
int CGameView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CLevelDlg dlg;
	int res =dlg.DoModal();	// domodal로 불렀으므로 이 함수안에서 해결해야됨.
	if (res == IDCANCEL) exit(1);	// x버튼을 누르면 바로 종료한다.
	
	if (res == IDOK){
		// 바탕색, 글 꼴 전달
		gv_bgColor = dlg.ld_rectColor;
		gv_font = dlg.ld_font;

		// 난이도 (행, 열, 시간) 전달
		CGameDoc *pDlg =GetDocument () ;	// Doc 포인터로
		pDlg ->gd_nRow =dlg.nRow ;		// 입력
		pDlg ->gd_nCol =dlg.nCol ;
		gv_lev =dlg.strLev ;
		gv_rank =dlg.strRank ;
		gv_sec = pDlg ->gd_time =dlg.time;

		while (gv_sec > 59){		// 초가 60이상이면 분으로 옮긴다.
			++gv_min ;
			gv_sec -=60 ;
		}
	}

	// 시간 바 기본 설정
	pTimeDlg =new CTimeDlg;
	pTimeDlg ->color =gv_bgColor ;	// 배경색을 넘겨줌
	pTimeDlg ->Create (IDD_TIME) ;	// 생성
	pTimeDlg ->SetParent (this) ;	// 뷰를 부모로 둔다.
	pTimeDlg ->timeCtrl.SetStep (-1) ;		// 스텝 값
	pTimeDlg ->OnInitDialog () ;		// 바탕색 및, 범위 지정

	return 0;
}


void CGameView::OnWhite()
{
	gv_bgColor = RGB(255, 255, 255); 
	Invalidate();

	pTimeDlg ->color =gv_bgColor ;
	pTimeDlg ->OnInitDialog () ;
}


void CGameView::OnBlack()
{
	gv_bgColor = RGB(0, 0, 0);
	Invalidate();

	pTimeDlg ->color =gv_bgColor ;
	pTimeDlg ->OnInitDialog () ;
}


void CGameView::OnUser()
{
	CColorDialog dlg;
	if(dlg.DoModal() == IDOK){
		gv_bgColor = dlg.GetColor();
		Invalidate();
	}
	pTimeDlg ->color =gv_bgColor ;
	pTimeDlg ->OnInitDialog () ;
}


void CGameView::OnDestroy()
{
	CView::OnDestroy();
	KillTimer (true) ;
}


void CGameView::OnTimer(UINT_PTR nIDEvent)
{
	// 심장소리 음악
	if (!gv_min && gv_sec < 10 && !gv_timeDV){	// 분이 0이고 초가 10초 미만일 때 시간제한 음악 켬
		StopSounds (gv_bgDV) ;
		PlaySounds (gv_timeDV) ;
	}

	CClientDC dc (this) ;
	CRect rect ;
	GetClientRect (&rect) ;
	CFont myFont, *pOldFont ;
	CString str;
	
	// Text 설정 Part
	myFont.CreateFont (25, 0, 0, 0, FW_BOLD, true, false, false, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, gv_font) ;
	pOldFont =dc.SelectObject (&myFont) ;
	dc.SetBkColor (gv_bgColor) ;		// 글자의 배경은 바탕과 같이 
	dc.SetTextColor (0x00FFFFFF & ~gv_bgColor) ;		// 글자 색을 바탕화면의 반대 색으로

	// 시간에 따른 출력부
	if (!gv_scoreTF) {
		if (gv_min < 10 && gv_min) gv_minStr.Format (_T(" 0%d"), gv_min) ;
		else if (!gv_min) gv_minStr.Format (_T (" 00")) ;
		else gv_minStr.Format (_T (" %d"), gv_min) ;
		
		if (gv_sec < 10 && gv_sec) gv_secStr.Format (_T("0%d"), gv_sec) ;
		else if (!gv_sec) gv_secStr.Format (_T ("00")) ;
		else gv_secStr.Format (_T ("%d"), gv_sec) ;

		if (gv_csec < 10 && gv_csec) gv_csecStr.Format (_T("0%d"), gv_csec) ;
		else if (!gv_csec) gv_csecStr.Format (_T ("00")) ;
		else gv_csecStr.Format (_T ("%d"), gv_csec) ;

		str.Format (_T (" %s : %s : %s "), gv_minStr, gv_secStr, gv_csecStr) ;
		dc.SetTextAlign (TA_RIGHT) ;
		dc.TextOutW (rect.Width () -20, rect.top +45, str) ;

		// 초계산
		if (--gv_csec < 0) {
			if (!gv_min && !gv_sec) OnSuccess () ;
			gv_csec =99 ;
			gv_sec-- ;
			pTimeDlg ->timeCtrl.StepIt () ;		// 초가 줄때마다 실행
		}
		if (gv_sec < 0 && gv_min) {
			gv_sec =59 ;
			gv_min-- ;
		}

		// 카운트 출력
		str.Format (_T (" CLICK COUNT : %u "), gv_cnt) ;
		dc.TextOutW (rect.Width () -20, rect.top +20, str) ;

		// 점수 출력
		dc.SetTextAlign (TA_CENTER) ;
		str.Format (_T (" Score : %u "), gv_point) ;
		dc.TextOutW (rect.Width () /2, rect.top +25, str) ;
	}
	else {
		while (gv_tempMin-- > 0) gv_tempSec +=60 ;		// 분이 남아 있다면 초로 옮김
		// 시간 감소 및 포인트 증가
		if (gv_tempSec > 0) {					// 초가 0보다 클때까지
			gv_point+=3 ; gv_tempSec-- ;			// 점수는 3증가 초는 1증감
			pTimeDlg ->timeCtrl.StepIt () ;
			dc.SetTextAlign (TA_CENTER) ;
			str.Format (_T (" Score :%u "), gv_point) ;	
			dc.TextOutW (rect.Width () /2 , rect.top +25, str) ;
		}
	}
	dc.SelectObject (myFont) ;
	myFont.DeleteObject () ;
	CView::OnTimer(nIDEvent);
}


// 사운드를 재생하기 위한 함수
void CGameView::PlaySounds(DWORD dv)
{
	MCI_PLAY_PARMS mciPlay ;
	// Play Part
	mciSendCommand (dv, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlay) ;	// MCI_PLAY 명령은 준다
}

// 음악파일 멈춤
void CGameView::StopSounds(DWORD dv)
{
	// Stop Part
	mciSendCommand (dv, MCI_CLOSE, 0, NULL) ;
}

// 음악파일의 디바이스를 연다.
DWORD CGameView::OpenSounds(CString name)
{
	// Load Part
	MCI_OPEN_PARMS mciOpen ;		// mci열기 플레이
	DWORD dv ;		// 디바이스

	mciOpen.lpstrElementName = name ;			//재생 할 파일 및경로
	mciOpen.lpstrDeviceType = L"MPEGVideo";		// 재생 할 타입

	mciSendCommand (NULL, MCI_OPEN, MCI_OPEN_ELEMENT|MCI_OPEN_TYPE,
		(DWORD)(LPVOID) &mciOpen) ;          //MCI_OPEN 명령을 준다.

	// Play Part
	dv = mciOpen.wDeviceID;        //디바이스 아이디를 받느다.
	
	return dv;
}

// 창크기가 변경 되었을때 타임바의 위치를 정하기 위한 함수
void CGameView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	CRect rect ;
	GetClientRect (&rect) ;

	// View의 정보를 넘겨줌
	pTimeDlg ->rectView =rect ;
	pTimeDlg ->OnInitDialog () ;
}
