#pragma once

/*
* �����������Eventͨ�ţ��̳г��������Զ�����Ϣ����
* δ������ʹ��json���������ݾ��ɴ���
* @date : 2018/10/22
* @author : jihang
*/

#include <QEvent>

class MsgEvent : public QEvent {
public:
	MsgEvent(QEvent::Type type) : QEvent(type) {}
	QString message;
};