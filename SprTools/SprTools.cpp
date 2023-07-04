// SprTools.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SprTools.h"
#include "MainFrm.h"

#include "ChildFrm.h"

#include "ImageDoc.h"
#include "ImageFrame.h"
#include "ImageView.h"
#include "ImageDoc.h"

#include "MotionDoc.h"
#include "MotionFrame.h"
#include "MotionView.h"
#include "MotionDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSprToolsApp

BEGIN_MESSAGE_MAP(CSprToolsApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CSprToolsApp::OnAppAbout)
	// Standard file based document commands
//	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
//	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
	ON_COMMAND(ID_FILE_OPEN, &CSprToolsApp::OnFileOpen)
	ON_COMMAND(ID_FILE_NEW,	 &CSprToolsApp::OnFileNew)


END_MESSAGE_MAP()


// CSprToolsApp construction

CSprToolsApp::CSprToolsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	
}


// The one and only CSprToolsApp object

CSprToolsApp theApp;


// CSprToolsApp initialization

BOOL CSprToolsApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	
	m_pMotionDocTemplate= new CMultiDocTemplate(
		IDR_MOTIONTYPE,
		RUNTIME_CLASS(CMotionDoc),
		RUNTIME_CLASS(CMotionFrame), 
		RUNTIME_CLASS(CMotionView));
	if (!m_pMotionDocTemplate)
		return FALSE;
	AddDocTemplate(m_pMotionDocTemplate);


	
	m_pImageDocTemplate= new CMultiDocTemplate(
		IDR_IMAGETYPE,
		RUNTIME_CLASS(CImageDoc),
		RUNTIME_CLASS(CImageFrame), 
		RUNTIME_CLASS(CImageView));
	if (!m_pImageDocTemplate)
		return FALSE;
	AddDocTemplate(m_pImageDocTemplate);
	

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CSprToolsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CSprToolsApp message handlers


void CSprToolsApp::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	
	TRACE(_T("CSprToolsApp::OnFileOpen()"));	


	TCHAR szFilter[] = _T("Bitmap Files (*.bmp)|*.bmp|Motion Files (*.mot)|*.mot|Group Files (*.grp)|*.grp|All Files(*.*)|*.*||");

	DWORD MAXFILE = 2562; //2562 is the max
	TCHAR* pc = new TCHAR[MAXFILE];                 

//	char szFilter[] = _T("Image (*.BMP, *.GIF, *.JPG) | *.BMP;*.GIF;*.JPG | All Files(*.*)|*.*||");


	CFileDialog dlg(TRUE, NULL, NULL,OFN_ALLOWMULTISELECT|OFN_HIDEREADONLY, szFilter);

	dlg.m_ofn.nMaxFile = MAXFILE;
	dlg.m_ofn.lpstrFile = pc;
	dlg.m_ofn.lpstrFile[0] = NULL;


	if(IDOK == dlg.DoModal())
	{
		for(POSITION pos=dlg.GetStartPosition(); pos != NULL;)
		{
			CString strFilePath=dlg.GetNextPathName(pos);


			CString temp=CString(strFilePath).Right(3);

			if (temp.CompareNoCase(_T("BMP"))==0)
			{
				if (m_pImageDocTemplate!=NULL)	m_pImageDocTemplate->OpenDocumentFile(strFilePath);				
			}
			else if (temp.CompareNoCase(_T("MOT"))==0)
			{
				if (m_pMotionDocTemplate!=NULL)  m_pMotionDocTemplate->OpenDocumentFile(strFilePath);
			}
			else if (temp.CompareNoCase(_T("GRP"))==0)
			{

			}
		}

	}
	delete []pc;

}

CImageDoc* CSprToolsApp::OpenImageDocTemplate(LPCTSTR lpszPathName)
{
	if (m_pImageDocTemplate==NULL)
	{
		return NULL;
	}

	return (CImageDoc*)m_pImageDocTemplate->OpenDocumentFile(lpszPathName);
}

CMotionDoc* CSprToolsApp::OpenMotionDocTemplate( LPCTSTR lpszPathName )
{
	if (m_pMotionDocTemplate==NULL)
	{
		return NULL;
	}
	return (CMotionDoc*)m_pMotionDocTemplate->OpenDocumentFile(lpszPathName);
		
}
void CSprToolsApp::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	TRACE(_T("CSprToolsApp::OnFileNew()\n"));	
/*
	CString filter =_T("Motion Files (*.mot)|*.mot||");
	
	CFileDialog Dlg(TRUE,_T("mot"),_T("*.mot"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,filter, NULL);


	if (Dlg.DoModal()==IDOK)
	{		
		OpenMotionDocTemplate(NULL)->SetTitle(Dlg.GetFileName());
	}
*/
/*
	static int cnt=0;
	CString strTileName;

	strTileName.Format(_T("Motion%d.mot"),++cnt);
	*/
	CMotionDoc *pDoc=OpenMotionDocTemplate(NULL);
/*
	if (pDoc!=NULL)
	{
		pDoc->SetTitle(strTileName);	
	}	
*/
}

BOOL CSprToolsApp::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CMainFrame* pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	if (pMainFrame->GetMotionAnmDlg().GetSafeHwnd()!=NULL)
	{
		pMainFrame->GetMotionAnmDlg().DrawMotion();
		return TRUE;
	}
	return CWinApp::OnIdle(lCount);
}

CDocument* CSprToolsApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// TODO: Add your specialized code here and/or call the base class
//	TRACE(_T("CSprToolsApp::OpenDocumentFile(%s)\n",lpszFileName));	
	CDocument* pDoc=NULL;

	CString temp=CString(lpszFileName).Right(3);

	if (temp.CompareNoCase(_T("BMP"))==0)
	{
		CSprToolsApp* pApp=(CSprToolsApp*)::AfxGetApp();
		pDoc=pApp->OpenImageDocTemplate(lpszFileName);
	}
	else if (temp.CompareNoCase(_T("MOT"))==0)
	{
		CSprToolsApp* pApp=(CSprToolsApp*)::AfxGetApp();
		pDoc=pApp->OpenMotionDocTemplate(lpszFileName);
	}
	return pDoc;
	

//  첫번째 템플릿만 열리므로 주석처리
//	return CWinApp::OpenDocumentFile(lpszFileName);
}
