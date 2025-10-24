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
    MigrationWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setupUI();
        setupConnections();
        applyStyling();
    }

private slots:
    void executeMigration() {
        if (menuList->currentRow() == -1) {
            QMessageBox::warning(this, "No Selection", "Please select a migration option first.");
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
        executeButton->setEnabled(menuList->currentRow() != -1);
    }

private:
    void setupUI() {
        // Main layout
        QVBoxLayout *mainLayout = new QVBoxLayout(this);

        // Banner with ASCII art (red)
        bannerLabel = new QLabel(this);
        bannerLabel->setText(
            " ░█████╗░██╗░░░░░░█████╗░██╗░░░██╗██████╗░███████╗███╗░░░███╗░█████╗░██████╗░░██████╗\n"
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
                    GrubTTYMigration migration;
                    migration.runMigration();
                    break;
                }
                case 2: {
                    KdeGrubMigration migration;
                    migration.runMigration();
                    break;
                }
                case 3: {
                    GnomeGrubMigration migration;
                    migration.runMigration();
                    break;
                }
                case 4: {
                    TTYSystemdBootMigration migration;
                    migration.runMigration();
                    break;
                }
                case 5: {
                    KdeSystemdBootMigration migration;
                    migration.runMigration();
                    break;
                }
                case 6: {
                    GnomeSystemdBootMigration migration;
                    migration.runMigration();
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
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application-wide style and properties
    app.setStyle("Fusion");
    app.setApplicationName("CachyOS Migration Tool");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("claudemods");

    MigrationWindow window;
    window.show();

    return app.exec();
}

#include "main.moc"
