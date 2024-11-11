#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSpacerItem>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <regex>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);//main layout
    QVBoxLayout *leftLayout = new QVBoxLayout;//left column layout
    QVBoxLayout *rightLayout = new QVBoxLayout;//left column layout

    //code Enter box
    ui->codeInputTextEdit = new QLineEdit;
    ui->codeInputTextEdit->setPlaceholderText("Enter code here...");
    ui->codeInputTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    leftLayout->addWidget(ui->codeInputTextEdit);
    QRegularExpression regex("^[0-9A-D][0-9A-F]{3}$");
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
    ui->codeInputTextEdit->setValidator(validator);
    ui->codeInputTextEdit->setToolTip("Each instruction must be exactly 4 characters.");

    //register table
    ui->registerTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->registerTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->registerTableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->registerTableWidget->setHorizontalHeaderLabels({"Registers"});
    leftLayout->addWidget(ui->registerTableWidget, 1);

    //adding leftside layout to the main
    mainLayout->addLayout(leftLayout,1);

    ui->MemoryLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    rightLayout->addWidget(ui->MemoryLabel);

    //adding memory to the right side of the layout
    ui->memoryTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->memoryTableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->memoryTableWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rightLayout->addWidget(ui->memoryTableWidget);
    mainLayout->addLayout(rightLayout,2);



    //layout for the bottom side
    QHBoxLayout *outputLayout = new QHBoxLayout;
    QVBoxLayout *bottomLayout = new QVBoxLayout;

    //Error Display
    ui->ErrorDisplay = new QLabel("Erros will be here!");
    ui->ErrorDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    outputLayout->addWidget(ui->ErrorDisplay,1);

    //Output Display
    ui->outputLabel = new QLabel("output will be here!");
    ui->outputLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    outputLayout->addWidget(ui->outputLabel,1);

    //IR and PC Display
    ui->IRandPCDisplay = new QLabel("IR:0 PC:0");
    ui->IRandPCDisplay->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    outputLayout->addWidget(ui->IRandPCDisplay,1);

    bottomLayout->addLayout(outputLayout);

    //upload button
    connect(ui->uploadButton, &QPushButton::clicked, this, &MainWindow::onUploadButtonClicked);


    //Buttons Raw
    QHBoxLayout *buttonLayout = new QHBoxLayout;

    //start address in memory
    buttonLayout->addWidget(ui->startAddressLineEdit,2);
    ui->startAddressLineEdit->setPlaceholderText("Enter start address here...");
    QRegularExpression re("^[0-9A-F]{2}$");
    QRegularExpressionValidator *validatorcounter = new QRegularExpressionValidator(re, this);
    ui->startAddressLineEdit->setValidator(validatorcounter);
    ui->startAddressLineEdit->setToolTip("Memory Address must be 2 characters.");

    //buttons
    buttonLayout->addWidget(ui->runButton,1);
    buttonLayout->addWidget(ui->runStepButton);
    buttonLayout->addWidget(ui->clearMemoryButton,1);
    buttonLayout->addWidget(ui->clearRegisterButton,1);
    buttonLayout->addWidget(ui->uploadButton);
    bottomLayout->addLayout(buttonLayout);//add buttons raw to bottom layout

    leftLayout->addLayout(bottomLayout);    //adding bottom layout to the main
    this->setCentralWidget(centralWidget);
    centralWidget->setLayout(mainLayout);

    memory = new Memory();
    is_step = false;
    can_set_start_address = true;

    //Setting up the tables and display default vlaues
    updateMemoryTable();
    updateRegisterTable();


    // Set up vertical and Horizontal header labels for the memory table
    QStringList memoryLabels;
    for (int i = 0; i < 16; ++i) {
        memoryLabels << QString::number(i, 16).toUpper(); // Convert to hex and make uppercase
    }
    ui->memoryTableWidget->setVerticalHeaderLabels(memoryLabels);
    ui->memoryTableWidget->setHorizontalHeaderLabels(memoryLabels);

    // Set up vertical header labels for the register table
    QStringList registerLabels;
    for (int i = 0; i < 16; ++i) {
        registerLabels << QString::number(i, 16).toUpper(); // Convert to hex and make uppercase
    }
    ui->registerTableWidget->setVerticalHeaderLabels(registerLabels);

    //connecting the gui to the actions
    connect(ui->codeInputTextEdit, &QLineEdit::returnPressed, this, &MainWindow::onInstructionEntered);
    connect(ui->runButton, &QPushButton::clicked, this, &MainWindow::onRunButtonClicked);
    connect(ui->runStepButton, &QPushButton::clicked, this, &MainWindow::onRunStepButtonClicked);
    connect(ui->clearMemoryButton, &QPushButton::clicked, this, &MainWindow::clearMemory);
    connect(ui->clearRegisterButton, &QPushButton::clicked, this, &MainWindow::clearRegisters);
    connect(ui->startAddressLineEdit, &QLineEdit::returnPressed, this, &MainWindow::setStartAddress);

    // Styling the GUI
    setStyleSheet("background-color: #2e2e2e; color: #ffffff;");

    // Memory table styling
    ui->memoryTableWidget->setStyleSheet(
        "QTableWidget { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555555; }"
        "QTableWidget::item { border: 1px solid #555555; padding: 5px; display: flex; }"
        );
    ui->memoryTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Register table styling
    ui->registerTableWidget->setStyleSheet(
        "QTableWidget { background-color: #3c3c3c; color: #ffffff; border: 1px solid #555555; }"
        "QTableWidget::item { border: 1px solid #555555; padding: 5px; }"
        );
    ui->registerTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Common button styling
    QString buttonStyle =
        "QPushButton { background-color: #3c3c3c; color: white; border: none; padding: 10px 20px; "
        "text-align: center; text-decoration: none; display: inline-block; font-size: 16px; "
        "margin: 4px 2px; cursor: pointer; border-radius: 5px; }"
        "QPushButton:hover { background-color: red; }";

    // Apply style to all buttons
    QString runButtonStyle = "QPushButton { background-color: #3c3c3c; color: white; border: none; padding: 10px 20px; "
                             "text-align: center; text-decoration: none; display: inline-block; font-size: 16px; "
                             "margin: 4px 2px; cursor: pointer; border-radius: 5px; }"
                             "QPushButton:hover { background-color: #45a049; }";
    ui->runButton->setStyleSheet(runButtonStyle);
    ui->runStepButton->setStyleSheet(runButtonStyle);
    ui->clearMemoryButton->setStyleSheet(buttonStyle);
    ui->clearRegisterButton->setStyleSheet(buttonStyle);
    ui->uploadButton->setStyleSheet(buttonStyle);

    // Set icon for the upload button
    QIcon uploadIcon("D:/Programming/OOP/Task 5 GUI/gui/upload.png");
    ui->uploadButton->setIcon(uploadIcon);
    ui->uploadButton->setIconSize(QSize(24, 24));
    ui->uploadButton->setText("");

    // Set icon for the Run button
    QIcon runIcon("D:/Programming/OOP/Task 5 GUI/gui/play.png");
    ui->runButton->setIcon(runIcon);
    ui->runButton->setIconSize(QSize(24, 24));
    ui->runButton->setText("");

    // Increase the size of the startAddressInput area
    ui->startAddressLineEdit->setMinimumSize(QSize(150, 30));
    ui->startAddressLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    QString labelDisplay = "QLabel { background-color: #2e2e2e; padding: 5px; border: 1px solid white; "
                           "border-radius: 3px; }";
    // Error display styling
    ui->ErrorDisplay->setStyleSheet(labelDisplay);

    // Output display styling
    ui->outputLabel->setStyleSheet(labelDisplay);


    // IR and PC display styling
    ui->IRandPCDisplay->setStyleSheet(labelDisplay);

    // Overall widget styling
    setStyleSheet("QWidget { font-size: 12pt; font-family: Arial; background-color: #2e2e2e; color: white}");

    this->showMaximized();





}
void MainWindow::onInstructionEntered() {
    QString code = ui->codeInputTextEdit->text(); // Get the text from the QLineEdit

    ui->ErrorDisplay->clear();
    can_set_start_address = false;
    // Check if the input is valid
    if (code.length() == 4) {
        std::string instruction1 = code.mid(0, 2).toStdString();
        std::string instruction2 = code.mid(2, 2).toStdString();

        //check if we are in the boundaries
        if(cpu.programCounter > 0 && cpu.programCounter < memory->memory_size -1) {
            memory->setCell(cpu.programCounter, instruction1);
            memory->setCell(cpu.programCounter + 1, instruction2);
            cpu.programCounter += 2;
            if(is_step){
                cpu.programCounter -= 2;
                cpu.runNextStep(*memory);
                ui->IRandPCDisplay->setText("IR: "+QString::fromStdString(cpu.IR)+" CP: " + QString::number(cpu.programCounter));
                ui->outputLabel->setText("Hex Value: "+QString::fromStdString(memory->getCell(0)));
                cpu.programCounter += 2;
                updateRegisterTable();
            }
            ui->IRandPCDisplay->setText("IR: "+QString::fromStdString(cpu.IR)+" CP: " + QString::number(cpu.programCounter));
            updateMemoryTable();
        } else  {
            // Display the error message in the QLabel if out of boundaries
            ui->ErrorDisplay->setText("Error: Address is out of bounds. Please enter a valid address.");
        }
    } else {
        ui->ErrorDisplay->setText("Please enter exactly 4 hexadecimal characters (0-9, A-F).");
    }

    ui->codeInputTextEdit->clear();
}

//to update the memory in the gui with any upadates to the memory declared above
void MainWindow::updateMemoryTable() {
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 16; ++col) {
            int address = row * 16 + col;
            std::string value = memory->getCell(address);
            ui->memoryTableWidget->setItem(row, col, new QTableWidgetItem(QString::fromStdString(value)));
        }
    }
}

//to update the memory in the gui with any upadates to the memory declared in cpu calss
void MainWindow::updateRegisterTable() {
    for (int row = 0; row < 16; ++row) {
            std::string value = cpu.reg.getCell(row);
            ui->registerTableWidget->setItem(row,0, new QTableWidgetItem(QString::fromStdString(value)));

    }
}

void MainWindow::onRunButtonClicked() {
    ui->ErrorDisplay->clear();
    bool instructionFound = false;

    // Iterate through the memory to check for valid "Cxxx" instructions
    for (int i = 1; i < memory->memory_size-1; i+=2) {
        std::string instruction = memory->getCell(i) + memory->getCell(i+1);
        if (std::regex_match(instruction, std::regex("^C([0-9A-Fa-f]{3})$"))) {
            instructionFound = true;
            break;
        }
    }
    if (instructionFound){
        cpu.resetProgramCounter();
        int memorySize = memory->memory_size;

        // Loop through memory until we find C000 or reach the end of memory
        while (cpu.programCounter < memorySize-1) { // Ensure programCounter does not exceed memory bounds
            std::string instruction = memory->getCell(cpu.programCounter) + memory->getCell(cpu.programCounter+1);
            if (instruction == "C000") {
                break;
            }
            cpu.runNextStep(*memory);
            cpu.incrementProgramCounter();
        }

        // Update the register table and memory table to reflect changes after execution
        ui->IRandPCDisplay->setText("IR: "+QString::fromStdString(cpu.IR)+" CP: " + QString::number(cpu.programCounter));
        ui->outputLabel->setText("Hex Value: "+QString::fromStdString(memory->getCell(0)));
        updateRegisterTable();
        updateMemoryTable();
    } else {
        ui->ErrorDisplay->setText("Error: No valid 'Cxxx' instruction found in memory.");

    }
}

//clear memory and reset each cell to default value 00
void MainWindow::clearMemory() {
    for (int i = 0; i < memory->memory_size; ++i) {
        memory->setCell(i, "00");
    }
    cpu.programCounter = 1;
    updateMemoryTable();
    ui->IRandPCDisplay->setText("IR: "+QString::fromStdString(cpu.IR)+" CP: " + QString::number(cpu.programCounter));
    ui->outputLabel->setText("Hex Value: "+QString::fromStdString(memory->getCell(0)));
    can_set_start_address = true;
}

//clear register and reset each cell to default value 00
void MainWindow::clearRegisters() {
    for (int i = 0; i < 16; ++i) {
        cpu.reg.setCell(i, "00");
    }
    cpu.IR="0";
    ui->IRandPCDisplay->setText("IR: "+QString::fromStdString(cpu.IR)+" CP: " + QString::number(cpu.programCounter));
    ui->outputLabel->setText("Hex Value: "+QString::fromStdString(memory->getCell(0)));
    updateRegisterTable();
}

//for flexiblity allow user to choose location in memory to edit
void MainWindow::setStartAddress() {
    ui->ErrorDisplay->clear();
    QString hexAddress = ui->startAddressLineEdit->text();
    std::string addressStr = hexAddress.toStdString();      // Convert QString to std::string
    int startAddress = alu.hexToDec(addressStr);
    if(can_set_start_address){
        if (startAddress>0 && startAddress<255) {
            cpu.programCounter=startAddress;
            ui->IRandPCDisplay->setText("IR: "+QString::fromStdString(cpu.IR)+" CP: " + QString::number(cpu.programCounter));
        } else if(startAddress==0){
            ui->ErrorDisplay->setText("Invalid start address can't edit in 00");
        } else {
            // Show an error message if the input is invalid
            ui->ErrorDisplay->setText("Invalid start address or out of boundaries. Please enter a hexadecimal value.");
        }
    } else{
        ui->ErrorDisplay->setText("Can't set start address in the middle of program!");
    }
    ui->startAddressLineEdit->clear();
}
void MainWindow::onRunStepButtonClicked(){
    if(!is_step){
        is_step = true;
        QIcon pauseIcon("D:/Programming/OOP/Task 5 GUI/gui/pause.png");
        ui->runStepButton->setIcon(pauseIcon);
        ui->runStepButton->setIconSize(QSize(24, 24));
        ui->runStepButton->setText("");
    } else{
        is_step = false;
        ui->runStepButton->setIcon(QIcon());
        ui->runStepButton->setText("Run Each Step");
    }
}

void MainWindow::onUploadButtonClicked() {
    clearMemory();
    can_set_start_address = false;
    cpu.programCounter = 1;
    // Open a file dialog to select a file
    QString filePath = QFileDialog::getOpenFileName(this, "Select a File", "", "All Files (*)");

    if (!filePath.isEmpty()) {
        // Process the selected file as needed
        QMessageBox::information(this, "File Selected", "File: " + filePath);

        QFile file(filePath);
        // Try to open the file in read-only mode
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "Error", "Unable to open the file.");
            return;
        }

        // Use QTextStream to read line-by-line
        static const QRegularExpression inPattern("^[0-9A-D][0-9A-F]{3}$");
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.length() != 4) {
                QMessageBox::warning(this, "Error", "Each line must be exactly 4 characters.");
                ui->ErrorDisplay->setText("Error: Each line must be exactly 4 characters.");
                break;
            }
            if (!inPattern.match(line).hasMatch()){
                QMessageBox::warning(this, "Error", "Unrecognized input.");
                ui->ErrorDisplay->setText("Error: Unrecognized input.");
                break;
            }
            std::string sline = line.toStdString();
            std::string instruction1 = string(1, sline[0])+string(1, sline[1]);
            std::string instruction2 = string(1, sline[2])+string(1, sline[3]);
            memory->setCell(cpu.programCounter, instruction1);
            memory->setCell(cpu.programCounter+1, instruction2);
            cpu.programCounter += 2;
        }
        ui->IRandPCDisplay->setText("IR: "+QString::fromStdString(cpu.IR)+" CP: " + QString::number(cpu.programCounter));
        updateMemoryTable();

        file.close();
    } else {
        QMessageBox::warning(this, "No File Selected", "Please select a file.");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
