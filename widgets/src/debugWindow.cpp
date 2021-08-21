#include <ui_debugWindow.h>

#include <widgets/include/debugWindow.hpp>

DebugWindowViewItem::DebugWindowViewItem(QWidget *parent) : QObject(parent) {}
DebugWindowViewItem::~DebugWindowViewItem() {}

void DebugWindowViewItem::updateMessage(const QStringList msg) {
    for (auto &i : msg) {
        savedMessage.append(i + " ");
    }
    savedMessage.append("\n");
}

// -------------------------------------------------------------

DebugWindow::DebugWindow(QWidget *parent)
    : QDialog(parent), ui(new Ui::DebugWindow), timer(new QTimer(this)) {
    ui->setupUi(this);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    timer->start(500);

    // connect(ui->debugOutConsole, &QTextBrowser::textChanged, this,
    //         &DebugWindow::textBrowserAutoScroll);
    connect(timer, &QTimer::timeout, this, &DebugWindow::reflush);
    connect(timer, &QTimer::timeout, this, &DebugWindow::textBrowserAutoScroll);

    setGeometry(0, 0, 900, 600);
    move(0, 0);
    show();
}

DebugWindow::~DebugWindow() { delete ui; }

void DebugWindow::errorCritical(const QUuid &id,
                                const QStringList &errorMessage) {
    QMessageBox::critical(this, "CNM,CNM！又写Bug出来了！！！！",
                          "工头喊一嗓子，加班加班");
}

void DebugWindow::printDebugMessage(const QString &label,
                                    const QStringList &message) {
    auto *item = labels.value(label, nullptr);

    if (item != nullptr) {
        item->updateMessage(message);
    } else {
        item = new DebugWindowViewItem(this);
        item->setText(label);
        labels.insert(label, item);
        ui->listWidget->addItem(item);

        item->updateMessage(message);
    }
}
void DebugWindow::printDebugMessage(const QUuid &id,
                                    const QStringList &message) {
    printDebugMessage(id.toString(), message);
}

// void DebugWindow::printDebugMessageL(const QString &label,
//                                      const QStringList &message) {
//     if (labels.value(label, nullptr) != nullptr) {
//         labels[label]->updateMessage(message);
//     } else {
//         DebugWindowViewItem *item = new DebugWindowViewItem(this);
//         labels.insert(label, item);
//         item->setText(label);
//         ui->listWidget->addItem(labels[label]);
//         item->updateMessage(message);
//     }

//     ui->listWidget->setCurrentItem(labels.value(label, 0));
// }

void DebugWindow::reflush() {
    DebugWindowViewItem *item =
        (DebugWindowViewItem *)ui->listWidget->currentItem();

    if (item != nullptr) {
        ui->debugOutConsole->setText(item->savedMessage);
    }
}

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

void DebugWindow::textBrowserAutoScroll() {
    QTextCursor tCur(ui->debugOutConsole->textCursor());
    tCur.movePosition(QTextCursor::End);
    ui->debugOutConsole->setTextCursor(tCur);
}
