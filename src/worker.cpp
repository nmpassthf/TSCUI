// #include <include/worker.hpp>

// void bugCollecter(WorkerInfo* bugInfo, unsigned short bugLevel,
//                   QString bugLocate) {
// #ifdef DEBUG
//     switch (bugLevel) {
//         case 0:
//             qDebug() << "[Info]Bug Trace! @=> " << bugLocate << "\n=>"
//                      << "\t\t" << bugInfo->getBaseWorkerDebugInfo();
//             break;
//         case 1:
//             qDebug() << "[Warn]Bug Trace! @=> " << bugLocate << "\n=>"
//                      << bugInfo->getBaseWorkerDebugInfo();
//             break;
//         case 2:
//             qDebug() << "[Error]Bug Trace! @=> " << bugLocate << "\n=>"
//                      << bugInfo->getBaseWorkerDebugInfo();
//             break;
//         default:
//             bugCollecter(bugInfo, 2, "bugCollecter");
//     }
// #endif
// }
// void bugCollecter(unsigned short bugLevel, QString bugLocate) {
// #ifdef DEBUG
//     switch (bugLevel) {
//         case 0:
//             qDebug() << "[Info]Bug Trace! @=> " << bugLocate << "\n=>";
//             break;
//         case 1:
//             qDebug() << "[Warn]Bug Trace! @=> " << bugLocate << "\n=>";
//             break;
//         case 2:
//             qDebug() << "[Error]Bug Trace! @=> " << bugLocate << "\n=>";
//             break;
//         default:
//             bugCollecter(2, "bugCollecter");
//     }
// #endif
// }

// WorkerInfo::WorkerInfo(const QFileInfo& aimFile, const QDir& workingDir,
//                        const QUuid workingMode, bool isDebugMode)
//     : aimFile(aimFile),
//       workingDir(workingDir),
//       uuid(QUuid::createUuid()),
//       workingMode(workingMode),
//       isDebugMode(isDebugMode) {
//     // 打印调试信息
//     if (isDebugMode) {
//         bugCollecter(this, 0, "A WorkerInfo Object is created");
//     }
// }

// WorkerInfo::~WorkerInfo() {
//     // 打印调试信息
//     if (isDebugMode) {
//         bugCollecter(this, 0, "A WorkerInfo Object is deleted");
//     }
// }

// bool WorkerInfo::operator==(const WorkerInfo& other) {
//     return this->uuid == other.uuid;
// }

// const QString WorkerInfo::getBaseWorkerDebugInfo() {
//     QDataStream ss;
//     ss << f("UUID", uuid.toString())
//        << f("Working Mode", workingMode.toString())
//        << f("\n\t\tActive\t|Clean\t|Done\t|ToStop\t|Stoped\t|Err\t|Debug",
//             "\n\t\t" + tf(isActive) + "\t|" + tf(isClean) + "\t|" +
//             tf(isDone) +
//                 "\t|" + tf(isToStop) + "\t|" + tf(isStoped) + "\t|" +
//                 tf(isErred) + "\t|" + tf(isDebugMode));

//     QString s;
//     ss >> s;
//     return s;
// }

// QString WorkerInfo::tf(bool b) { return b ? "T" : "F"; }
// QString WorkerInfo::f(QString s, QString r) {
//     return QString("") + "{" + s + "->" + r + "}\t";
// }

// // -----------------------------------

// WorkerMsg::WorkerMsg(const WorkerInfo& info, const QString& msg)
//     : WorkerInfo(info), msg(msg) {}
// WorkerMsg::WorkerMsg(const WorkerInfo& info, const char* message)
//     : WorkerInfo(info), msg(msg) {}

// WorkerMsg::~WorkerMsg() {}

// // -----------------------------------

// WorkerEvent::WorkerEvent(const WorkerInfo& info, const QUuid& EventsType,
//                          const QUuid& EventsAttr)
//     : WorkerInfo(info), type(EventsType), attr(EventsAttr) {}

// WorkerEvent::~WorkerEvent() {}

// // -----------------------------------

// BaseWorker::BaseWorker() {
//     // 本地的事件环回
//     connect(this, &BaseWorker::workEventIn,
//     &BaseWorker::workFlagChangeEvent);
// }
// BaseWorker::~BaseWorker() {
//     emit workEventOut(WorkerEvent(*this, Events::Type::toStop));

//     while (isStoped) {
//         QThread::msleep(20);
//     }

//     if (isDebugMode) {
//         bugCollecter(this, 0, "Destructed BaseWorker!");
//     }
// }

// void BaseWorker::workFlagChangeEvent(WorkerEvent event) {
//     /*
//         events:
//             type:
//                 toRun
//                 toStop
//             attr:
//                 toPublic
//                 toPrivate
//     */
//     if (event.attr == Events::Attr::toPublic || event.uuid == this->uuid) {
//         if (event.type == Events::Type::toStop) {
//             {
//                 QMutexLocker locker(&mutex);
//                 if (isDebugMode) {
//                     bugCollecter(this, 0, "Change Flag isToStop");
//                 }
//                 // 置位
//                 this->isToStop = true;
//             }
//         } else {
//             if (event.type == Events::Type::active) {
//             } else if (event.type == Events::Type::clean) {
//             } else if (event.type == Events::Type::done) {
//             } else if (event.type == Events::Type::stop) {
//             } else if (event.type == Events::Type::error) {
//             } else {
//                 emit workEventOut(WorkerEvent(*this, Events::Type::error));
//                 emit workEventOut(WorkerEvent(*this, Events::Type::toStop));
//                 bugCollecter(&event, 0,
//                              "get err event worFlagChangeEvent" +
//                                  getBaseWorkerDebugInfo());
//             }
//         }
//     } else {
//         return;
//     }
// }
