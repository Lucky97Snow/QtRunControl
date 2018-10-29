
/*
* RunControlAdapter层源文件，完成以下功能：
* 1. 有一个界面类和适配器类的对象，界面类对象主要用来让适配器发event使用
* 2. 界面类对象的存在限制了main函数必须在这个类里
* 3. 适配器自己的对象，是给其他类extern用的，保证适配器类全局唯一
* 4. 适配器的构造函数和回调方法
* 5. 运行流程为主函数实例化适配器，适配器实例化界面类，获取自身对象句柄，主函数显示界面类
* 6. 界面类和适配器类的通信方式为post event，因为信号槽机制要求发送SIGNAL方必须为QObject
* @date : 2018/10/22
* @author : jihang
*/

#include <QtWidgets/QApplication>

#include "HLAPortAdapter.h"
#include "QtRunControl.h"

#include "MsgEvent.h"
#include "ControlCode.h"

#include "CallbackCode.h"

//界面类对象
QtRunControl *qrc;
//适配器对象
HLAPortAdapter *ha;

HLAPortAdapter::HLAPortAdapter() {
	//初始化界面类
	qrc = new QtRunControl();
	//初始化适配器
	ha = this;

	//成员属性
	m_fedFile = "testfom.fed";
	m_federationName = "ExampleFederation";
	m_federateName = "runControl";
	isPause = false;
	isEnd = false;

	//仿真控制
	objectPause.className = "ObjectRoot.PAUSE";
	objectPause.attributeList.push_back("pause");

	objectContinue.className = "ObjectRoot.CONTINUE";
	objectContinue.attributeList.push_back("continue");

	objectEnd.className = "ObjectRoot.End";
	objectEnd.attributeList.push_back("end");

	//订阅
	objectA.className = "ObjectRoot.A";
	objectA.attributeList.push_back("aa");
	objectA.attributeList.push_back("ab");
	objectA.attributeList.push_back("ac");

	objectB.className = "ObjectRoot.B";
	objectB.attributeList.push_back("ba");
	objectB.attributeList.push_back("bb");
	objectB.attributeList.push_back("bc");
}

//逻辑处理函数
void HLAPortAdapter::initial() {
	
	MsgEvent *event0 = new MsgEvent(QEvent::Type(7777));
	event0->message = initOneHandle(objectPause)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event0);

	MsgEvent *event1 = new MsgEvent(QEvent::Type(7777));
	event1->message = initOneHandle(objectContinue)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event1);

	MsgEvent *event2 = new MsgEvent(QEvent::Type(7777));
	event2->message = initOneHandle(objectEnd)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event2);

	//订阅
	MsgEvent *event3 = new MsgEvent(QEvent::Type(7777));
	event3->message = initOneHandle(objectA)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event3);

	MsgEvent *event4 = new MsgEvent(QEvent::Type(7777));
	event4->message = initOneHandle(objectB)->getMessage().c_str();
	QCoreApplication::postEvent(qrc, event4);
}

void HLAPortAdapter::pubAndSub() {
	//仿真控制
	publishOne(objectPause);
	publishOne(objectContinue);
	publishOne(objectEnd);
	//订阅
	subscribeOne(objectA);
	subscribeOne(objectB);
}

void HLAPortAdapter::rigister() {
	//注册控制类
	handlePause = registerObject(objectPause.className)->getData();
	handleContinue = registerObject(objectContinue.className)->getData();
	handleEnd = registerObject(objectEnd.className)->getData();
}

void HLAPortAdapter::simulate() {
	while (true) {
		if (isEnd) {
			del();
			resignFederation();
			//由运行管理器销毁
			destroyFederation();
			break;
		}
		if (isPause) {
			while (isPause) {
				//暂停状态
			}
		}
		advanceTime(1.0);
	}
}

void HLAPortAdapter::del() {
	deleteOne(handlePause);
	deleteOne(handleContinue);
	deleteOne(handleEnd);
}

void HLAPortAdapter::send(string type) {
	/*
	//发送控制信息
	RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create(1);
	char controlInfo[16];
	if (type == ControlType::getPauseType()) {
		sprintf(controlInfo, "you should pause");
		attributes->add(objectPause.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		sendObject(handlePause, attributes);
		//可能会在下个步长收到，但运行管理器如果卡住，实际上收不到了
		//可以考虑的是订阅自己，这样的话肯定全网都会在下个步长收到
		isPause = true;
	}
	else if (type == ControlType::getContinueType()) {
		sprintf(controlInfo, "you should continue");
		attributes->add(objectContinue.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		sendObject(handleContinue, attributes);
		isPause = false;
	}
	else if (type == ControlType::getEndType()) {
		sprintf(controlInfo, "you should end");
		attributes->add(objectEnd.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		sendObject(handleEnd, attributes);
		isEnd = true;
	}
	*/
}
//特殊回调
void HLAPortAdapter::processData(RTI::ObjectHandle theObject,
	const RTI::AttributeHandleValuePairSet& theAttributes, const char* theTag)
	throw(RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes,
		RTI::FederateInternalError) {

	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getProcessDataCode()));
	QString s1 = "data data.";
	event0->message = s1;
	QCoreApplication::postEvent(qrc, event0);

	//预设的，收到仿真控制信息
	//先由对象句柄获取对象类句柄
	//int classHandle = getObjectClass(theObject);
	//根据对象类句柄判断
	//if (classHandle == objectHandlesPause->classHandle) {
	//	cout << "i should pause" << endl;
	//	isPause = true;
	//}
	//if (classHandle == objectHandlesContinue->classHandle) {
	//	cout << "i should continue" << endl;
	//	isPause = false;
	//}
	//if (classHandle == objectHandlesEnd->classHandle) {
	//	cout << "i should end" << endl;
	//	isEnd = true;
	//}
	//自己的，用户添加，收到数据信息
}

//处理回调函数
void HLAPortAdapter::syncSuccess(const char *label) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getSyncSuccessCode()));
	QString s1 = "Successfully registered sync point : ";
	QString s2 = label;
	event0->message = s1 + s2;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::HLAPortAdapter::syncFailure(const char *label) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getSyncFailureCode()));
	QString s1 = "Failed to register sync point : ";
	QString s2 = label;
	event0->message = s1 + s2;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::announceSync(const char *label, const char *tag) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getAnnounceSyncCode()));
	QString s1 = "Synchronization point announced : ";
	QString s2 = label;
	event0->message = s1 + s2;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::syncComplete(const char *label) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getSyncCompleteCode()));
	QString s1 = "Federation Synchronized : ";
	QString s2 = label;
	event0->message = s1 + s2;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::discover(RTI::ObjectHandle theObject,
	RTI::ObjectClassHandle theObjectClass,
	const char * theObjectName) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getDiscoverCode()));
	QString s1 = "Discoverd Object.";
	event0->message = s1;
	QCoreApplication::postEvent(qrc, event0);
}

void HLAPortAdapter::remove(RTI::ObjectHandle theObject, const char *theTag) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getRemoveCode()));
	QString s1 = "Object Removed.";
	event0->message = s1;
	QCoreApplication::postEvent(qrc, event0);
}

//主函数在这！！！
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	HLAPortAdapter *ha = new HLAPortAdapter();
	qrc->show();
	return a.exec();
}