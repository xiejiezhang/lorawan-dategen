
// lorawan-note-datagenDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#pragma pack(push,1)
struct lorawan_fhdr{
	union {
	unsigned char devaddr[4];
	unsigned int  address;
	};
	
	union {
		unsigned char fctrl;
		struct {
			unsigned int foptslen:4;
			unsigned int fpending:1;
			unsigned int ack:1;
			unsigned int adrackreq:1;
			unsigned int adr:1;
		};
	};
	union {
		unsigned char fcntb[2];
		unsigned short fcnt;
	};
	unsigned char fopts[15];
};
#pragma pack(pop)

#pragma pack(push,1)
struct lorawan_macplayload{
	struct lorawan_fhdr fhdr;
	unsigned char fport;
	unsigned char playload[256];

};
#pragma pack(pop)

#pragma pack(push,1)
struct lorawan_phyplayload{
	union {
		unsigned char mhdr;
		struct {
			unsigned int Major:2;
			unsigned int RFU:3;
			unsigned int mType:3;
		};
	};
	struct lorawan_macplayload macplayload;

	union {
		unsigned char mic[4];
		unsigned int  cmac;
	};
};
#pragma pack(pop)

// ClorawannotedatagenDlg 对话框
class ClorawannotedatagenDlg : public CDialogEx
{
// 构造
public:
	ClorawannotedatagenDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_LORAWANNOTEDATAGEN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit4();
	CString m_activeMode;
	CString m_nwkskey;
	CString m_appskey;
	CString m_devaddr;
	CString m_fcntup;
	CString m_msgtype;
	BOOL m_adr;
	BOOL m_adrackreq;
	BOOL m_ack;
	CString m_foptslen;
	CString m_opts;
	CString m_fport;
	CString m_frmplayload;
	CString m_phyplayload;
	CEdit m_phyplayload_bytes;
	afx_msg void OnBnClickedGen();
	CComboBox m_activeModeCtrl;
	CComboBox m_msgtypeCtrl;

	unsigned char nwkskey[16];
	unsigned char appskey[16];
	CComboBox m_optslenCtrl;

	struct lorawan_phyplayload phyplayload;
	CString m_phy_bytes;
	BOOL m_hexshow;
	afx_msg void OnBnClickedRadio1();
	CStatic m_fcnt_ctrl;
	BOOL m_isDownLinx;
	afx_msg void OnBnClickedCheck2();
};
