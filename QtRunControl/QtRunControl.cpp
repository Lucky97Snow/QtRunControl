
/*
* �������GUI
* @date : 2018/10/13
* @author : jihang
*/

#include "QtRunControl.h"

#include "HLAPortAdapter.h"

//��ʵֻ�Ǹ����������õ�
extern HLAPortAdapter *ha;

QtRunControl::QtRunControl(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);
	//�������ź�-��
	flowThread = new FlowThread();
	connect(flowThread, SIGNAL(FlowSignal(QString)), this, SLOT(DisplayFlow(QString)));
	connect(flowThread, SIGNAL(StateSignal(int)), this, SLOT(DisplayState(int)));
	//TODO:״̬ͼ��ʼ��
}

void QtRunControl::DisplayFlow(QString msg) {
	ui.textBrowser->append(msg);
}

void QtRunControl::DisplayState(int state) {
	//TODO:��״̬ͼ��
	ui.textBrowser->append("hehe, " + QString::number(state));
}

//TODO:���¶�Ҫ��״̬���ж�
void QtRunControl::prepare() {
	//create���������߳�ִ�У�������������������
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
	//TODO:׼�������س�������״̬Ϊ
}

void QtRunControl::continuE() {
	//TODO
}