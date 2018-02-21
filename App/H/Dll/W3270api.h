
#ifdef __cplusplus
  #define  API_STRUCT_PACK    1
  #pragma pack(push, API_STRUCT_PACK)
#endif
/*****************************************************************************/
/*  Return code table                                                        */
/*****************************************************************************/

#define         API_SYSTEM_ERROR             -1
#define         API_EMU_NOTEXIST             -2
#define         API_EMU_DISCONNECT           -3
#define         API_CONNECT_FAILURE          -4
#define         API_SESS_AP_UNLINK           -5
#define         API_SESS_ISUSED              -6
#define         API_SESS_ISUNUSED            -7
#define         API_INSTANCE_ERROR           -8
#define         API_LUNO_ERROR               -9
#define         API_NAME_ERROR               -10
#define         API_ROW_ERROR                -11
#define         API_COL_ERROR                -12
#define         API_FILE_OPEN_ERROR          -13
#define         API_OPTION_CODE_ERROR        -14
#define         API_STATUS_ERROR             -15
#define         API_TIMEOUT                  -16
#define         API_INTERNAL_ERROR           -17
#define         API_DISK_ERROR               -18
#define         API_AIDKEY_ERROR             -19
#define         API_SEND_LENGTH_ERROR        -20
#define         API_CMD_ERROR                -21
#define         API_INP_INHIBIT              -22
#define         API_TEXT_ERROR               -23
#define         API_PRINTER_SESS             -24
#define         API_CURSOR_ERROR             -25
#define         API_MEMORY_ERROR             -26
#define         API_TRANS_HAD_SET_ON         -27
#define         API_STOP_FILETRANS           -28
#define         API_NO_FREE_SESS             -29
#define         API_COMMUNICATION_ERROR      -30
#define         API_SESS_ISUSEDQUIT          -31

#define         APPC_NOT_BOUND               -48
#define         APPC_ALREADY_ALLOC           -49
#define         APPC_NOT_ALLOC               -50

// Parameter Error  -1000~-1099;  -1004 is Parameter-4 Error 
#define         API_PARAM_ERROR            -1000 
/*****************************************************************************/
/*  AID key                                                                  */
/*****************************************************************************/

#define         AID_PF1                       0
#define         AID_PF2                       1
#define         AID_PF3                       2
#define         AID_PF4                       3
#define         AID_PF5                       4
#define         AID_PF6                       5
#define         AID_PF7                       6
#define         AID_PF8                       7
#define         AID_PF9                       8
#define         AID_PF10                      9
#define         AID_PF11                      10
#define         AID_PF12                      11
#define         AID_PF13                      12
#define         AID_PF14                      13
#define         AID_PF15                      14
#define         AID_PF16                      15
#define         AID_PF17                      16
#define         AID_PF18                      17
#define         AID_PF19                      18
#define         AID_PF20                      19
#define         AID_PF21                      20
#define         AID_PF22                      21
#define         AID_PF23                      22
#define         AID_PF24                      23
#define         AID_PA1                       24
#define         AID_PA2                       25
#define         AID_PA3                       26
#define         AID_CLEAR                     27
#define         AID_ENTER                     28
#define         AID_SYSREQ                    29
#define         AID_ATTN                      30
#define         AID_CRSEL                     31
#define         AID_ERINP                     32
#define         AID_EREOF                     33
#define         AID_NEWLINE                   34
#define         AID_RESET                     35
#define         AID_DEVCN                     36
#define         AID_DUP                       37
#define         AID_FM                        38
#define         AID_ALTCSR                    39
#define         AID_CLICK                     40
#define         AID_UP                        41
#define         AID_DOWN                      42
#define         AID_LEFT                      43
#define         AID_RIGHT                     44
#define         AID_DLEFT                     45
#define         AID_DRIGHT                    46
#define         AID_HOME                      47
#define         AID_TAB                       48
#define         AID_BACKTAB                   49
#define         AID_INSERT                    50
#define         AID_DELETE                    51

/*****************************************************************************/
/*  SysStartEmuModule(nCmdShow)                                              */
/*****************************************************************************/
// nCmdShow is one of the following value
/*
#define SW_HIDE             0
#define SW_SHOWNORMAL       1
#define SW_NORMAL           1
#define SW_SHOWMINIMIZED    2
#define SW_SHOWMAXIMIZED    3
#define SW_MAXIMIZE         3
#define SW_SHOWNOACTIVATE   4
#define SW_SHOW             5
#define SW_MINIMIZE         6
#define SW_SHOWMINNOACTIVE  7
#define SW_SHOWNA           8
#define SW_RESTORE          9
*/

/*****************************************************************************/
/*  parameter for setting pacing mode                                        */
/******************************************************************************/
#define NORMALEMURSPMODE    0
#define APIAUTORSPMODE      1
#define APCARERSPMODE       2

/*****************************************************************************/
/*  parameter for setting +/- response mode                                  */
/******************************************************************************/
#define EMUISSUERSPMODE     0
#define APISSUERSPMODE      1

/*****************************************************************************/
/*  OIA information                                                          */
/******************************************************************************/
typedef struct tagOIAINFO
{
    BYTE   ucLuAddr;               /* LU address            */
    BYTE   ucLuType;               /* LU type               */
    BYTE   ucSessBound;            /* Session bound?        */
    BYTE   ucSessOwner;            /* Session owner         */
    BYTE   ucOpStatus;             /* Inhibit/Busy/System   */
    BYTE   ucPrintStatus;          /* Printer status        */
    WORD   wErrStatus;             /* Comm check/Prog check */
} OIAINFO;
typedef OIAINFO far * LPOIAINFO;
/*------------------------------------------*/
/* ucLuType                                 */
/*------------------------------------------*/
#define    OIA_TYPE_TERMINAL        0
#define    OIA_TYPE_PRINTER         1
#define    OIA_TYPE_UNDEFINE        2

/*------------------------------------------*/
/* ucSessBound                              */
/*------------------------------------------*/
#define    OIA_SESS_UNBOUND         0
#define    OIA_SESS_BOUND           1

/*------------------------------------------*/
/* ucSessOwner                              */
/*------------------------------------------*/
#define    OIA_OWNER_DISCONNECT    'D'
#define    OIA_OWNER_UNOWN         '?'
#define    OIA_OWNER_SSCP_LU       'S'
#define    OIA_OWNER_LU_LU         'L'

/*------------------------------------------*/
/* ucOpStatus                               */
/*------------------------------------------*/
#define    OIA_X_TIME               1
#define    OIA_X_SYSTEM_LOCK        2
#define    OIA_X_WHAT               3
#define    OIA_X_MINUS_FUNCTION     4
#define    OIA_X_MINUS_UNAUTHOR     5
#define    OIA_X_SECURITY           6
#define    OIA_X_PRINTER_NOT_WORK   7
#define    OIA_X_PRINTER_BUSY       8
#define    OIA_X_PRINTER_VERY_BUSY  9
#define    OIA_X_OP_UNAUTHOR       10
#define    OIA_X_GO_ELSEWHERE      11
#define    OIA_X_MORE_THAN         12
#define    OIA_X_NUMERIC           13
#define    OIA_X_WHAT_NUMBER       14
#define    OIA_X_QUESTION_CARD     15
#define    OIA_X_ACCENT_PLUS_WHAT  16
#define    OIA_X_MINUS_SYMBOL      17
#define    OIA_X_NEED_CHINESE      18
#define    OIA_X_NEED_ENGLISH      19

/*------------------------------------------*/
/* ucErrStatus                              */
/*------------------------------------------*/
// ucErrStatus = ERROR number (501, 505, 532, 240...)

/*------------------------------------------*/
/* ucPrntStatus                             */
/*------------------------------------------*/
#define    OIA_PRNT_PRINTER_ASSIGN  1
#define    OIA_PRNT_WHAT_PRINTER    2
#define    OIA_PRNT_PRINTING        3
#define    OIA_PRNT_FAILURE         4
#define    OIA_PRNT_ASSIGN_PRINTER  5
/*****************************************************************************/
/*  extended information                                                     */
/******************************************************************************/
typedef struct tagEXTINFO
{
    WORD   wPacingMode;            // current pacing response mode
    BYTE   ucPacingReq;            // pacing request exist ?
    BYTE   ucPacingRsp;            // pacing response queued ?
    BYTE   ucSdtOn;                // data traffic on ?
    BYTE   ucRHState;              // status for RH
    BYTE   ucReserved3;            // reserve now
    BYTE   ucReserved4;            // reserve now
    WORD   wLuState;               // reserve now
    WORD   wReserved2;             // reserve now
    WORD   wReserved3;             // reserve now
    WORD   wReserved4;             // reserve now
} EXTINFO;
typedef EXTINFO far * LPEXTINFO;
/*------------------------------------------*/
/* wPacingMode                              */
/*------------------------------------------*/
#define         PACING_RSP_AUTO         0
#define         PACING_RSP_WHEN_RCV     1
#define         PACING_RSP_AP           2
/*------------------------------------------*/
/* ucPacingReq                              */
/*------------------------------------------*/
#define         NO_PACING_REQ           0
#define         PACING_REQ_EXIST        1
/*------------------------------------------*/
/* ucPacingRsp                              */
/*------------------------------------------*/
#define         NO_PACING_RSP           0
#define         PACING_RSP_QUEUE        1
/*------------------------------------------*/
/* ucStdOn                                  */
/*------------------------------------------*/
#define         DATA_TRAFFIC_OFF        0
#define         DATA_TRAFFIC_ON         1
/*------------------------------------------*/
/* ucRHState                                */
/*------------------------------------------*/
#define         RHChainState            0x03
  #define       RHBeginChain            0x02
  #define       RHEndChain              0x01
  #define       RHOnlyChain             0x03
  #define       RHMiddleChain           0x00
#define         RHSpeaker               0x0c
  #define       RHSpeakerPLU            0x08
  #define       RHSpeakerSLU            0x04
  #define       RHSpeakerContention     0x00
#define         RHBracketState          0x10
  #define       RHInBracket             0x10
  #define       RHBetweenBracket        0x00
/*------------------------------------------*/
/* wLuState                                 */
/*------------------------------------------*/
#define         DR1RSPNEED              0x0080
#define         DR2RSPNEED              0x0040
#define         APCARERSP               0x0020
#define         ERIRSPNEED              0x0010

/*------------------------------------------*/
/* response type for SysResponse()          */
/*------------------------------------------*/
#define         NEGRESPONSE             0x00
#define         POSRESPONSE             0x01

/*****************************************************************************/
/*  Product information                                                      */
/*****************************************************************************/
typedef struct tagPRODUCTINFO
{
    int  nProduct;
    int  nEmuVersion;
    int  nApiVersion;
} PRODUCTINFO;
typedef PRODUCTINFO far * LPPRODUCTINFO;
/*------------------------------------------*/
/* nProduct                                 */
/*------------------------------------------*/
#define    PDT_SYSLINK              1
#define    PDT_SYSGATE              2
#define    PDT_SYSPATH              3
#define    PDT_SYSCNVT              4
#define    PDT_SYSCOMM              5
#define    PDT_SYSSNA               6
#define    PDT_SYSTN                7
#define    PDT_SYSLUA               8
#define    PDT_SYSDLC               9

/*****************************************************************************/
/*  Send data structure                                                      */
/*****************************************************************************/
typedef struct tagSENDDATA
{
    WORD     wAidKey;
    WORD     wTimeOut;
    WORD     wSendLen;
    OIAINFO  stOIAInfo;
    LPSTR    lpData;
} SENDDATA;
typedef SENDDATA far * LPSENDDATA;


/*****************************************************************************/
/*  Receive data structure                                                   */
/*****************************************************************************/
/*  lpRcvData->wMode    */
#define  RCV_NOACTION    0
#define  RCV_DELETE      1
#define  RCV_REPLACE     2
//.......... 
#define  RCV_CUTFMH   0x10 

typedef struct tagRCVDATA
{
    WORD     wTimeOut;
    WORD     wMaxSize;
    WORD     wRcvLen;
    WORD     wRowPos;
    WORD     wColPos;
    WORD     wMode;
    OIAINFO  stOIAInfo;
    LPSTR    lpData;
} RCVDATA;
typedef RCVDATA far * LPRCVDATA;
//--------------
typedef struct tagRCVTPDATA
{
  WORD    wTimeOut;
  WORD    wMaxSize;
  WORD    wRcvLen;
  WORD    wSessNo;
  WORD    wNoUse;
  WORD    wMode;
  OIAINFO stOIAInfo;
  LPSTR   lpData;
} RCVTPDATA;
typedef RCVTPDATA far *LPRCVTPDATA;

/*------------------------------------------*/
/* RecvType definition code                 */
/*------------------------------------------*/
#define    APPC_GDS_DATA            0
#define    APPC_NONGDS_DATA         1
#define    APPC_SENSE_DATA          2
#define    APPC_FMH7_DATA           7

//============================ 這是新增畫面API
typedef struct tagSCREEN
{
  int   stSize;
  int   iScrRow;
  int   iScrCol;
  int   iReserve;
  LPWORD  pwScrText;
  LPWORD  pwScrAttr;
} SCREEN;
typedef SCREEN far * LPSCREEN;

typedef struct tagAPIATTR
{
  BYTE  b3PS:3;
  BYTE  b3EXT:3;
  BYTE  b2ATT:2;
  BYTE  bGT:1;
  BYTE  bGL:1;
  BYTE  bGB:1;
  BYTE  bGR:1;
  BYTE  bCT:1;
  BYTE  b3FM:3;
} APIATTR;
typedef APIATTR far * LPAPIATTR;

typedef struct tagAPITEXT
{
  BYTE  PC;
  BYTE  Host;
} APITEXT;
typedef APITEXT far * LPAPITEXT;

#define   PS_SNULL          0x00
#define   PS_NODSP          0x00
#define   PS_INTENECT       0x01
#define   PS_DSPDECT        0x02
#define   PS_DSPNODECT      0x03
#define   PS_PROTECT        0x04

#define   UNPT_NODECT       0x03

#define   FM_DB             0x04
#define   FM_SB             0x00
#define   FM_DS             0x01
#define   FM_CFIRST         0x06
#define   FM_CSECOND        0x04

#define   ATT_NORMAL        0x00
#define   ATT_UNDERLINE     0x01
#define   ATT_REVERSE       0x02
#define   ATT_FLASH         0x03



/*****************************************************************************/
/*  define AP hook type                                                      */
/*****************************************************************************/
#define  PASS_FX_BYTE   101

//------------------------------------------------
#if defined(SYSLIBAPI)
   #undef   SYSLIBAPI
#endif

//#if !defined(_W32API_)
#ifndef _W32API_
   #ifdef __cplusplus
     #define   SYSLIBAPI    extern "C" _declspec(dllimport)
   #else
     #define   SYSLIBAPI    _declspec(dllimport)
   #endif
#else
   #define   SYSLIBAPI    _declspec(dllexport)
#endif

#if (defined(_W32API_) || defined(_W32OUTB_))
//--------------------------
LPSTR       pScrnData;           /* Screen data pointer       */
LPATTR      pScrnAttr;           /* Screen attribute pointer  */
LPEXTATTR   pScrnEAttr;          /* Screen extended attribute pointer */
LPGRID      pScrnGAttr;          /* Screen grid-line attribute pointer */
LPCLRGRID   pScrnCGrid;          /* Screen Color and Grid Line pointer */
LPSTR       pPrnBuffer;          // Printer buffer pointer
LPSTR       pLuRU;               /* RU pointer                        */
LPBIU       pLuSendBIU;          /* Send BIU pointer                  */
LPBIU       pLuRcvBIU;           /* Receive BIU pointer               */
LPDFTFORM   pLuAEEP;             // send data buffer pointer for AEEP
LPDFTFORM   pLuRDAT;             // send data buffer pointer for RDAT
LPTRANS     pLuTRANS;
LPSTR       pLuAPI;

TPOINTBUF  SrvPoint[MAX_LU];
//--------------------------
#endif

SYSLIBAPI   void CALLBACK SysInitSvrPtr(LPSTR);

/*      Check Emulation Status      */
SYSLIBAPI   int  CALLBACK SysIsEmuExist(void);
SYSLIBAPI   int  CALLBACK SysIsEmuConnect(void);
SYSLIBAPI   int  CALLBACK SysIsApLink(HINSTANCE, int);
SYSLIBAPI   int  CALLBACK SysIsLuCorrect(int);

/*      Handle Emulation Status     */
SYSLIBAPI   int  CALLBACK SysShowEmuWnd(WORD);
SYSLIBAPI   int  CALLBACK SysMoveEmuWnd(int, int, int, int, BOOL);
SYSLIBAPI   int  CALLBACK SysStartEmuModule(int);
SYSLIBAPI   int  CALLBACK SysEndEmuModule(void);
SYSLIBAPI   int  CALLBACK SysEmuConnect(void);
SYSLIBAPI   int  CALLBACK SysEmuDisConnect(void);

/*      Handle LU Status            */
SYSLIBAPI   int  CALLBACK SysApLinkLu( HINSTANCE, int );
SYSLIBAPI   int  CALLBACK SysApUnlinkLu( HINSTANCE, int );
SYSLIBAPI   int  CALLBACK SysForceApUnlinkLu( int );
SYSLIBAPI   int  CALLBACK SysApAutoLinkLu(HINSTANCE, LPINT, LPSTR, int, BYTE);
SYSLIBAPI   int  CALLBACK SysSwitchToSession( int, LPOIAINFO );
SYSLIBAPI   int  CALLBACK SysSwitchNext(int);
SYSLIBAPI   int  CALLBACK SysHookReleaseMsg( int, HWND, WPARAM, int );
SYSLIBAPI   int  CALLBACK SysUnhookReleaseMsg( int );

/*      Get LU Status               */
SYSLIBAPI   int  CALLBACK SysGetLuNo( LPSTR );
SYSLIBAPI   int  CALLBACK SysGetLuAddr( int );

/*      Handle Screen Cursor Position      */
SYSLIBAPI   int  CALLBACK SysSetCurPos( HINSTANCE, int, int, int, LPOIAINFO );
SYSLIBAPI   int  CALLBACK SysGetCurPos( int, LPINT, LPINT, LPOIAINFO );

/*      Get Information             */
SYSLIBAPI   int  CALLBACK SysGetProductInfo( LPPRODUCTINFO );
SYSLIBAPI   int  CALLBACK SysGetOIAInfo(int, LPOIAINFO);
SYSLIBAPI   int  CALLBACK SysGetEmuActWnd(void);
SYSLIBAPI   int  CALLBACK SysGetEmuActLu(void);
SYSLIBAPI   int  CALLBACK SysGetOIAInfoExt(int, LPOIAINFO, LPEXTINFO);

/*      Convertion                  */
SYSLIBAPI   int  CALLBACK SysCvtPc2Host(LPBYTE, int, LPBYTE, int, LPINT);
SYSLIBAPI   int  CALLBACK SysCvtHost2Pc(LPBYTE, int, LPBYTE, int, LPINT);
SYSLIBAPI   int  CALLBACK SysTransPc2Host(LPBYTE, int);
SYSLIBAPI   int  CALLBACK SysTransHost2Pc(LPBYTE, int);
SYSLIBAPI   void CALLBACK SysTransAsc2Ebc(LPBYTE, int);
SYSLIBAPI   void CALLBACK SysTransEbc2Asc(LPBYTE, int);
SYSLIBAPI   int  CALLBACK SysTrans552Host(LPBYTE, int);

SYSLIBAPI   int  CALLBACK SysCvtUni2Host(LPBYTE, int, LPBYTE, int, LPINT);
SYSLIBAPI   int  CALLBACK SysCvtHost2Uni(LPBYTE, int, LPBYTE, int, LPINT);

/*      Send or Receive             */
SYSLIBAPI   int  CALLBACK SysSendNormal( HINSTANCE, int, LPSENDDATA );
SYSLIBAPI   int  CALLBACK SysReceiveNormal( int, LPRCVDATA );

SYSLIBAPI   int  CALLBACK SysSendNoCheck( HINSTANCE, int, LPSENDDATA );
SYSLIBAPI   int  CALLBACK SysSendText( HINSTANCE, int , int , int ,  LPSENDDATA );

SYSLIBAPI   int  CALLBACK SysEnableTransparent( HINSTANCE, int, BOOL, int, LPOIAINFO );
SYSLIBAPI   int  CALLBACK SysSendTransparent( HINSTANCE, int, LPSENDDATA );
SYSLIBAPI   int  CALLBACK SysReceiveTransparent( int, LPRCVDATA );
SYSLIBAPI   int  CALLBACK SysIsReceiveOK( int, WORD, LPOIAINFO );

SYSLIBAPI   int  CALLBACK SysSendAidKey( HINSTANCE, int, WORD, LPOIAINFO );

SYSLIBAPI   int  CALLBACK SysFreeTransBuffEx(HINSTANCE hInst, int nLu, LPOIAINFO lpOIAInfo);
SYSLIBAPI   int  CALLBACK SysReceiveTransparentEx( int nLu, LPRCVTPDATA lpRcvTP );
SYSLIBAPI   int  WINAPI   SysTransparentCallBackEx( int nLu, FARPROC pcRcvBack );
SYSLIBAPI   int  CALLBACK SysFileTransferEx(HINSTANCE, int, LPSTR, LPSTR, LPSTR, LPSTR);
typedef     VOID          (CALLBACK *LPRCVCALLBACK) ( int, int);

// ------  Jansen write in 10,16,97 ------------
SYSLIBAPI   int  CALLBACK SysTermSelf(HINSTANCE, int, BYTE, int, LPSTR);
SYSLIBAPI   BOOL CALLBACK SysCommCheck(HINSTANCE, int, LPWORD);
SYSLIBAPI   BOOL CALLBACK SysSendBIU(HINSTANCE, int, BYTE, BYTE, int, LPSTR);
// ------  Jansen write in 10,16,97 ------------

SYSLIBAPI   int  CALLBACK SysInitSelf( HINSTANCE, int nLu, LPBYTE lpData, int iLen );
SYSLIBAPI   int  CALLBACK SysInitSelfU( HINSTANCE, int nLu, LPBYTE lpData, int iLen, LPSTR lpUser, LPSTR lpPass );
SYSLIBAPI   int  CALLBACK SysSendLUSTAT( HINSTANCE hInst , int nLu, WORD wCode, WORD wCnt);
SYSLIBAPI   int  CALLBACK SysSendLUSTATH( HINSTANCE hInst , int nLu, WORD wCode, WORD wCnt);

SYSLIBAPI   int  CALLBACK SysInitSelfUD( HINSTANCE hInst,int nLu, LPBYTE lpData, int iLen,
                                         LPSTR lpUser, LPSTR lpPass, LPSTR lpUData );
/*      File Transfer               */
SYSLIBAPI   int  CALLBACK SysUpLoadFileTransfer  ( HINSTANCE, int, LPSTR, LPSTR, LPSTR, LPOIAINFO);
SYSLIBAPI   int  CALLBACK SysDownLoadFileTransfer( HINSTANCE, int, LPSTR, LPSTR, LPSTR, LPOIAINFO);
SYSLIBAPI   int  CALLBACK SysIndFileUpLoad  ( HINSTANCE, int, LPSTR, LPSTR, LPSTR, LPOIAINFO);
SYSLIBAPI   int  CALLBACK SysIndFileDownLoad ( HINSTANCE, int, LPSTR, LPSTR, LPSTR, LPOIAINFO);
SYSLIBAPI   int  CALLBACK SysStopFX( int );
SYSLIBAPI   int  CALLBACK SysGetFxErrMsg( int, LPSTR );

/*      Memory Copy                 */
SYSLIBAPI   void CALLBACK SysMemCopy(LPSTR, LPSTR, int);

/*   Caption function */
SYSLIBAPI   void CALLBACK SysShowCaption(int);

SYSLIBAPI   int  CALLBACK SysReceiveNormalAttr( int, LPRCVDATA );

SYSLIBAPI   int  CALLBACK SysGetOIAIBMGroup( int, LPBYTE);
SYSLIBAPI   int  CALLBACK SysGetOIAIBM( int, LPBYTE);
SYSLIBAPI   int  CALLBACK SysWait( int, DWORD, LPLONG );
SYSLIBAPI   int  CALLBACK SysQueryCursor( int, LPLONG );
SYSLIBAPI   int  CALLBACK SysPAUSE ( int, DWORD, LPLONG );
SYSLIBAPI   int  CALLBACK SysHLLSetCursor( int, LPLONG );
SYSLIBAPI   int  CALLBACK SysSendNormalEBCDIC( HINSTANCE, int, LPSENDDATA );
SYSLIBAPI   int  CALLBACK SysReceiveNormalEBCDIC( int, LPRCVDATA );
SYSLIBAPI   int  CALLBACK SysQueryFieldAttr( HINSTANCE, int, LPLONG, LPBYTE, LPBYTE);
SYSLIBAPI   int  CALLBACK SysFindFieldPos( HINSTANCE, int, LPSTR, LPLONG, LPLONG);
SYSLIBAPI   int  CALLBACK SysFindFieldLen( HINSTANCE, int, LPSTR, LPLONG, LPLONG);
SYSLIBAPI   int  CALLBACK SysReceiveNormalNoTranslate( int, LPRCVDATA );
SYSLIBAPI   int  CALLBACK SysSetPacingMode( int, int );
SYSLIBAPI   int  CALLBACK SysRspPacing( int );
SYSLIBAPI   int  CALLBACK SysIssueRspMode( int, int );
SYSLIBAPI   int  CALLBACK SysResponse( int, int, LPBYTE );
SYSLIBAPI   int  CALLBACK SysExecMacro( int nLu, int Mode, LPSTR lpStr );
SYSLIBAPI   BOOL CALLBACK SysIsEndBracket( int nLu );
SYSLIBAPI   BOOL CALLBACK SysIsEndChain( int nLu );
SYSLIBAPI   BOOL CALLBACK SysIsSdtOn( int nLu );

SYSLIBAPI   int  CALLBACK SysKeyboardLock(HINSTANCE, int, BOOL);
SYSLIBAPI   int  CALLBACK SysStopFileTransfer(int nLu);

SYSLIBAPI   int  CALLBACK SysCheckText( int nLu, LPBYTE lpText, WORD wTimeOut);
SYSLIBAPI   int  CALLBACK SysGetFXInfo(int nLu, LPDWORD FxSize, LPINT Status);
SYSLIBAPI   int  CALLBACK SysStartTrans(int nLu, LPSTR str);
SYSLIBAPI   int  WINAPI   SysGetScreenHTML( int nLu, LPBYTE lpDATA, int iSize );
//----- Printer API
SYSLIBAPI   int  WINAPI  SysEnablePrintAPI( int nLu, int EnMode );
SYSLIBAPI   int  WINAPI  SysCheckPrintData( int nLu );
SYSLIBAPI   int  WINAPI  SysGetPrintData( int nLu, LPBYTE lpData, int iSize );
SYSLIBAPI   int  WINAPI  SysViewPrintData( int nLu, int ViewMode, LPBYTE lpData, int iSize );
SYSLIBAPI   int  WINAPI  SysPutPrintData(int nLu, LPBYTE lpData, int iLen);

//----- APPC function call prototype -----
SYSLIBAPI   int  CALLBACK AppcOpen( HINSTANCE, int, int, LPOIAINFO );
SYSLIBAPI   int  CALLBACK AppcClose( HINSTANCE, int, int, LPOIAINFO );
SYSLIBAPI   int  CALLBACK AppcAlloc( int, LPBYTE );
SYSLIBAPI   int  CALLBACK AppcDeAlloc( int );
SYSLIBAPI   int  CALLBACK AppcAttach( int, LPBYTE, int );
SYSLIBAPI   int  CALLBACK AppcStartBracket( int );
SYSLIBAPI   int  CALLBACK AppcCEndBracket( int );
SYSLIBAPI   int  CALLBACK AppcSendData( int, LPBYTE, int );
SYSLIBAPI   int  CALLBACK AppcRecvData( int, WORD, LPBYTE, WORD, LPWORD, LPBYTE );
SYSLIBAPI   int  CALLBACK AppcInitSelf( int, LPBYTE, LPBYTE, int, LPBYTE, int );
SYSLIBAPI   int  CALLBACK AppcTermSelf( int, LPBYTE, int, LPBYTE, int );

SYSLIBAPI   int  CALLBACK SysEmuConnectSession(HINSTANCE ,int ,int);
SYSLIBAPI   int  CALLBACK SysEmuDisConnectSession(int,int);

//---------------- New
SYSLIBAPI   int  CALLBACK SysCheckRCText( int nLu, int Row, int Col, LPBYTE lpText, WORD wTimeOut);
SYSLIBAPI   int  WINAPI SysGetEmuNum( int nItem );
SYSLIBAPI   int  WINAPI SysGetScreen(int nLu, LPSCREEN lpScreen, int Mode);
/*------------------------------------------*/
#define  MX_LUNUM     1
#define  MX_ROWNUM    2
#define  MX_COLNUM    3
#define  AP_PROCESS   100
#define  AP_PROCESSID 101
#define  AP_THREAD    102
#define  AP_THREADID  103
SYSLIBAPI   int  WINAPI SysEnableCheckText( int nLu, int ckEnable, FARPROC pcCkBack );
SYSLIBAPI   int  WINAPI SysSetCheckText( int nLu, int ckID, LPBYTE lpText, int iLen, int ckRow, int ckCol, int ckMode ); 
typedef     VOID        (CALLBACK *LPCKSCALLBACK) ( int );

SYSLIBAPI   void  WINAPI InitLuTable(); 
SYSLIBAPI   int   WINAPI SysFileTrans  ( HINSTANCE, int, int , LPSTR, LPSTR, LPSTR, LPOIAINFO);

