#include "stdafx.h"
#include "CmnLibEx.h"
#include "Hancode.h"

BOOL	hanja, Continue;
#define	SO			0x0e
#define	SI			0x0f
#define	KSCSPACE	0xa1a1
#define IBMSPACE	0x4040

////////////////////////////////////////
/// main function
////////////////////////////////////////
int WINAPI CodeCvt(void* inbuffb, void* outbuffb, int length, int attr) 
{
	BYTE*	inbuff = (BYTE*)inbuffb;
	BYTE*	outbuff = (BYTE*)outbuffb;
	BYTE	EditBuff[STRBUFFSIZE];
	int		retlength = 0;
	
	switch (attr)
	{
		case ASCEBC:
			retlength = CvtAsc2Ebc(inbuff,EditBuff,length);
			break;

		case EBCASC:
			retlength = CvtEbc2Asc(inbuff,EditBuff,length);
			break;

		case KSIBM:
			retlength = CvtKs2Ibm(inbuff,EditBuff,length);
			break;

		case IBMKS:
			retlength = CvtIbm2Ks(inbuff,EditBuff,length);
			break;

		case NBIBM:
			retlength = CvtNbyte2Ibm(inbuff,EditBuff,length);
			break;

		case NBKS:
			retlength = CvtNbyte2Ks(inbuff,EditBuff,length,NBKS);
			break;

		case NBKS_PSB_DISP:
			retlength = CvtNbyte2Ks(inbuff,EditBuff,length,NBKS_PSB_DISP);
			break;

		case NBKS_PSB_PBM:
			retlength = CvtNbyte2Ks(inbuff,EditBuff,length,NBKS_PSB_PBM);
			break;

		default:
			return 0;
			break;
	}

    memcpy(outbuff,EditBuff,retlength);
	return retlength;
}

////////////////////////////////////////
// KS mixed 2byte and 1byte -> IBM mixed 2byte and 1byte
// return value : edited length
int WINAPI KSC2IBM2(unsigned char *src,unsigned char *dst,int len)
{
	int	i, j;
	unsigned short	ss;
	unsigned char	cc1;

	i = 0;
	for (j=0; (j+1) < len; i+=2) {
		if (src[j] < 0xb0) {		/* special character */
			if (src[j] == 0x20)
			{
				dst[i] = 0x40;
				dst[i+1] = 0x40;
			}
			else
			{
				dst[i] = 0x42;
				CvtAsc2Ebc(&src[j],&cc1,1);
				dst[i+1] = cc1;
			}
			j++;
		}
		else
		{
			ss = ((unsigned short)src[j] << 8) + src[j+1];
			j++;
			ss = ksibm(ss);
			dst[i] = (unsigned char)(ss >> 8);
			dst[i+1] = (unsigned char)(ss & 0xff);
			j++;
		}
	}
	return i;
}

////////////////////////////////////////
/// sub function called by main function
////////////////////////////////////////
// ascii -> ebc code conversion
// return value : edited length
int	CvtAsc2Ebc(BYTE *instr, BYTE *outstr, int lengthofstring)
{
	for (int n = 0; n  < lengthofstring; n++)
		outstr[n] = aetbl[instr[n] & 0xff];

	return n++;
}

// ebc -> ascii code conversion
// return value : edited length
int	CvtEbc2Asc(BYTE *instr, BYTE *outstr, int lengthofstring)
{
	for (int n = 0; n < lengthofstring; n++)
		outstr[n] = eatbl[instr[n] & 0xff];

	return n++;
}

// ibm 2byte -> ks 2byte code conversion
// return value : ks 2byte edited length
int	CvtIbm2Ks(BYTE *instr, BYTE *outstr, int lengthofstring)
{
	WORD	ksstr;
	WORD	ibmstr;

	for (int s = 0; s < (lengthofstring/2) ; s++)
	{
		ibmstr = ((WORD)instr[s*2] << 8) | (WORD)instr[s*2+1];
		ksstr = ibmks(ibmstr);

        outstr[s*2] = (BYTE)((ksstr >> 8) & 0x00ff);
		outstr[s*2+1] = (BYTE)(ksstr & 0x00ff);
	}

	return lengthofstring; 
}

// ks 2byte -> ibm 2byte code conversion
// return value : ibm 2byte edited length
int	CvtKs2Ibm(BYTE *instr, BYTE *outstr, int lengthofstring)
{
	WORD	ksstr;
	WORD	ibmstr;

	for (int s = 0; s < (lengthofstring/2) ; s++)
	{
		ksstr = ((WORD)instr[s*2] << 8) | (WORD)instr[s*2+1];
		ibmstr = ksibm(ksstr);

        outstr[s*2] = (BYTE)((ibmstr >> 8) & 0x00ff);
		outstr[s*2+1] = (BYTE)(ibmstr & 0x00ff);
	}

	return lengthofstring; 
}

// n byte -> ibm 2byte code conversion
// return value : ibm 2byte edited length
int CvtNbyte2Ibm(BYTE *inbuff,BYTE *EditBuff,int length)
{
    return (Nbyte2ibm(inbuff,EditBuff,length,NBKS)); 
}

// n byte -> ks 2byte code conversion
// return value : ks 2byte edited length
int CvtNbyte2Ks(BYTE *inbuff,BYTE *EditBuff,int length,int CodeSel)
{
	BYTE	tmpbuff[STRBUFFSIZE];

    int		ibmlen = Nbyte2ibm(inbuff,tmpbuff,length,CodeSel); 
    int		kslen = CvtIbm2Ks(tmpbuff, EditBuff,ibmlen);

	return kslen;
}


////////////////////////////////////////
/// second sub function 
////////////////////////////////////////
// Ibm -> Ks 1 code conversion execution process
// Return value : Word type Ks code
WORD ibmks(WORD icode)
{
	WORD	ss;

	if (icode < 0x4040)
	{
		ss = 0xa1a1;
		return ss;
	}

	ss = tblproc(icode);
	if ((ss == 0) || (ss == 1) || (ss == -1))
		ss = 0xa1a1;	// KS 2 Bytes Space
	return ss;
}

// Ks -> Ibm 1 code conversion execution process
// Return value : Word type Ibm code
WORD ksibm(WORD incode)
{
	WORD	code1, kcode, yy, zz, lb1, hb1;

	hanja = FALSE;
	code1 = 0;
	kcode = incode;

	if ((incode > 0xa1a0) && (incode < 0xc9ff))
	{
		hanja = FALSE;
		zz = incode;
		lb1 = zz & 0x00ff;

		if ((lb1 < 0xa1) || (lb1  == 0xff))
			kcode = 0xa1a1;

		code1 = kcode;
		hb1 = (kcode >> 8) - 0x00a1;
		code1 = code1 - 0xa1a0 - hb1 * 160;
	}
	else
	{
		if ((0xcaa0 < incode) && (incode < 0xfeff))
		{
			hanja = TRUE;
			zz = incode;
			lb1 = zz & 0x00ff;

			if ((lb1 < 0x00a1) || (lb1 == 0x00ff))
				kcode = 0xa1a1;

			code1 = kcode;
			hb1 = (kcode >> 8) - 0x00c1;
			code1 = code1 - 0xcaa0 - hb1 * 160;
		}
	}

	if ((code1 == 0) || (hanja == TRUE))
		code1 = 1;

	yy = tblproc(code1);

	if ((yy == 0) || (yy == 1) || (yy == -1))
		yy = 0x4040;
	
	return yy;
}

// Table seek and assemble execution process
// Return value : Word type 1 Code
WORD tblproc(WORD code3)
{
	WORD	num1,num2,num3, number;
	WORD	saddr, eaddr, rtval, lp, rtval1, diff;
    WORD	j,d;
	
	j = 0;
	rtval1 = 1;

	if (code3 < 0x4040) 
	{							// 한글 스페이스 이하
		if (hanja == TRUE )
			return 0x4040;
		return tbl1[code3];
	}
	else 
	{							// 일반 글자 
		rtval = 0;
		saddr = 0;
		if ((code3 >= 0x4040) && (code3 < 0x4699)) 
		{
			saddr = getarray(0xa1a1);
			eaddr = getarray(0xabf9);

	
			for(d=saddr;d<=eaddr-1;d++) 
			{
				if (code3 == tbl1[d]) 
				{
					rtval = d + (d/96) * 160 + 0xa1a0;
					break;
				}
			}
			saddr = 0;
		} 
		else if (((code3 & 0x03ff) == 0x41) &&
	             (((code3 & 0xfc00) >= 0x8800) &&
	              ((code3 & 0xfc00) <= 0xd000)))
		{
			saddr = getarray(0xa4a1);
			eaddr = getarray(0xa4be);
			for(d=saddr;d<=eaddr;d++) 
			{
				if (code3 == tbl1[d]) 
				{
					rtval = d + (d/96) * 160 + 0xa1a0;
					break;
				}
			}
			return rtval;
		}
		else 
		{
			number = GetHangulCase(code3);
			switch (number) 
			{
				case GA :		// 0
					saddr = getarray(0xb0a1);
					eaddr = getarray(0xb3ab);
					break;
				case NA :		// 1
					saddr = getarray(0xb3aa);
					eaddr = getarray(0xb4da);
					break;
				case DA :		// 2
					saddr = getarray(0xb4d9);
					eaddr = getarray(0xb6f4);
					break;
				case LA :		// 3
					saddr = getarray(0xb6f3);
					eaddr = getarray(0xb8b7);
					break;
				case MA :		// 4
					saddr = getarray(0xb8b6);
					eaddr = getarray(0xb9da);
					break;
				case BA :		// 5
					saddr = getarray(0xb9d9);
					eaddr = getarray(0xbbe8);
					break;
				case SA :		// 6
					saddr = getarray(0xbbe7);
					eaddr = getarray(0xbec7);
					break;
				case A	:		// 7
					saddr = getarray(0xbec6);
					eaddr = getarray(0xc0db);
					break;
				case JA :		// 8
					saddr = getarray(0xc0da);
					eaddr = getarray(0xc2f8);
					break;
				case CHA :		// 9
					saddr = getarray(0xc2f7);
					eaddr = getarray(0xc4ac);
					break;
				case KA	 :		// 10
					saddr = getarray(0xc4ab);
					eaddr = getarray(0xc5b9);
					break;
				case TA  :		// 11
					saddr = getarray(0xc0b8);
					eaddr = getarray(0xc6c5);
					break;
				case PA  :		// 12
					saddr = getarray(0xc6c4);
					eaddr = getarray(0xc7d0);
					break;
				case HA  :		// 13
					saddr = getarray(0xc7cf);
					eaddr = getarray(0xc8ff);
					break;
				default:
					break;
			}
		}

		if (saddr != 0) 
		{
			lp = 0;
			saddr = saddr - 1;
			diff = (eaddr - saddr) / 2;
			d = saddr + diff;

			Continue = TRUE;
				// Binary Search
			while ((code3 != tbl1[d]) && (Continue == TRUE)) 
			{
				if (code3 == tbl1[saddr]) 
				{
					d = saddr;
					Continue = FALSE;
					continue;
				}

				if (code3 == tbl1[eaddr]) 
				{
					d = eaddr;
					Continue = FALSE;
					continue;
				}

				d++;
				if (code3 == tbl1[d]) 
				{
					Continue = FALSE;
					continue;
				}

				d = d - 2;
				if (code3 == tbl1[d]) 
				{
					Continue = FALSE;						
					continue;
				}

				d++;
				if (tbl1[d] == 0xffff) 
				{
					d--;
					if (tbl1[d] == 0xffff)
						d--;
				}

				if (code3 > tbl1[d]) 
				{
					saddr = saddr + diff;
					diff = (eaddr - saddr) / 2;
					d = saddr + diff;
				}
				else 
				{
					eaddr = saddr + diff;
					diff = (eaddr - saddr) / 2;
					d = eaddr - diff;
				}

				if (lp >= 9) 
				{
					rtval1 = 0;
					Continue = FALSE;
					continue;
				}
				lp++;
			}	// while
			rtval = d + (d/96) * 160 + 0xa1a0;
			if (rtval1 == 0)
				rtval = 0;
		}

		if (rtval == 0) 
		{
			num1 = code3 & 0xff00;
			num2 = code3 & 0x00ff;
			num3 = code3 & 0xf00f;
			if ((num1 == 0x8400) || (num2 == 0x0041) || (num3 == 0x8001)) 
			{
				saddr = getarray(0xa4a1);
				eaddr = getarray(0xa4fe);
				Continue = TRUE;
				d = saddr;
				while ((Continue == TRUE) && (d <= eaddr)) 
				{
					if (code3 == tbl1[d]) 
					{
						rtval = d + (d/96) * 160 + 0xa1a0;
						Continue = FALSE;
					}
					d++;
				} // while
			}  // if num1, num2, num3 end
		}

		if (rtval1 == 0) 
			rtval = 0xa1a1;

		if ((code3 >= 0x5041) && (code3 < 0x67c3)) 
		{
			rtval = 0xa1a1;
		}
		return rtval;
	}
}

// Decide start/end scope for binary search
// Return value : Word type scope code
WORD getarray(WORD code)
{
	WORD	highbyte, tmpcode;

	tmpcode = code;
	highbyte = (code >> 8) - 0x00a1;
	return	(tmpcode - 0xa1a0 - highbyte * 160);
}

// Decide 한글 범위 by enterted code
// Return value : Word type scope code
WORD GetHangulCase(WORD code3)
{
	typedef struct code_type 
	{
		WORD	low_val;
		WORD	high_val;
	} tblnum;

	tblnum table[] = 
	{
				0x8861, 0x9061,	// 0	가
				0x9061, 0x9461,	// 1	나
				0x9461, 0x9c61,	// 2	다
				0x9c61, 0xa061,	// 3	라
				0xa061, 0xa461,	// 4	마
				0xa461, 0xac61,	// 5	바
				0xac61, 0xb461,	// 6	사
				0xb461, 0xb861,	// 7	아
				0xb861, 0xc061,	// 8	자
				0xc061, 0xc461,	// 9	차
				0xc461, 0xc861,	// 10	카
				0xc861,	0xcc61,	// 11	타
				0xcc61, 0xd061,	// 12	파
				0xd061, 0xd3b8	// 13	하
	};

    WORD retv = 0;
	Continue = TRUE;
	int n = 0;

	while ((Continue == TRUE) && (n < 14))
	{
		if ((table[n].low_val <= code3) && (code3 < table[n].high_val))
		{
			retv = n;
			Continue = FALSE;		// Out of Continue
		}
		n++;					   
	}

	return retv;
}

#define		JA_EUM		1
#define		MO_EUM		2
//
// n_byte 한글을 초성, 중성, 종성으로 분리 -> ibm code conversion 
// Return value : ibm edited length
//
int Nbyte2ibm(BYTE *nBuff, BYTE *ibmBuff, int nlength, int CodeSel) 
{
	BYTE	tmp[STRBUFFSIZE];
	BYTE	sBuff[STRBUFFSIZE];
	BYTE	wkb[10];
	int		hanf;
	int		hw,kw,pb,enb,i,rval;
	short	jamoDelm;		// Ja/Mo Delemitor
	short	jamoFunc;		// 자음 모음에 대한 각 제어 처리 분기 

	hanf = 0;	
	memset(tmp, 0x00, sizeof(tmp));
	memset(sBuff, 0x00, sizeof(sBuff));

    // user nbyte hangul code -> standard nbyte hangul code
	for(i=0; i < nlength; i++)
	{
		if (CodeSel == NBKS_PSB_DISP)
			sBuff[i] = PsbDispCode[nBuff[i]];
		else
		if (CodeSel == NBKS_PSB_PBM)
			sBuff[i] = PsbPbmCode[nBuff[i]];
		else
			sBuff[i] = UserHanCode[nBuff[i]];
	}

	if (nlength == 0) 
	{
		rval = 0;
		return rval;
	}

	hw = 0;
 	pb = 0;
	hw = 0;
	kw = 0;
	memset(wkb, 0x00, sizeof(wkb));
	enb = 0;

	while (hw != nlength) 
	{
		jamoDelm = jamo[sBuff[hw]];
		switch (pb) 
		{
			// 초성 자음 
			case 0:
				switch (jamoDelm) 
				{
 					case 0:
						jamoFunc = 12;
						break;
					case 1:
						jamoFunc = 0;
						break;
					case 2:
						jamoFunc = 10;
						break;
					default:
						break;
				}
				break;
			// 초성의 첫번째 자음 처리 [이 앞에 최소한 1개의 만들어진 글자가 있다]
			case 1:
				switch (jamoDelm) 
				{
	  				case 0:
		 				jamoFunc = 13;
						break;
					case 1:
						jamoFunc = 1;
						break;
					case 2:
						jamoFunc = 15;
						break;
					default:
						break;
				}
				break;
			// 종성에 자음을 인식함
			// 종성에 쌍자음이 올 경우가 있으므로 처음 자음 다음의 글자가 
			// 자음인지 모음인지를 검사하는 루틴 
			case 2:
				switch (jamoDelm) 
				{
					case 0:
						jamoFunc = 9;
						break;
					case 1:
						jamoFunc = 2;
						break;
					case 2:
						jamoFunc = 11;
						break;
					default:
						break;
				}
	 			break;
			// 중성 모음 
			case 3:
				switch (jamoDelm) 
				{
					case 0:
						jamoFunc = 3;
						break;
					case 1:
						jamoFunc = 3;
						break;
					case 2:
						jamoFunc = 4;
						break;
					default:
						break;
				}
				break;
			// 종성 처음 자음 [쌍자음의]
			case 4:
				switch (jamoDelm) 
				{
					case 0:
						jamoFunc = 9;
						break;
					case 1:
						jamoFunc = 5;
						break;
					case 2:
						jamoFunc = 6;
						break;
					default:
						break;
				}
				break;

			case 5:
				switch (jamoDelm) 
				{
					case 0:
						jamoFunc = 15;
						break;
					case 1:
						jamoFunc = 7;
						break;
					case 2:
						jamoFunc = 15;
						break;
					default:
						break;
				}
				break;

			case 6:
				switch (jamoDelm) 
				{
					case 0:
						jamoFunc = 9;
						break;
					case 1:
						jamoFunc = 8;
						break;
					case 2:
						jamoFunc = 15;
						break;
					default:
						break;
			  }
			  break;

			case 7:
				switch (jamoDelm) 
				{
					case 0:
						jamoFunc = 12;
						break;
					case 1:
						jamoFunc = 14;
						break;
					case 2:
						jamoFunc = 14;
						break;
					default:
						break;
				}

			default:
				break;
		}
///////////////////////////////////////////////////////////////////////
	    switch (jamoFunc) 
		{
			// 초성 자음 처리 
			case 0:
				hanf = 0xff;
				memset(wkb, 0x00, sizeof(wkb));
				wkb[1] = sBuff[hw];
				hw++;
				pb = 3;
				break;

			// 초성 자음 [이 앞에 최소한 1개의 만들어진 글자가 있다]
			case 1:
				memset(wkb, 0x00, sizeof(wkb));
				wkb[1] = sBuff[hw];
				hw++;
				pb = 3;
				break;
			// 종성 자음 
			case 2:
				switch (jamo[sBuff[hw+1]]) 
				{
					// 종성이 쌍 자음임.
					case JA_EUM:
						pb = 5;
						break;
					// 종성 자음뒤의 쌍자음 검사시 모음이 인식됨.
					case MO_EUM:
					// 종성 자음 뒤의 문자가 자음도 모음도 아님.
					default:
						// 입력글자가 종료시 종성으로 인식시킴. 
						if (hw+1 == nlength) 
						{
							wkb[5] = sBuff[hw];
							hw++;
						}
						// 자모를 IBM 2 BYTE로 변환함.
						kw += Jamo2ibm(&tmp[kw], wkb);
						pb = 1;
						break;
				}
				break;

			case 3:
				hw--;
				hanf = 0;	
				pb = 7;
				break;
			// 중성 모음 처리 
			case 4:
				wkb[3] = sBuff[hw];
				hw++;
				if (hw == nlength) 
					enb = 1;
				pb = 4;
				break;
			// 종성 자음이 인식됨.
			// 다음 글자가 자음인지 모음인지 검사함.
			case 5:
				pb = 2;
				break;

			case 6:
				wkb[2] = sBuff[hw];
				hw++;
				if (hw == nlength) 
					enb = 1;
				pb = 2;
				break;
			// 종성 자음2개 인식시 선 자음 종성 처리 
			case 7:
				wkb[5] = sBuff[hw];
				hw++;
				if (hw == nlength) 
					enb = 1;
				pb = 6;
				break;
			// 종성 쌍자음 종료시 
			//
			case 8:
				if ((jamo[sBuff[hw+1]] != 0x02) || ((hw+1) == nlength)) 
				{
					// ㄱ 다음에 올수 있는 쌍자음 종성은 ㅅ 밖에 없다.
					// for Nacf Release
					if (wkb[5] == 0x44) 
					{
						if (sBuff[hw] == 0x68) 
						{
							wkb[4] = sBuff[hw];
							hw++;
						}
					}
					else 
					{
						wkb[4] = sBuff[hw];
						hw++;
					}
				}
				kw += Jamo2ibm(&tmp[kw], wkb);
				pb = 1;
				break;

			case 9:
				kw += Jamo2ibm(&tmp[kw], wkb);
				hanf=0;
				pb=0;
				break;

			case 10:
				tmp[kw] = sBuff[hw];
				kw++;
				hw++;
				pb = 7;
				break;

			case 11:
				kw += Jamo2ibm(&tmp[kw], wkb);
				hanf = 0;
				pb = 7;
				break;

			case 12:
				i=0;
				while (spechar[i].leng1 != 0xff) 
				{
					if (sBuff[hw] != spechar[i].dta1[0]) 
						i++;
					else 
					{
						if ((hw+spechar[i].leng1) >= nlength)
							goto hang10;
						memcpy(&tmp[kw],&sBuff[hw],spechar[i].leng1);
						hw += spechar[i].leng1;
						kw += spechar[i].leng1;

						while(sBuff[hw] != spechar[i].dta2[0]) 
						{
							if (hw >= nlength)
								goto hang10;

							tmp[kw] = sBuff[hw];
							hw++;
							kw++;
						}

						if ((hw+spechar[i].leng1) >= nlength)
							goto hang10;

						memcpy(&tmp[kw], &sBuff[hw], spechar[i].leng2);
						hw += spechar[i].leng2;
						kw += spechar[i].leng2;
						goto hang20;
					}
				}

				if (sBuff[hw] == 0x34) 
				{
					if ((hw+3) >= nlength)
						goto hang10;
					memcpy(&tmp[kw],&sBuff[hw],3);
					hw += 3;
					kw += 3;
					goto hang20;
				}

				tmp[kw] = sBuff[hw];
				kw++;
				hw++;
hang20:
				pb=0;
				break;

			case 13:
				hanf = 0;
				pb = 0;
				break;

			case 14:
				tmp[kw] = sBuff[hw];
				kw++;
				hw++;
				pb = 7;
				memset(wkb, 0x00, sizeof(wkb));
				break;

			case 15:
				goto errptr;
			default:
				break;
		}	 
	}

	if (enb != 0) 
		kw += Jamo2ibm(&tmp[kw],wkb);

	rval	= kw;
	memcpy(ibmBuff, tmp, rval);
	if (rval > nlength)
		rval = nlength;
	return rval;

hang10:
	memcpy(ibmBuff, tmp, nlength - hw);
	rval = kw + (nlength - hw);
	return rval;

errptr:
	memcpy(nBuff, ibmBuff, nlength);
	return nlength;
}

// 한 글자의 초성, 중성, 종성을 ibm 2byte로 conversion한다.
// Return value : ibm edited length
//	
int Jamo2ibm(BYTE *tmp, BYTE *wkb)
{
	BYTE	area[2];
	WORD	bw[3];
    WORD	hapw;
	int		ib,xb,rtval=0;

    memset(area,0x00,2);

    for (int i=0; i < 3; i++) 
	{
		bw[i] = (WORD)wkb[i*2] | ((WORD)wkb[i*2+1] << 8);
	}
    ib = 0;
    hapw = 0;
	while (ib != 3) 
	{
		xb = 0;
		while (bw[ib] != hanjamo[xb].aw[ib*2]) 
		{
			if (hanjamo[xb].aw[ib*2] == 0xffff)
				break;
			else xb++;
		}
        hapw = (hapw | hanjamo[xb].aw[ib*2+1]);
        ib++;
	}

	if (hapw == 0xffff) 
	{
		ib=0;
		while (ib != 3) 
		{
			WORD tw = (BYTE)(bw[ib] >> 8);
			tw |= (bw[ib] << 8) & 0xff00;
			bw[ib] = tw;
			ib++;
		}
	}
	else 
	{
        area[0] = (BYTE)((hapw >> 8) & 0x00ff);
		area[1] = (BYTE)(hapw & 0x00ff);
		memcpy(tmp, area, 2);
		rtval=2;
	}

	return rtval;
}

CString WINAPI EmSpace2NormalSpace(void* inBuff, int length)
{
	WORD	cvtstr;
	BYTE	inbuff[10240];
	BYTE	outbuff[10240];

	memset(inbuff,  0x00, sizeof(inbuff));
	memset(outbuff, 0x00, sizeof(outbuff));
	memcpy(inbuff, inBuff, __min(length, sizeof(inbuff)));
	
    for (int s = 0; s < (length/2); s++)
    {
            cvtstr = ((WORD)inbuff[s*2] << 8) | (WORD)inbuff[s*2+1];

            if (cvtstr == 0xa1a1)
                    cvtstr = 0x2020;

    outbuff[s*2] = (BYTE)((cvtstr >> 8) & 0x00ff);
            outbuff[s*2+1] = (BYTE)(cvtstr & 0x00ff);
    }

	CString strReturn("");
	strReturn.Format("%s", outbuff);

	return strReturn;
}