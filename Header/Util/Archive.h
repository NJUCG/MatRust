#pragma once
#include"EventAdapter.h"
// �־û���
// �浵�ļ�����������һ���ļ�����
class Archive
{
public:
	Archive();
	void static save_config(QString);
	void static load_config(QString); 
	void static save_output(QString);
};

