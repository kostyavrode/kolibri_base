#include "mainwindow.h"

#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QProgressBar>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QSpinBox>
#include <QTextEdit>

// - ui_mainwindow.h будет сгенерирован только при наличии .ui-файла
// - здесь мы определяем минимальный набор виджетов вручную, поэтому ui_mainwindow.h по факту не используется

class SimpleUiMainWindow
{
public:
    QWidget *centralWidget = nullptr;

    QLineEdit *inputDirEdit = nullptr;
    QPushButton *browseInputButton = nullptr;

    QLineEdit *maskEdit = nullptr;

    QLineEdit *outputDirEdit = nullptr;
    QPushButton *browseOutputButton = nullptr;

    QCheckBox *deleteInputCheck = nullptr;

    QRadioButton *overwriteRadio = nullptr;
    QRadioButton *counterRadio = nullptr;

    QCheckBox *timerCheck = nullptr;
    QSpinBox *intervalSpin = nullptr;

    QLineEdit *xorKeyEdit = nullptr;

    QPushButton *processOnceButton = nullptr;
    QPushButton *toggleTimerButton = nullptr;

    QProgressBar *progressBar = nullptr;
    QLabel *statusLabel = nullptr;
    QTextEdit *logEdit = nullptr;
};

namespace Ui {
class MainWindow : public SimpleUiMainWindow {};
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->centralWidget = new QWidget(this);
    setCentralWidget(ui->centralWidget);

    auto *mainLayout = new QVBoxLayout;
    ui->centralWidget->setLayout(mainLayout);

    auto *inputLayout = new QHBoxLayout;
    ui->inputDirEdit = new QLineEdit;
    ui->browseInputButton = new QPushButton(tr("Обзор..."));
    inputLayout->addWidget(new QLabel(tr("Каталог входных файлов:")));
    inputLayout->addWidget(ui->inputDirEdit);
    inputLayout->addWidget(ui->browseInputButton);
    mainLayout->addLayout(inputLayout);

    auto *maskLayout = new QHBoxLayout;
    ui->maskEdit = new QLineEdit;
    maskLayout->addWidget(new QLabel(tr("Маска файлов (*.txt;test.bin):")));
    maskLayout->addWidget(ui->maskEdit);
    mainLayout->addLayout(maskLayout);

    auto *outputLayout = new QHBoxLayout;
    ui->outputDirEdit = new QLineEdit;
    ui->browseOutputButton = new QPushButton(tr("Обзор..."));
    outputLayout->addWidget(new QLabel(tr("Каталог вывода:")));
    outputLayout->addWidget(ui->outputDirEdit);
    outputLayout->addWidget(ui->browseOutputButton);
    mainLayout->addLayout(outputLayout);

    ui->deleteInputCheck = new QCheckBox(tr("Удалять исходные файлы после обработки"));
    mainLayout->addWidget(ui->deleteInputCheck);

    auto *existLayout = new QHBoxLayout;
    existLayout->addWidget(new QLabel(tr("Если выходной файл существует:")));
    ui->overwriteRadio = new QRadioButton(tr("Перезаписать"));
    ui->counterRadio = new QRadioButton(tr("Добавить счётчик к имени"));
    ui->counterRadio->setChecked(true);
    existLayout->addWidget(ui->overwriteRadio);
    existLayout->addWidget(ui->counterRadio);
    existLayout->addStretch();
    mainLayout->addLayout(existLayout);

    auto *timerLayout = new QHBoxLayout;
    ui->timerCheck = new QCheckBox(tr("Работа по таймеру"));
    ui->intervalSpin = new QSpinBox;
    ui->intervalSpin->setRange(1, 24 * 60 * 60);
    ui->intervalSpin->setValue(5);
    timerLayout->addWidget(ui->timerCheck);
    timerLayout->addWidget(new QLabel(tr("Период (сек):")));
    timerLayout->addWidget(ui->intervalSpin);
    timerLayout->addStretch();
    mainLayout->addLayout(timerLayout);

    auto *xorLayout = new QHBoxLayout;
    ui->xorKeyEdit = new QLineEdit;
    xorLayout->addWidget(new QLabel(tr("8-байтовый ключ XOR (hex):")));
    xorLayout->addWidget(ui->xorKeyEdit);
    mainLayout->addLayout(xorLayout);

    auto *buttonsLayout = new QHBoxLayout;
    ui->processOnceButton = new QPushButton(tr("Однократная обработка"));
    ui->toggleTimerButton = new QPushButton(tr("Запустить таймер"));
    buttonsLayout->addWidget(ui->processOnceButton);
    buttonsLayout->addWidget(ui->toggleTimerButton);
    buttonsLayout->addStretch();
    mainLayout->addLayout(buttonsLayout);

    ui->progressBar = new QProgressBar;
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(0);
    mainLayout->addWidget(ui->progressBar);

    ui->statusLabel = new QLabel(tr("Готово"));
    mainLayout->addWidget(ui->statusLabel);

    ui->logEdit = new QTextEdit;
    ui->logEdit->setReadOnly(true);
    ui->logEdit->setMinimumHeight(120);
    mainLayout->addWidget(ui->logEdit);

    setupConnections();

    m_timer.setSingleShot(false);
}

MainWindow::~MainWindow()
{
    resetWorker();
    delete ui;
}

void MainWindow::setupConnections()
{
    connect(ui->browseInputButton, &QPushButton::clicked,
            this, &MainWindow::onBrowseInput);
    connect(ui->browseOutputButton, &QPushButton::clicked,
            this, &MainWindow::onBrowseOutput);
    connect(ui->processOnceButton, &QPushButton::clicked,
            this, &MainWindow::onProcessOnce);
    connect(ui->toggleTimerButton, &QPushButton::clicked,
            this, &MainWindow::onToggleTimer);
    connect(&m_timer, &QTimer::timeout,
            this, &MainWindow::onTimerTick);
}

ProcessingConfig MainWindow::readConfig(bool *ok) const
{
    ProcessingConfig cfg;
    *ok = false;

    cfg.inputDirectory = ui->inputDirEdit->text().trimmed();
    cfg.fileMask = ui->maskEdit->text().trimmed();
    cfg.outputDirectory = ui->outputDirEdit->text().trimmed();
    cfg.deleteInput = ui->deleteInputCheck->isChecked();
    cfg.existingFileAction = ui->overwriteRadio->isChecked()
            ? ProcessingConfig::ExistingFileAction::Overwrite
            : ProcessingConfig::ExistingFileAction::AddCounter;
    cfg.useTimer = ui->timerCheck->isChecked();
    cfg.timerIntervalMs = ui->intervalSpin->value() * 1000;

    QString keyText = ui->xorKeyEdit->text().trimmed();
    if (keyText.startsWith("0x", Qt::CaseInsensitive)) {
        keyText.remove(0, 2);
    }
    if (keyText.isEmpty() || keyText.length() > 16) {
        QMessageBox::warning(nullptr, tr("Ошибка"), tr("Ключ XOR должен быть hex-строкой до 16 символов"));
        return cfg;
    }

    bool okHex = false;
    quint64 value = keyText.toULongLong(&okHex, 16);
    if (!okHex) {
        QMessageBox::warning(nullptr, tr("Ошибка"), tr("Не удалось разобрать XOR-ключ как hex-значение"));
        return cfg;
    }
    cfg.xorKey = value;

    if (cfg.inputDirectory.isEmpty()) {
        QMessageBox::warning(nullptr, tr("Ошибка"), tr("Укажите каталог входных файлов"));
        return cfg;
    }

    *ok = true;
    return cfg;
}

QStringList MainWindow::findInputFiles(const ProcessingConfig &config) const
{
    QDir dir(config.inputDirectory);
    if (!dir.exists()) {
        return {};
    }

    QString maskText = config.fileMask;
    QStringList patterns;
    for (const QString &part : maskText.split(';', Qt::SkipEmptyParts)) {
        QString t = part.trimmed();
        if (t.isEmpty()) {
            continue;
        }
        if (!t.contains('*') && !t.contains('?')) {
            if (t.startsWith('.')) {
                patterns << ("*" + t);
            } else {
                patterns << t;
            }
        } else {
            patterns << t;
        }
    }

    if (patterns.isEmpty()) {
        patterns << "*";
    }

    QFileInfoList list = dir.entryInfoList(patterns, QDir::Files | QDir::NoSymLinks);
    QStringList result;
    for (const QFileInfo &fi : list) {
        result << fi.absoluteFilePath();
    }
    return result;
}

void MainWindow::startProcessing(const QStringList &files, const ProcessingConfig &config)
{
    if (files.isEmpty()) {
        ui->statusLabel->setText(tr("Нет файлов для обработки"));
        return;
    }

    if (m_processing) {
        ui->statusLabel->setText(tr("Обработка уже запущена"));
        return;
    }

    m_processing = true;
    ui->progressBar->setValue(0);
    ui->statusLabel->setText(tr("Обработка запущена"));

    resetWorker();

    m_workerThread = new QThread(this);
    m_processor = new FileProcessor;
    m_processor->moveToThread(m_workerThread);

    // - классический паттерн worker thread в qt

    connect(m_workerThread, &QThread::started,
            [this, files, config]() {
                m_processor->processFiles(files, config);
            });

    connect(m_processor, &FileProcessor::fileProgress,
            this, &MainWindow::onFileProgress);
    connect(m_processor, &FileProcessor::fileFinished,
            this, &MainWindow::onFileFinished);
    connect(m_processor, &FileProcessor::batchFinished,
            this, &MainWindow::onBatchFinished);
    connect(m_processor, &FileProcessor::errorOccurred,
            this, &MainWindow::onProcessingError);

    connect(m_workerThread, &QThread::finished,
            m_processor, &QObject::deleteLater);
    connect(m_workerThread, &QThread::finished,
            m_workerThread, &QObject::deleteLater);

    m_workerThread->start();
}

void MainWindow::resetWorker()
{
    if (m_workerThread) {
        m_workerThread->quit();
        m_workerThread->wait();
        m_workerThread = nullptr;
        m_processor = nullptr;
    }
}

void MainWindow::onBrowseInput()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите каталог входных файлов"));
    if (!dir.isEmpty()) {
        ui->inputDirEdit->setText(dir);
    }
}

void MainWindow::onBrowseOutput()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите каталог вывода"));
    if (!dir.isEmpty()) {
        ui->outputDirEdit->setText(dir);
    }
}

void MainWindow::onProcessOnce()
{
    bool ok = false;
    ProcessingConfig cfg = readConfig(&ok);
    if (!ok) {
        return;
    }

    QStringList files = findInputFiles(cfg);
    if (files.isEmpty()) {
        QMessageBox::information(this, tr("Информация"), tr("Подходящих файлов не найдено"));
        return;
    }

    ui->logEdit->append(tr("Запуск однократной обработки, файлов: %1").arg(files.size()));
    startProcessing(files, cfg);
}

void MainWindow::onToggleTimer()
{
    if (m_timer.isActive()) {
        m_timer.stop();
        ui->toggleTimerButton->setText(tr("Запустить таймер"));
        ui->statusLabel->setText(tr("Таймер остановлен"));
        return;
    }

    bool ok = false;
    ProcessingConfig cfg = readConfig(&ok);
    if (!ok) {
        return;
    }

    if (!cfg.useTimer) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Включите режим работы по таймеру"));
        return;
    }

    m_timer.start(cfg.timerIntervalMs);
    ui->toggleTimerButton->setText(tr("Остановить таймер"));
    ui->statusLabel->setText(tr("Таймер запущен"));

    onTimerTick();
}

void MainWindow::onTimerTick()
{
    bool ok = false;
    ProcessingConfig cfg = readConfig(&ok);
    if (!ok) {
        return;
    }

    QStringList files = findInputFiles(cfg);
    if (files.isEmpty()) {
        ui->statusLabel->setText(tr("Файлов для обработки пока нет"));
        return;
    }

    if (!m_processing) {
        ui->logEdit->append(tr("Таймер - найдено файлов: %1").arg(files.size()));
        startProcessing(files, cfg);
    }
}

void MainWindow::onFileProgress(const QString &filePath, qint64 processedBytes, qint64 totalBytes)
{
    if (totalBytes <= 0) {
        return;
    }

    int percent = static_cast<int>((processedBytes * 100) / totalBytes);
    ui->progressBar->setValue(percent);
    ui->statusLabel->setText(tr("Обработка: %1 (%2%)").arg(QFileInfo(filePath).fileName()).arg(percent));
}

void MainWindow::onFileFinished(const QString &filePath)
{
    ui->logEdit->append(tr("Файл обработан: %1").arg(filePath));
}

void MainWindow::onBatchFinished()
{
    m_processing = false;
    ui->progressBar->setValue(100);
    ui->statusLabel->setText(tr("Обработка завершена"));
}

void MainWindow::onProcessingError(const QString &message)
{
    ui->logEdit->append(tr("Ошибка: %1").arg(message));
}

