#pragma once
#include"EventAdapter.h"
// 持久化类
// 存档文件将被保存在一个文件夹里
class Archive
{
public:
	Archive();
	void static save_config(QString);
	void static load_config(QString); 
	void static save_output(QString);
};

