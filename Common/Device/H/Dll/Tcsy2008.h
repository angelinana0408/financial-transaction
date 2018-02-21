/////////////////////////////////////////////////////////////////////////////
// Tcsy2008.h : main header file for the TCSY2008 DLL                      //
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCSY2008_H__0EEBA4CA__INCLUDED_)
#define AFX_TCSY2008_H__0EEBA4CA__INCLUDED_

/*=///////////////////////////////////////////////////////////////////////=*/
/*= ȫ�ֳ������ṹ����ʶ�ȵĶ���      sck007  Updated On 2007.08.16       =*/
/*=///////////////////////////////////////////////////////////////////////=*/

#define IsOK(r)			     ((r) == 0)				/* �Ƿ�ɹ� */

#define TCR_SUCC				0x00				/* �ɹ����� */
#define TCR_FAIL				0x01				/* ʧ�ܽ�� */
#define TCR_ERRO				0x02				/* У����� */
#define TCR_TMOT				0x03				/* ������ʱ */
#define TCR_NODV				0x04				/* δ���豸 */
#define TCR_FLSH				0x05				/* д����� */
#define TCR_NOFP				0x06				/* δ����ث */
#define TCR_NOSM				0x07				/* ֵ����� */
#define TCR_NOMT				0x08				/* ֵ��ƥ�� */
#define TCR_EPLB				0x09				/* ����Ϊ�� */
#define TCR_TOLV				0x0A				/* ��̧���� */
#define TCR_NMEM				0x0B				/* �ڴ治�� */

#define TCR_LOST				0xFE				/* �����Ч */

#define TCR_BUSY				0xFF				/* ����æ�� */

/*=///////////////////////////////////////////////////////////////////////=*/
/*= �������õľֲ����ܺ����������ⲿҲ���Ե��õ�                          =*/
/*=///////////////////////////////////////////////////////////////////////=*/

#define TCSY2008_API extern "C" __declspec(dllexport)

/*===============================================================*/

/* ��hWnd�����У���ʾָ��(��X��)��ָ��ͼ�� */
TCSY2008_API LONG ShowFprImage(LONG hWindow, BYTE *pbyImage,
				LONG nWide, LONG nHigh, LONG bFlip);

/* �õ�ָ���ļ��Ĵ�С������ֵΪ�ֽ��� */
TCSY2008_API LONG GetFileLength(LPCTSTR chFile);

/* ȷ�ϸ����ļ����ڵ�·������'\\'֮ǰ��ΪĿ¼��Ч��ʶ */
TCSY2008_API LONG MakeFilePath(LPCTSTR chFile);

/*===============================================================*/

/* ����������ļ����������ĳ��ȣ����Զ�ȷ���ļ�·�� */
TCSY2008_API LONG SaveBinaryFile(LPCTSTR chFile, CONST BYTE *pbyBinary,
							   LONG nLength);

/* ���ض������ļ����ȸ�������ĳ��ȣ�������ʵ��ʹ�ó��� */
TCSY2008_API LONG LoadBinaryFile(LPCTSTR chFile, BYTE *pbyBinary,
							   LONG nLength);

/*===============================================================*/

/* �����ͼ��ĵ�������ΪBMP�ļ�(���Դ�����4���������Ŀ��) */
TCSY2008_API LONG SaveTotalImage(LPCTSTR chFile,	/* �����ļ��� */
				CONST BYTE *pbyImage,				/* ͼ����ָ�� */
				LONG nWide, LONG nHigh);			/* ͼ������� */

/* ������ͨBMP��������(�ɶ�ȡ��4���������ͼ�񣬷���ʵ�ó���) */
TCSY2008_API LONG LoadBmpFileMem(LPCTSTR chFile,	/* �����ȡ�ļ� */
				BYTE *pbyImage, LONG nLength,		/* ͼ���ڴ����� */
				WORD *pwWide, WORD *pwHigh);		/* ��Ӧ������ */

/*===============================================================*/

/* Allocate memory from memory management system */
TCSY2008_API LONG TcMalloc(DWORD dwSize);

/* Free allocated memory, remove from free list */
TCSY2008_API VOID TcFree(LONG pPtrUsr);

/* Reallocate a packet to a new size */
TCSY2008_API LONG TcRealloc(LONG pPtrUsr, DWORD dwSize);

/* Allocate and clear memory from mem mgmt system */
TCSY2008_API LONG TcCalloc(DWORD dwNum, DWORD dwSize);

/* ����8-Bit�ĳ��ȣ�����ֵ�����ָ�������XOR������ */
TCSY2008_API BYTE GenXOR(BYTE byChk, const VOID *pData, DWORD dwLen);

/*===============================================================*/

/* ���´���ͨѶ�ĳ�ʱ���ã�Ŀ�ģ��ȴ�������ɣ�ֱ����ʱ */
TCSY2008_API BOOL UpdateCommTimeOut(HANDLE hPort, DWORD dwTimeOut);

/* �Ӵ�����������ʽ��ȡָ�������Ķ��������ݣ���ѡ��ַ�ʽ */
TCSY2008_API BOOL ReadComm(HANDLE hPort, VOID *pBuff, DWORD dwLen,
						   BOOL bSplit = 0);

/* ��ָ�������Ķ���������д�봮�ڣ�������ʽ����ѡ��ַ�ʽ */
TCSY2008_API BOOL WritComm(HANDLE hPort, const VOID *pBuff, DWORD dwLen,
						   BOOL bSplit = 0);

/*=///////////////////////////////////////////////////////////////////////=*/
/*= ����֧�ֿ�LIB������ӿ�������hComm��֧��ͬʱ�򿪶������              =*/
/*=///////////////////////////////////////////////////////////////////////=*/

/* ��ȡ��DLLģ�������汾�ţ�������ʹ�� */
TCSY2008_API VOID TcGetVersion(BYTE *pbyHiv, BYTE *pbyLov);

/*===============================================================*/

/* ��ͨѶ����ָ���Ĵ��ڣ������ʣ����ع������ */
TCSY2008_API LONG TcOpenComm(LONG nComm, LONG nRate,
							 LONG nFmt, HWND hDadWnd);

/* �ر�ָ�������������ͨѶ���ͷ�������Դ */
TCSY2008_API LONG TcCloseComm(LONG hComm);

/* �жϴ���ͨѶ�Ƿ����ڽ�����... */
TCSY2008_API BOOL TcIsCommBusy(LONG hComm);

/*===============================================================*/

/* ORD_INFO����ȡ�ں���Ϣ���汾��ͼ���ߡ����������ڴ�ʹ������� */
TCSY2008_API LONG TcGetInfo(LONG hComm, LONG nStyle, BYTE *pbyBuff,
							DWORD *pdwLen);

/* ORD_SCFG�������������ݣ������ں˵Ĳ���״̬(SCK007ר��) */
TCSY2008_API LONG TcSetPara(LONG hComm, LONG nStyle, CONST BYTE *pbyBuff,
							LONG nLen);

/* ORD_BAUD���貨���ʣ���ʧ����ر�&�ͷű�ͨѶ */
TCSY2008_API LONG TcSetBaud(LONG hComm, LONG nRate);

/*===============================================================*/

/* ORD_FLSR������������С��0x20000������Ϊϵͳ���������ɶ�д */
TCSY2008_API LONG TcFlashGet(LONG hComm, BYTE *pBuff, LONG nAddr,
							 LONG nLen);

/* ORD_FLSW��д��������С��0x20000������Ϊϵͳ���������ɶ�д */
TCSY2008_API LONG TcFlashSet(LONG hComm, CONST BYTE *pBuff, LONG nAddr,
							 LONG nLen);

/*===============================================================*/

/* ORD_LDCR��ץ��ͼ������ͼ�����ص�PIMG���Ա���ȡ����(SCK007ר��) */
TCSY2008_API LONG TcLoadrToPimg(LONG hComm, LONG nStyle, BYTE *pImage,
								DWORD *pdwLen);

/* ORD_MNUT����PIMGȡ������ֵ���ݴ��������Ƿ񴫳���ѡ(SCK007ר��) */
TCSY2008_API LONG TcMinutFrPimg(LONG hComm, LONG nIndex,
					LONG nRetrn, BYTE *pMinut, DWORD *pdwLen);

/* ORD_MTCH���Ƚ��Ѵ���ö��ʱ������⣬���ط������ʶ(SCK007ר��) */
TCSY2008_API LONG TcMatchInside(LONG hComm, LONG nStyle, BYTE *pbyBuff,
								DWORD *pdwLen);

/*===============================================================*/

/* ORD_LOAD�����豸���´�FLASH�м��������⣬wSumFtr�������� */
TCSY2008_API LONG TcLoadFtrLib(LONG hComm, WORD *pwSumFtr);

/* ORD_HARD��ʹ�豸��������ѭ�����Բ�����Ҫ����ģ�� */
TCSY2008_API LONG TcHardTest(LONG hComm, BOOL bToMany);

/* ORD_CTRL�����豸�����豸(��������LED��)����һ��ʱ�� */
TCSY2008_API LONG TcSubDev(LONG hComm, BYTE byWho, BYTE byTim);

/* ORD_RSTX������Ӳ����λָ���FLASH���¼��ش��벢���� */
TCSY2008_API LONG TcResetX(LONG hComm);

/*===============================================================*/

/* ORD_REGI��������ö�������غϳ�һö����ѡ�Ƿ񲹲⣬�������� */
TCSY2008_API LONG TcGetRegist(LONG hComm, LONG nStyle, BYTE *pMinut,
							  DWORD *pdwLen);

/* ORD_VERI������һ�Σ����豸��һ�Զ���������ѡ�Ƿ�Ҫ���� */
TCSY2008_API LONG TcVeriMatch(LONG hComm, LONG nStyle, BYTE *pbyBuff,
							  DWORD *pdwLen);

/*===============================================================*/

/* ORD_REGT��ǿ����ע��ָ�������ӿڣ��ɷ���������ͼ����С���� */
TCSY2008_API LONG TcSuperRegist(LONG hComm, LONG nStyle, LONG nRetun,
				   BYTE *pbyBuff, DWORD *pdwLen);

/* ORD_CBRT���궨UPEK�豸ָ�һ����Sensor��һ����ֻ����һ�� */
TCSY2008_API LONG TcCalibrateUPEK(LONG hComm);

/*=///////////////////////////////////////////////////////////////////////=*/

#endif // !defined(AFX_TCSY2008_H__0EEBA4CA__INCLUDED_)

/*=///////////////////////////////////////////////////////////////////////=*/
/*= The end of this file. =*/
