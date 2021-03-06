
/*
* 线程类
* @date : 2018/10/28
* @author : jihang
*/

#include "FlowThread.h"
#include "HLAPortAdapter.h"
#include "StateMachineCode.h"

extern HLAPortAdapter *ha;

//构造并初始化状态
FlowThread::FlowThread(QObject *parent) : QThread(parent) {
	state = StateMachineCode::getUnstartCode();
}

void FlowThread::run() {
	InterfaceResponse<long> *ir0;
	InterfaceResponse<bool> *ir1;
	if (state == StateMachineCode::getUnstartCode()) {
		//输出靠自己
		ir0 = ha->joinFederation();
		if (!ir0->isSuccessful()) {
			emit FlowSignal("HERE IS AN ERROR");
		}
		emit FlowSignal(ir0->getMessage().c_str());
		emit FlowSignal("Federate handle is " + QString::number(ir0->getData()));
		state++;
		emit StateSignal(state);

		//输出靠适配器自己
		ha->initial();
		state++;
		emit StateSignal(state);

		//输出靠回调
		ha->registerSynchronization(READY_TO_RUN);
		emit FlowSignal("#Flow Thread Complete# Please start other federates...");
		state++;
		emit StateSignal(state);
	}
	else if (state == StateMachineCode::getRegisPointCode()) {
		//输出靠回调
		ha->synchronizationAchieved(READY_TO_RUN);
		state++;
		emit StateSignal(state);

		//输出靠自己
		ir1 = ha->enableTimePolicy(0.01);
		emit FlowSignal(ir1->getMessage().c_str());
		state++;
		emit StateSignal(state);

		//输出靠适配器自己
		ha->pubAndSub();
		state++;
		emit StateSignal(state);

		//输出靠适配器自己
		ha->rigister();
		state++;
		emit StateSignal(state);
		emit FlowSignal("#Flow Thread Complete# Please start simulation...");
	}
	else if (state == StateMachineCode::getRegisteredCode()) {
		state++;
		emit StateSignal(state);
		ha->simulate();
		state++;
		emit StateSignal(state);
		emit FlowSignal("#Flow Thread Complete# Simulation Ends.");
	}
}

int FlowThread::getState() {
	return state;
}