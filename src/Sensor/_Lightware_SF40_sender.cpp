/*
 *  Created on: Nov 4, 2016
 *      Author: yankai
 */
#include "_Lightware_SF40_sender.h"

namespace kai
{
_Lightware_SF40_sender::_Lightware_SF40_sender()
{
	m_pSerialPort = NULL;
	m_dAngle = 1;
}

_Lightware_SF40_sender::~_Lightware_SF40_sender()
{
	MBS(0);
}

bool _Lightware_SF40_sender::init(void* pKiss)
{
	IF_F(!this->_ThreadBase::init(pKiss));

	return true;
}

bool _Lightware_SF40_sender::start(void)
{
	m_bThreadON = true;
	int retCode = pthread_create(&m_threadID, 0, getUpdateThread, this);
	if (retCode != 0)
	{
		LOG(ERROR)<< retCode;
		m_bThreadON = false;
		return false;
	}

	return true;
}

bool _Lightware_SF40_sender::link(void)
{
	return true;
}

void _Lightware_SF40_sender::update(void)
{
	while (m_bThreadON)
	{
		if (m_pSerialPort==NULL)continue;
		if (!m_pSerialPort->isOpen())continue;

		//Regular update loop
		this->autoFPSfrom();
		LD();
		this->autoFPSto();
	}

}

void _Lightware_SF40_sender::MBS(uint8_t MBS)
{
	NULL_(m_pSerialPort);
	IF_(!m_pSerialPort->isOpen());

	if(MBS>3)MBS = 3;

	char str[32];
	sprintf(str, "#MBS,%d\x0d\x0a", MBS);
	m_pSerialPort->write((uint8_t*)str, strlen(str));
}


void _Lightware_SF40_sender::LD(void)
{
	NULL_(m_pSerialPort);
	IF_(!m_pSerialPort->isOpen());

	//?LD,aaa.a<CR><LF> <space>dd.dd<CR><LF>

	char str[128];

	for(double angle=0; angle<DEG_AROUND; angle+=m_dAngle)
	{
		sprintf(str, "?LD,%.1f\x0d\x0a", angle);
		m_pSerialPort->write((uint8_t*)str, strlen(str));
	}

}

void _Lightware_SF40_sender::Map(void)
{
	string TM = "?TM,360,0\x0d\x0a";

	m_pSerialPort->write((uint8_t*)TM.c_str(), TM.length());
}

bool _Lightware_SF40_sender::draw(void)
{
	return true;
}


}
