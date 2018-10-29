#pragma once

/*
* 适配器与界面Event通信，继承出来可以自定义信息内容
* 未来考虑使用json，所有数据均可处理
* @date : 2018/10/22
* @author : jihang
*/

#include <QEvent>

class MsgEvent : public QEvent {
public:
	MsgEvent(QEvent::Type type) : QEvent(type) {}
	QString message;
};