#pragma once
#include<unordered_map>
#include"string_defs.h"
#include<string>
#include<qfile.h>
#include<qjsondocument.h>
#include<qjsonarray.h>
#include<qjsonobject.h>
#include<qjsonvalue.h>
using namespace std;
class StringHelper {
public:
	static StringHelper* shared;
	static void init_string_helper() {
		shared = new StringHelper();
	}
	static QString fetch(QString idx) {
		if (shared && shared->strings.find(idx) != shared->strings.end()) {
			return shared->strings[idx];
		}
		return "";
	}
protected:
	StringHelper() {
		QFile strings_file(QStringLiteral("./strings.json"));
		if (!strings_file.open(QIODevice::ReadOnly)) {
			qWarning() << "string data lost.";
			return;
		}

		QByteArray strings_data = strings_file.readAll();
		QString local_str = QString::fromLocal8Bit(strings_data.data());

		QJsonDocument strings_doc(QJsonDocument::fromJson(local_str.toStdString().data()));

		QJsonObject obj = strings_doc.object();

		QStringList l = obj.keys();

		// QTextCodec* code = QTextCodec::codecForName("gb18030");

		for (QString key : obj.keys())
		{
			QJsonValue v = obj.take(key);
			QString str = QString::fromLocal8Bit(v.toString().toLocal8Bit().data());
			/*
			string std_str;
			if (code) {
				std_str = code->fromUnicode(str).data();
			}
			*/
			strings[key] = str;
		}
		
	}
	unordered_map<QString, QString> strings;
};