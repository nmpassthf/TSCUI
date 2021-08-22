#include <ui_debugWindow.h>

#include <include/debugWindow.hpp>

// -------------------------------------------------------------

DebugWindowViewItem::DebugWindowViewItem(QWidget *parent) : QObject(parent) {}
DebugWindowViewItem::~DebugWindowViewItem() {
    for (auto i : savedMessage) delete i;
}

void DebugWindowViewItem::storageMessage(UIType::MsgT msg) {
    QMutexLocker locker(&mutex);
    savedMessage.append(new UIType::MsgT(msg));
}

QList<UIType::MsgT *> DebugWindowViewItem::getAllMessage() {
    return savedMessage;
}

// -------------------------------------------------------------

DebugWindow::DebugWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::DebugWindow), pool(new QThreadPool(this)) {
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(ui->listWidget, &QListWidget::itemClicked,
            [&]() { ui->debugOutConsole->setAutoScroll(true); });

    connect(ui->listWidget, &QListWidget::itemClicked, ui->debugOutConsole,
            &TextBrowser::doScroll);

    connect(ui->listWidget, &QListWidget::itemClicked, [&]() {
        auto item =
            static_cast<DebugWindowViewItem *>(ui->listWidget->currentItem());
        ui->debugOutConsole->setText("");
        {
            QMutexLocker locker(&item->mutex);
            for (auto i : item->getAllMessage())
                ui->debugOutConsole->setAppendFormated(*i);
        }
    });

    setGeometry(0, 0, 900, 600);
    move(1000, 0);
    show();
}

DebugWindow::~DebugWindow() { delete ui; }

void DebugWindow::onDebugMessageIn(const QString &aimLabel, UIType::MsgT msg) {
    auto *item = items.value(aimLabel, nullptr);

    // 如果目标项不存在，new
    if (item == nullptr) {
        item = new DebugWindowViewItem(this);
        item->setText(aimLabel);
        items.insert(aimLabel, item);
        ui->listWidget->addItem(item);
    }

    // 存储新送达的消息
    item->storageMessage(msg);
    // 刷新显示
    reflush(item, msg);
}
void DebugWindow::onDebugMessageIn(const QUuid &id, UIType::MsgT msg) {
    onDebugMessageIn(id.toString(), msg);
}

void DebugWindow::reflush(DebugWindowViewItem *item, UIType::MsgT msg) {
    // 如果送达项与当前项相符，append新项
    if (item == ui->listWidget->currentItem()) {
        ui->debugOutConsole->setAppendFormated(msg);
    }
}
// void DebugWindow::reflushUpdate(const QString &upd, const QString &label) {
//     if (ui->listWidget->currentItem() != nullptr)
//         if (ui->listWidget->currentItem()->text() == label) {
//             ui->debugOutConsole->insertPlainText(upd);
//         }
// }

void DebugWindow::resizeEvent(QResizeEvent *event) {
    this->setUpdatesEnabled(false);
    QSize eventSize = event->size();
    ui->listWidget->setGeometry(0, 0,

                                eventSize.width() / 3, eventSize.height());
    QRect listWidgetRect = ui->listWidget->geometry();

    ui->debugOutConsole->setGeometry(
        listWidgetRect.width() + 3, 0,
        eventSize.width() - ui->listWidget->geometry().width() - 3,
        eventSize.height());
    this->setUpdatesEnabled(true);
}
