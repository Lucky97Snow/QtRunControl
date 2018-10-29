#pragma once

/*
* 线程类
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
	//发送给界面的信号
	void FlowSignal(QString);
	void StateSignal(int);

private:
	//状态，根据状态机推进
	int state;

public:
	//获取当前状态
	int getState();
};