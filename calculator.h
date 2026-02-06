#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class Calculator;

}

QT_END_NAMESPACE

enum class TokenType {number, add, sub, mult, div, root, pow, rightPar, leftPar, equal, end};

struct Token
{
    TokenType type;
    double value;
};

double calculate(double minBindPow);

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();
    void displayError();

private slots:
    void on_btn_pow_clicked();
    void on_btn_equal_clicked();
    void on_btn_dot_clicked();
    void on_btn_clear_clicked();
    void on_btn_root_clicked();
    void btnNumberClicked();
    void btnOperatorClicked();
    void on_btn_rightPar_clicked();
    void on_btn_leftPar_clicked();
    void on_btn_backspace_clicked();

private:
    Ui::Calculator *ui;
    bool newNumber = true;
    bool clearScndDisplay;
    bool dotUsed = false;
    bool opPressed = false;
    int openPar = 0;
};
#endif // CALCULATOR_H
