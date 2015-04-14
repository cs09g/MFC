
// GameDoc.cpp : CGameDoc 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Game.h"
#endif

#include "GameDoc.h"
#include "GameView.h"
#include "MainFrm.h"
#include "LevelDlg.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGameDoc

IMPLEMENT_DYNCREATE(CGameDoc, CDocument)

BEGIN_MESSAGE_MAP(CGameDoc, CDocument)
END_MESSAGE_MAP()


// CGameDoc 생성/소멸

CGameDoc::CGameDoc()
{
	gd_time = 19;
	gd_bmCell = CSize(125, 115);	// 셀크기 초기화
	gd_nRow = 3;	// 격자 초기 행,열 값
	gd_nCol = 4;	// 난이도 선택 안할시 초기값을 진행
}

CGameDoc::~CGameDoc()
{
}

BOOL CGameDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CGameDoc serialization

void CGameDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CGameDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CGameDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGameDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGameDoc 진단

#ifdef _DEBUG
void CGameDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGameDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGameDoc 명령


void CGameDoc::ResizeWindow(void)
{
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();

	CREATESTRUCT st;
	CREATESTRUCT& rst = st;

	pMain->PreCreateWindow(rst);
}


void CGameDoc::OnRandom(void)
{
	gd_bRandom =false ;		// 랜덤함수를 다시 부르지않기위해 초기화

	srand((unsigned)time(NULL)) ;

	BOOL bInsert = TRUE; // 비트맵 삽입 여부
	int nGrating = gd_nRow *gd_nCol; // 격자의 총 수

	for(int n=0;n<nGrating;n++){
		gd_nRnd[n] = (rand()%(nGrating/2)) + 1; // 일단 난수 저장
		// 중복체크
		for(int m=0;m<n;m++)
			if(gd_nRnd[n] == gd_nRnd[m]) {
				if(!bInsert){
					n--;
					bInsert = TRUE;
					break;
				}
				bInsert = FALSE;
			}
	}
	int nCount = 0;

	for(int n=0;n<gd_nRow;n++)
		for(int m=0;m<gd_nCol;m++)
			gd_nBmpID[n][m] = gd_nRnd[nCount++];
}


void CGameDoc::InitBoard(void)
{
	gd_bRandom = TRUE;
	for(int n=0;n<gd_nRow;n++)
		for(int m=0;m<gd_nCol;m++)
			gd_bShow[n][m] = FALSE;

	// 아무 그림도 선택하지 않았다고 초기화.
	gd_nBmpFirstID = gd_nBmpSecondID = gd_bMouse =0;
	ResizeWindow();
	OnRandom();
}

