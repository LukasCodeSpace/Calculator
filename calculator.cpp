#include "calculator.h"
#include "./ui_calculator.h"
#include <cmath>


Calculator::Calculator(QWidget *parent)
    :QMainWindow(parent),ui(new Ui::Calculator)
{
    ui->setupUi(this);
    QPushButton* btnNumber[10] = {ui->btn_0, ui->btn_1, ui->btn_2, ui->btn_3, ui->btn_4, ui->btn_5, ui->btn_6, ui->btn_7, ui->btn_8, ui->btn_9};
    QPushButton* btnOperator[4] = {ui->btn_add, ui->btn_sub, ui->btn_mult, ui->btn_div};

    for(QPushButton* btnPressed : btnOperator)
    {
        connect(btnPressed, &QPushButton::clicked, this, &Calculator::btnOperatorClicked);
    }


    for(QPushButton* btnPressed : btnNumber)
    {
        connect(btnPressed, &QPushButton::clicked, this, &Calculator::btnNumberClicked);
    }
}


Calculator::~Calculator()
{
    delete ui;
}


//lexer
QVector <Token> lexerTokens;
QString userInput;
int userInputPos;

void tokenizeNumber()
{
    QString numberText;

    //stores the value of the current input until the input length is reached
    //only stores digits and dots
    while(userInputPos < userInput.length() && (userInput[userInputPos].isDigit() || userInput[userInputPos] == '.'))
    {
        numberText += userInput[userInputPos];
        userInputPos++;
    }

    double inputNumber = numberText.toDouble();
    lexerTokens.push_back({TokenType::number, inputNumber});
}


QVector <Token> runLexer()
{
    while(userInputPos < userInput.length())
    {
        QChar currentInput = userInput[userInputPos];

        if(currentInput.isSpace())
        {
            userInputPos++;
            continue;
        }

        if(currentInput.isDigit() || currentInput == '.')
        {
            tokenizeNumber();
            continue;
        }

        switch(currentInput.unicode())
        {
            case '+' : lexerTokens.push_back({TokenType::add}); break;
            case '-' : lexerTokens.push_back({TokenType::sub}); break;
            case '*' :
            case 0x00D7 : lexerTokens.push_back({TokenType::mult}); break;
            case '/' :
            case 0x00F7 : lexerTokens.push_back({TokenType::div}); break;
            case '^' : lexerTokens.push_back({TokenType::pow}); break;
            case 0x221A : lexerTokens.push_back({TokenType::root}); break;
            case '(' : lexerTokens.push_back({TokenType::leftPar}); break;
            case ')' : lexerTokens.push_back({TokenType::rightPar}); break;
            //case '=' : lexerTokens.push_back({TokenType::equal}); break;
            default:
                throw std::runtime_error("Error! Invalid character.");
        }
        userInputPos++;
    }
        lexerTokens.push_back({TokenType::end});
        return lexerTokens;
}

//parser

int parserTokensPos = 0;
QVector<Token> parserTokens;
double leftBindPow = 0.0;
double rightBindPow = 0.0;

Token& peek()
{
    return parserTokens[parserTokensPos];
}

Token next()
{
    return parserTokens[parserTokensPos++];
}

void leftBinding(TokenType type)
{
    switch(type)
    {
        case TokenType::add:
        case TokenType::sub: leftBindPow = 1.1; break;
        case TokenType::mult:
        case TokenType::div: leftBindPow = 2.1; break;
        case TokenType::pow:
        case TokenType::root: leftBindPow = 3.1; break;
        default: leftBindPow = 0.0;
    }
}

void rightBinding(TokenType type)
{
    switch(type)
    {
        case TokenType::add:
        case TokenType::sub: rightBindPow = 1.0; break;
        case TokenType::mult:
        case TokenType::div: rightBindPow = 2.0; break;
        case TokenType::pow: rightBindPow = 3.9; break;
        case TokenType::root: rightBindPow = 3.0; break;
        default: rightBindPow = 0.0;
    }
}

double calculate(double minBindPow);

double parseTokenValue()
{
    Token tokenInParser = next();

    switch(tokenInParser.type)
    {
        case TokenType::number : return tokenInParser.value;
        case TokenType::leftPar:
        {
            double parValueReset = calculate(0.0);
            if(peek().type != TokenType::rightPar)
            {
                throw std::runtime_error("Error! Missing ')'!");
            }
            next();
            return parValueReset;
        }
        case TokenType::root : return std::sqrt(calculate(3.5));
        case TokenType::sub : return -calculate(3.5);
        default: throw std::runtime_error("Error! Invalid expression.");
    }
}

double calculate(double minBindPow)
{
    double leftSide = parseTokenValue();
    while(true)
    {
        TokenType currentOperator = peek().type;

        if(currentOperator == TokenType::end || currentOperator == TokenType::rightPar)
        {
            break;
        }

        leftBinding(currentOperator);
        rightBinding(currentOperator);

        if(leftBindPow < minBindPow)
        {
            break;
        }
        next();

        double rightSide = calculate(rightBindPow);

        switch(currentOperator)
        {
            case TokenType::add : leftSide += rightSide; break;
            case TokenType::sub : leftSide -= rightSide; break;
            case TokenType::mult : leftSide *= rightSide; break;
            case TokenType::div :
                if(rightSide == 0)
                {
                    throw std::runtime_error("Error! Division by 0 is not defined.");
                }
                else { leftSide /= rightSide;} break;
            case TokenType::pow : leftSide = std::pow(leftSide, rightSide); break;
            default:
                return leftSide;
        }
    }
    return leftSide;
}

//buttons

void Calculator::btnNumberClicked()
{
    QPushButton *btnNumber = qobject_cast<QPushButton*>(sender());

    if(clearDisplay)
    {
        ui->displayInput->clear();
        ui->displayResult->clear();
    }
    opPressed = false;
    clearDisplay = false;
    QString displayNumber = btnNumber->text();
    ui->displayInput->setText(ui->displayInput->text() + displayNumber);
}

void Calculator::btnOperatorClicked()
{
    QPushButton *btnOperator = qobject_cast<QPushButton*>(sender());
    QString displayOperator = btnOperator->text();

    if(clearDisplay)
    {
        ui->displayInput->setText(ui->displayResult->text() + displayOperator);
        ui->displayResult->clear();
        opPressed = true;
        clearDisplay = false;
        dotUsed = false;
        return;
    }

    if(ui->displayInput->text().isEmpty() && displayOperator == "-")
    {
        ui->displayInput->setText("-");
        opPressed = true;
    }

    if(!opPressed && !ui->displayInput->text().isEmpty())
    {
        ui->displayInput->setText(ui->displayInput->text() + displayOperator);
        opPressed = true;
        dotUsed = false;
    }
}

void Calculator::on_btn_backspace_clicked()
{
    ui->displayInput->backspace();
}

void Calculator::on_btn_pow_clicked()
{
    if(!opPressed)
    {
        ui->displayInput->setText(ui->displayInput->text() + "^");
        opPressed = true;
        dotUsed = false;
    }
    clearDisplay = false;
}

void Calculator::on_btn_root_clicked()
{
    ui->displayInput->setText(ui->displayInput->text() + "√(");
    clearDisplay = false;
    dotUsed = false;
    opPressed = false;
    openPar++;
}

void Calculator::on_btn_dot_clicked()
{
    if(!dotUsed)
    {
        ui->displayInput->setText(ui->displayInput->text() + ".");
        clearDisplay = false;
        dotUsed = true;
    }
}

void Calculator::on_btn_leftPar_clicked()
{
    ui->displayInput->setText(ui->displayInput->text() + "(");
    clearDisplay = false;
    openPar++;
}

void Calculator::on_btn_rightPar_clicked()
{
    if(openPar > 0)
    {
        ui->displayInput->setText(ui->displayInput->text() + ")");
        clearDisplay = false;
        openPar--;
    }
}

void Calculator::on_btn_clear_clicked()
{
    ui->displayInput->setText("");
    ui->displayResult->setText("");
    clearDisplay = false;
    dotUsed = false;
    openPar = 0;
    opPressed = false;
}

void Calculator::on_btn_equal_clicked()
{
    if(ui->displayInput->text().isEmpty())
    {
        return;
    }

    while(openPar > 0)
    {
        ui->displayInput->setText(ui->displayInput->text() + ")");
        openPar--;
    }
    try
    {
        userInput = (ui->displayInput->text());
        userInputPos = 0;
        lexerTokens.clear();
        QVector <Token> lexerOutput = runLexer();
        parserTokens = lexerOutput;
        parserTokensPos = 0;

        double result = calculate(0.0);
        ui->displayResult->setText(QString::number(result));
    }
    catch(std::runtime_error &except)
    {
        ui->displayInput->setText(QString(except.what()));
    }
    opPressed = false;
    clearDisplay = true;
    dotUsed = false;
    openPar = 0;
}
