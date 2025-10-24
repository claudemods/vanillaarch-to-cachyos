#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QScrollArea>
#include <QFrame>
#include <QFont>
#include <QFontDatabase>
#include <QPalette>
#include <QScrollBar>
#include <QProcess>
#include <QMessageBox>
#include <QThread>
#include <QTextCursor>
#include <QTimer>
#include <QInputDialog>

// Include your header files
#include "grubtty.h"
#include "kdegrub.h"
#include "gnomegrub.h"
#include "ttysystemd-boot.h"
#include "kdesystemd-boot.h"
#include "gnomesystemd-boot.h"

class MigrationWindow : public QWidget {
    Q_OBJECT

private:
    QListWidget *menuList;
    QTextEdit *logOutput;
    QPushButton *executeButton;
    QPushButton *clearLogButton;
    QLabel *bannerLabel;
    QLabel *titleLabel;
    QString sudoPassword;
    bool hasSudoAccess;

    struct MenuOption {
        int choice;
        QString description;
        bool useInternal;
    };

    QVector<MenuOption> options = {
        {1, "TTY Only (No Desktop) - GRUB", true},
        {2, "Full KDE Plasma - GRUB", true},
        {3, "Full GNOME - GRUB", true},
        {4, "TTY Only (No Desktop) - systemd-boot", true},
        {5, "Full KDE Plasma - systemd-boot", true},
        {6, "Full GNOME - systemd-boot", true}
    };

public:
    MigrationWindow(QWidget *parent = nullptr) : QWidget(parent), hasSudoAccess(false) {
        requestSudoPassword();
        if (hasSudoAccess) {
            setupUI();
            setupConnections();
            applyStyling();
        }
    }

private slots:
    void executeMigration() {
        if (menuList->currentRow() == -1) {
            QMessageBox::warning(this, "No Selection", "Please select a migration option first.");
            return;
        }

        if (!hasSudoAccess) {
            QMessageBox::critical(this, "No Sudo Access", "Sudo access is required to perform migrations.");
            requestSudoPassword();
            return;
        }

        int choice = menuList->currentRow() + 1;
        logOutput->append(QString("\n🚀 Executing: %1\n").arg(options[choice-1].description));
        logOutput->moveCursor(QTextCursor::End);

        // Disable execute button during migration
        executeButton->setEnabled(false);

        // Run migration in a separate thread to keep UI responsive
        QTimer::singleShot(100, [this, choice]() {
            runMigrationTask(choice);
        });
    }

    void clearLog() {
        logOutput->clear();
    }

    void onSelectionChanged() {
        executeButton->setEnabled(menuList->currentRow() != -1 && hasSudoAccess);
    }

private:
    void requestSudoPassword() {
        bool ok;
        QString password = QInputDialog::getText(this, 
            "Sudo Authentication",
            "Enter your sudo password:",
            QLineEdit::Password,
            "",
            &ok);
        
        if (ok && !password.isEmpty()) {
            // Test sudo access
            if (testSudoAccess(password)) {
                sudoPassword = password;
                hasSudoAccess = true;
                logOutput->append("✅ Sudo authentication successful!\n");
            } else {
                QMessageBox::critical(this, "Authentication Failed", 
                    "Sudo authentication failed. Please check your password and try again.");
                hasSudoAccess = false;
                QTimer::singleShot(0, this, &QWidget::close);
            }
        } else {
            QMessageBox::information(this, "Authentication Required", 
                "Sudo access is required to run migration tasks.");
            hasSudoAccess = false;
            QTimer::singleShot(0, this, &QWidget::close);
        }
    }

    bool testSudoAccess(const QString &password) {
        QProcess process;
        process.start("sudo", QStringList() << "-S" << "echo" << "sudo_test");
        
        if (!process.waitForStarted()) {
            return false;
        }
        
        process.write(password.toUtf8() + "\n");
        process.closeWriteChannel();
        
        if (!process.waitForFinished(5000)) {
            return false;
        }
        
        return (process.exitCode() == 0);
    }

    void setupUI() {
        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // Banner with ASCII art (red)
        bannerLabel = new QLabel(this);
        bannerLabel->setText(
            " ░█████╗░██╗░░░░░░█████╗░██║░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗\n"
            " ██╔══██╗██║░░░░░██╔══██╗██║░░░██║██╔══██╗██╔════╝████╗░████║██╔══██╗██╔══██╗██╔════╝\n"
            " ██║░░╚═╝██║░░░░░███████║██║░░░██║██║░░██║█████╗░░██╔████╔██║██║░░██║██║░░██║╚█████╗░\n"
            " ██║░░██╗██║░░░░░██╔══██║██║░░░██║██║░░██║██╔══╝░░██║╚██╔╝██║██║░░██║██║░░██║░╚═══██╗\n"
            " ╚█████╔╝███████╗██║░░██║╚██████╔╝██████╔╝███████╗██║░╚═╝░██║╚█████╔╝██████╔╝██████╔╝\n"
            " ░╚════╝░╚══════╝╚═╝░░░░░░╚═════╝░╚══════╝░╚══════╝╚═╝░░░░░╚═╝░╚════╝░╚═════╝░╚═════╝░"
        );
        bannerLabel->setAlignment(Qt::AlignCenter);
        bannerLabel->setStyleSheet("color: #FF0000; font-family: monospace; font-size: 8pt;");

        // Title (cyan)
        titleLabel = new QLabel(this);
        titleLabel->setText("claudemods Vanilla Arch to CachyOS Migration Script");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet("color: #00FFFF; font-family: sans-serif; font-size: 14pt; font-weight: bold; margin: 10px;");

        // Content area
        QHBoxLayout *contentLayout = new QHBoxLayout();

        // Left panel - Menu
        QFrame *menuFrame = new QFrame(this);
        menuFrame->setFrameStyle(QFrame::StyledPanel);
        QVBoxLayout *menuLayout = new QVBoxLayout(menuFrame);

        QLabel *menuTitle = new QLabel("Migration Options", this);
        menuTitle->setStyleSheet("color: #00FFFF; font-weight: bold; font-size: 12pt;");

        menuList = new QListWidget(this);
        for (const auto &option : options) {
            menuList->addItem(option.description);
        }

        executeButton = new QPushButton("Execute Migration", this);
        executeButton->setEnabled(false);

        menuLayout->addWidget(menuTitle);
        menuLayout->addWidget(menuList);
        menuLayout->addWidget(executeButton);

        // Right panel - Log
        QFrame *logFrame = new QFrame(this);
        logFrame->setFrameStyle(QFrame::StyledPanel);
        QVBoxLayout *logLayout = new QVBoxLayout(logFrame);

        QLabel *logTitle = new QLabel("Migration Log", this);
        logTitle->setStyleSheet("color: #00FFFF; font-weight: bold; font-size: 12pt;");

        logOutput = new QTextEdit(this);
        logOutput->setReadOnly(true);
        logOutput->setPlaceholderText("Migration log will appear here...");

        clearLogButton = new QPushButton("Clear Log", this);

        logLayout->addWidget(logTitle);
        logLayout->addWidget(logOutput);
        logLayout->addWidget(clearLogButton);

        // Add panels to content layout
        contentLayout->addWidget(menuFrame, 1);
        contentLayout->addWidget(logFrame, 2);

        // Assemble main layout
        mainLayout->addWidget(bannerLabel);
        mainLayout->addWidget(titleLabel);
        mainLayout->addLayout(contentLayout);

        // Set window properties
        setWindowTitle("CachyOS Migration Tool - claudemods");
        setMinimumSize(1000, 700);
    }

    void setupConnections() {
        connect(executeButton, &QPushButton::clicked, this, &MigrationWindow::executeMigration);
        connect(clearLogButton, &QPushButton::clicked, this, &MigrationWindow::clearLog);
        connect(menuList, &QListWidget::currentRowChanged, this, &MigrationWindow::onSelectionChanged);
    }

    void applyStyling() {
        // Set cyan theme
        setStyleSheet(
            "QWidget {"
            "    background-color: #0a0a1a;"
            "    color: #00FFFF;"
            "    font-family: 'Segoe UI', Arial, sans-serif;"
            "}"
            "QFrame {"
            "    background-color: #0f0f2d;"
            "    border: 2px solid #00FFFF;"
            "    border-radius: 8px;"
            "    padding: 10px;"
            "}"
            "QListWidget {"
            "    background-color: #1a1a3a;"
            "    color: #00FFFF;"
            "    border: 1px solid #00FFFF;"
            "    border-radius: 4px;"
            "    padding: 5px;"
            "    font-size: 10pt;"
            "}"
            "QListWidget::item {"
            "    padding: 8px;"
            "    border-bottom: 1px solid #00FFFF;"
            "}"
            "QListWidget::item:selected {"
            "    background-color: #00FFFF;"
            "    color: #0a0a1a;"
            "    font-weight: bold;"
            "}"
            "QListWidget::item:hover {"
            "    background-color: #1a3a5a;"
            "}"
            "QTextEdit {"
            "    background-color: #1a1a3a;"
            "    color: #00FFFF;"
            "    border: 1px solid #00FFFF;"
            "    border-radius: 4px;"
            "    font-family: 'Courier New', monospace;"
            "    font-size: 10pt;"
            "}"
            "QPushButton {"
            "    background-color: #00FFFF;"
            "    color: #0a0a1a;"
            "    border: 2px solid #00FFFF;"
            "    border-radius: 6px;"
            "    padding: 8px 16px;"
            "    font-weight: bold;"
            "    font-size: 10pt;"
            "}"
            "QPushButton:hover {"
            "    background-color: #0a0a1a;"
            "    color: #00FFFF;"
            "}"
            "QPushButton:pressed {"
            "    background-color: #00AAAA;"
            "}"
            "QPushButton:disabled {"
            "    background-color: #555555;"
            "    color: #888888;"
            "    border-color: #666666;"
            "}"
        );

        // Custom scrollbar styling
        logOutput->verticalScrollBar()->setStyleSheet(
            "QScrollBar:vertical {"
            "    border: none;"
            "    background: #1a1a3a;"
            "    width: 15px;"
            "    border-radius: 7px;"
            "}"
            "QScrollBar::handle:vertical {"
            "    background: #00FFFF;"
            "    border-radius: 7px;"
            "    min-height: 20px;"
            "}"
            "QScrollBar::handle:vertical:hover {"
            "    background: #00AAAA;"
            "}"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {"
            "    border: none;"
            "    background: none;"
            "}"
        );
    }

    void runMigrationTask(int choice) {
        logOutput->append("🔄 Starting migration process...");

        try {
            // Execute the appropriate migration based on selection
            switch(choice) {
                case 1: {
                    GrubTTYMigration *migration = new GrubTTYMigration(this);
                    connect(migration, &GrubTTYMigration::migrationOutput, this, &MigrationWindow::onMigrationOutput);
                    migration->runMigration();
                    migration->deleteLater();
                    break;
                }
                case 2: {
                    KdeGrubMigration *migration = new KdeGrubMigration(this);
                    connect(migration, &KdeGrubMigration::migrationOutput, this, &MigrationWindow::onMigrationOutput);
                    migration->runMigration();
                    migration->deleteLater();
                    break;
                }
                case 3: {
                    GnomeGrubMigration *migration = new GnomeGrubMigration(this);
                    connect(migration, &GnomeGrubMigration::migrationOutput, this, &MigrationWindow::onMigrationOutput);
                    migration->runMigration();
                    migration->deleteLater();
                    break;
                }
                case 4: {
                    TTYSystemdBootMigration *migration = new TTYSystemdBootMigration(this);
                    connect(migration, &TTYSystemdBootMigration::migrationOutput, this, &MigrationWindow::onMigrationOutput);
                    migration->runMigration();
                    migration->deleteLater();
                    break;
                }
                case 5: {
                    KdeSystemdBootMigration *migration = new KdeSystemdBootMigration(this);
                    connect(migration, &KdeSystemdBootMigration::migrationOutput, this, &MigrationWindow::onMigrationOutput);
                    migration->runMigration();
                    migration->deleteLater();
                    break;
                }
                case 6: {
                    GnomeSystemdBootMigration *migration = new GnomeSystemdBootMigration(this);
                    connect(migration, &GnomeSystemdBootMigration::migrationOutput, this, &MigrationWindow::onMigrationOutput);
                    migration->runMigration();
                    migration->deleteLater();
                    break;
                }
                default:
                    throw std::runtime_error("Invalid migration option");
            }

            logOutput->append("✅ Migration completed successfully!");

        } catch (const std::exception &e) {
            logOutput->append(QString("❌ Migration failed: %1").arg(e.what()));
        } catch (...) {
            logOutput->append("❌ Migration failed with unknown error");
        }

        logOutput->moveCursor(QTextCursor::End);
        executeButton->setEnabled(true);
    }

private slots:
    void onMigrationOutput(const QString &output) {
        logOutput->append(output);
        logOutput->moveCursor(QTextCursor::End);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application-wide style and properties
    app.setStyle("Fusion");
    app.setApplicationName("CachyOS Migration Tool");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("claudemods");

    MigrationWindow window;
    if (window.isVisible()) { // Only show if sudo authentication was successful
        window.show();
        return app.exec();
    } else {
        return 1; // Exit if sudo authentication failed
    }
}

#include "main.moc"
