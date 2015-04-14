
// GameDoc.h : CGameDoc 클래스의 인터페이스
//


#pragma once
#include "afxwin.h"


class CGameDoc : public CDocument
{
protected: // serialization에서만 만들어집니다.
	CGameDoc();
	DECLARE_DYNCREATE(CGameDoc)

// 특성입니다.
public:
// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 구현입니다.
public:
	virtual ~CGameDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 검색 처리기에 대한 검색 콘텐츠를 설정하는 도우미 함수
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	CBitmap	gd_bmp[6][6];	// 비트맵 저장
	CSize		gd_bmCell;		// 셀에 들어갈 그림 하나의 크기
	
	int gd_nRow;		// 격자수를 저장
	int gd_nCol;
	int gd_nRnd[36];			// 랜덤으로 발생한 그림위치수를 저장할 변수
	int gd_time;
	int gd_nRowIndex;
	int gd_nColIndex;

	bool gd_bRandom;	// 게임중 / 랜덤의 유무 파악
	bool gd_bShow[6][6];
	bool gd_bMouse;

	void ResizeWindow(void);
	void OnRandom(void);
	void InitBoard(void);
		
	UINT gd_nBmpID[6][6];	// 그림의 ID값을 배열에 저장
	UINT gd_nBmpFirstID;	// 첫번째 눌린 그림의 아이디 값
	UINT gd_nBmpSecondID;	// 두번째 눌린 그림의 아이디 값
};
