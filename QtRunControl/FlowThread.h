#pragma once

/*
* �߳���
* @date : 2018/10/28
* @author : jihang
*/

#include <QThread>

class FlowThread : public QThread {

	Q_OBJECT

public:
	explicit FlowThread(QObject *parent = 0);

protected:
	void run();

signals:
	//���͸�������ź�
	void FlowSignal(QString);
	void StateSignal(int);

private:
	//״̬������״̬���ƽ�
	int state;

public:
	//��ȡ��ǰ״̬
	int getState();
};