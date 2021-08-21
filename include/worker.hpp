// #pragma once
// #include <include/includeQt>
// #include <include/threadMgr.hpp>

// namespace WorkingModes {
// const QUuid exec("e84a72a9-6a9b-4abc-9af1-e1e2526da426");
// }  // namespace WorkingModes

// namespace Events {

// namespace Type {
// const QUuid active("2a411281-457b-4c12-aa2e-d6be5e519c8f");
// const QUuid clean("f34380b8-9994-4e3e-a30f-24077771a088");
// const QUuid done("9305abd8-b211-4240-8046-bff8217034b1");
// const QUuid stop("87020444-5ebb-406f-bca4-2da66cf93e50");
// const QUuid error("7b536e80-ca57-46cd-b71f-5b7c6769fb40");

// // const QUuid debug("e9385aeb-7035-4e43-818c-60b54a20d800");

// // const QUuid toRun("0dac8a46-16ee-4756-a491-768d5e064d2e");
// const QUuid toStop("71812c9e-bd0b-46d9-ad18-87a581d04584");
// }  // namespace Type

// namespace Attr {
// const QUuid toPublic("1f658a82-0b9f-4e52-9e52-65b775a9d947");
// const QUuid toPrivate("41209570-bc66-45e0-a1c5-31c185d17c9e");
// }  // namespace Attr

// }  // namespace Events
// class WorkerInfo;
// void bugCollecter(WorkerInfo* bugInfo, unsigned short bugLevel,
//                   QString bugLocate);
// void bugCollecter(unsigned short bugLevel, QString bugLocate);

// // 工作线程的基本信息
// class WorkerInfo {
//    public:
//     // 默认使用的构造函数
// #ifdef DEBUG
//     explicit WorkerInfo(const QFileInfo& aimFile = QFileInfo(),
//                         const QDir& workingDir = QDir(),
//                         const QUuid workingMode = WorkingModes::exec,
//                         bool isDebugMode = true);
// #else
//     explicit WorkerInfo(const QFileInfo& aimFile,
//                         const QDir& workingDir = QDir(),
//                         const QUuid workingMode = WorkingModes::exec,
//                         bool isDebugMode = false);
// #endif

//     explicit WorkerInfo(const WorkerInfo&) = default;
//     WorkerInfo& operator=(const WorkerInfo&) = default;
//     explicit WorkerInfo(WorkerInfo&&) = default;

//     virtual ~WorkerInfo();

//     // 比较运算符比较自身的UUID
//     virtual bool operator==(const WorkerInfo&);

//     // *公共接口信息：
//     //  自身的UUID
//     const QUuid uuid;
//     // 工作模式
//     const QUuid workingMode;

//     const QFileInfo aimFile;
//     const QDir workingDir;

//     //  当前的基本worker信息
//     // 由主执行函数管理
//     bool isActive = false;
//     bool isClean = false;
//     bool isDone = false;
//     bool isStoped = false;
//     bool isErred = false;

//     // 由onWorkStop槽管理
//     bool isToStop = false;

//     // *DEBUG SETTINGS
//     const bool isDebugMode = false;

//    public:
//     const QString getBaseWorkerDebugInfo();

//    protected:
//     QString tf(bool);
//     QString f(QString, QString);
// };

// class WorkerMsg : public WorkerInfo {
//    public:
//     explicit WorkerMsg(const WorkerInfo& info, const QString& message);
//     explicit WorkerMsg(const WorkerInfo& info, const char* message);
//     ~WorkerMsg();

//     const QString msg;
// };

// class WorkerEvent : public WorkerInfo {
//    public:
//     explicit WorkerEvent(const WorkerInfo& info, const QUuid& EventsType,
//                          const QUuid& EventsAttr = Events::Attr::toPrivate);
//     ~WorkerEvent();

//     const QUuid type;
//     const QUuid attr;
// };

// class BaseWorker : public virtual WorkerInfo, public virtual QObject {
//     Q_OBJECT
//    public:
//     BaseWorker();
//     virtual ~BaseWorker();

//    public slots:
//     // 处理外部事件
//     virtual void workEventIn(WorkerEvent) = 0;
//     virtual void workEventInWithCommand(WorkerEvent, WorkerMsg) = 0;

//     // 处理本地的to事件置位
//     void workFlagChangeEvent(WorkerEvent);

//    signals:
//     void workMessageOut(WorkerMsg);
//     void workEventOut(WorkerEvent);

//    protected:
//     QMutex mutex;
// };
