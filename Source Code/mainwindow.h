#include "CPU.h"
#include "Register.h"
#include "Memory.h"
#include "ALU.h"
#include "CU.h"
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private slots:
    void onRunButtonClicked();
    void clearMemory();
    void clearRegisters();
    void onInstructionEntered();
    void updateMemoryTable();
    void updateRegisterTable();
    void setStartAddress();
    void onUploadButtonClicked();
    void onRunStepButtonClicked();
private:
    CPU cpu;
    bool is_step;
    bool can_set_start_address;
    Register *reg;
    Memory *memory;
    ALU alu;
    CU cu;


};
#endif // MAINWINDOW_H
