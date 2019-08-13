
// lorawan-note-datagenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "lorawan-note-datagen.h"
#include "lorawan-note-datagenDlg.h"
#include "afxdialogex.h"
extern "C"{
#include "secure-element.h"
#include "LoRaMacCrypto.h"
};

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ClorawannotedatagenDlg 对话框



ClorawannotedatagenDlg::ClorawannotedatagenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ClorawannotedatagenDlg::IDD, pParent)
	, m_activeMode(_T(""))
	, m_nwkskey(_T(""))
	, m_appskey(_T(""))
	, m_devaddr(_T(""))
	, m_fcntup(_T(""))
	, m_msgtype(_T(""))
	, m_adr(FALSE)
	, m_adrackreq(FALSE)
	, m_ack(FALSE)
	, m_foptslen(_T(""))
	, m_opts(_T(""))
	, m_fport(_T(""))
	, m_frmplayload(_T(""))
	, m_phyplayload(_T(""))
	, m_phy_bytes(_T(""))
	, m_hexshow(FALSE)
	, m_isDownLinx(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void ClorawannotedatagenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_activeMode, m_activeMode);
	DDX_Text(pDX, IDC_nwkskey, m_nwkskey);
	DDX_Text(pDX, IDC_appskey, m_appskey);
	DDX_Text(pDX, IDC_devaddr, m_devaddr);
	DDX_Text(pDX, IDC_fcntup, m_fcntup);
	DDX_CBString(pDX, IDC_msgtype, m_msgtype);
	DDX_Check(pDX, IDC_adr, m_adr);
	DDX_Check(pDX, IDC_adrackreq, m_adrackreq);
	DDX_Check(pDX, IDC_ack, m_ack);
	DDX_CBString(pDX, IDC_Foptslen, m_foptslen);
	DDX_Text(pDX, IDC_Fopts, m_opts);
	DDX_Text(pDX, IDC_FPort, m_fport);
	DDX_Text(pDX, IDC_FRMPlayload, m_frmplayload);
	DDX_Text(pDX, IDC_PHYPlayload, m_phyplayload);
	DDX_Control(pDX, IDC_bytes, m_phyplayload_bytes);
	DDX_Control(pDX, IDC_activeMode, m_activeModeCtrl);
	DDX_Control(pDX, IDC_msgtype, m_msgtypeCtrl);
	DDX_Control(pDX, IDC_Foptslen, m_optslenCtrl);
	DDX_Text(pDX, IDC_bytes, m_phy_bytes);
	DDX_Check(pDX, IDC_CHECK1, m_hexshow);
	DDX_Control(pDX, IDC_STATIC_FCnt, m_fcnt_ctrl);
	DDX_Check(pDX, IDC_CHECK2, m_isDownLinx);
}

BEGIN_MESSAGE_MAP(ClorawannotedatagenDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GEN, &ClorawannotedatagenDlg::OnBnClickedGen)
	ON_BN_CLICKED(IDC_CHECK2, &ClorawannotedatagenDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// ClorawannotedatagenDlg 消息处理程序

BOOL ClorawannotedatagenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
	m_activeModeCtrl.AddString(_T("ABP")); //ABP入网模式
	m_activeModeCtrl.SetCurSel(0);

	m_msgtypeCtrl.AddString(_T("Join Request"));   //
	m_msgtypeCtrl.AddString(_T("Join Accept"));   //
	m_msgtypeCtrl.AddString(_T("Unconfirmed Data Up"));   //
	m_msgtypeCtrl.AddString(_T("Unconfirmed Data Down"));   //
	m_msgtypeCtrl.AddString(_T("Confirmed Data Up"));   //
	m_msgtypeCtrl.AddString(_T("Confirmed Data Down"));   //
	m_msgtypeCtrl.AddString(_T("RFU"));   //
	m_msgtypeCtrl.AddString(_T("Proprietary"));   //
	m_msgtypeCtrl.SetCurSel(2);

	int n = 0;
	CString str;
	for (n = 0; n < 16; n++) {
		str.Format(_T("%d"), n);
		m_optslenCtrl.AddString(str);
	}
	m_optslenCtrl.SetCurSel(0);

	UpdateData(TRUE);
	m_nwkskey = "785f2467e8fbec2bdcb35fe8ae06701a";
	m_appskey = "4dee9e9a1a984641944fffe2b83d164c";
	m_devaddr = "00d96511";
	m_fcntup  = "0";
	m_fport   = "1";
	UpdateData(FALSE);

	// Initialize the Secure Element driver
    if( SecureElementInit( NULL ) != SECURE_ELEMENT_SUCCESS )
    {
        TRACE("SecureElementInit failed\n");
    }

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void ClorawannotedatagenDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR ClorawannotedatagenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void ClorawannotedatagenDlg::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
static int strToHexBuf(CString str, unsigned char *hexbuf)
 {
	 unsigned char Ret;
	 unsigned char i,ch,Tmp,n =0, tt  = 0;
 
	 str = str.Trim();
	 Ret = 0;
	 for(i=0;i!=str.GetLength();++i)
	 {
		 ch = str.GetAt(i);
		 if (ch == ' ') {
			 tt = 0;
			 continue;
		 }

		 if((ch >= '0') && (ch <='9')){
			 Tmp = ch-'0';
		 }else if((ch >= 'A') && (ch <='F')){
			 Tmp = (ch-'A')+10;
		 }else if((ch >= 'a') && (ch <='f')){
			 Tmp = (ch-'a')+10;
		 }else{
			 Tmp = 0;
		 }
		 if ((tt%2) == 0) {
			Ret = Tmp;
		 } else {
			*hexbuf = Ret*16+Tmp;
			hexbuf++;
			n++;
		 }
		 tt++;
	 }
 
	 return n;
 }

static int msgType2int(CString str)
{
	if (str == "Join Request") {
		return 0;
	} else if (str == "Join Accept") {
		return 1;
	} else if (str == "Unconfirmed Data Up") {
		return 2;
	} else if (str == "Unconfirmed Data Down") {
		return 3;
	} else if (str == "Confirmed Data Up") {
		return 4;
	} else if (str == "Confirmed Data Down") {
		return 5;
	} else if (str == "RFU") {
		return 6;
	} else if (str == "Proprietary") {
		return 7;
	}
	return 0;
}

/*
 * Encrypts the payload
 *
 * \param[IN]  keyID            - Key identifier
 * \param[IN]  address          - Address
 * \param[IN]  dir              - Frame direction ( Uplink or Downlink )
 * \param[IN]  frameCounter     - Frame counter
 * \param[IN]  size             - Size of data
 * \param[IN/OUT]  buffer       - Data buffer
 * \retval                      - Status of the operation
 */
static int PayloadEncrypt( uint8_t* buffer, int16_t size, KeyIdentifier_t keyID, uint32_t address, uint8_t dir, uint32_t frameCounter )
{
    if( buffer == 0 )
    {
        return -1;
    }

    uint8_t bufferIndex = 0;
    uint16_t ctr = 1;
    uint8_t sBlock[16] = { 0 };
    uint8_t aBlock[16] = { 0 };

    aBlock[0] = 0x01;

    aBlock[5] = dir;

    aBlock[6] = address & 0xFF;
    aBlock[7] = ( address >> 8 ) & 0xFF;
    aBlock[8] = ( address >> 16 ) & 0xFF;
    aBlock[9] = ( address >> 24 ) & 0xFF;

    aBlock[10] = frameCounter & 0xFF;
    aBlock[11] = ( frameCounter >> 8 ) & 0xFF;
    aBlock[12] = ( frameCounter >> 16 ) & 0xFF;
    aBlock[13] = ( frameCounter >> 24 ) & 0xFF;

    while( size > 0 )
    {
        aBlock[15] = ctr & 0xFF;
        ctr++;
        if( SecureElementAesEncrypt( aBlock, 16, keyID, sBlock ) != SECURE_ELEMENT_SUCCESS )
        {
            return -2;
        }

        for( uint8_t i = 0; i < ( ( size > 16 ) ? 16 : size ); i++ )
        {
            buffer[bufferIndex + i] = buffer[bufferIndex + i] ^ sBlock[i];
        }
        size -= 16;
        bufferIndex += 16;
    }

    return 0;
}

#define UP_LINK   0
#define DOWN_LINK 1
void ClorawannotedatagenDlg::OnBnClickedGen()
{
	
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	memset(nwkskey, 0, sizeof(nwkskey));
	memset(appskey, 0, sizeof(appskey));

	int len = strToHexBuf(m_nwkskey, nwkskey);
	ASSERT(len == 16);
	len = strToHexBuf(m_appskey, appskey);
	ASSERT(len == 16);
	TRACE("\n==================<GEN Data>==================\n");
	TRACE("nwkskey= %S\n",m_nwkskey);
	TRACE("appskey= %S\n",m_appskey);

	len = strToHexBuf(m_opts, phyplayload.macplayload.fhdr.fopts);
	ASSERT(len <= 15);

	//mhdr
	int msgtype = msgType2int(m_msgtype);
	phyplayload.mType = msgtype;
	phyplayload.RFU   = 0;
	phyplayload.Major = 0;
	TRACE("msgtype=(%d) %S\n",msgtype, m_msgtype);
	//
	strToHexBuf(m_devaddr, (unsigned char *)phyplayload.macplayload.fhdr.devaddr);
	UINT8 temp = phyplayload.macplayload.fhdr.devaddr[3];
	phyplayload.macplayload.fhdr.devaddr[3] = phyplayload.macplayload.fhdr.devaddr[0];
	phyplayload.macplayload.fhdr.devaddr[0] = temp;
	temp = phyplayload.macplayload.fhdr.devaddr[1];
	phyplayload.macplayload.fhdr.devaddr[1] = phyplayload.macplayload.fhdr.devaddr[2];
	phyplayload.macplayload.fhdr.devaddr[2] = temp;

	TRACE("devaddr= %02X%02X%02X%02X\n",phyplayload.macplayload.fhdr.devaddr[0],phyplayload.macplayload.fhdr.devaddr[1],phyplayload.macplayload.fhdr.devaddr[2],phyplayload.macplayload.fhdr.devaddr[3]);
	TRACE("address=%08X\n", phyplayload.macplayload.fhdr.address);
	phyplayload.macplayload.fhdr.fcnt       = _ttoi(m_fcntup);
	phyplayload.macplayload.fhdr.adr        = m_adr?1:0;
	phyplayload.macplayload.fhdr.adrackreq  = m_adrackreq?1:0;
	phyplayload.macplayload.fhdr.ack        = m_ack?1:0;
	phyplayload.macplayload.fhdr.fpending   = 0;
	phyplayload.macplayload.fhdr.foptslen   = _ttoi(m_foptslen);

	phyplayload.macplayload.fport = _ttoi(m_fport);
    TRACE("fcnt=%d, adr=%d, adrackreq=%d,ack=%d, fpending=%d, foptslen=%d \n",phyplayload.macplayload.fhdr.fcnt,
		phyplayload.macplayload.fhdr.adr,
		phyplayload.macplayload.fhdr.adrackreq,
		phyplayload.macplayload.fhdr.ack,
		phyplayload.macplayload.fhdr.fpending,
		phyplayload.macplayload.fhdr.foptslen);

	int playloadlen = strToHexBuf(m_frmplayload, phyplayload.macplayload.playload );
	TRACE("playload(%d):\n", playloadlen);
	for (int m = 0; m < playloadlen; m++) {
		TRACE("%02X ", phyplayload.macplayload.playload[m]);
		if ((m+1)%8 == 0) {
			TRACE("\n");
		} 
	}
	//m_nwkskey = "F2CF38D4026CC547B5D85F0D2CCD1AF3";
	//m_appskey = "B993D76F47B16EBB9946B4104C3D1A27";

	//设定密钥
	if(SECURE_ELEMENT_SUCCESS != SecureElementSetKey(APP_S_KEY, appskey)) {
		TRACE("APP_S_KEY set failed\n");
	}
	if(SECURE_ELEMENT_SUCCESS != SecureElementSetKey(F_NWK_S_INT_KEY, nwkskey)) {
		TRACE("F_NWK_S_INT_KEY set failed\n");
	}
	if(SECURE_ELEMENT_SUCCESS != SecureElementSetKey(S_NWK_S_INT_KEY, nwkskey)) {
		TRACE("S_NWK_S_INT_KEY set failed\n");
	}
	if(SECURE_ELEMENT_SUCCESS != SecureElementSetKey(NWK_S_ENC_KEY, nwkskey)) {
		TRACE("NWK_S_ENC_KEY set failed\n");
	}

	PayloadEncrypt(phyplayload.macplayload.playload, 
		playloadlen, 
		APP_S_KEY,  
		phyplayload.macplayload.fhdr.address,
		m_isDownLinx?DOWN_LINK:UP_LINK,
		phyplayload.macplayload.fhdr.fcnt);

	CString strenplayload;
	for (int n = 0; n <playloadlen; n++ ) {
		strenplayload.AppendFormat(_T("%02X "), phyplayload.macplayload.playload[n]);
	}
	m_phyplayload = strenplayload;
	UpdateData(FALSE);

	TRACE("enplayload(%d):\n", playloadlen);
	for (int m = 0; m < playloadlen; m++) {
		TRACE("%02X ", phyplayload.macplayload.playload[m]);
		if ((m+1)%8 == 0) {
			TRACE("\n");
		} 
	}
	
	//ComputeCmacB0(phyplayload.macplayload.playload, playloadlen, F_NWK_S_INT_KEY, phyplayload.macplayload.fhdr.ack, UP_LINK,phyplayload.macplayload.fhdr.address, phyplayload.macplayload.fhdr.fcnt , &phyplayload.cmac);
	//TRACE("MIC=%08X\n", phyplayload.cmac);

	TRACE("****************************\n");
	TRACE("DIR=%s\n",    m_isDownLinx?"DOWN_LINK":"UP_LINK");
	TRACE("MHDR=%02X\n",    phyplayload.mhdr);
	TRACE("DevAddr=%04X\n", phyplayload.macplayload.fhdr.address);
	TRACE("Fctrl=%02X\n", phyplayload.macplayload.fhdr.fctrl);
	TRACE("Fcnt=%d\n", phyplayload.macplayload.fhdr.fcnt);
	if (phyplayload.macplayload.fhdr.foptslen > 0) {
		TRACE("Fopts(%d): ");
		for (int t = 0; t < phyplayload.macplayload.fhdr.foptslen; t++) {
			TRACE("%02X ", phyplayload.macplayload.fhdr.fopts[t]);
		}
		TRACE("\n");
	}
	TRACE("FPort=%d\n", phyplayload.macplayload.fport);
	if (playloadlen > 0) {
		TRACE("enplayload(%d): ", playloadlen);
		for (int m = 0; m < playloadlen; m++) {
			TRACE("%02X ", phyplayload.macplayload.playload[m]);
			//if ((m+1)%8 == 0) {
			//	TRACE("\n");
			//} 
		}
		TRACE("\n");
	}
	//TRACE("MIC=%08X\n", phyplayload.cmac);


	unsigned char *pphydata = (unsigned char *)malloc(1+4+1+2+1+phyplayload.macplayload.fhdr.foptslen+playloadlen+4 + 4);
	ASSERT(pphydata != NULL);
	int n = 0;
	pphydata[n++] = phyplayload.mhdr;
	memcpy(&pphydata[n], &phyplayload.macplayload.fhdr.address, 4);
	n += 4;
	pphydata[n++] = phyplayload.macplayload.fhdr.fctrl;
	memcpy(&pphydata[n], &phyplayload.macplayload.fhdr.fcnt, 2);
	n += 2;

	if (phyplayload.macplayload.fhdr.foptslen > 0) {
		memcpy(&pphydata[n], phyplayload.macplayload.fhdr.fopts, phyplayload.macplayload.fhdr.foptslen);
		n += phyplayload.macplayload.fhdr.foptslen;
		
	}
	pphydata[n++] = phyplayload.macplayload.fport;
	if (playloadlen > 0) {
		memcpy(&pphydata[n],  phyplayload.macplayload.playload, playloadlen);
		n += playloadlen;
	}

	ComputeCmacB0(pphydata, 
		n, 
		F_NWK_S_INT_KEY, 
		phyplayload.macplayload.fhdr.ack,
		m_isDownLinx?DOWN_LINK:UP_LINK,
		phyplayload.macplayload.fhdr.address,
		phyplayload.macplayload.fhdr.fcnt ,
		&phyplayload.cmac);
	TRACE("MIC=%08X\n", phyplayload.cmac);


	memcpy(&pphydata[n], & phyplayload.cmac, 4);
	n += 4;
	TRACE("\n-------------------------\n");
	TRACE("phyplayload(%d):\n", n);
	strenplayload = "";
	for (int m = 0; m < n; m++) {
		TRACE("%02X ", pphydata[m]);
		if (m_hexshow) {
			strenplayload.AppendFormat(_T("0x%02X, "), pphydata[m]);
		} else {
			strenplayload.AppendFormat(_T("%02X "), pphydata[m]);
		}
		if ((m+1)%8 == 0) {
			TRACE("\n");
		} 
	}
	m_phyplayload = strenplayload;
	strenplayload = "";
	strenplayload.Format(_T("%d"), n);
	m_phy_bytes   = strenplayload;
	UpdateData(FALSE);

	free(pphydata);
}


void ClorawannotedatagenDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	
}


void ClorawannotedatagenDlg::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_isDownLinx) {
		TRACE("选择下行数据计算\n");
		m_fcnt_ctrl.SetWindowTextW(_T("下行链路计数(FCntDown)"));
	} else {
		TRACE("选择上行数据计算\n");
		m_fcnt_ctrl.SetWindowTextW(_T("上行链路计数(FCntUp)"));
	}
}
