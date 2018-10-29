
/*
* 仿真控制GUI
* @date : 2018/10/13
* @author : jihang
*/

#include "QtRunControl.h"

#include "HLAPortAdapter.h"

//其实只是给创建联邦用的
extern HLAPortAdapter *ha;

QtRunControl::QtRunControl(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
	//绑定流程信号-槽
	flowThread = new FlowThread();
	connect(flowThread, SIGNAL(FlowSignal(QString)), this, SLOT(DisplayFlow(QString)));
	connect(flowThread, SIGNAL(StateSignal(int)), this, SLOT(DisplayState(int)));
	//TODO:状态图初始化
}

void QtRunControl::DisplayFlow(QString msg) {
	ui.textBrowser->append(msg);
}

void QtRunControl::DisplayState(int state) {
	//TODO:画状态图用
	ui.textBrowser->append("hehe, " + QString::number(state));
}

//TODO:以下都要加状态机判断
void QtRunControl::prepare() {
	//create必须是主线程执行！！！！！！！！！！
	ui.textBrowser->append("Preparing Environment, Please Wait...");
	QCoreApplication::processEvents();
	InterfaceResponse<void> *ir = ha->createFederation();
	if (!ir->isSuccessful()) {
		ui.textBrowser->append("HERE IS AN ERROR");
	}
	ui.textBrowser->append(ir->getMessage().c_str());
	flowThread->start();
}

void QtRunControl::ready() {
	flowThread->start();
}

void QtRunControl::run() {
	//TODO
}

void QtRunControl::pause() {
	//TODO:准备是在县城里设置状态为
}

void QtRunControl::continuE() {
	//TODO
}