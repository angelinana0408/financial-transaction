/////////////////////////////////////////////////////////////////////////////
// Tcsy2008.h : main header file for the TCSY2008 DLL                      //
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCSY2008_H__0EEBA4CA__INCLUDED_)
#define AFX_TCSY2008_H__0EEBA4CA__INCLUDED_

/*=///////////////////////////////////////////////////////////////////////=*/
/*= 全局常量，结构，标识等的定义      sck007  Updated On 2007.08.16       =*/
/*=///////////////////////////////////////////////////////////////////////=*/

#define IsOK(r)			     ((r) == 0)				/* 是否成功 */

#define TCR_SUCC				0x00				/* 成功操作 */
#define TCR_FAIL				0x01				/* 失败结果 */
#define TCR_ERRO				0x02				/* 校验错误 */
#define TCR_TMOT				0x03				/* 操作超时 */
#define TCR_NODV				0x04				/* 未连设备 */
#define TCR_FLSH				0x05				/* 写闪存错 */
#define TCR_NOFP				0x06				/* 未按或孬 */
#define TCR_NOSM				0x07				/* 值不相关 */
#define TCR_NOMT				0x08				/* 值不匹配 */
#define TCR_EPLB				0x09				/* 库链为空 */
#define TCR_TOLV				0x0A				/* 请抬起手 */
#define TCR_NMEM				0x0B				/* 内存不足 */

#define TCR_LOST				0xFE				/* 句柄无效 */

#define TCR_BUSY				0xFF				/* 真是忙啊 */

/*=///////////////////////////////////////////////////////////////////////=*/
/*= 本地所用的局部功能函数声明，外部也可以调用的                          =*/
/*=///////////////////////////////////////////////////////////////////////=*/

#define TCSY2008_API extern "C" __declspec(dllexport)

/*===============================================================*/

/* 在hWnd窗体中，显示指定(宽X高)的指纹图像 */
TCSY2008_API LONG ShowFprImage(LONG hWindow, BYTE *pbyImage,
				LONG nWide, LONG nHigh, LONG bFlip);

/* 得到指定文件的大小：返回值为字节数 */
TCSY2008_API LONG GetFileLength(LPCTSTR chFile);

/* 确认给定文件所在的路径，以'\\'之前作为目录有效标识 */
TCSY2008_API LONG MakeFilePath(LPCTSTR chFile);

/*===============================================================*/

/* 保存二进制文件，按给定的长度，且自动确认文件路径 */
TCSY2008_API LONG SaveBinaryFile(LPCTSTR chFile, CONST BYTE *pbyBinary,
							   LONG nLength);

/* 加载二进制文件，先给定缓存的长度，并返回实际使用长度 */
TCSY2008_API LONG LoadBinaryFile(LPCTSTR chFile, BYTE *pbyBinary,
							   LONG nLength);

/*===============================================================*/

/* 保存的图像的点阵数据为BMP文件(可以存贮非4的整数倍的宽度) */
TCSY2008_API LONG SaveTotalImage(LPCTSTR chFile,	/* 欲存文件名 */
				CONST BYTE *pbyImage,				/* 图像区指针 */
				LONG nWide, LONG nHigh);			/* 图像区宽高 */

/* 加载普通BMP的数据区(可读取非4整数倍宽度图像，返回实用长度) */
TCSY2008_API LONG LoadBmpFileMem(LPCTSTR chFile,	/* 输入读取文件 */
				BYTE *pbyImage, LONG nLength,		/* 图像内存区域 */
				WORD *pwWide, WORD *pwHigh);		/* 对应区域宽高 */

/*===============================================================*/

/* Allocate memory from memory management system */
TCSY2008_API LONG TcMalloc(DWORD dwSize);

/* Free allocated memory, remove from free list */
TCSY2008_API VOID TcFree(LONG pPtrUsr);

/* Reallocate a packet to a new size */
TCSY2008_API LONG TcRealloc(LONG pPtrUsr, DWORD dwSize);

/* Allocate and clear memory from mem mgmt system */
TCSY2008_API LONG TcCalloc(DWORD dwNum, DWORD dwSize);

/* 基于8-Bit的长度，及初值，求出指定区域的XOR检验码 */
TCSY2008_API BYTE GenXOR(BYTE byChk, const VOID *pData, DWORD dwLen);

/*===============================================================*/

/* 更新串口通讯的超时设置，目的：等待操作完成，直至超时 */
TCSY2008_API BOOL UpdateCommTimeOut(HANDLE hPort, DWORD dwTimeOut);

/* 从串口以阻塞方式读取指定数量的二进制数据，可选拆分方式 */
TCSY2008_API BOOL ReadComm(HANDLE hPort, VOID *pBuff, DWORD dwLen,
						   BOOL bSplit = 0);

/* 将指定数量的二进制数据写入串口，阻塞方式，可选拆分方式 */
TCSY2008_API BOOL WritComm(HANDLE hPort, const VOID *pBuff, DWORD dwLen,
						   BOOL bSplit = 0);

/*=///////////////////////////////////////////////////////////////////////=*/
/*= 驱动支持库LIB，对外接口声明，hComm可支持同时打开多个串口              =*/
/*=///////////////////////////////////////////////////////////////////////=*/

/* 获取本DLL模块的软件版本号，检查更新使用 */
TCSY2008_API VOID TcGetVersion(BYTE *pbyHiv, BYTE *pbyLov);

/*===============================================================*/

/* 打开通讯，按指定的串口，和速率，返回关联句柄 */
TCSY2008_API LONG TcOpenComm(LONG nComm, LONG nRate,
							 LONG nFmt, HWND hDadWnd);

/* 关闭指定句柄所关联的通讯，释放所用资源 */
TCSY2008_API LONG TcCloseComm(LONG hComm);

/* 判断串口通讯是否正在进行中... */
TCSY2008_API BOOL TcIsCommBusy(LONG hComm);

/*===============================================================*/

/* ORD_INFO：获取内核信息：版本、图像宽高、库容量、内存使用情况等 */
TCSY2008_API LONG TcGetInfo(LONG hComm, LONG nStyle, BYTE *pbyBuff,
							DWORD *pdwLen);

/* ORD_SCFG：发送配置数据，更改内核的参数状态(SCK007专用) */
TCSY2008_API LONG TcSetPara(LONG hComm, LONG nStyle, CONST BYTE *pbyBuff,
							LONG nLen);

/* ORD_BAUD：设波特率，若失败则关闭&释放本通讯 */
TCSY2008_API LONG TcSetBaud(LONG hComm, LONG nRate);

/*===============================================================*/

/* ORD_FLSR：读闪存区，小于0x20000的区域为系统保留，不可读写 */
TCSY2008_API LONG TcFlashGet(LONG hComm, BYTE *pBuff, LONG nAddr,
							 LONG nLen);

/* ORD_FLSW：写闪存区，小于0x20000的区域为系统保留，不可读写 */
TCSY2008_API LONG TcFlashSet(LONG hComm, CONST BYTE *pBuff, LONG nAddr,
							 LONG nLen);

/*===============================================================*/

/* ORD_LDCR：抓拍图或闪存图，加载到PIMG，以备提取特征(SCK007专用) */
TCSY2008_API LONG TcLoadrToPimg(LONG hComm, LONG nStyle, BYTE *pImage,
								DWORD *pdwLen);

/* ORD_MNUT：从PIMG取出特征值，暂存索引，是否传出可选(SCK007专用) */
TCSY2008_API LONG TcMinutFrPimg(LONG hComm, LONG nIndex,
					LONG nRetrn, BYTE *pMinut, DWORD *pdwLen);

/* ORD_MTCH：比较已存两枚临时或链表库，返回分数或标识(SCK007专用) */
TCSY2008_API LONG TcMatchInside(LONG hComm, LONG nStyle, BYTE *pbyBuff,
								DWORD *pdwLen);

/*===============================================================*/

/* ORD_LOAD：让设备重新从FLASH中加载特征库，wSumFtr已载数量 */
TCSY2008_API LONG TcLoadFtrLib(LONG hComm, WORD *pwSumFtr);

/* ORD_HARD：使设备进入无限循环，以测试主要功能模块 */
TCSY2008_API LONG TcHardTest(LONG hComm, BOOL bToMany);

/* ORD_CTRL：让设备的子设备(蜂鸣器或LED灯)工作一定时长 */
TCSY2008_API LONG TcSubDev(LONG hComm, BYTE byWho, BYTE byTim);

/* ORD_RSTX：发送硬件复位指令，从FLASH重新加载代码并运行 */
TCSY2008_API LONG TcResetX(LONG hComm);

/*===============================================================*/

/* ORD_REGI：按捺三枚，检查相关合成一枚，可选是否补测，返回特征 */
TCSY2008_API LONG TcGetRegist(LONG hComm, LONG nStyle, BYTE *pMinut,
							  DWORD *pdwLen);

/* ORD_VERI：按捺一次，在设备内一对多搜索，可选是否要特征 */
TCSY2008_API LONG TcVeriMatch(LONG hComm, LONG nStyle, BYTE *pbyBuff,
							  DWORD *pdwLen);

/*===============================================================*/

/* ORD_REGT：强化的注册指纹特征接口，可返回特征，图像缩小传出 */
TCSY2008_API LONG TcSuperRegist(LONG hComm, LONG nStyle, LONG nRetun,
				   BYTE *pbyBuff, DWORD *pdwLen);

/* ORD_CBRT：标定UPEK设备指令，一般在Sensor的一生中只运行一次 */
TCSY2008_API LONG TcCalibrateUPEK(LONG hComm);

/*=///////////////////////////////////////////////////////////////////////=*/

#endif // !defined(AFX_TCSY2008_H__0EEBA4CA__INCLUDED_)

/*=///////////////////////////////////////////////////////////////////////=*/
/*= The end of this file. =*/
