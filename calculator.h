#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Calculator;
}

QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private slots:
    void on_btn_add_clicked();
    void on_btn_sub_clicked();
    void on_btn_mult_clicked();
    void on_btn_div_clicked();
    void on_btn_dot_clicked();
    void on_btn_clear_clicked();
    void on_btn_equal_clicked();
    void on_btn_power_clicked();
    void on_btn_root_clicked();
    void btnClicked();
    void calculate(double value);

private:
    Ui::Calculator *ui;
    double currentValue = 0.0;
    bool newNumber = true;
    bool clearScndDisplay;

    enum class Operators {none, add, sub, mult, div, root, pow};
    Operators pendingOp = Operators::none;

};
#endif // CALCULATOR_H
