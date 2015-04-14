
// GameView.h : CGameView 클래스의 인터페이스
//

#pragma once
#pragma comment(lib, "winmm.lib")
#include <MMSystem.h>
#include "TimeDlg.h"

class CGameView : public CView
{
protected: // serialization에서만 만들어집니다.
	CGameView();
	DECLARE_DYNCREATE(CGameView)
	
// 특성입니다.
public:
	CGameDoc* GetDocument() const;

// 작업입니다.
public:
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CGameView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	// 현재 마우스가 좌표값으로 그림의 위치를 계산하여 저장하고 있는 인덱스
	UINT gv_nRowTempIndex;
	UINT gv_nColTempIndex;

	UINT gv_cnt;	// L버튼 짝수 단위로 누른 갯수
	UINT gv_point;	// 점수변수
	UINT gv_timeUpper;	// 시간바의 최대 범위를 가질 변수

	BOOL isInit;
	BOOL gv_TF;	// L버튼 클릭시 짝수 단위로 계산
	BOOL gv_scoreTF;	// 종료후 점수 계산을 위한 구분자

	COLORREF gv_bgColor;	// 배경색

	// 시간 int
	int gv_min;	// 분
	int gv_sec;	// 초
	int gv_csec;	// 1/100
	int gv_tempMin;	// 임시 분
	int gv_tempSec;	// 임시 초

	int gv_nMatchCount;	// 맞춘 그림의 갯수를 세는 변수 선언
	int gv_levCnt;	// 난이도 int
	
	// 시간 String
	CString gv_minStr;	// 분
	CString gv_secStr;	// 초
	CString gv_csecStr;	// 1/100초

	void OnMatching(void);
	void OnSuccess(void);
	void PlaySounds (DWORD);		// 사운드를 재생하기 위한 함수
	void StopSounds(DWORD);	// 음악파일 멈춤

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWhite();
	afx_msg void OnBlack();
	afx_msg void OnUser();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	CString gv_lev;	// 난이도 명
	CString gv_rank;	// .txt의 이름
	CString gv_font;	// 글자체

	DWORD gv_timeDV;	// 시간제한의 따른 음악의 디바이스
	DWORD gv_bgDV;	// 배경음악 사운드의 디바이스
	DWORD OpenSounds(CString name);	// 음악파일의 디바이스를 연다.
	DWORD gv_failDV;	// 실패시 음악 디바이스
	DWORD gv_successDV;	// 성공시 음악의 디바이스
	
	CTimeDlg *pTimeDlg;
};

#ifndef _DEBUG  // GameView.cpp의 디버그 버전
inline CGameDoc* CGameView::GetDocument() const
   { return reinterpret_cast<CGameDoc*>(m_pDocument); }
#endif

