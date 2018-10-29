
/*
* �߳���
* @date : 2018/10/28
* @author : jihang
*/

#include "FlowThread.h"
#include "HLAPortAdapter.h"
#include "StateMachineCode.h"

extern HLAPortAdapter *ha;

//���첢��ʼ��״̬
FlowThread::FlowThread(QObject *parent) : QThread(parent) {
	state = StateMachineCode::getUnstartCode();
}

void FlowThread::run() {

	InterfaceResponse<long> *ir0;
	if (state == StateMachineCode::getUnstartCode()) {
		//������Լ�
		ir0 = ha->joinFederation();
		if (!ir0->isSuccessful()) {
			emit FlowSignal("HERE IS AN ERROR");
		}
		emit FlowSignal(ir0->getMessage().c_str());
		emit FlowSignal("Federate handle is " + QString::number(ir0->getData()));
		state = StateMachineCode::getJoinedCode();
		emit StateSignal(state);

		//������������Լ�
		ha->initial();
		state = StateMachineCode::getInitialedCode();
		emit StateSignal(state);

		//������ص�
		ha->registerSynchronization(READY_TO_RUN);
		emit FlowSignal("#Flow Thread Complete# Please start other federates...");
		state = StateMachineCode::getRegisteredCode();
		emit StateSignal(state);
	}
	else if (state == StateMachineCode::getRegisteredCode()) {
		emit FlowSignal("hehe, go on go on!");
	}
}

int FlowThread::getState() {
	return state;
}

/*	//����ͬ����
	ui.textBrowser->append("Going to synchronization point...");
	QCoreApplication::processEvents();
	ha->synchronizationAchieved(READY_TO_RUN);

	//ʱ�����
	ha->enableTimePolicy(0.01);
	ui.textBrowser->append("Time policy enabled.");
	QCoreApplication::processEvents();

	//��������
	ha->pubAndSub();
	ui.textBrowser->append("Published and Subscribed.");
	QCoreApplication::processEvents();

	ha->rigister();
	ui.textBrowser->append("Object Class Registered.");
	QCoreApplication::processEvents();
*/