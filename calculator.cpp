#include "calculator.h"
#include "./ui_calculator.h"
#include <cmath>

Calculator::Calculator(QWidget *parent)
    :QMainWindow(parent),ui(new Ui::Calculator)
{
    ui->setupUi(this);

    connect(ui->btn_0, &QPushButton::clicked,this, &Calculator::btnClicked);
    connect(ui->btn_1, &QPushButton::clicked,this, &Calculator::btnClicked);
    connect(ui->btn_2, &QPushButton::clicked,this, &Calculator::btnClicked);
    connect(ui->btn_3, &QPushButton::clicked,this, &Calculator::btnClicked);
    connect(ui->btn_4, &QPushButton::clicked,this, &Calculator::btnClicked);
    connect(ui->btn_5, &QPushButton::clicked,this, &Calculator::btnClicked);
    connect(ui->btn_6, &QPushButton::clicked,this, &Calculator::btnClicked);
    connect(ui->btn_7, &QPushButton::clicked,this, &Calculator::btnClicked);
    connect(ui->btn_8, &QPushButton::clicked,this, &Calculator::btnClicked);
    connect(ui->btn_9, &QPushButton::clicked,this, &Calculator::btnClicked);
}

Calculator::~Calculator()
{
    delete ui;
}

class inputLexer
{
    public:

    inputLexer(const QString &userInput) : userInput(userInput), pos(0){}

    void runLexer()
    {

        while(pos < userInput.length())
        {
            QChar currentInput = userInput[pos];

            if(currentInput.isDigit() || currentInput == '.')
            {
                tokenizeNumber();
            }
            else if(currentInput == '+'){
                tokens.push_back({TokenType::add});
                pos++;
            }
            else if(currentInput == '-'){
                tokens.push_back({TokenType::sub});
                pos++;
            }
            else if(currentInput == '*' || currentInput == QChar(0x00D7)){
                tokens.push_back({TokenType::mult});
                pos++;
            }
            else if(currentInput == '/' || currentInput == QChar(0x00F7)){
                tokens.push_back({TokenType::div});
                pos++;
            }
            else if(currentInput == '('){
                tokens.push_back({TokenType::leftPar});
                pos++;
            }
            else if(currentInput == ')'){
                tokens.push_back({TokenType::rightPar});
                pos++;
            }
            else if(currentInput == '^'){
                tokens.push_back({TokenType::pow});
                pos++;
            }
            else if(currentInput == QChar(0x221A)){
                tokens.push_back({TokenType::root});
                pos++;
            }
            else if(currentInput == '='){
                tokens.push_back({TokenType::equal});
                pos++;
            }
            else {pos++;}
        }
    }

    QVector<Token> tokens;

    public:
        QString userInput;
        int pos;

        void tokenizeNumber()
        {
            QString number;

            while(pos < userInput.length() && (userInput[pos].isDigit() || userInput[pos] == '.'))
            {
                number += userInput[pos];
                pos++;
            }

            double numberValue = number.toDouble();
            tokens.push_back({TokenType::number, numberValue});
        }
};

void Calculator::calculate(double value)
{
    switch(pendingOp)
    {
        case Operators::add : currentValue += value; break;
        case Operators::sub : currentValue -= value; break;
        case Operators::mult : currentValue *= value; break;
        case Operators::div : if(value == 0)
        {
            ui->displayIn->setText("Error");
            ui->displayOut->setText("");
            currentValue = 0;
            pendingOp = Operators::none;
            return;
        }
        currentValue /= value; break;
        case Operators::root : currentValue = std::sqrt(currentValue); break;
        case Operators::pow : currentValue = std::pow(currentValue, value); break;
        case Operators::none : currentValue = value; break;
    }
        ui->displayIn->setText(QString::number(currentValue));
}

void Calculator::btnClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    if(newNumber)
    {
        ui->displayIn->clear();
        newNumber = false;
    }

    if(clearScndDisplay)
    {
        ui->displayOut->clear();
    }
    clearScndDisplay = false;
    QString digit = button->text();
    ui->displayIn->setText(ui->displayIn->text() + digit);
    ui->displayOut->setText(ui->displayOut->text() + digit);
}

void Calculator::on_btn_backspace_clicked()
{

}

void Calculator::on_btn_add_clicked()
{
    calculate(ui->displayIn->text().toDouble());
    newNumber = true;
    clearScndDisplay = false;
    pendingOp = Operators::add;
    ui->displayOut->setText(ui->displayOut->text() + "+");
}

void Calculator::on_btn_sub_clicked()
{
    calculate(ui->displayIn->text().toDouble());
    newNumber = true;
    clearScndDisplay = false;
    pendingOp = Operators::sub;
    ui->displayOut->setText(ui->displayOut->text() + "-");
}

void Calculator::on_btn_mult_clicked()
{
    calculate(ui->displayIn->text().toDouble());
    newNumber = true;
    clearScndDisplay = false;
    pendingOp = Operators::mult;
    ui->displayOut->setText(ui->displayOut->text() + "×");
}

void Calculator::on_btn_div_clicked()
{
    calculate(ui->displayIn->text().toDouble());
    newNumber = true;
    clearScndDisplay = false;
    pendingOp = Operators::div;
    ui->displayOut->setText(ui->displayOut->text() + "÷");
}

void Calculator::on_btn_power_clicked()
{
    calculate(ui->displayIn->text().toDouble());
    newNumber = true;
    clearScndDisplay = false;
    pendingOp = Operators::pow;
    ui->displayOut->setText(ui->displayOut->text() + "^");
}

void Calculator::on_btn_root_clicked()
{
    if(!ui->displayIn->text().contains(QChar(0x221A)))
    {
        calculate(ui->displayIn->text().toDouble());
        newNumber = true;
        clearScndDisplay = false;
        pendingOp = Operators::root;
        ui->displayOut->setText(QChar(0x221A) + ui->displayOut->text());
    }
}

void Calculator::on_btn_dot_clicked()
{
    if(!ui->displayIn->text().contains("."))
    {
        ui->displayIn->setText(ui->displayIn->text() + ".");
        ui->displayOut->setText(ui->displayOut->text() + ".");
        newNumber = false;
        clearScndDisplay = false;
    }
}

void Calculator::on_btn_leftPar_clicked()
{
    if(!ui->displayIn->text().contains("("))
    {
        ui->displayIn->setText(ui->displayIn->text() + "(");
        ui->displayOut->setText(ui->displayOut->text() + "(");
        newNumber = false;
        clearScndDisplay = false;
    }
}

void Calculator::on_btn_rightPar_clicked()
{
    if(!ui->displayIn->text().contains(")"))
    {
        ui->displayIn->setText(ui->displayIn->text() + ")");
        ui->displayOut->setText(ui->displayOut->text() + ")");
        newNumber = false;
        clearScndDisplay = false;
    }
}

void Calculator::on_btn_clear_clicked()
{
    ui->displayOut->setText("");
    ui->displayIn->setText("");
    currentValue = 0.0;
    pendingOp = Operators::none;

    newNumber = true;
    clearScndDisplay = false;
}

void Calculator::on_btn_equal_clicked()
{
    inputLexer lexer(ui->displayOut->text());
    lexer.runLexer();
    calculate(ui->displayIn->text().toDouble());
    newNumber = true;
    clearScndDisplay = true;

    if(!ui->displayOut->text().contains("="))
    {
        ui->displayOut->setText(ui->displayOut->text() + "=");
    }
    currentValue = 0.0;
    pendingOp = Operators::none;
}
