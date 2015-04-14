#pragma once
#include "afxwin.h"
#include "afx.h"
#include "afxcmn.h"


// CRankDlg 대화 상자입니다.

class CRankDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRankDlg)

public:
	CRankDlg(CString, CString, CString, CString, UINT, CString, CString, UINT, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRankDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RANK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:	
	// 다이얼로그 처리
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	
	// 데이터 저장/불러오기
	void AutoSave(void);
	void AutoLoad(void);

	CString member;	// 이름
	CString nMin;	// 분
	CString nSec;	// 초
	CString nCsec;	// 1/100초
	UINT nCnt;		// 클릭 수
	UINT nRank;	// 랭크
	
	CListCtrl rankList;	// 리스트 컨트롤 변수
	CString strLev;	// 난이도 변수
	CString rankName;	// 난이도별로 저장될 랭크.txt의 이름
	UINT nPoint;	// 스코어 점수
	
	/************ 랭킹 처리 ************/
	void SortRank(CString rankName);
	void SwapRank(int val1, int val2);
	int CompareRank(int * val1, int * val2, int, int, int);
	void NaturalMergeSort(int top, int bottom);	// 순위를 바꾼후 바뀐 순위대로 재 정렬
	void QuickSort(int first, int last);	// 퀵 소트
	void SwapItem(int val1, int val2);	// 리스트 컨트롤의 아이템을 교환
	/********************************/

	/********** 클릭 이벤트 **********/
	afx_msg void OnBnClickedButtonEasy();
	afx_msg void OnBnClickedButtonNormal();
	afx_msg void OnBnClickedButtonHard();
	afx_msg void OnBnClickedButtonDelall();
	/*****************************/

	// 흘러간 시간을 나타내기 위한 함수
	void TimePoint(void);
};
