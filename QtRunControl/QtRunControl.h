#pragma once

/*
* 仿真控制GUI
* @date : 2018/10/13
* @author : jihang
*/

#include <QtWidgets/QMainWindow>

#include "ui_QtRunControl.h"
#include "msgEvent.h"
#include "FlowThread.h"

class QtRunControl : public QMainWindow {

	Q_OBJECT

public:
	explicit QtRunControl(QWidget *parent = Q_NULLPTR);

	//处理适配器传回的事件信息
	virtual void customEvent(QEvent *e) {
		//if (e->type() == ControlCode::getSyncSuccessCode()) {
		//}
		MsgEvent *event0 = (MsgEvent*)e;
		ui.textBrowser->append(event0->message);
	}

public slots:
	void prepare();
	void ready();
	void run();
	void pause();
	void continuE();
	void end();

private slots:
	//处理线程传回的信号信息
	void DisplayFlow(QString);
	//处理线程传回的状态信息
	void DisplayState(int);

private:
	Ui::QtRunControlClass ui;
	FlowThread *flowThread;
};