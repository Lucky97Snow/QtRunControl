
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

	postUIMsg(7777, initOneHandle(objectPause)->getMessage().c_str());
	postUIMsg(7777, initOneHandle(objectContinue)->getMessage().c_str());
	postUIMsg(7777, initOneHandle(objectEnd)->getMessage().c_str());

	//订阅
	postUIMsg(7777, initOneHandle(objectA)->getMessage().c_str());
	postUIMsg(7777, initOneHandle(objectB)->getMessage().c_str());
}

void HLAPortAdapter::pubAndSub() {

	postUIMsg(7777, publishOne(objectPause)->getMessage().c_str());
	postUIMsg(7777, publishOne(objectContinue)->getMessage().c_str());
	postUIMsg(7777, publishOne(objectEnd)->getMessage().c_str());

	//订阅
	postUIMsg(7777, subscribeOne(objectA)->getMessage().c_str());
	postUIMsg(7777, subscribeOne(objectB)->getMessage().c_str());
}

void HLAPortAdapter::rigister() {
	InterfaceResponse<long> *ir;

	ir = registerObject(objectPause.className);
	postUIMsg(7777, ir->getMessage().c_str());
	handlePause = ir->getData();

	ir = registerObject(objectContinue.className);
	postUIMsg(7777, ir->getMessage().c_str());
	handleContinue = ir->getData();

	ir = registerObject(objectEnd.className);
	postUIMsg(7777, ir->getMessage().c_str());
	handleEnd = ir->getData();
}

void HLAPortAdapter::simulate() {
	while (true) {
		if (isEnd) {
			del();
			//TODO：不能撤销的bug，显示PAUSE里的两个句柄没del
			//ha->resignFederation();
			//所以也消毁不了
			postUIMsg(7777, ha->destroyFederation()->getMessage().c_str());
			break;
		}
		if (isPause) {
			while (isPause) {
				//暂停状态
			}
		}
		//postUIMsg(7777, advanceTime(1.0)->getMessage().c_str());
		advanceTime(1.0);
	}
}

void HLAPortAdapter::del() {
	postUIMsg(7777, deleteOne(handlePause)->getMessage().c_str());
	postUIMsg(7777, deleteOne(handleContinue)->getMessage().c_str());
	postUIMsg(7777, deleteOne(handleEnd)->getMessage().c_str());
}

void HLAPortAdapter::send(string type) {
	RTI::AttributeHandleValuePairSet *attributes = RTI::AttributeSetFactory::create(1);
	char controlInfo[16];
	if (type.c_str() == ControlCode::getPauseType()) {
		sprintf(controlInfo, "you should pause");
		attributes->add(objectPause.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		postUIMsg(7777, sendObject(handlePause, attributes)->getMessage().c_str());
		//可能会在下个步长收到，但运行管理器如果卡住，实际上收不到了
		//可以考虑的是订阅自己，这样的话肯定全网都会在下个步长收到
		isPause = true;
	}
	else if (type.c_str() == ControlCode::getContinueType()) {
		sprintf(controlInfo, "you should continue");
		attributes->add(objectContinue.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		postUIMsg(7777, sendObject(handleContinue, attributes)->getMessage().c_str());
		isPause = false;
	}
	else if (type.c_str() == ControlCode::getEndType()) {
		sprintf(controlInfo, "you should end");
		attributes->add(objectEnd.handleList[0], controlInfo, (RTI::ULong)strlen(controlInfo) + 1);
		postUIMsg(7777, sendObject(handleEnd, attributes)->getMessage().c_str());
		isEnd = true;
	}
}
//特殊回调
void HLAPortAdapter::processData(RTI::ObjectHandle theObject,
	const RTI::AttributeHandleValuePairSet& theAttributes, const char* theTag)
	throw(RTI::ObjectNotKnown, RTI::AttributeNotKnown, RTI::FederateOwnsAttributes,
		RTI::FederateInternalError) {

	MsgEvent *event0 = new MsgEvent(QEvent::Type(CallbackCode::getProcessDataCode()));
	QString s1 = "#callback# data data data.";
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

void HLAPortAdapter::syncSuccess(const char *label) {
	QString s1 = "#callback# Successfully registered sync point : ";
	QString s2 = label;
	postUIMsg(CallbackCode::getSyncSuccessCode(), s1 + s2);
}

void HLAPortAdapter::HLAPortAdapter::syncFailure(const char *label) {
	QString s1 = "#callback# Failed to register sync point : ";
	QString s2 = label;
	postUIMsg(CallbackCode::getSyncFailureCode(), s1 + s2);
}

void HLAPortAdapter::announceSync(const char *label, const char *tag) {
	QString s1 = "#callback# Synchronization point announced : ";
	QString s2 = label;
	postUIMsg(CallbackCode::getAnnounceSyncCode(), s1 + s2);
}

void HLAPortAdapter::syncComplete(const char *label) {
	QString s1 = "#callback# Federation Synchronized : ";
	QString s2 = label;
	postUIMsg(CallbackCode::getSyncCompleteCode(), s1 + s2);
}

void HLAPortAdapter::discover(RTI::ObjectHandle theObject,
	RTI::ObjectClassHandle theObjectClass,
	const char * theObjectName) {
	char str1[25], str2[25];
	_ultoa_s(theObject, str1, 10);
	_ultoa_s(theObjectClass, str2, 10);
	QString s1 = "#callback# Discoverd Object, name = ";
	QString s2 = theObjectName;
	QString s3 = " handle = ";
	QString s4 = str1;
	QString s5 = " classhandle = ";
	QString s6 = str2;
	postUIMsg(CallbackCode::getDiscoverCode(), s1 + s2 + s3 + s4 + s5 + s6);
}

void HLAPortAdapter::remove(RTI::ObjectHandle theObject, const char *theTag) {
	char str1[25];
	_ultoa_s(theObject, str1, 10);
	QString s1 = "#callback# Object Removed, handle = ";
	QString s2 = str1;
	postUIMsg(CallbackCode::getRemoveCode(), s1 + s2);
}

void HLAPortAdapter::postUIMsg(int type, QString msg) {
	MsgEvent *event0 = new MsgEvent(QEvent::Type(type));
	event0->message = msg;
	QCoreApplication::postEvent(qrc, event0);
}

//主函数在这！！！
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	HLAPortAdapter *ha = new HLAPortAdapter();
	qrc->show();
	return a.exec();
}